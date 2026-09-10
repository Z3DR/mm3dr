#include <bitset>

#include "common/advanced_context.h"
#include "common/debug.h"
#include "common/flags.h"
#include "common/types.h"
#include "game/context.h"
#include "game/player.h"
#include "game/sound.h"
#include "game/ui/layouts/message_window.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

namespace rnd {
  // Offsets this file pokes at directly. Verified against the struct so a layout change breaks
  // the build rather than the ocarina.
  static_assert(offsetof(game::GlobalContext, msg_context.ocarinaSongActionId) == 0x8368);
  static_assert(offsetof(game::GlobalContext, msg_context.lastPlayedSong) == 0x836A);
  static_assert(offsetof(game::CommonData, save.player_form) == 0x26);


  // keepAudio: the caller is about to replay the melody itself, so the fadeout must not be armed.
  // ocarinaMgrSetFadeOut (0x4FE0BC) only stores a duration into OcarinaMgr[0x1C0], and 20-25
  // frames is roughly two notes -- long enough that the replayed song audibly starts and then
  // dies. The replay path never gets here, which is why Epona's Song was unaffected.
  static void EndOcarinaSession(game::ui::MessageWindow* window, bool keepAudio) {
    auto* gctx = GetContext().gctx;

    if (!keepAudio) {
      constexpr int fade_durations[] = {20, 25, 25, 20, 20};
      const auto set_ocarina_fadeout = util::GetPointer<void(int zero, int duration)>(0x4FE0BC);
      set_ocarina_fadeout(0, fade_durations[u8(gctx->GetPlayerActor()->active_form)]);
    }

    const auto set_ocarina_mode = util::GetPointer<void(game::ui::MessageWindow*, game::OcarinaMode mode)>(0x1D1A18);
    set_ocarina_mode(window, game::OcarinaMode::OCARINA_MODE_ACTIVE);

    // Disable BGM fadeout
    util::Write(gctx, 0x8422, 1);
    gctx->msg_context.ocarinaMode = game::OcarinaMode::OCARINA_MODE_END;
  }

  static bool SongReplaySkipEnabled() {
    const u8 mode = gExtSaveData.options.skipSongReplays;
    // 2-bit field, so 3 is representable but is not a valid setting. Anything unrecognised
    // must fall back to vanilla rather than half-skipping.
    return mode == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_NO_SFX ||
           mode == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX;
  }

  // Restarts the melody without the visual replay. Both skip paths need this: the replay path
  // for ordinary songs, and HandleOcarinaSong for the ones it claims outright, which never reach
  // the replay code and so were silent under "Skip (Keep SFX)".
  static void PlaySkippedSongAudio(game::OcarinaSong song) {
    static const u8 kOcarinaInstruments[] = {0x01, 0x07, 0x08, 0x09};
    const auto set_instrument = util::GetPointer<void(u8)>(0x1DF440);
    u8 form = (u8)game::GetCommonData().save.player_form;
    if (form >= ARR_SIZE(kOcarinaInstruments)) {
      form = 0;  // Human is index 4 and folds back onto 0
    }
    set_instrument(0x01);
    set_instrument(kOcarinaInstruments[form]);

    // AudioOcarina_SetPlaybackSong
    util::GetPointer<void(u8, u8)>(0x1CF15C)(u8(u16(song) + 1), 1);
  }

  bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
    if (u16(song) > 0x16) {
      return false;
    }
    if (!SongReplaySkipEnabled()) {
      return false;
    }

    // Songs handled here skip straight to their event instead of sitting through the replay.
    if (song != game::OcarinaSong::SongOfTime && song != game::OcarinaSong::SongOfSoaring &&
        song != game::OcarinaSong::SongOfDoubleTime) {
      return false;
    }

    auto* gctx = GetContext().gctx;
    if (gctx == nullptr) {
      return false;
    }

    const bool keepAudio =
        gExtSaveData.options.skipSongReplays == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX;
    EndOcarinaSession(self, keepAudio);
    gctx->msg_context.lastPlayedSong = song;
    gctx->msg_context.ocarinaMode = game::OcarinaMode::OCARINA_MODE_EVENT;
    self->song = u16(song);
    if (song == game::OcarinaSong::SongOfSoaring) {
      util::Write<bool>(gctx, 0x83EC, false);
    }
    if (keepAudio) {
      PlaySkippedSongAudio(song);
    }
    util::GetPointer<void(game::ui::MessageWindow*)>(0x1D78F0)(self);
    return true;
  }

  bool SongReplayTrySkip() {
    if (!SongReplaySkipEnabled()) {
      return false;  // vanilla: the hook leaves state 0x12 and the timer untouched
    }

    auto* gctx = GetContext().gctx;
    if (gctx == nullptr) {
      return false;  // no context to fix up, so let vanilla run
    }

    if (gctx->msg_context.lastPlayedSong == game::OcarinaSong::SongOfDoubleTime) {
      return false;
    }

    const auto prompt = static_cast<game::OcarinaSongActionId>(util::BitCastPtr<u16>(gctx, 0x8368));
    if (prompt == game::OcarinaSongActionId::OCARINA_ACTION_FREE_PLAY) {
      util::Write<u16>(gctx, 0x8368,
                       (u16)game::OcarinaSongActionId::OCARINA_ACTION_FREE_PLAY_DONE);
    } else if (prompt == game::OcarinaSongActionId::OCARINA_ACTION_CHECK_NOTIME) {
      util::Write<u16>(gctx, 0x8368,
                       (u16)game::OcarinaSongActionId::OCARINA_ACTION_CHECK_NOTIME_DONE);
    } else {
      return false;
    }
    util::Write<u16>(gctx, 0x8366, 1);

    if (gExtSaveData.options.skipSongReplays == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX) {
      PlaySkippedSongAudio(gctx->msg_context.lastPlayedSong);
    }

    return true;
  }

}  // namespace rnd

extern "C" {
bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
  return rnd::HandleOcarinaSong(self, song);
}

bool ShouldSkipSongReplay() {
  return rnd::SongReplayTrySkip();
}
}
