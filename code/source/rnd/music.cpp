#include "rnd/music.h"

#include "common/utils.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

namespace rnd {
  constexpr u32 kBgmFlagsTable = 0x6A0DEC;

  static bool IsFanfare(u32 index) {
    return (util::BitCastPtr<u32>(reinterpret_cast<const void*>(kBgmFlagsTable), index * 4) & 2) != 0;
  }

  static bool ShuffleAllowed(u32 index) {
    const u8 mode = gExtSaveData.options.shuffleMusic;
    if (mode == (u8)ShuffleMusicSetting::SHUFFLEMUSIC_OFF)
      return false;
    if (mode == (u8)ShuffleMusicSetting::SHUFFLEMUSIC_ALL)
      return true;
    return IsFanfare(index) ? mode == (u8)ShuffleMusicSetting::SHUFFLEMUSIC_FANFARES_ONLY :
                              mode == (u8)ShuffleMusicSetting::SHUFFLEMUSIC_BGM_ONLY;
  }

  static u32 MusicOverrideImpl(u32 original) {
    const u32 i = original - BGM_BASE;
    if (i >= BGM_COUNT)
      return original;
    if (!ShuffleAllowed(i))
      return original;
    const u32 repl = rBGMOverrides[i];
    if (repl - BGM_BASE >= BGM_COUNT)
      return original;
    return repl;
  }

  extern "C" {
  u32 rBGMOverrides[BGM_COUNT] = {0};
  u32 Music_OverridePlay(u32 original) {
    return MusicOverrideImpl(original);
  }
  }
}  // namespace rnd
