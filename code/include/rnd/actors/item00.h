#ifndef _RND_ACTORS_ITEM00_H_
#define _RND_ACTORS_ITEM00_H_

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
  void SpawnItem00Model(game::act::Actor*);
  u8 DrawItem00Model(game::act::Actor*);
  }
}  // namespace rnd
#endif