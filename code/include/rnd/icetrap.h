#pragma once

#include "common/advanced_context.h"
#include "game/common_data.h"
#include "game/player.h"
#include "game/sound.h"

namespace rnd {
  void IceTrap_Push();
  void IceTrap_Give();
  bool IceTrap_IsPending();
  /// True while a timed trap (jinx or inverted controls) is running. Pending traps wait until it ends.
  bool IceTrap_IsTimedActive();
  /// Runs once per game tick. Counts down the inverted controls and applies them to the pad state.
  void IceTrap_Update();
  /// Stops a running inverted-controls trap, e.g. on loading a save.
  void IceTrap_Reset();
}  // namespace rnd
