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
  struct Dm_Char03 : public game::act::Actor {
    game::as::ActorUtil actor_util;
    void* field_284;
    u16 field_288;
    u8 field_28a;
    u8 object_id;
    z3dVec3f some_pos;
    void* skelAnimeModel;
  };
  static_assert(sizeof(Dm_Char03) == 0x29C);
  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  extern "C" s32 Dm_Char03_OverrideModelDraw(game::act::SkeletonAnimationModel*, game::act::Actor*);
  extern "C" s32 Dm_Char03_OverrideAnimatedModelDraw(Dm_Char03*);
  void Dm_Char03_Destroy(game::act::Actor* actor, game::GlobalContext* gctx);

}  // namespace rnd
