#pragma once
#include "common/types.h"

#define SFX_BASE 0x01000000
#define SFX_COUNT 0xA36

namespace rnd {
  // Returns 0 to suppress the effect entirely, otherwise the id to play.
  extern "C" u32 Sfx_Filter(u32 id);

  // Builds the table for the selected Shuffle Sound Effects mode the first time it is chosen.
  void Sfx_Update();
}  // namespace rnd
