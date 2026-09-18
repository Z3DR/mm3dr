#include "rnd/sfx.h"

#include "rnd/savefile.h"
#include "rnd/settings.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#endif

namespace rnd {
  extern "C" {
  SfxData rSfxData = {};
  }

  constexpr u8 kHeldFrames = 3;
  static u16 sFrame;
  static u16 sLastPlayedFrame[SFX_COUNT];
  static u8 sPlayedRun[SFX_COUNT];
  static u32 sHeld[(SFX_COUNT + 31) / 32];

  static bool Sfx_IsHeld(u32 index) {
    return (sHeld[index / 32] >> (index % 32)) & 1;
  }

  // Counts the consecutive frames this sound has been played on, and pins it once it looks held.
  static void Sfx_TrackPlay(u32 index) {
    const u16 gap = sFrame - sLastPlayedFrame[index];
    if (gap == 1) {
      if (sPlayedRun[index] < kHeldFrames)
        sPlayedRun[index]++;
    } else if (gap != 0 || sPlayedRun[index] == 0) {
      sPlayedRun[index] = 1;
    }
    sLastPlayedFrame[index] = sFrame;
    if (sPlayedRun[index] >= kHeldFrames && !Sfx_IsHeld(index)) {
      sHeld[index / 32] |= 1u << (index % 32);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      util::Print("SFX held %03X, no longer shuffled\n", index);
#endif
    }
  }

  constexpr u8 kTableCount = 2;

  static const u16* Sfx_Table(s32 table) {
    return table == 0 ? rSfxData.categorical : rSfxData.chaos;
  }

  static s32 Sfx_TableForMode(u8 mode) {
    if (mode == (u8)ShuffleSFXSetting::SHUFFLESFX_CATEGORICAL)
      return 0;
    if (mode == (u8)ShuffleSFXSetting::SHUFFLESFX_CHAOS)
      return 1;
    return -1;
  }

  void Sfx_Update() {
    sFrame++;
  }

  static bool Sfx_InSet(const u32* set, u32 index) {
    return (set[index / 32] >> (index % 32)) & 1;
  }

  // Whether the in-game toggles leave this sound alone.
  static bool Sfx_IsExcludedByOption(u32 index) {
    return (gExtSaveData.sfxOptions.shuffleFootsteps == 0 && Sfx_InSet(rSfxData.linkFootsteps, index)) ||
           (gExtSaveData.sfxOptions.shuffleLinkVoice == 0 && Sfx_InSet(rSfxData.linkVoice, index));
  }

  static u32 Sfx_Remap(s32 table, u32 id) {
    const u32 i = id - SFX_BASE;
    if (table < 0 || i >= SFX_COUNT) {
      return id;
    }
    const u16 replacement = Sfx_Table(table)[i];
    if (replacement == 0 || replacement >= SFX_COUNT) {
      return id;
    }
    return SFX_BASE + replacement;
  }

  extern "C" {
  void Sfx_StopByIdVanilla(void* mgr, u32 id);
  s32 Sfx_IsPlayingByIdVanilla(void* mgr, u32 id);

  // One call per sound effect, shared by both entry points. Muting is checked first and applies to
  // every id, including ones outside the table, so behaviour matches the standalone mute gates this
  // replaced.
  u32 Sfx_Filter(u32 id) {
    if (gExtSaveData.options.muteSoundEffects != 0) {
      return 0;  // suppress
    }
    const u32 index = id - SFX_BASE;
    if (index < SFX_COUNT) {
      if (!Sfx_InSet(rSfxData.heldShuffled, index)) {
        Sfx_TrackPlay(index);
      }
      if (Sfx_IsHeld(index) || Sfx_IsExcludedByOption(index)) {
        return id;
      }
    }
    const u32 remapped = Sfx_Remap(Sfx_TableForMode(gExtSaveData.options.shuffleSFX), id);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    if (remapped != id) {
      util::Print("SFX play %03X -> %03X\n", id - SFX_BASE, remapped - SFX_BASE);
    }
#endif
    return remapped;
  }

  // A sound's handle keeps the id it was started under, and stopping by the vanilla id alone misses a
  // shuffled one. It then outlives the object that started it and writes into that object's freed
  // memory when it ends. So this stops the vanilla id and its image in both tables; cutting an
  // unrelated sound short is the lesser risk.
  void Sfx_StopById(void* mgr, u32 id) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("SFX stop %03X (%03X %03X)\n", id - SFX_BASE, Sfx_Remap(0, id) - SFX_BASE,
                Sfx_Remap(1, id) - SFX_BASE);
#endif
    Sfx_StopByIdVanilla(mgr, id);
    for (s32 table = 0; table < kTableCount; table++) {
      const u32 remapped = Sfx_Remap(table, id);
      if (remapped != id) {
        Sfx_StopByIdVanilla(mgr, remapped);
      }
    }
  }

  s32 Sfx_IsPlayingById(void* mgr, u32 id) {
    if (Sfx_IsPlayingByIdVanilla(mgr, id)) {
      return 1;
    }
    for (s32 table = 0; table < kTableCount; table++) {
      const u32 remapped = Sfx_Remap(table, id);
      if (remapped != id && Sfx_IsPlayingByIdVanilla(mgr, remapped)) {
        return 1;
      }
    }
    return 0;
  }
  }
}  // namespace rnd
