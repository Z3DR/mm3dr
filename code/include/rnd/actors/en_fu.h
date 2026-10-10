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
  struct En_Fu : public game::act::Actor {
    u8 gap_1f8[268];
    void* next_fn;
    u8 gap_308[3438];
    u16 text_id;
  };

  ItemOverride_Key En_Fu_GetItemKeyBasedOnDay(game::act::Actor*, s16);
}  // namespace rnd