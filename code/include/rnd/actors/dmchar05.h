#ifndef _RND_ACTORS_DMCHAR05_H_
#define _RND_ACTORS_DMCHAR05_H_

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  extern "C" {
  void SpawnDmChar05Model(game::act::Actor*);
  u8 DrawDmChar05Model(game::act::Actor*);
  void DmChar05_rDestroy(game::act::Actor*, game::GlobalContext*);
  }
} // namespace rnd
#endif