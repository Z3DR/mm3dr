#pragma once

#include "game/actor.h"
#include "game/context.h"
#include "rnd/custom_models.h"
#include "rnd/models.h"
#include "rnd/objects.h"
#include "z3d/z3DVec.h"
namespace rnd {
  struct En_Mag : public game::act::Actor {
    u8 gap_1f8[36];
    game::act::SkeletonAnimationModel* skelAnime;
    game::act::SkeletonAnimationModel* field_220;
    game::act::SkeletonAnimationModel* field_224;
    game::act::SkeletonAnimationModel* field_228;
    game::act::SkeletonAnimationModel* field_22c;
    u8 gap_230[54];
    u8 field_266;
    u8 gap_267[13];
    u32 field_274;
    u32 field_278;
    u32 field_27c;
    u8 gap_280[4];
    u16 field_284;
    u16 field_286;
    u16 field_288;
    u16 field_28a;
    u32 field_28c;
    u32 field_290;
  };
  static_assert(sizeof(En_Mag) == 0x294);

  void En_Mag_rInit(game::act::Actor*, game::GlobalContext*);
}  // namespace rnd
