#include "rnd/tunic.h"
#include "common/advanced_context.h"
#include "common/utils.h"
#include "game/cmb.h"
#include "game/context.h"
#include "game/resarchiveheader.h"
#include "rnd/objects.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#endif

// Green channel of the vanilla tunic (Kokiri Green). Tunic texels this bright get the full target colour.
#define TUNIC_REFERENCE_GREEN 0x8A

namespace rnd {
  using TexFormat = game::cmb::TextureFormatGL;

  struct TunicTexture {
    TunicForm form;
    const char* model;
    const char* texture;
    TexFormat format;
    bool blend;
    u8* pristine;
    u32 capacity;
    bool captured;
  };

  // Same order as tunicColorNames[1..] and the app's tunicColors.
  static constexpr u32 TunicPresetColors[] = {
      0x168A0E, 0x96000E, 0x002A8E, 0x303030, 0xFFFFFF, 0x139ED8, 0x13E9D8, 0xC23333, 0xD10044, 0x8C008C, 0x400040,
      0x641E7F, 0xAA0565, 0x710093, 0xE9144A, 0xE5DA32, 0xFF7FED, 0xD92994, 0xDB557F, 0xDD3E00, 0x808080, 0xCECE00,
      0xA3C0C0, 0xFF794E, 0x00CA65, 0x400000, 0xCE2500, 0x000070, 0x00D000, 0x005218, 0x0F50BC,
  };

  static u8 sChild01[0x2000], sChild00[0x2000], sNuts00[0x2000], sNutsF00[0x2000], sNutsF00a[0x2000];
  static u8 sGoron00[0x10000], sGoronIwa[0x4000], sGoronGuard[0x4000], sZora00[0x10000], sZoraFin[0x800],
      sZoraShield[0x800];

  static TunicTexture TunicTextures[] = {
      // Human: tunic and the hat strip.
      {TunicForm::Human, "link_child", "link_child_01", TexFormat::ETC1, false, sChild01, sizeof(sChild01), false},
      {TunicForm::Human, "link_child", "link_child_00", TexFormat::ETC1, false, sChild00, sizeof(sChild00), false},
      // Deku: tunic. The leaves are yellow-green so they are left alone.
      {TunicForm::Deku, "link_deknuts", "link_nuts_00", TexFormat::ETC1, false, sNuts00, sizeof(sNuts00), false},
      {TunicForm::Deku, "link_deknuts", "link_nuts_f00", TexFormat::ETC1, false, sNutsF00, sizeof(sNutsF00), false},
      {TunicForm::Deku, "link_deknuts", "link_nuts_f00a", TexFormat::ETC1, false, sNutsF00a, sizeof(sNutsF00a), false},
      // Goron: hat and cloth, plus the rolling ball and guard models.
      {TunicForm::Goron, "link_goron", "link_goron_00", TexFormat::RGB565, false, sGoron00, sizeof(sGoron00), false},
      {TunicForm::Goron, "link_goron_iwa", "link_goron_iwa_", TexFormat::ETC1A4, false, sGoronIwa, sizeof(sGoronIwa),
       false},
      {TunicForm::Goron, "link_goron_guar", "link_goron_guar", TexFormat::ETC1A4, false, sGoronGuard,
       sizeof(sGoronGuard), false},
      // Zora: cloth, horns, head fin tip, arm fins and the fin shield.
      {TunicForm::Zora, "link_zora", "link_zora_00", TexFormat::RGB565, true, sZora00, sizeof(sZora00), false},
      {TunicForm::Zora, "link_zora", "link_zora_fin_0", TexFormat::ETC1, true, sZoraFin, sizeof(sZoraFin), false},
      {TunicForm::Zora, "link_zora", "link_zora_shiel", TexFormat::ETC1, true, sZoraShield, sizeof(sZoraShield), false},
  };

  const char* const tunicColorNames[] = {
      "Seed Default", "Kokiri Green", "Goron Red",    "Zora Blue",  "Black",         "White",         "Azure Blue",
      "Vivid Cyan",   "Light Red",    "Fuchsia",      "Purple",     "Majora Purple", "Twitch Purple", "Magenta",
      "Violet",       "Persian Rose", "Dirty Yellow", "Blush Pink", "Hot Pink",      "Rose Pink",     "Orange",
      "Gray",         "Yellow",       "Silver",       "Beige",      "Teal",          "Blood Red",     "Blood Orange",
      "Royal Blue",   "NES Green",    "Dark Green",   "Lumen",
  };
  static_assert(ARR_SIZE(tunicColorNames) == tunicColorCount);
  static_assert(ARR_SIZE(tunicColorNames) == ARR_SIZE(TunicPresetColors) + 1);
  static_assert(tunicColorCount <= 32);

