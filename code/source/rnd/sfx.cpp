#include "rnd/sfx.h"

#include "rnd/savefile.h"
#include "rnd/settings.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#include "z3d/z3DVec.h"  // ARR_SIZE
#endif

namespace rnd {
  extern "C" {
  u16 rSfxOverrides[SFX_COUNT] = {0};

  // One call per sound effect, shared by both entry points. Muting is checked first and applies to
  // every id, including ones outside the table, so behaviour matches the standalone mute gates this
  // replaced.
  u32 Sfx_Filter(u32 id) {
    if (gExtSaveData.options.muteSoundEffects != 0) {
      return 0;  // suppress
    }
    const u32 i = id - SFX_BASE;
    if (i >= SFX_COUNT) {
      return id;  // unsigned: catches under+overflow
    }
    if (gExtSaveData.options.shuffleSFX == (u8)ShuffleSFXSetting::SHUFFLESFX_OFF) {
      return id;
    }
    const u16 repl = rSfxOverrides[i];

    if (repl == 0) {
      return id;  // generator wrote nothing for this slot
    }
    return SFX_BASE + repl;
  }
  }
}  // namespace rnd
