#include "game/ocarina_mgr.h"

#include "common/utils.h"

namespace game::sound {

  OcarinaMgr& GetOcarinaMgr() {
    return rnd::util::GetInstance<OcarinaMgr>(0x7CB2D0, 0x6B0A30, 0x1CA684);
  }

  void SetOcarinaInstrument(u8 instrument) {
    rnd::util::GetPointer<void(u8)>(0x1DF440)(instrument);
  }

  void SetOcarinaPlaybackSong(u8 songIndexPlusOne, u8 playbackState) {
    rnd::util::GetPointer<void(u8, u8)>(0x1CF15C)(songIndexPlusOne, playbackState);
  }

  void SetOcarinaFadeOut(u8 param, u8 duration) {
    rnd::util::GetPointer<void(u8, u8)>(0x4FE0BC)(param, duration);
  }

}  // namespace game::sound