  const char* const tunicFormNames[] = {"Human", "Deku", "Goron", "Zora"};
  static_assert(ARR_SIZE(tunicFormNames) == (u32)TunicForm::Count);

  // memcpy2, which handles any alignment. The game's memcpy (0x1F28E8) expects word aligned buffers.
  static void GameMemcpy(void* dst, const void* src, u32 size) {
    util::GetPointer<void*(void*, const void*, u32)>(0x300154)(dst, src, size);
  }

  static s32 GameStrcmp(const char* lhs, const char* rhs) {
    return util::GetPointer<s32(const char*, const char*)>(0x302E3C)(lhs, rhs);
  }

  static void GSP_FlushDataCache(const void* addr, u32 size) {
    util::GetPointer<void(const void*, u32)>(0x16BB14)(addr, size);
  }

  // Copy into GPU memory, run with the next command list.
  static void nngxAddVramDmaCommand(void* dst, const void* src, u32 size) {
    util::GetPointer<void(void*, const void*, u32)>(0x180C48)(dst, src, size);
  }

  struct TunicRGB {
    u32 r, g, b;
  };

  static u32 Tunic_Expand5(u32 value5) {
    return (value5 << 3) | (value5 >> 2);
  }

  static u32 Tunic_Quantize5(u32 value) {
    const u32 quantized = (value * 31 + 127) / 255;
    return quantized > 31 ? 31 : quantized;
  }

  static u32 Tunic_Quantize4(u32 value) {
    const u32 quantized = (value * 15 + 127) / 255;
    return quantized > 15 ? 15 : quantized;
  }

  static s32 Tunic_SignExtend3(u32 value3) {
    return value3 >= 4 ? (s32)value3 - 8 : (s32)value3;
  }

  // Two base colours from the high word of an ETC1 block.
  static void Tunic_DecodeEtc1Bases(u32 blockHi, TunicRGB bases[2]) {
    if (blockHi & 2) {
      // Differential: 5-bit base and a 3-bit signed delta.
      const u32 red5 = (blockHi >> 27) & 31;
      const u32 green5 = (blockHi >> 19) & 31;
      const u32 blue5 = (blockHi >> 11) & 31;
      bases[0] = {Tunic_Expand5(red5), Tunic_Expand5(green5), Tunic_Expand5(blue5)};
      bases[1] = {Tunic_Expand5((red5 + Tunic_SignExtend3((blockHi >> 24) & 7)) & 31),
                  Tunic_Expand5((green5 + Tunic_SignExtend3((blockHi >> 16) & 7)) & 31),
                  Tunic_Expand5((blue5 + Tunic_SignExtend3((blockHi >> 8) & 7)) & 31)};
    } else {
      // Individual: two 4-bit colours.
      bases[0] = {((blockHi >> 28) & 15) * 17, ((blockHi >> 20) & 15) * 17, ((blockHi >> 12) & 15) * 17};
      bases[1] = {((blockHi >> 24) & 15) * 17, ((blockHi >> 16) & 15) * 17, ((blockHi >> 8) & 15) * 17};
    }
  }

  // Keeps the intensity tables and flip bit. Differential mode is kept when it fits, so an untouched base comes back
  // the same.
  static u32 Tunic_EncodeEtc1Bases(u32 blockHi, const TunicRGB bases[2]) {
    const u32 tablesAndFlip = blockHi & 0xFD;
    const s32 first5[3] = {(s32)Tunic_Quantize5(bases[0].r), (s32)Tunic_Quantize5(bases[0].g),
                           (s32)Tunic_Quantize5(bases[0].b)};
    const s32 second5[3] = {(s32)Tunic_Quantize5(bases[1].r), (s32)Tunic_Quantize5(bases[1].g),
                            (s32)Tunic_Quantize5(bases[1].b)};
    s32 delta[3];
    bool fits = true;
    for (u32 i = 0; i < 3; ++i) {
      delta[i] = second5[i] - first5[i];
      fits &= delta[i] >= -4 && delta[i] <= 3;
    }
    if (fits) {
      return tablesAndFlip | 2 | (first5[0] << 27) | ((delta[0] & 7) << 24) | (first5[1] << 19) |
             ((delta[1] & 7) << 16) | (first5[2] << 11) | ((delta[2] & 7) << 8);
    }
    return tablesAndFlip | (Tunic_Quantize4(bases[0].r) << 28) | (Tunic_Quantize4(bases[1].r) << 24) |
           (Tunic_Quantize4(bases[0].g) << 20) | (Tunic_Quantize4(bases[1].g) << 16) |
           (Tunic_Quantize4(bases[0].b) << 12) | (Tunic_Quantize4(bases[1].b) << 8);
  }

