#ifndef _RND_ACTORS_EN_DOOR_1_H_
#define _RND_ACTORS_EN_DOOR_1_H_

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  void Dm_Hina_Init(game::act::Actor*, game::GlobalContext*);
  void Dm_Hina_Draw(game::act::Actor*, game::GlobalContext*);
  void Dm_Hina_Draw(game::act::Actor*, game::GlobalContext*);
  void Dm_Hina_Destroy(game::act::Actor*, game::GlobalContext*);
}  // namespace rnd
#endif