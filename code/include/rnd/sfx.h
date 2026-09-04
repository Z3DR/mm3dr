#pragma once
#include "common/types.h"

#define SFX_BASE 0x01000000
#define SFX_COUNT 0xA36

namespace rnd {
  // Written by the generator; zero means "no override for this slot".
  extern "C" u16 rSfxOverrides[SFX_COUNT];

  // Returns 0 to suppress the effect entirely, otherwise the id to play.
  extern "C" u32 Sfx_Filter(u32 id);
}  // namespace rnd
