#include "rnd/tunic.h"
#include "common/advanced_context.h"
#include "common/utils.h"
#include "game/context.h"
#include "rnd/custom_models.h"
#include "rnd/objects.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#endif

// Tunic colours, with pre-coloured textures.
//
// The tunic shares its textures with skin, belts and the like, so the colours are made ahead of time by
// gen_tunic_assets.py. Each colour has its own archive (zelda2_tunic_NN.gar.lzs, 00 being vanilla and the rest the
// presets in tunicColorNames order) with a two-frame CMAB per model (vanilla, then that colour's tunic textures). Only
// the current form's colour is loaded; changing colour or transforming into a form with another colour loads the
// new archive and attaches it again. The CMAB runs in the face animation slot the game leaves free.

// Eyes, mouth and an unused third slot, set up in player init (0x224550).
#define PLAYER_FACE_TEXANIMS 0x12168
#define PLAYER_BODY_MODEL 0x368
// Goron models made in player init (0x2168D0): link_goron_iwa, and link_goron_guar's in the ActorUtil at 0x474.
#define PLAYER_GORON_BALL_MODEL 0x12674
#define PLAYER_GORON_GUARD_MODEL 0x4A8
#define TUNIC_FACE_SLOT 2
// Frame 0 of each CMAB is vanilla, frame 1 the archive's colour.
#define TUNIC_COLOR_FRAME 1.0f

