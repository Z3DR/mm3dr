#include "rnd/tunic.h"
#include "common/advanced_context.h"
#include "common/utils.h"
#include "game/cmb.h"
#include "game/context.h"
#include "rnd/custom_models.h"
#include "rnd/objects.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#endif

#define PLAYER_FACE_TEXANIMS 0x12168
#define PLAYER_BODY_MODEL 0x368
#define PLAYER_GORON_BALL_MODEL 0x12674
#define PLAYER_GORON_GUARD_MODEL 0x4A8
#define TUNIC_FACE_SLOT 2

namespace rnd {
  using game::cmb::Combiner;
  using game::cmb::CombinerMode;
  using game::cmb::CombinerOp;
  using game::cmb::CombinerScale;
  using game::cmb::CombinerSrc;
  using game::cmb::TextureFormatGL;

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

  enum class TunicTintCMAB : u32 { HumanBody, DekuBody, GoronBody, ZoraBody, GoronBall, GoronGuard };

  struct TunicObjectContext {
    ExtendedObjectContext ctx;
    u8 unk_1834[0x2E28 - sizeof(ExtendedObjectContext)];
  };

  // The tint archive, loaded once a scene.
  static TunicObjectContext sTunicTintObject;

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

  static s32 GameStrcmp(const char* lhs, const char* rhs) {
    return util::GetPointer<s32(const char*, const char*)>(0x302E3C)(lhs, rhs);
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

  // Kokiri Green for vanilla, which on the greyscale tunic looks like the original.
  static u32 Tunic_GetTintColor(TunicForm form) {
    u32 rgb = TunicPresetColors[0];
    Tunic_GetColor(Tunic_GetChoice(form), &rgb);
    return rgb;
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
  // alive, and the game's key is just the object id.
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

  // Null if the archive is missing.
  static void* Tunic_GetTintCMAB(TunicTintCMAB index) {
    ExtendedObjectContext* ctx = &sTunicTintObject.ctx;
    const s16 objectId = static_cast<s16>(ObjectId::OBJECT_TUNIC);
    if (!Object_IsLoaded(ctx, 0)) {
      Object_Clear(ctx);
      if (!Tunic_LoadObject(ctx, objectId, 0x40000000 | objectId)) {
        Object_Clear(ctx);
        return nullptr;
      }
    }
    return GAR_GetCMABByIndex(&ctx->status[0].archive, static_cast<u32>(index));
  }

  // --- CMB edits ----------------------------------------------------------------------------------------------------

  // The tunic materials' combiner stages, as the game has them and as they become. The stage lists have to stay as
  // they are (adding a stage or sharing a combiner makes a material fall back to texture * light), so the tint takes
  // the first stage and what it did moves into the rest, dropping the least needed part. "Light" is the fragment
  // lighting, "overlay" previous * constant 5 alpha + constant 5.
  enum class TunicStages : u8 {
    // light * texture, overlay -> tint, previous * light + constant 5.
    LightTexture,
    // (texture 0 + texture 1) * light, overlay -> tint, (previous + texture 1) * light.
    TwoTextures,
    // light * texture 0, light * texture 1 + previous, overlay -> tint, previous * light, light * texture 1 + previous.
    LightTextureSpecular,
    // specular light * texture 1, (previous + texture 0) * light, overlay
    // -> tint, specular light * texture 1 + previous, previous * light.
    GoronSpecular,
    // texture 0 * (1 - constant 4 alpha), constant 4 alpha * texture 1 + previous, previous * light, overlay: the
    // Deku's two tunic textures, blended. The blend also runs on the alpha, then the tint uses the previous stage's
    // alpha: -> blend, blend, (constant 0 + 1 - previous alpha) * previous, previous * light + constant 5.
    BlendedTextures,
    // specular light * shine mask, (previous + texture) * light, overlay: the Goron's guard, whose shine mask becomes
    // the tunic mask -> tint, previous * light, overlay.
    ShineMask,
    // The same first two stages, then a second texture blended in through the combiner buffer: the Goron's ball.
    BallBlend,
    // texture 2, (previous + texture 0) * light, then the ball's blend -> tint, (previous + texture 2) * light.
    BallEnvironment,
  };

  struct TunicMaterialEdit {
    u8 material;
    TunicStages stages;
  };

  struct TunicTextureEdit {
    const char* name;
    TextureFormatGL format;
    u8 bytesPerTexelTimesTwo;  // of the new format
  };

  struct TunicModelEdit {
    const char* model;
    TunicTextureEdit textures[3];
    TunicMaterialEdit materials[5];
    u8 textureCount;
    u8 materialCount;
  };

  static const TunicModelEdit TunicModelEdits[] = {
      {"link_child",
       {{"link_child_01", TextureFormatGL::ETC1A4, 2}},
       {{0, TunicStages::TwoTextures}, {1, TunicStages::LightTexture}},
       1,
       2},
      {"link_deknuts",
       {{"link_nuts_00", TextureFormatGL::ETC1A4, 2},
        {"link_nuts_f00", TextureFormatGL::ETC1A4, 2},
        {"link_nuts_f00a", TextureFormatGL::ETC1A4, 2}},
       {{1, TunicStages::LightTexture}, {6, TunicStages::BlendedTextures}},
       3,
       2},
      {"link_goron",
       {{"link_goron_00", TextureFormatGL::RGBA5551, 4}},
       {{0, TunicStages::TwoTextures}, {1, TunicStages::GoronSpecular}},
       1,
       2},
      {"link_zora",
       {{"link_zora_00", TextureFormatGL::ETC1A4, 2},
        {"link_zora_fin_0", TextureFormatGL::ETC1A4, 2},
        {"link_zora_shiel", TextureFormatGL::ETC1A4, 2}},
       {{0, TunicStages::TwoTextures},
        {1, TunicStages::LightTextureSpecular},
        {2, TunicStages::LightTexture},
        {6, TunicStages::LightTextureSpecular},
        {11, TunicStages::LightTextureSpecular}},
       3,
       5},
      {"link_goron_iwa",
       {{"link_goron_iwa_", TextureFormatGL::ETC1A4, 2}},
       {{0, TunicStages::BallEnvironment}, {1, TunicStages::BallBlend}},
       1,
       2},
      {"link_goron_guar", {{"link_goron_guar", TextureFormatGL::ETC1A4, 2}}, {{0, TunicStages::ShineMask}}, 1, 1},
  };
  static_assert(ARR_SIZE(TunicModelEdits) == (u32)TunicTintCMAB::GoronGuard + 1);

  static void Tunic_SetCombinerColor(Combiner& combiner, CombinerMode mode, CombinerScale scale, CombinerSrc source0,
                                     CombinerOp operand0, CombinerSrc source1, CombinerOp operand1, CombinerSrc source2,
                                     CombinerOp operand2) {
    combiner.combinerModeColor = mode;
    combiner.scaleColor = scale;
    combiner.sourceColor0 = source0;
    combiner.sourceColor1 = source1;
    combiner.sourceColor2 = source2;
    combiner.operandColor0 = operand0;
    combiner.operandColor1 = operand1;
    combiner.operandColor2 = operand2;
  }

  static void Tunic_SetVertexAlpha(Combiner& combiner) {
    combiner.combinerModeAlpha = CombinerMode::Replace;
    combiner.scaleAlpha = CombinerScale::_One;
    combiner.sourceAlpha0 = CombinerSrc::PrimaryColor;
    combiner.operandAlpha0 = CombinerOp::Alpha;
  }

  static void Tunic_SetTintStage(Combiner& combiner, CombinerSrc source) {
    Tunic_SetCombinerColor(combiner, CombinerMode::AddMult, CombinerScale::_One, CombinerSrc::ConstantCol,
                           CombinerOp::Color, source, CombinerOp::OneMinusAlpha, source, CombinerOp::Color);
    combiner.constantIndex = 0;
    Tunic_SetVertexAlpha(combiner);
  }

  static bool Tunic_StagesMatch(const game::cmb::Material& material, const Combiner* combiners, TunicStages stages) {
    static constexpr CombinerMode LightTexture[] = {CombinerMode::Modulate, CombinerMode::MultAdd};
    static constexpr CombinerMode TwoTextures[] = {CombinerMode::AddMult, CombinerMode::MultAdd};
    static constexpr CombinerMode LightTextureSpecular[] = {CombinerMode::Modulate, CombinerMode::MultAdd,
                                                            CombinerMode::MultAdd};
    static constexpr CombinerMode GoronSpecular[] = {CombinerMode::Modulate, CombinerMode::AddMult,
                                                     CombinerMode::MultAdd};
    static constexpr CombinerMode BlendedTextures[] = {CombinerMode::Modulate, CombinerMode::MultAdd,
                                                       CombinerMode::Modulate, CombinerMode::MultAdd};
    static constexpr CombinerMode ShineMask[] = {CombinerMode::Modulate, CombinerMode::AddMult, CombinerMode::MultAdd};
    static constexpr CombinerMode BallBlend[] = {CombinerMode::Modulate,    CombinerMode::AddMult,
                                                 CombinerMode::Modulate,    CombinerMode::AddMult,
                                                 CombinerMode::Interpolate, CombinerMode::MultAdd};
    static constexpr CombinerMode BallEnvironment[] = {CombinerMode::Replace,     CombinerMode::AddMult,
                                                       CombinerMode::Modulate,    CombinerMode::AddMult,
                                                       CombinerMode::Interpolate, CombinerMode::MultAdd};
    const CombinerMode* modes = nullptr;
    u32 count = 0;
    switch (stages) {
    case TunicStages::LightTexture:
      modes = LightTexture;
      count = ARR_SIZE(LightTexture);
      break;
    case TunicStages::TwoTextures:
      modes = TwoTextures;
      count = ARR_SIZE(TwoTextures);
      break;
    case TunicStages::LightTextureSpecular:
      modes = LightTextureSpecular;
      count = ARR_SIZE(LightTextureSpecular);
      break;
    case TunicStages::GoronSpecular:
      modes = GoronSpecular;
      count = ARR_SIZE(GoronSpecular);
      break;
    case TunicStages::BlendedTextures:
      modes = BlendedTextures;
      count = ARR_SIZE(BlendedTextures);
      break;
    case TunicStages::ShineMask:
      modes = ShineMask;
      count = ARR_SIZE(ShineMask);
      break;
    case TunicStages::BallBlend:
      modes = BallBlend;
      count = ARR_SIZE(BallBlend);
      break;
    case TunicStages::BallEnvironment:
      modes = BallEnvironment;
      count = ARR_SIZE(BallEnvironment);
      break;
    }
    if (material.texEnvStageUsed != count)
      return false;
    for (u32 i = 0; i < count; ++i) {
      if (material.texEnvStagesIndices[i] < 0 ||
          combiners[material.texEnvStagesIndices[i]].combinerModeColor != modes[i])
        return false;
    }
    return true;
  }

  static void Tunic_EditStages(const game::cmb::Material& material, Combiner* combiners, TunicStages stages) {
    Combiner* stage[6];
    for (u32 i = 0; i < material.texEnvStageUsed && i < ARR_SIZE(stage); ++i)
      stage[i] = &combiners[material.texEnvStagesIndices[i]];

    switch (stages) {
    case TunicStages::LightTexture:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::MultAdd, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::FragmentPrimaryColor, CombinerOp::Color,
                             CombinerSrc::ConstantCol, CombinerOp::Color);
      break;
    case TunicStages::TwoTextures:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::AddMult, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::Texture_1, CombinerOp::Color,
                             CombinerSrc::FragmentPrimaryColor, CombinerOp::Color);
      break;
    case TunicStages::LightTextureSpecular:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::Modulate, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::FragmentPrimaryColor, CombinerOp::Color,
                             CombinerSrc::ConstantCol, CombinerOp::Color);
      Tunic_SetCombinerColor(*stage[2], CombinerMode::MultAdd, CombinerScale::_One, CombinerSrc::FragmentPrimaryColor,
                             CombinerOp::Color, CombinerSrc::Texture_1, CombinerOp::Color, CombinerSrc::Previous,
                             CombinerOp::Color);
      break;
    case TunicStages::GoronSpecular:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::MultAdd, CombinerScale::_One, CombinerSrc::FragmentSecondaryColor,
                             CombinerOp::Color, CombinerSrc::Texture_1, CombinerOp::Color, CombinerSrc::Previous,
                             CombinerOp::Color);
      Tunic_SetCombinerColor(*stage[2], CombinerMode::Modulate, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::FragmentPrimaryColor, CombinerOp::Color,
                             CombinerSrc::ConstantCol, CombinerOp::Color);
      break;
    case TunicStages::BlendedTextures:
      // The blend's alpha: texture 0 alpha * (1 - constant alpha), then constant alpha * texture 1 alpha + previous.
      stage[0]->combinerModeAlpha = CombinerMode::Modulate;
      stage[0]->sourceAlpha0 = CombinerSrc::ConstantCol;
      stage[0]->operandAlpha0 = CombinerOp::OneMinusAlpha;
      stage[0]->sourceAlpha1 = CombinerSrc::Texture_0;
      stage[0]->operandAlpha1 = CombinerOp::Alpha;
      stage[1]->combinerModeAlpha = CombinerMode::MultAdd;
      stage[1]->sourceAlpha0 = CombinerSrc::ConstantCol;
      stage[1]->operandAlpha0 = CombinerOp::Alpha;
      stage[1]->sourceAlpha1 = CombinerSrc::Texture_1;
      stage[1]->operandAlpha1 = CombinerOp::Alpha;
      stage[1]->sourceAlpha2 = CombinerSrc::Previous;
      stage[1]->operandAlpha2 = CombinerOp::Alpha;
      Tunic_SetTintStage(*stage[2], CombinerSrc::Previous);
      Tunic_SetCombinerColor(*stage[3], CombinerMode::MultAdd, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::FragmentPrimaryColor, CombinerOp::Color,
                             CombinerSrc::ConstantCol, CombinerOp::Color);
      Tunic_SetVertexAlpha(*stage[3]);
      break;
    case TunicStages::ShineMask:
    case TunicStages::BallBlend:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::Modulate, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::FragmentPrimaryColor, CombinerOp::Color,
                             CombinerSrc::ConstantCol, CombinerOp::Color);
      break;
    case TunicStages::BallEnvironment:
      Tunic_SetTintStage(*stage[0], CombinerSrc::Texture_0);
      Tunic_SetCombinerColor(*stage[1], CombinerMode::AddMult, CombinerScale::_Two, CombinerSrc::Previous,
                             CombinerOp::Color, CombinerSrc::Texture_2, CombinerOp::Color,
                             CombinerSrc::FragmentPrimaryColor, CombinerOp::Color);
      break;
    }
  }

  static game::cmb::TextureEntry* Tunic_FindTexture(void* cmb, const char* name) {
    game::cmb::Tex* tex = game::cmb::Cmb_GetTex(cmb);
    if (tex == nullptr)
      return nullptr;
    for (s32 i = 0; i < tex->textureCount; ++i) {
      if (GameStrcmp(tex->entry[i].name, name) == 0)
        return &tex->entry[i];
    }
    return nullptr;
  }

  // Called for each CMB in Link's form object before its models are built. The raw CMB stays loaded, so this only edits
  // it once.
  void Tunic_EditCMB(void* cmb) {
    auto* head = static_cast<game::cmb::CMB_HEAD*>(cmb);
    const TunicModelEdit* edit = nullptr;
    u32 index = 0;
    for (; index < ARR_SIZE(TunicModelEdits); ++index) {
      if (GameStrcmp(head->name, TunicModelEdits[index].model) == 0) {
        edit = &TunicModelEdits[index];
        break;
      }
    }
    if (edit == nullptr)
      return;

    // Not a CMB this was made for, or already edited (the first stage no longer matches).
    game::cmb::TextureEntry* textures[3];
    for (u32 i = 0; i < edit->textureCount; ++i) {
      textures[i] = Tunic_FindTexture(cmb, edit->textures[i].name);
      if (textures[i] == nullptr)
        return;
    }
    game::cmb::Mats* mats = game::cmb::Cmb_GetMatsChunk(cmb);
    Combiner* combiners = game::cmb::Cmb_GetCombiners(cmb);
    for (u32 i = 0; i < edit->materialCount; ++i) {
      const TunicMaterialEdit& material = edit->materials[i];
      if (material.material >= mats->materialCount ||
          !Tunic_StagesMatch(mats->material[material.material], combiners, material.stages))
        return;
    }
    // Without the textures to swap in, the edited entries would show the original data in the new formats.
    if (Tunic_GetTintCMAB(static_cast<TunicTintCMAB>(index)) == nullptr)
      return;

    for (u32 i = 0; i < edit->materialCount; ++i) {
      const TunicMaterialEdit& material = edit->materials[i];
      Tunic_EditStages(mats->material[material.material], combiners, material.stages);
    }
    // The entries' own data is never shown, but it's uploaded when the model is built, so it has to stay inside the
    // CMB: a texture that grows past the end reads from the start of the texture data instead.
    const u32 textureDataSize = head->size - head->textureDataOffset;
    for (u32 i = 0; i < edit->textureCount; ++i) {
      game::cmb::TextureEntry* texture = textures[i];
      texture->format = edit->textures[i].format;
      texture->dataLength = texture->width * texture->height * edit->textures[i].bytesPerTexelTimesTwo / 2;
      if (texture->dataOffset + texture->dataLength > textureDataSize)
        texture->dataOffset = 0;
    }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: %s edited for the tunic tint\n", __func__, edit->model);
#endif
  }

  struct CmabKeys {
    u16 interpolation;
    u16 count;
    u16 firstFrame;
    u16 lastFrame;
    f32 scale;
    f32 bias;
    struct {
      u32 frame;
      f32 value;
    } keys[];
  };

  struct CmabMaterialAnim {
    char magic[4];
    u32 type;  // 2: texture swap, 4: constant colour
    u32 material;
    u32 index;              // Mapper or constant colour
    u16 channelOffsets[4];  // Constant colour: R, G, B, A keys from here, 0 if not animated.
  };

  // Writes the colour into the keys of each constant colour animation; they're read every frame.
  static void Tunic_SetTintColor(void* cmabMan, u32 rgb) {
    u8* cmab = *reinterpret_cast<u8**>(static_cast<u8*>(cmabMan) + 4);
    u8* anim = cmab + *reinterpret_cast<u32*>(cmab + 0x14);
    u8* mads = anim + *reinterpret_cast<u32*>(anim + 0xC);
    const u32 count = *reinterpret_cast<u32*>(mads + 4);
    for (u32 i = 0; i < count; ++i) {
      auto* materialAnim = reinterpret_cast<CmabMaterialAnim*>(mads + reinterpret_cast<u32*>(mads + 8)[i]);
      if (materialAnim->type != 4)
        continue;
      for (u32 channel = 0; channel < 3; ++channel) {
        if (materialAnim->channelOffsets[channel] == 0)
          continue;
        auto* keys =
            reinterpret_cast<CmabKeys*>(reinterpret_cast<u8*>(materialAnim) + materialAnim->channelOffsets[channel]);
        const f32 value = ((rgb >> (16 - channel * 8)) & 0xFF) / 255.0f;
        for (u32 key = 0; key < keys->count; ++key)
          keys->keys[key].value = value;
      }
    }
  }

  static PlayerFaceTexAnims* Tunic_GetFaceTexAnims(game::act::Player* player) {
    return reinterpret_cast<PlayerFaceTexAnims*>(reinterpret_cast<u8*>(player) + PLAYER_FACE_TEXANIMS);
  }

  // The Goron's ball and guard: their own models' animation, unused otherwise.
  static void Tunic_AttachToModel(game::act::Player* player, u32 modelOffset, TunicTintCMAB index, u32 rgb) {
    auto* model = *reinterpret_cast<game::act::SkeletonAnimationModel**>(reinterpret_cast<u8*>(player) + modelOffset);
    void* cmab = Tunic_GetTintCMAB(index);
    if (model == nullptr || model->texAnim == nullptr || cmab == nullptr)
      return;
    TexAnim_Attach(model->texAnim, cmab);
    model->texAnim->anim_speed = 0.0f;
    model->texAnim->anim_mode = 0;
    model->texAnim->cur_frame = 0.0f;
    Tunic_SetTintColor(cmab, rgb);
  }

  static void Tunic_Attach(game::act::Player* player, TunicForm form) {
    PlayerFaceTexAnims* faces = Tunic_GetFaceTexAnims(player);
    auto* body = *reinterpret_cast<u8**>(reinterpret_cast<u8*>(player) + PLAYER_BODY_MODEL);
    void* cmab = Tunic_GetTintCMAB(static_cast<TunicTintCMAB>(form));
    if (body == nullptr || cmab == nullptr)
      return;

    if (!faces->active[TUNIC_FACE_SLOT]) {
      game::act::SA_TextureAnimation* texAnim = &faces->slots[TUNIC_FACE_SLOT];
      TexAnim_Construct(texAnim);
      // Binds the animation to the body model, the same value the eye and mouth slots get.
      texAnim->field_08 = *reinterpret_cast<game::act::TexAnim_Unk_10**>(body + 0x9C);
      TexAnim_Attach(texAnim, cmab);
      texAnim->anim_speed = 0.0f;
      texAnim->anim_mode = 0;
      texAnim->cur_frame = 0.0f;
      faces->active[TUNIC_FACE_SLOT] = 1;
    }
    const u32 rgb = Tunic_GetTintColor(form);
    Tunic_SetTintColor(cmab, rgb);

    if (form == TunicForm::Goron) {
      Tunic_AttachToModel(player, PLAYER_GORON_BALL_MODEL, TunicTintCMAB::GoronBall, rgb);
      Tunic_AttachToModel(player, PLAYER_GORON_GUARD_MODEL, TunicTintCMAB::GoronGuard, rgb);
    }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: %s tunic colour %06X attached\n", __func__, tunicFormNames[(u32)form], rgb);
#endif
  }

  void Tunic_AttachTexAnim(game::act::Player* player) {
    TunicForm form;
    if (player->id == game::act::Id::Player && Tunic_GetForm(player->active_form, &form))
      Tunic_Attach(player, form);
  }

  // With the other objects on scene change.
  void Tunic_ClearObjects() {
    Object_Clear(&sTunicTintObject.ctx);
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
