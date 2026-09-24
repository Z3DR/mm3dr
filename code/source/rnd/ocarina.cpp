#include <bitset>

#include "common/advanced_context.h"
#include "common/debug.h"
#include "common/flags.h"
#include "common/types.h"
#include "game/context.h"
#include "game/ocarina_mgr.h"
#include "game/player.h"
#include "game/sound.h"
#include "game/ui/layouts/message_window.h"
#include "rnd/ocarina.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

namespace rnd {
  static_assert(offsetof(game::GlobalContext, msg_context.ocarinaSongActionId) == 0x8368);
  static_assert(offsetof(game::GlobalContext, msg_context.lastPlayedSong) == 0x836A);
  static_assert(offsetof(game::CommonData, save.player_form) == 0x26);

  // Set while a fade we armed is counting down. When it expires, OcarinaMgr turns the instrument
  // off and resets its state, including the song flags and playing staff. Vanilla never lets the
  // player reach a new session within that window, but a skipped replay does: the reset then lands
  // mid-song, the staff reports 0xFF, and the window shows the red X failure.
  static bool sOwnFadePending = false;

  static void ArmOcarinaFadeOut() {
    constexpr int fade_durations[] = {20, 25, 25, 20, 20};
    u8 form = 0;
    if (auto* gctx = GetContext().gctx; gctx != nullptr && gctx->GetPlayerActor() != nullptr) {
      form = u8(gctx->GetPlayerActor()->active_form);
    }
    if (form >= ARR_SIZE(fade_durations)) {
      form = 0;
    }
    game::sound::SetOcarinaFadeOut(0, fade_durations[form]);
    sOwnFadePending = true;
  }

  static void EndOcarinaSession(game::ui::MessageWindow* window, bool keepAudio) {
    auto* gctx = GetContext().gctx;

    if (!keepAudio) {
      ArmOcarinaFadeOut();
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

  static bool sFadeAfterPlayback = false;

  static bool IsOcarinaPlaybackActive() {
    return game::sound::GetOcarinaMgr().playbackState != 0;
  }

  static bool IsNewOcarinaSessionActive() {
    return game::sound::GetOcarinaMgr().songFlags != 0;
  }

  // Restarts the melody without the visual replay. Both skip paths need this: the replay path
  // for ordinary songs, and HandleOcarinaSong for the ones it claims outright, which never reach
  // the replay code and so were silent under "Skip (Keep SFX)".
  static void PlaySkippedSongAudio(game::OcarinaSong song) {
    static const u8 kOcarinaInstruments[] = {0x01, 0x07, 0x08, 0x09};
    u8 form = (u8)game::GetCommonData().save.player_form;
    if (form >= ARR_SIZE(kOcarinaInstruments)) {
      form = 0;  // Human is index 4 and folds back onto 0
    }
    game::sound::SetOcarinaInstrument(0x01);
    game::sound::SetOcarinaInstrument(kOcarinaInstruments[form]);
    game::sound::SetOcarinaPlaybackSong(u8(u16(song) + 1), 1);
    sFadeAfterPlayback = true;
  }

  void Ocarina_Update() {
    auto& ocarinaMgr = game::sound::GetOcarinaMgr();
    if (sOwnFadePending && ocarinaMgr.fadeOutTimer == 0) {
      sOwnFadePending = false;
    }

    // Nothing from the skipped song may outlive into the next session: the fade would reset it,
    // and a still-running playback mutes the player's notes and feeds the song check.
    if (IsNewOcarinaSessionActive()) {
      if (sOwnFadePending) {
        ocarinaMgr.fadeOutTimer = 0;
        sOwnFadePending = false;
      }
      if (sFadeAfterPlayback) {
        if (IsOcarinaPlaybackActive()) {
          game::sound::SetOcarinaPlaybackSong(0, 0);
        }
        sFadeAfterPlayback = false;
      }
      return;
    }

    if (!sFadeAfterPlayback || IsOcarinaPlaybackActive()) {
      return;
    }
    sFadeAfterPlayback = false;
    ArmOcarinaFadeOut();
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

    const bool keepAudio = gExtSaveData.options.skipSongReplays == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX;
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
      return false;
    }

    auto* gctx = GetContext().gctx;
    if (gctx == nullptr) {
      return false;
    }

    if (gctx->msg_context.lastPlayedSong == game::OcarinaSong::SongOfDoubleTime) {
      return false;
    }

    auto& action = gctx->msg_context.ocarinaSongActionId;
    game::OcarinaSongActionId doneAction;
    if (action == game::OcarinaSongActionId::OCARINA_ACTION_FREE_PLAY) {
      doneAction = game::OcarinaSongActionId::OCARINA_ACTION_FREE_PLAY_DONE;
    } else if (action == game::OcarinaSongActionId::OCARINA_ACTION_CHECK_NOTIME) {
      doneAction = game::OcarinaSongActionId::OCARINA_ACTION_CHECK_NOTIME_DONE;
    } else {
      return false;
    }

    if (gctx->msg_context.lastPlayedSong == game::OcarinaSong::SongOfStorms) {
      // Spawns oca_eff for the song of storms as that is what causes beans to grow in soil.
      util::GetPointer<void(game::GlobalContext*)>(0x3B7B84)(gctx);
    }
    action = doneAction;
    gctx->msg_context.ocarinaMode = game::OcarinaMode::OCARINA_MODE_ACTIVE;

    if (gExtSaveData.options.skipSongReplays == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX) {
      PlaySkippedSongAudio(gctx->msg_context.lastPlayedSong);
    } else {
      ArmOcarinaFadeOut();
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
