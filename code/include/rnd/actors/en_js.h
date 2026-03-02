#ifndef _RND_ACTOR_EN_JS_H_
#define _RND_ACTOR_EN_JS_H_
#include "game/common_data.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"
namespace rnd {
  extern "C" {
  u16 En_Js_CurrentMasksInInventory();
  u16 En_Js_CheckVictoryRequirements();
  }
}  // namespace rnd

#endif