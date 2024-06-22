#ifndef _RND_ACTORS_ITEM_B_HEART_H_
#define _RND_ACTORS_ITEM_B_HEART_H_

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  void ItemBHeart_Init(game::act::Actor*, game::GlobalContext*);
  void ItemBHeart_Draw(game::act::Actor*, game::GlobalContext*);
  void ItemBHeart_Destroy(game::act::Actor*, game::GlobalContext*);
}  // namespace rnd
#endif