  // Straps, belts, skin, hair and Deku leaves are red-dominant or yellow-green, so they fail this.
  static bool Tunic_IsGreen(const TunicRGB& color) {
    const u32 strongestOther = color.r > color.b ? color.r : color.b;
    return color.g > strongestOther && (color.g - strongestOther) * 4 >= color.g;
  }

  // 0 to 256 for how far green leads blue, covering the Zora's yellow and green fins. Cyan skin (green level with
  // blue) comes out 0, and so do the orange hexagon and red trim, where red leads green.
  static u32 Tunic_GetBlendWeight(const TunicRGB& color) {
    if (color.g == 0 || color.g <= color.b || color.r * 8 > color.g * 9)
      return 0;
    const u32 weight = (color.g - color.b) * 512 / color.g;
    return weight > 256 ? 256 : weight;
  }

  static u32 Tunic_Tint(u32 channel, u32 green) {
    const u32 tinted = (channel * green + TUNIC_REFERENCE_GREEN / 2) / TUNIC_REFERENCE_GREEN;
    return tinted > 255 ? 255 : tinted;
  }

  static TunicRGB Tunic_TintColor(const TunicRGB& color, u32 rgb) {
    return {Tunic_Tint((rgb >> 16) & 0xFF, color.g), Tunic_Tint((rgb >> 8) & 0xFF, color.g),
            Tunic_Tint(rgb & 0xFF, color.g)};
  }

  // Returns false if the colour is left alone.
  static bool Tunic_RecolorColor(TunicRGB& color, u32 rgb, bool blend) {
    if (!blend) {
      if (!Tunic_IsGreen(color))
        return false;
      color = Tunic_TintColor(color, rgb);
      return true;
    }
    const u32 weight = Tunic_GetBlendWeight(color);
    if (weight == 0)
      return false;
    const TunicRGB tinted = Tunic_TintColor(color, rgb);
    color = {(color.r * (256 - weight) + tinted.r * weight) / 256, (color.g * (256 - weight) + tinted.g * weight) / 256,
             (color.b * (256 - weight) + tinted.b * weight) / 256};
    return true;
  }

  // Only the two base colours of each block change; the per-texel modifiers keep the shading. ETC1A4 blocks have 8
  // bytes of alpha first. Texture data is 0x80 aligned in the CMB, so blocks are accessed in place.
  static void Tunic_RecolorEtc1(u8* dst, u32 size, u32 rgb, bool blend, bool withAlpha) {
    const u32 stride = withAlpha ? 16 : 8;
    for (u32 offset = 0; offset + stride <= size; offset += stride) {
      u32* blockHi = reinterpret_cast<u32*>(dst + offset + stride - 4);
      TunicRGB bases[2];
      Tunic_DecodeEtc1Bases(*blockHi, bases);
      bool changed = false;
      for (TunicRGB& base : bases)
        changed |= Tunic_RecolorColor(base, rgb, blend);
      if (changed)
        *blockHi = Tunic_EncodeEtc1Bases(*blockHi, bases);
    }
  }

  // Per texel, so the tiled order doesn't matter.
  static void Tunic_RecolorRGB565(u8* dst, u32 size, u32 rgb, bool blend) {
    u16* texels = reinterpret_cast<u16*>(dst);
    for (u32 i = 0; i < size / 2; ++i) {
      const u16 texel = texels[i];
      const u32 green6 = (texel >> 5) & 63;
      TunicRGB color = {Tunic_Expand5(texel >> 11), (green6 << 2) | (green6 >> 4), Tunic_Expand5(texel & 31)};
      if (!Tunic_RecolorColor(color, rgb, blend))
        continue;
      const u32 newGreen6 = (color.g * 63 + 127) / 255;
      texels[i] =
          (u16)((Tunic_Quantize5(color.r) << 11) | ((newGreen6 > 63 ? 63 : newGreen6) << 5) | Tunic_Quantize5(color.b));
    }
  }

