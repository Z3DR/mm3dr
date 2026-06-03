#include "rnd/custom_models.h"
#include <string.h>
#include "z3d/z3DVec.h"

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

  void CustomModels_EditItemCMB(void* ZARBuf, u16 objectId, s8 special) {
    void* cmb;

    switch (objectId) {
    case OBJECT_CUSTOM_SMALL_KEY:
      cmb = ((char*)ZARBuf) + 0x84;  // 0x84 bytes, view zeld_gi_key.gar.lzs to see offset for cmb.
      CustomModel_ApplyColorEditsToSmallKey(cmb, special);
      break;
    }
  }

  // TODO: Change this for MM3D.
  void CustomModel_EditTitleScreenLogo(void* titleScreenZAR) {
    char* BASE_ = (char*)titleScreenZAR;

    // copy_nintendo.cmb:
    EDIT_BYTE(0x4F3, 0x40);
    EDIT_BYTE(0x5905, 0x00);
    EDIT_BYTE(0x5906, 0x01);  // Change texture dataLength
    EDIT_BYTE(0x590A, 0x01);  // IsETC1 = true
    EDIT_BYTE(0x590D, 0x02);  // Width  = 512
    EDIT_BYTE(0x590E, 0x80);  // Height = 128
    EDIT_BYTE(0x5910, 0x5B);  // ETC1a4
    // Edit positionOffset of each shape
    EDIT_BYTE(0x597A, 0x80);
    EDIT_BYTE(0x597B, 0x3F);
    EDIT_BYTE(0x597C, 0x33);
    EDIT_BYTE(0x597D, 0x33);
    EDIT_BYTE(0x597E, 0x33);
    EDIT_BYTE(0x597F, 0x40);
    // Edit vertices/UVs
    EDIT_BYTE(0x5AFE, 0xA0);
    EDIT_BYTE(0x5B02, 0xA0);
    EDIT_BYTE(0x5B0A, 0xA0);
    EDIT_BYTE(0x5B0E, 0xA0);
    EDIT_BYTE(0x5B16, 0xA0);
    EDIT_BYTE(0x5B1A, 0xA0);
    EDIT_BYTE(0x5B22, 0xA0);
    EDIT_BYTE(0x5B26, 0xA0);

    // title_logo_us.cmb: Edit positionOffset of each shape
    EDIT_BYTE(0x36BF3, 0x40);
    EDIT_BYTE(0x36D33, 0x40);
    EDIT_BYTE(0x36E73, 0x40);
    EDIT_BYTE(0x36FB3, 0x40);
    EDIT_BYTE(0x370F3, 0x40);
    EDIT_BYTE(0x37233, 0x40);
    EDIT_BYTE(0x37373, 0x40);
    EDIT_BYTE(0x374B3, 0x40);
    EDIT_BYTE(0x375F3, 0x40);
    EDIT_BYTE(0x37733, 0x40);
    EDIT_BYTE(0x37873, 0x40);
    EDIT_BYTE(0x379B3, 0x40);
    EDIT_BYTE(0x37AF3, 0x40);
    EDIT_BYTE(0x37C33, 0x40);
    EDIT_BYTE(0x37D73, 0x40);
    EDIT_BYTE(0x37EB3, 0x40);
    EDIT_BYTE(0x37FF3, 0x40);
    EDIT_BYTE(0x38133, 0x40);
    EDIT_BYTE(0x38273, 0x40);
    EDIT_BYTE(0x383B3, 0x40);
    EDIT_BYTE(0x384F3, 0x40);
    EDIT_BYTE(0x38633, 0x40);

    // g_title_fire.cmab
    EDIT_BYTE(0x5E570, 0x01);  // Change keyframe count to 1 so we only have to change one keyframe
    EDIT_BYTE(0x5E580, 0x0A);
    EDIT_BYTE(0x5E581, 0xD7);
    EDIT_BYTE(0x5E582, 0x23);
    EDIT_BYTE(0x5E583, 0x3D);  // Red to 0.04
    EDIT_BYTE(0x5E660, 0x01);
    EDIT_BYTE(0x5E670, 0x91);
    EDIT_BYTE(0x5E671, 0xED);
    EDIT_BYTE(0x5E672, 0x5C);
    EDIT_BYTE(0x5E673, 0x3F);  // Green 0.863

    // g_title_fire_ura.cmab
    EDIT_BYTE(0x5EA80, 0x01);
    EDIT_BYTE(0x5EA90, 0x0A);
    EDIT_BYTE(0x5E581, 0xD7);
    EDIT_BYTE(0x5E582, 0x23);
    EDIT_BYTE(0x5E583, 0x3D);
    EDIT_BYTE(0x5EB70, 0x01);
    EDIT_BYTE(0x5EB80, 0x91);
    EDIT_BYTE(0x5EB81, 0xED);
    EDIT_BYTE(0x5EB82, 0x5C);
    EDIT_BYTE(0x5EB83, 0x3F);
  }
}  // namespace rnd
