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
  struct Fish_Heart : public game::act::Actor {
    void* next_fn;
    u8 gap_1fc[8];
    float scale;
    u8 gap_208[88];
    game::act::sa_unk_d4* skelAnimeModel;
    u8 field_264;
    u8 gap_265[3];
  };
  static_assert(sizeof(Fish_Heart) == 0x268);
  void Fish_Heart_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  extern "C" s32 Fish_Heart_OverrideModelDraw(game::act::sa_unk_d4*, game::act::Actor*);
  void Fish_Heart_Destroy(game::act::Actor* actor, game::GlobalContext* gctx);

}  // namespace rnd
