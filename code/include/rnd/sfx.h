#pragma once
#include "common/types.h"

#define SFX_BASE 0x01000000
#define SFX_COUNT 0xA36

namespace rnd {
  // Link's footsteps and voice are the only sounds of his that move, each only among its own group in
  // both tables. The bitsets mark them, so the in-game toggles can leave them vanilla. heldShuffled marks
  // held sounds the generator shuffles only among other held ones (Link's floor slides), which the
  // held-sound guard must not pin.
  struct SfxData {
    u16 categorical[SFX_COUNT];
    u16 chaos[SFX_COUNT];
    u32 linkFootsteps[(SFX_COUNT + 31) / 32];
    u32 linkVoice[(SFX_COUNT + 31) / 32];
    u32 heldShuffled[(SFX_COUNT + 31) / 32];
  };
  extern "C" SfxData rSfxData;

  extern "C" u32 Sfx_Filter(u32 id);

  void Sfx_Update();
}  // namespace rnd
