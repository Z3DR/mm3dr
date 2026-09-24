#pragma once

#include <cstddef>

#include "common/types.h"
#include "game/context.h"

namespace game::sound {

  struct OcarinaMgr {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 input;
    u32 field_10;
    u32 field_14;
    u8 instrument;  // 0 = off; any change (including to 0xF0) resets the manager
    u8 gap_19[0x3];
    u32 songFlags;  // sOcarinaFlags: songs checked for; non-zero while a session is listening
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 playbackState;  // non-zero while a playback song (AudioOcarina_SetPlaybackSong) runs
    u32 field_24;
    u8 field_28;
    u8 gap_29[0x1];
    u16 field_2A;
    u16 field_2C;
    u8 field_2E;
    u8 gap_2F[0x1];
    u32 field_30;
    OcarinaStaff playingStaff;  // sPlayingStaff, returned by AudioOcarina_GetPlayingStaff
    u8 field_37;
    u8 field_38;
    u8 field_39;
    u8 field_3A;
    u8 field_3B;
    u8 field_3C;
    u8 gap_3D[0x9];
    u8 field_46;
    u8 field_47;
    u32 field_48;
    u8 gap_4C[0x1C];
    u8 field_68;
    u8 field_69;
    u8 field_6A;
    u8 field_6B;
    float field_6C;
    float field_70;
    u16 inputX;
    u16 inputY;
    u8 field_78;
    u8 field_79;
    u8 field_7A;
    u8 field_7B;
    u32 field_7C;
    u8 field_80;
    u8 field_81;
    u8 field_82;
    u8 field_83;
    u8 field_84;
    u8 field_85;
    u8 field_86;
    u8 inputEnabled;  // set by AudioOcarina_Start, cleared by reset
    u8 field_88;
    u8 field_89;
    u8 field_8A;
    u8 field_8B;
    u8 field_8C;
    u8 field_8D;
    u8 field_8E;
    u8 field_8F;
    u8 field_90;
    u8 field_91;
    u8 field_92;
    u8 detectedSong;  // song index + 1, consumed into playingStaff.song by calc
    u8 field_94;
    u8 gap_95[0x8];
    u8 field_9D;
    u8 field_9E;
    u8 gap_9F[0x1];
    u32 field_A0;
    u32 field_A4;
    u8 gap_A8[0xA8];
    u8 field_150;
    u8 gap_151[0x5F];
    u16 field_1B0;
    u8 gap_1B2[0x2];
    float field_1B4;
    float field_1B8;
    u32 field_1BC;
    u8 fadeOutTimer;  // counts down in calc; on expiry turns the instrument off and resets
    u8 field_1C1;
    u8 field_1C2;
    u8 field_1C3;
    u32 field_1C4;
    u8 field_1C8;
    u8 field_1C9;
    u8 field_1CA;
    u8 field_1CB;
  };
  static_assert(offsetof(OcarinaMgr, instrument) == 0x18);
  static_assert(offsetof(OcarinaMgr, songFlags) == 0x1C);
  static_assert(offsetof(OcarinaMgr, playbackState) == 0x23);
  static_assert(offsetof(OcarinaMgr, playingStaff) == 0x34);
  static_assert(offsetof(OcarinaMgr, inputEnabled) == 0x87);
  static_assert(offsetof(OcarinaMgr, detectedSong) == 0x93);
  static_assert(offsetof(OcarinaMgr, fadeOutTimer) == 0x1C0);
  static_assert(sizeof(OcarinaMgr) == 0x1CC);

  OcarinaMgr& GetOcarinaMgr();

  // AudioOcarina_SetInstrument
  void SetOcarinaInstrument(u8 instrument);
  // AudioOcarina_SetPlaybackSong. songIndexPlusOne 0 stops the current playback.
  void SetOcarinaPlaybackSong(u8 songIndexPlusOne, u8 playbackState);
  // Arms fadeOutTimer.
  void SetOcarinaFadeOut(u8 param, u8 duration);

}  // namespace game::sound
