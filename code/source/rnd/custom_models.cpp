#include "rnd/custom_models.h"
#include <string.h>
#include "game/cmb.h"
#include "game/resarchiveheader.h"

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

  void CustomModels_SpawnTexAnim(game::act::SA_TextureAnimation* texAnim, void* cmabMan, float specialFrame) {
    if (texAnim == nullptr || cmabMan == nullptr)
      return;
    TexAnim_Spawn(texAnim, cmabMan);
    texAnim->anim_speed = 0.00f;
    texAnim->anim_mode = 0;
    texAnim->cur_frame = specialFrame;
    return;
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
    default:
      break;
    }
  }

  // The model's bounds from its root qtrs node, scaled by the root bone's rest scale (Garo's Mask is the one item
  // whose root bone scales the mesh, and its stored bounds leave that out).
  bool CustomModels_GetItemBounds(void* ZARBuf, z3dVec3f* outMin, z3dVec3f* outMax) {
    void* cmb = game::ResArchive_GetFileByType(ZARBuf, game::ResFileType::CMB);
    if (cmb == NULL || !game::cmb::Cmb_GetBounds(cmb, outMin, outMax))
      return false;

    game::cmb::CMB_HEAD* head = (game::cmb::CMB_HEAD*)cmb;
    if (head->sklOffset == 0)
      return true;
    game::cmb::Skeleton* skl = (game::cmb::Skeleton*)((u8*)cmb + head->sklOffset);
    if (skl->boneCount == 0)
      return true;

    const z3dVec3f& scale = skl->bone[0].scale;
    f32* mins[3] = {&outMin->x, &outMin->y, &outMin->z};
    f32* maxs[3] = {&outMax->x, &outMax->y, &outMax->z};
    f32 scales[3] = {scale.x, scale.y, scale.z};
    for (u32 axis = 0; axis < 3; ++axis) {
      f32 lo = *mins[axis] * scales[axis];
      f32 hi = *maxs[axis] * scales[axis];
      *mins[axis] = lo < hi ? lo : hi;
      *maxs[axis] = lo < hi ? hi : lo;
    }
    return true;
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
      CustomModels_SpawnTexAnim(model->texAnim, cmabMan, special);
      break;
    default:
      break;
    }
  }

}  // namespace rnd
