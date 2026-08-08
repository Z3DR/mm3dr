#pragma once

#include "common/types.h"
#include "rnd/models.h"
#include "shared_defs.h"
#include "z3d/z3DVec.h"

namespace rnd {
  enum class ObjectId : u16 { OBJECT_CUSTOM_SMALL_KEY = 0x1B, OBJECT_CUSTOM_SONGS = 0x1E, OBJECT_CUSTOM_ASSETS = 0xCB };
  void CustomModels_EditItemCMB(void*, u16, s8);
  void CustomModels_ApplyItemCMAB(game::act::SkeletonAnimationModel*, u16, s8);
  bool CustomModels_ComputeItemAabb(void* ZARBuf, z3dVec3f* outMin, z3dVec3f* outMax);
}  // namespace rnd
