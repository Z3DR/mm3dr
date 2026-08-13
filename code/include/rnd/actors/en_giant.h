#pragma once

#include "rnd/settings.h"
namespace rnd {
  extern "C" {
  void En_Giant_ShouldDrawGiant(game::act::Actor*);
  bool En_Giant_KillAfterCutscene(game::act::Actor*);
  }
}  // namespace rnd
