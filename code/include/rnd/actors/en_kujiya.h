#pragma once

#include "game/actor.h"
#include "game/common_data.h"
#include "rnd/item_override.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif

namespace rnd {
  struct En_Kujiya : public game::act::Actor {
    void* next_fn;
    u16 frame_timer;
    u16 text_id;
    game::act::SkeletonAnimationModel* skel_anime;
  };
  static_assert(sizeof(En_Kujiya) == 0x204);
  ItemOverride_Key EnKujiya_GetItemKeyBasedOnDay(s16);
  extern "C" void EnKujiya_SetLotteryRevealed();
}  // namespace rnd
