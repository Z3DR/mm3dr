#include "rnd/custom_models.h"
#include <string.h>
#include "game/cmb.h"
#include "game/resarchiveheader.h"

#define EDIT_BYTE(offset_, val_) (BASE_[offset_] = val_)
#define EDIT_U32(offset_, val_)                                                                                        \
  (EDIT_BYTE((offset_) + 0, (val_) >> 24), EDIT_BYTE((offset_) + 1, (val_) >> 16),                                     \
   EDIT_BYTE((offset_) + 2, (val_) >> 8), EDIT_BYTE((offset_) + 3, (val_)))
namespace rnd {
  static constexpr game::cmb::RGBA OpaqueBlack{0, 0, 0, 255};
  static constexpr game::cmb::RGBA OpaqueWhite{255, 255, 255, 255};

  struct KeyColorData {
    u32 emission;
    u32 ambient;
    u32 diffuse;
  };

  KeyColorData SmallKeyData[] = {
      {0x00000000, 0x00800000, 0x00CC0000},  // Woodfall
      {0xFFFFFF00, 0xFFFFFF00, 0x7F7FFFFF},  // Snowhead
      {0x00000000, 0x0000DA00, 0x0000FFFF},  // Great Bay
      {0x00000000, 0x80550000, 0xFFAA0000},  // Stone Tower
  };

  game::cmb::RGBA SongColors[] = {
      0xFF0000FF,  // Goron
      0x00FF00FF,  // Elegy
      0x800080FF,  // Oath
      0xFFFF00FF,  // Sonata
      0xFFA500FF,  // Epona
      0x00008BFF,  // NWBN
      0xFFFFFFFF,  // Soaring
      0x65809FFF,  // Storms
      0xFF96B0FF,  // Healing
  };

  static u8 Clamp8(u8 v) {
    return static_cast<u8>(v < 0 ? 0 : (v > 255 ? 255 : v));
  }

  static u8 Lerp(u8 a, u8 b, float t) {
    return Clamp8(static_cast<int>(a + (int(b) - int(a)) * t));
  }

  static game::cmb::RGBA LerpRGB(game::cmb::RGBA& dst, game::cmb::RGBA a, game::cmb::RGBA b, float t) {
    dst.R = Lerp(a.R, b.R, t);
    dst.G = Lerp(a.G, b.G, t);
    dst.B = Lerp(a.B, b.B, t);
    return dst;
  }

  [[maybe_unused]] static game::cmb::RGBA LerpRGBA(game::cmb::RGBA& dst, game::cmb::RGBA a, game::cmb::RGBA b,
                                                   float t) {
    LerpRGB(dst, a, b, t);
    dst.A = Lerp(a.A, b.A, t);
    return dst;
  }

  static void CustomModel_ApplyColorEditsToSmallKey(void* smallKeyCMB, s32 keyType) {
    const KeyColorData& c = SmallKeyData[keyType];
    game::cmb::Material* material = game::cmb::Cmb_GetMaterial(smallKeyCMB, 0);

    if (material == nullptr)
      return;

    material->emissionColor = c.emission;
    material->ambientColor = c.ambient;
    material->diffuse = c.diffuse;
  }

  static void CustomModel_ApplyColorEditsToOcarina(void* cmb, s32 songType) {
    game::cmb::Material* baseMaterial = game::cmb::Cmb_GetMaterial(cmb, 0);
    game::cmb::Material* triforceMaterial = game::cmb::Cmb_GetMaterial(cmb, 1);

    if (baseMaterial == nullptr || triforceMaterial == nullptr)
      return;

    if ((songType >= 0 && songType < 9)) {
      auto songColor = SongColors[songType];

      baseMaterial->diffuse = SongColors[songType];
      LerpRGB(baseMaterial->ambientColor, songColor, OpaqueBlack, 0.55f);
      LerpRGB(baseMaterial->specular0, songColor, OpaqueWhite, 0.35f);

      triforceMaterial->specular0 = baseMaterial->specular0;
    } else {
      baseMaterial->diffuse = 0x3B39FFFF;
      baseMaterial->specular0 = 0x592AB200;
    }
  }

  void CustomModels_EditItemCMB(void* ZARBuf, u16 objectId, s8 special) {
    void* cmb = game::ResArchive_GetFileByType(ZARBuf, game::ResFileType::CMB);
    if (cmb == nullptr)
      return;

    switch ((ObjectId)objectId) {
    case ObjectId::OBJECT_CUSTOM_SMALL_KEY:
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      // Always apply as basepatch for testing.
      CustomModel_ApplyColorEditsToSmallKey(cmb, special);
#else
      if (gSettingsContext.coloredKeys == 1) {
        CustomModel_ApplyColorEditsToSmallKey(cmb, special);
      }
#endif

      break;
    case ObjectId::OBJECT_CUSTOM_SONGS:
      CustomModel_ApplyColorEditsToOcarina(cmb, special);
      break;
    case ObjectId::OBJECT_CUSTOM_ASSETS:
      break;
    }
  }

  void CustomModels_ApplyItemCMAB(game::act::SkeletonAnimationModel* model, u16 objectId, s8 special) {
    void* cmabMan;

    switch ((ObjectId)objectId) {
    case ObjectId::OBJECT_CUSTOM_SONGS:
      cmabMan = ExtendedObject_GetCMABByIndex(static_cast<s16>(ObjectId::OBJECT_CUSTOM_ASSETS),
                                              static_cast<u32>(TexAnimCustomAssets::TEXANIM_SONG));

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: Special is %u\n", __func__, special);
#endif
      TexAnim_Spawn(model->texAnim, cmabMan);
      model->texAnim->anim_speed = 0.00f;
      model->texAnim->anim_mode = 0;
      model->texAnim->cur_frame = special;
      break;
    case ObjectId::OBJECT_CUSTOM_ASSETS:
      break;
    case ObjectId::OBJECT_CUSTOM_SMALL_KEY:
      break;
    }
  }

}  // namespace rnd
