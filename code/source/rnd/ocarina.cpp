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

  static void EndOcarinaSession(game::ui::MessageWindow* window) {
    auto* gctx = GetContext().gctx;

    constexpr int fade_durations[] = {20, 25, 25, 20, 20};
    const auto set_ocarina_fadeout = util::GetPointer<void(int zero, int duration)>(0x4FE0BC);
    set_ocarina_fadeout(0, fade_durations[u8(gctx->GetPlayerActor()->active_form)]);

    const auto set_ocarina_mode = util::GetPointer<void(game::ui::MessageWindow*, game::OcarinaMode mode)>(0x1D1A18);
    set_ocarina_mode(window, game::OcarinaMode::OCARINA_MODE_ACTIVE);

    // Disable BGM fadeout
    util::Write(gctx, 0x8422, 1);
    gctx->msg_context.ocarinaMode = game::OcarinaMode::OCARINA_MODE_END;
  }

  // Pure predicate: is the fast-song option enabled? No side effects -- safe to call anywhere.
  static bool SongReplaySkipEnabled() {
    const u8 mode = gExtSaveData.options.skipSongReplays;
    // 2-bit field, so 3 is representable but is not a valid setting. Anything unrecognised
    // must fall back to vanilla rather than half-skipping.
    return mode == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_NO_SFX ||
           mode == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX;
  }

  bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
    if (u16(song) > 0x16) {
      return false;  // sentinels reach us before the game's own filter at 0x604D8C
    }
    if (!SongReplaySkipEnabled()) {
      return false;
    }

    // Songs 6 and 9 are the only ones 0x1D78F0 dispatches, and state 0x18 has no path that
    // sets their mode -- it just flags 0x8366 and returns. Everything else is applied
    // downstream from that flag, so it falls through to the state-machine skip instead.
    if (song != game::OcarinaSong::SongOfTime && song != game::OcarinaSong::SongOfSoaring) {
      return false;
    }

    auto* gctx = GetContext().gctx;
    if (gctx == nullptr) {
      return false;
    }

    EndOcarinaSession(self);
    gctx->msg_context.lastPlayedSong = song;
    gctx->msg_context.ocarinaMode = game::OcarinaMode::OCARINA_MODE_EVENT;
    self->song = u16(song);
    if (song == game::OcarinaSong::SongOfSoaring) {
      // Soaring only: must be false or its handler refuses to show the map screen.
      util::Write<bool>(gctx, 0x83EC, false);
    }
    util::GetPointer<void(game::ui::MessageWindow*)>(0x1D78F0)(self);
    return true;
  }

  // Side-effecting: called from hook_SkipSongReplay immediately before the ocarina state is
  // forced to 0x18. Returns true only when the caller should actually skip the replay.
  bool SongReplayTrySkip() {
    if (!SongReplaySkipEnabled()) {
      return false;  // vanilla: the hook leaves state 0x12 and the timer untouched
    }

    auto* gctx = GetContext().gctx;
    if (gctx == nullptr) {
      return false;  // no context to fix up, so let vanilla run
    }

    // State 0x13 advances the prompt context before 0x18 reads it. Jumping straight to 0x18
    // leaves it at its old value, so 0x18 treats the song as an unanswered prompt and applies
    // no effect. Replicate the transition here -- 0x32 is the "free play, apply it" value.
    const u16 prompt = util::BitCastPtr<u16>(gctx, 0x8368);
    if (prompt == 1) {
      util::Write<u16>(gctx, 0x8368, 0x32);
    } else if (prompt == 0x38) {
      util::Write<u16>(gctx, 0x8368, 0x39);
    }
    util::Write<u16>(gctx, 0x8366, 1);

    // 0x13 is also what starts the melody, so without it the skip is silent. This is what
    // separates the two skip modes.
    if (gExtSaveData.options.skipSongReplays == (u8)SongReplaysSetting::SONGREPLAYS_SKIP_KEEP_SFX) {
      static const u8 kOcarinaInstruments[] = {0x01, 0x07, 0x08, 0x09};
      const auto set_instrument = util::GetPointer<void(u8)>(0x1DF440);
      u8 form = util::BitCastPtr<u8>(&game::GetCommonData(), 0x26);
      if (form >= ARR_SIZE(kOcarinaInstruments)) {
        form = 0;  // Human is index 4 and folds back onto 0
      }
      set_instrument(0x01);
      set_instrument(kOcarinaInstruments[form]);

      const u16 songId = util::BitCastPtr<u16>(gctx, 0x8364);
      util::GetPointer<void(u8, u8)>(0x1CF15C)(u8(songId + 1), 1);  // AudioOcarina_SetPlaybackSong
    }

    return true;
  }

}  // namespace rnd

// These wrappers exist because the asm hooks need C linkage, and the rnd::util helpers are
// not reachable from an extern "C" definition at global scope.
extern "C" {
bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
  return rnd::HandleOcarinaSong(self, song);
}

bool ShouldSkipSongReplay() {
  return rnd::SongReplayTrySkip();
}
}