  // Returns false for vanilla.
  static bool Tunic_GetColor(u8 choice, u32* rgb) {
    if (choice == 0) {
      if (!gSettingsContext.customTunicColors)
        return false;
      *rgb = gSettingsContext.customTunicColor;
      return true;
    }
    if (choice > ARR_SIZE(TunicPresetColors))
      return false;
    *rgb = TunicPresetColors[choice - 1];
    return true;
  }

  static s32 Tunic_FindTexture(void* cmb, const char* name) {
    game::cmb::Tex* tex = game::cmb::Cmb_GetTex(cmb);
    if (tex == nullptr)
      return -1;
    for (s32 i = 0; i < tex->textureCount; ++i) {
      if (GameStrcmp(tex->entry[i].name, name) == 0)
        return i;
    }
    return -1;
  }

  // Recolours the raw texture data. A CmbMan made later uploads it as is, one that already exists gets a DMA.
  static void Tunic_ApplyToCMB(void* cmb, game::ObjectBank::CmbMan* cmbMan) {
    const char* model = static_cast<game::cmb::CMB_HEAD*>(cmb)->name;
    for (TunicTexture& entry : TunicTextures) {
      if (GameStrcmp(model, entry.model) != 0)
        continue;
      const s32 index = Tunic_FindTexture(cmb, entry.texture);
      if (index < 0)
        continue;
      const game::cmb::TextureEntry* texture = game::cmb::Cmb_GetTexture(cmb, index);
      const u32 size = texture->dataLength;
      if (texture->format != entry.format || size > entry.capacity)
        continue;
      u8* data = game::cmb::Cmb_GetTextureData(cmb, texture);
      const u8 choice = Tunic_GetChoice(entry.form);
      u32 rgb = 0;

      // Nothing else edits these, so the first time we see them they are vanilla.
      if (!entry.captured) {
        GameMemcpy(entry.pristine, data, size);
        entry.captured = true;
      }

      GameMemcpy(data, entry.pristine, size);
      if (Tunic_GetColor(choice, &rgb)) {
        if (entry.format == TexFormat::RGB565)
          Tunic_RecolorRGB565(data, size, rgb, entry.blend);
        else
          Tunic_RecolorEtc1(data, size, rgb, entry.blend, entry.format == TexFormat::ETC1A4);
      }
      GSP_FlushDataCache(data, size);

      game::ObjectBank::GfxTexture* uploaded = nullptr;
      if (cmbMan != nullptr && cmbMan->textures != nullptr)
        uploaded = cmbMan->textures[index];
      if (uploaded != nullptr && uploaded->gpuData != nullptr && uploaded->size == size)
        nngxAddVramDmaCommand(uploaded->gpuData, data, size);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: %s %s choice %u uploaded %u\n", __func__, entry.model, entry.texture, choice,
                       uploaded != nullptr);
#endif
    }
  }

  void Tunic_ApplyToObject(game::ObjectBank::ObjectBankArchive* archive) {
    if (archive == nullptr || archive->archive.raw == nullptr)
      return;
    for (u32 i = 0;; ++i) {
      void* cmb = game::ResArchive_GetFileByType(archive->archive.raw, game::ResFileType::CMB, i);
      if (cmb == nullptr)
        break;
      Tunic_ApplyToCMB(cmb, archive->cmb_files != nullptr ? archive->cmb_files[i] : nullptr);
    }
  }

  // Looked up every time, a scene change frees the object.
  static void Tunic_RefreshPlayer() {
    game::GlobalContext* gctx = GetContext().gctx;
    if (gctx == nullptr)
      return;
    game::act::Player* player = gctx->GetPlayerActor();
    if (player == nullptr)
      return;
    game::ActorResource::ActorResource* entry = Object_GetEntry(player->object_id);
    if (entry == nullptr || (s16)entry->object_id <= 0)
      return;
    Tunic_ApplyToObject(&entry->archive);
  }

  u8 Tunic_GetChoice(TunicForm form) {
    if (form == TunicForm::Human)
      return gExtSaveData.sfxOptions.tunicColor;
    return gExtSaveData.formTunicColors[(u32)form - 1];
  }

  void Tunic_SetChoice(TunicForm form, u8 choice) {
    if (form == TunicForm::Human)
      gExtSaveData.sfxOptions.tunicColor = choice;
    else
      gExtSaveData.formTunicColors[(u32)form - 1] = choice;
    // Other forms aren't loaded, they pick up the change on the next transformation.
    Tunic_RefreshPlayer();
  }

  u32 Tunic_GetChoiceColor(u8 choice) {
    u32 rgb = TunicPresetColors[0];
    Tunic_GetColor(choice, &rgb);
    return rgb;
  }
}  // namespace rnd
