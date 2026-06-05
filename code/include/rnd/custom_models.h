#pragma once

#include "common/types.h"
#include "rnd/models.h"
namespace rnd {
  enum class ObjectId : u16 { OBJECT_CUSTOM_SMALL_KEY = 0x1B, OBJECT_CUSTOM_SONGS = 0x1E, OBJECT_CUSTOM_ASSETS = 0xCB };
  void CustomModel_EditTitleScreenLogo(void* titleScreenGAR);
  void CustomModels_EditItemCMB(void*, u16, s8);
}  // namespace rnd
