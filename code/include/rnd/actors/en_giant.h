#pragma once

#include "rnd/savefile.h"
#include "rnd/settings.h"
namespace rnd {
  extern "C" {
  void En_Giant_ShouldDrawGiant(game::act::Actor*);
  bool En_Giant_KillAfterCutscene(game::act::Actor*);
  u32 En_Giant_ChamberCutsceneOrder(u32 boss);
  }
}  // namespace rnd
