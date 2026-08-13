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
  struct Obj_Moon_Stone : public game::act::Actor {
    u8 gap_1f8[88];
    void* next_fn;
    u32 field_254;
    game::act::SkeletonAnimationModel* skelAnimeModelOne;
    game::act::SkeletonAnimationModel* skelAnimeModelTwo;
    game::act::SkeletonAnimationModel* skelAnimeModelThree;
  };
  static_assert(sizeof(Obj_Moon_Stone) == 0x264);
  void Obj_Moon_Stone_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  extern "C" s32 Obj_Moon_Stone_OverrideModelDraw(game::act::SkeletonAnimationModel*, game::act::Actor*);
  void Obj_Moon_Stone_Destroy(game::act::Actor* actor, game::GlobalContext* gctx);

}  // namespace rnd
