#include "rnd/custom_models.h"
#include <string.h>

#define EDIT_BYTE(offset_, val_) (BASE_[offset_] = val_)
#define EDIT_U32(offset_, val_)                                                                                        \
  (EDIT_BYTE((offset_) + 0, (val_) >> 24), EDIT_BYTE((offset_) + 1, (val_) >> 16),                                     \
   EDIT_BYTE((offset_) + 2, (val_) >> 8), EDIT_BYTE((offset_) + 3, (val_)))
namespace rnd {
  u32 SmallKeyData[][3] = {
      {0x00000000, 0x00800000, 0x00CC0000},  // Woodfall
      {0xFFFFFF00, 0xFFFFFF00, 0x7F7FFFFF},  // Snowhead
      {0x00000000, 0x0000DA00, 0x0000FFFF},  // Great Bay
      {0x00000000, 0x80550000, 0xFFAA0000}   // Stone Tower
  };

  static void CustomModel_ApplyColorEditsToSmallKey(void* smallKeyCMB, s32 keyType) {
    char* BASE_ = (char*)smallKeyCMB;
    const u32* color = SmallKeyData[keyType];

    EDIT_U32(0x16C, color[0]);  // Emission
    EDIT_U32(0x170, color[1]);  // Ambient
    EDIT_U32(0x174, color[2]);  // Diffuse
  }

  static void CustomModels_SetOcarinaToRGBA565(void* ocarinaCMB) {
    char* BASE_ = (char*)ocarinaCMB;
    //  TODO: When we get the custom gar.lzs or .gar then we can modify these.
    EDIT_BYTE(0x16C, SmallKeyData[3][0]); // Emission
    EDIT_BYTE(0x170, SmallKeyData[3][1]); // Ambient
    EDIT_BYTE(0x174, SmallKeyData[3][2]); // Diffuse
    // EDIT_BYTE(0x538, 0x5A);
  }

  void CustomModels_EditItemCMB(void* ZARBuf, u16 objectId, s8 special) {
    void* cmb;

    switch ((ObjectId)objectId) {
    case ObjectId::OBJECT_CUSTOM_SMALL_KEY:
      cmb = ((char*)ZARBuf) + 0x84;  // 0x84 bytes, view zeld_gi_key.gar.lzs to see offset for cmb.
      CustomModel_ApplyColorEditsToSmallKey(cmb, special);
      break;
    case ObjectId::OBJECT_CUSTOM_SONGS:
      cmb = ((char*)ZARBuf) + 0x8C;
      CustomModel_ApplyColorEditsToSmallKey(cmb, 2);
      break;
    case ObjectId::OBJECT_CUSTOM_ASSETS:
      break;
    }
  }

  void CustomModels_ApplyItemCMAB(game::act::SkeletonAnimationModel* model, u16 objectId, s8 special) {
    void* cmabMan;

    switch((ObjectId)objectId) {
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
