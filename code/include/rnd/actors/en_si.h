#ifndef _RND_ACTORS_EN_SI_H_
#define _RND_ACTORS_EN_SI_H_

#include "game/actor.h"
#include "rnd/models.h"
#include "rnd/item_override.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  extern "C" {
  void En_Si_Init(game::act::Actor* actor, game::GlobalContext* gctx);

  void En_Si_Draw(game::act::Actor* actor, game::GlobalContext* gctx);

  void En_Si_Destroy(game::act::Actor* self, game::GlobalContext* gctx);
  }
}  // namespace rnd
#endif