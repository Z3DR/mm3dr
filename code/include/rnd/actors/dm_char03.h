#ifndef _RND_ACTORS_DM_CHAR03_H_
#define _RND_ACTORS_DM_CHAR03_H_

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
    game::as::ActorUtil* actor_util;
    u8 gap_1fc[136];
    void* field_284;
    u16 field_288;
    u8 field_28a;
    u8 object_id;
    z3dVec3f some_pos;
    void* skelAnimeModel;
  };
  static_assert(sizeof(Dm_Char03) == 0x29C);

  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx);
  void Dm_Char03_Draw(game::act::Actor* actor, game::GlobalContext* gctx);

}  // namespace rnd
#endif