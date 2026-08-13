#pragma once

#include "game/actor.h"
#include "rnd/models.h"
#include "rnd/settings.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  struct En_Pm : public game::act::Actor {
    game::as::ActorUtil* actor_util;
    u8 gap_1fc[1696];
    game::act::SkeletonAnimationModel* skel_anime_01;
    game::act::SkeletonAnimationModel* skel_anime_02;
    game::act::SkeletonAnimationModel* skel_anime_03;
    u32 field_8A8;
    u8 gap_8AC[492];
    void* next_fn;
    u8 gap_A9C[4];
    float field_AA0;
    u8 gap_AA4[84];
    int field_AF8;
    u8 gap_AFC[76];
    z3dVec3f vec_b48;
    u8 gap_b54[12];
    u32 field_B60;
    z3dVec3f field_B64;
    float field_B70;
    int field_B74;
    int field_B78;
    int field_B7C;
    int pm_schedule_position;
    u8 schedule_result;
    u8 gap_b85[59];
    game::act::SkeletonAnimationModel* skel_anime_04;
    u8 gap_bc4[16];
    u16 field_BD4;
    u16 time_tracker;
    u8 gap_BD8[4];
    u16 time_speed;
    u8 gap_BDE[14];
    u32 field_BEC;
    u8 gap_BF0[4];
    u32 field_BF4;
  };
  static_assert(sizeof(En_Pm) == 0xBF8);
  void En_Pm_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  extern "C" s32 En_Pm_OverrideModelHatDraw(game::act::SkeletonAnimationModel*, game::act::Actor*);
  void En_Pm_Destroy(game::act::Actor* actor, game::GlobalContext* gctx);

}  // namespace rnd
