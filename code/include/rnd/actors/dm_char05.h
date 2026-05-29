#pragma once

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  struct Dm_Char05 : public game::act::Actor {
    game::as::ActorUtil actor_util;
    void* skelAnimeModel;
    void* calc_fn;
    u16 field_28c;
    u8 field_28e;
    u8 object_idx;
    float field_290[4];
    u8 gap_2a0[24];
  };
  static_assert(sizeof(Dm_Char05) == 0x2B8);

  enum DmChar05Param {
    DMCHAR05_GORON_MASK = 0x0,
    DMCHAR05_ZORA_MASK = 0x1,
    DMCHAR05_GIBDO_MASK = 0x2,
    DMCHAR05_MAJORA_MASK = 0x3,
    DMCHAR05_COUPLE_MASK = 0x4,
    DMCHAR05_BOMBERS_NOTEBOOK = 0xD,
  };

  void Dm_Char05_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  void Dm_Char05_Destroy(game::act::Actor* self, game::GlobalContext* gctx);
}  // namespace rnd