namespace rnd {
  // Same order as tunicColorNames[1..] and the app's tunicColors.
  static constexpr u32 TunicPresetColors[] = {
      0x168A0E, 0x96000E, 0x002A8E, 0x303030, 0xFFFFFF, 0x139ED8, 0x13E9D8, 0xC23333, 0xD10044, 0x8C008C, 0x400040,
      0x641E7F, 0xAA0565, 0x710093, 0xE9144A, 0xE5DA32, 0xFF7FED, 0xD92994, 0xDB557F, 0xDD3E00, 0x808080, 0xCECE00,
      0xA3C0C0, 0xFF794E, 0x00CA65, 0x400000, 0xCE2500, 0x000070, 0x00D000, 0x005218, 0x0F50BC,
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

  // CMAB order in each archive
  enum class TunicCMAB : u32 { HumanBody, DekuBody, GoronBody, GoronBall, GoronGuard, ZoraBody };

  static constexpr TunicCMAB TunicBodyCMABs[] = {
      TunicCMAB::HumanBody,
      TunicCMAB::DekuBody,
      TunicCMAB::GoronBody,
      TunicCMAB::ZoraBody,
  };
  static_assert(ARR_SIZE(TunicBodyCMABs) == (u32)TunicForm::Count);

  // Object_Clear (0x14E8F4) also reads a count at 0x2E24, past the fields ExtendedObjectContext has, so each gets the
  // full size of the game's context; the padding stays zero.
  struct TunicObjectContext {
    ExtendedObjectContext ctx;
    u8 unk_1834[0x2E28 - sizeof(ExtendedObjectContext)];
  };

  // Two, so a new colour is attached before the old one is freed.
  static TunicObjectContext sTunicObjects[2];
  static u8 sTunicObjectCurrent = 0;
  static s8 sTunicLoadedColor = -1;

  struct PlayerFaceTexAnims {
    u8 active[4];
    game::act::SA_TextureAnimation slots[3];
  };
  static_assert(sizeof(PlayerFaceTexAnims) == 0x1F0);

  static void TexAnim_Construct(game::act::SA_TextureAnimation* texAnim) {
    util::GetPointer<void(game::act::SA_TextureAnimation*)>(0x1F224C)(texAnim);
  }

  static void TexAnim_Attach(game::act::SA_TextureAnimation* texAnim, void* cmabMan) {
    util::GetPointer<void(game::act::SA_TextureAnimation*, void*)>(0x229DA4)(texAnim, cmabMan);
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

  // Fierce Deity has no tunic.
  static bool Tunic_GetForm(game::act::Player::Form playerForm, TunicForm* form) {
    switch (playerForm) {
    case game::act::Player::Form::Human:
      *form = TunicForm::Human;
      return true;
    case game::act::Player::Form::Deku:
      *form = TunicForm::Deku;
      return true;
    case game::act::Player::Form::Goron:
      *form = TunicForm::Goron;
      return true;
    case game::act::Player::Form::Zora:
      *form = TunicForm::Zora;
      return true;
    default:
      return false;
    }
  }

  // Vanilla (00) for the seed default, since a custom colour can't be pre-made.
  static u8 Tunic_GetArchiveColor(TunicForm form) {
    const u8 choice = Tunic_GetChoice(form);
    return choice < tunicColorCount ? choice : 0;
  }

  static void* FileEntity_Create(const char* path) {
    return util::GetPointer<void*(const char*)>(0x20ABEC)(path);
  }

  static void FileEntity_Start(void* entity) {
    util::GetPointer<void(void*, void*)>(0x1F1BA0)(reinterpret_cast<void*>(0x6F8178), entity);
  }

  static void FileEntity_Wait(void* entity) {
    util::GetPointer<void(void*)>(0x160FDC)(entity);
  }

  static void FileEntity_Delete(void* entity) {
    util::GetPointer<void(void*)>(0x1DE364)(entity);
  }

  static void ObjectBankArchive_Init(game::ObjectBank::ObjectBankArchive* archive, u16 objectId,
                                     game::ResArchiveHeader* data, u32 size) {
    util::GetPointer<void(game::ObjectBank::ObjectBankArchive*, u32, game::ResArchiveHeader*, u32, u8)>(0x1D4844)(
        archive, objectId, data, size, 0);
  }

  // loadActorResource (0x4C01CC), with the file's cache key passed in. Loaded files are shared by key while they're
  // alive, and the game's key is just the object id, so each colour needs its own key or loading one returns the colour
  // that is still loaded.
  static bool Tunic_LoadObject(ExtendedObjectContext* ctx, s16 objectId, u32 cacheKey) {
    game::ActorResource::ActorResource* entry = &ctx->status[ctx->num];
    entry->object_id = objectId;
    entry->file_data = nullptr;
    entry->file_size = 0;

    u8* entity = static_cast<u8*>(FileEntity_Create(game::ActorResource::GetActorResourcePathTable()[objectId].path));
    *reinterpret_cast<u32*>(entity + 0xC) = cacheKey;
    FileEntity_Start(entity);
    FileEntity_Wait(entity);
    const bool loaded = *reinterpret_cast<s32*>(entity + 0x8) >= 0;
    if (loaded) {
      entry->file_data = *reinterpret_cast<game::ResArchiveHeader**>(entity + 0x14);
      entry->file_size = *reinterpret_cast<u32*>(entity + 0x10);
      ObjectBankArchive_Init(&entry->archive, objectId, entry->file_data, entry->file_size);
    } else {
      entry->object_id = 0;
    }
    FileEntity_Delete(entity);
    ctx->num++;
    ctx->numPersistent = ctx->num;
    return loaded;
  }

  // Loads a colour's archive into the spare context. Returns false, keeping the current one, if it fails.
  static bool Tunic_LoadArchive(u8 color) {
    ExtendedObjectContext* next = &sTunicObjects[sTunicObjectCurrent ^ 1].ctx;
    const s16 objectId = static_cast<s16>(ObjectId::OBJECT_TUNIC);
    char* path = game::ActorResource::GetActorResourcePathTable()[objectId].path;
    // "rom:/actors/zelda2_tunic_NN.gar.lzs"
    char* digits = path + 25;
    digits[0] = '0' + color / 10;
    digits[1] = '0' + color % 10;

    Object_Clear(next);
    if (!Tunic_LoadObject(next, objectId, 0x40000000 | (color << 16) | objectId)) {
      Object_Clear(next);
      return false;
    }
    sTunicObjectCurrent ^= 1;
    sTunicLoadedColor = color;
    return true;
  }

  // Null if the archive is missing.
  static void* Tunic_GetCMAB(TunicCMAB index) {
    ExtendedObjectContext* ctx = &sTunicObjects[sTunicObjectCurrent].ctx;
    if (!Object_IsLoaded(ctx, 0))
      return nullptr;
    return GAR_GetCMABByIndex(&ctx->status[0].archive, static_cast<u32>(index));
  }

  static PlayerFaceTexAnims* Tunic_GetFaceTexAnims(game::act::Player* player) {
    return reinterpret_cast<PlayerFaceTexAnims*>(reinterpret_cast<u8*>(player) + PLAYER_FACE_TEXANIMS);
  }

  static void Tunic_ShowColor(game::act::SA_TextureAnimation* texAnim, void* cmab) {
    TexAnim_Attach(texAnim, cmab);
    texAnim->anim_speed = 0.0f;
    texAnim->anim_mode = 0;
    texAnim->cur_frame = TUNIC_COLOR_FRAME;
  }

  static void Tunic_ShowColorOnModel(game::act::Player* player, u32 modelOffset, TunicCMAB index) {
    auto* model = *reinterpret_cast<game::act::SkeletonAnimationModel**>(reinterpret_cast<u8*>(player) + modelOffset);
    void* cmab = Tunic_GetCMAB(index);
    if (model != nullptr && model->texAnim != nullptr && cmab != nullptr)
      Tunic_ShowColor(model->texAnim, cmab);
  }

  static void Tunic_Attach(game::act::Player* player, TunicForm form) {
    PlayerFaceTexAnims* faces = Tunic_GetFaceTexAnims(player);
    auto* body = *reinterpret_cast<u8**>(reinterpret_cast<u8*>(player) + PLAYER_BODY_MODEL);
    if (body == nullptr)
      return;

    const u8 color = Tunic_GetArchiveColor(form);
    const s8 previousColor = sTunicLoadedColor;
    ExtendedObjectContext* previous = nullptr;
    if (color != sTunicLoadedColor) {
      previous = &sTunicObjects[sTunicObjectCurrent].ctx;
      if (!Tunic_LoadArchive(color))
        return;
    }
    void* cmab = Tunic_GetCMAB(TunicBodyCMABs[(u32)form]);
    if (cmab == nullptr) {
      // Go back to the colour that is attached, so the next load doesn't free it.
      if (previous != nullptr) {
        Object_Clear(&sTunicObjects[sTunicObjectCurrent].ctx);
        sTunicObjectCurrent ^= 1;
        sTunicLoadedColor = previousColor;
      }
      return;
    }

    game::act::SA_TextureAnimation* texAnim = &faces->slots[TUNIC_FACE_SLOT];
    if (!faces->active[TUNIC_FACE_SLOT]) {
      TexAnim_Construct(texAnim);
      // Binds the animation to the body model, the same value the eye and mouth slots get.
      texAnim->field_08 = *reinterpret_cast<game::act::TexAnim_Unk_10**>(body + 0x9C);
    }
    Tunic_ShowColor(texAnim, cmab);
    faces->active[TUNIC_FACE_SLOT] = 1;

    if (form == TunicForm::Goron) {
      Tunic_ShowColorOnModel(player, PLAYER_GORON_BALL_MODEL, TunicCMAB::GoronBall);
      Tunic_ShowColorOnModel(player, PLAYER_GORON_GUARD_MODEL, TunicCMAB::GoronGuard);
    }
    // Nothing uses the old colour now.
    if (previous != nullptr)
      Object_Clear(previous);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: %s tunic colour %u attached\n", __func__, tunicFormNames[(u32)form], color);
#endif
  }

  void Tunic_AttachTexAnim(game::act::Player* player) {
    TunicForm form;
    if (player->id == game::act::Id::Player && Tunic_GetForm(player->active_form, &form))
      Tunic_Attach(player, form);
  }

  void Tunic_ClearObjects() {
    for (TunicObjectContext& object : sTunicObjects)
      Object_Clear(&object.ctx);
    sTunicLoadedColor = -1;
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

    // Show it now if Link is in that form; other forms pick it up when he transforms.
    game::GlobalContext* gctx = GetContext().gctx;
    game::act::Player* player = gctx != nullptr ? gctx->GetPlayerActor() : nullptr;
    TunicForm current;
    if (player == nullptr || !Tunic_GetForm(player->active_form, &current) || current != form)
      return;
    if (Tunic_GetFaceTexAnims(player)->active[TUNIC_FACE_SLOT])
      Tunic_Attach(player, form);
  }

  u32 Tunic_GetChoiceColor(u8 choice) {
    u32 rgb = TunicPresetColors[0];
    Tunic_GetColor(choice, &rgb);
    return rgb;
  }
}  // namespace rnd
