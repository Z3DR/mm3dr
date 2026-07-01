#pragma once

#include "game/actor.h"
#include "rnd/item_override.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  bool En_Si_IsTokenCollectedAndNonRepeatable(game::act::Actor*, game::SceneId, ItemOverride*);
  void En_Si_SetSkullCollected(u16, game::SceneId, game::act::Type);
  extern "C" {
  void En_Si_Init(game::act::Actor* actor, game::GlobalContext* gctx);

  void En_Si_Draw(game::act::Actor* actor, game::GlobalContext* gctx);

  void En_Si_Destroy(game::act::Actor* self, game::GlobalContext* gctx);
  }
}  // namespace rnd
