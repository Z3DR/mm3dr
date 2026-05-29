#pragma once

#include "game/actor.h"
#include "game/as.h"
#include "game/collision.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  struct En_Elforg : public game::act::Actor {
    game::as::ActorUtil actor_util;
    u8 gap_284[336];
    z3d_nn_math_MTX34 some_mtx;
    u8 gap_404[528];
    game::CollisionInfoCylinder collider;
    u8 gap_664[8];
    game::act::Actor* attached_enemy;
    u16 strayFairyFlags;
    s16 direction;
    u8 gap_674[4];
    s32 timer;
    float secondaryTimer;
    float targetSpeedXZ;
    float targetDistanceFromHome;
    void* next_fn;
    u32 field_68C;
  };
  static_assert(sizeof(En_Elforg) == 0x690);
  int En_Elforg_getFairyIndex(game::SceneId);
  void En_Elforg_Init(game::act::Actor*, game::GlobalContext*);
  extern "C" {
  s32 En_Elforg_OverrideModelDraw(game::act::sa_unk_d4*, game::act::Actor*);
  void En_Elforg_UpdateExtFairyBits(game::act::Actor*, game::GlobalContext*);
  bool En_Elforg_Chest_IsFairyObtained(u32 param, game::GlobalContext*);
  }
  void En_Elforg_Destroy(game::act::Actor*, game::GlobalContext*);

}  // namespace rnd
