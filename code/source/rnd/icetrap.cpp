#include "rnd/icetrap.h"

namespace rnd {
  // The game ticks 30 times a second, so a timed trap lasts 20 seconds.
  static constexpr u16 rTimedTrapTicks = 21 * 30;
  static constexpr s16 rTimerTotal = 20;

  enum class IceTrapKind : u8 {
    Freeze,
    Jinx,
    Fire,
    InvertedControls,
    Shock,

    Count,  // Always last in array.
  };

  enum class IceDamageType : int {
    Ice = 3,
    Shock,
  };

  static u8 gPendingFreezes = 0;
  static u16 rInvertTicks = 0;

  static s16 IceTrap_Rand(int base, int range) {
    return util::GetPointer<s16(int, int)>(0x1F7698)(base, range);
  }

  // Mirrors the game's own "set Link on fire" sequence (inlined in the player damage check at 0x20C2E4).
  static void IceTrap_Burn(game::GlobalContext* gctx, game::act::Player* link) {
    util::GetPointer<void(game::act::Player*, u32)>(0x226BBC)(link, 0x1000801);
    for (u8& timer : link->body_flame_timers) {
      timer = (u8)IceTrap_Rand(0, 200);
    }
    link->body_is_burning = 1;
    // Returns non-zero if the burn killed Link or voided out.
    if (util::GetPointer<int(game::GlobalContext*, game::act::Player*)>(0x20DE6C)(gctx, link) != 0) {
      return;
    }
    // Small knockback, same speeds as the game uses for fire.
    util::GetPointer<void(game::GlobalContext*, game::act::Player*, int, int16_t, char, int, float, float)>(0x239794)(
        gctx, link, 1, 20, 0, 1, 4.0f, 5.0f);
    game::sound::EmitFormSpecificSoundForPlayer(game::sound::EffectId::NA_SE_VO_LI_FALL_L);
  }

  static void IceTrap_Shock(game::GlobalContext* gctx, game::act::Player* link) {
    s16 depleteHpBy = IceTrap_Rand(4, 32);  // Randomly select from 1-4 hearts being lost.
    util::GetPointer<void(game::GlobalContext*, game::act::Player*, int, int16_t, char, int)>(0x239794)(
        gctx, link, static_cast<int>(IceDamageType::Shock), 1, 0, 1);
    util::GetPointer<void(game::GlobalContext*, int)>(0x21e524)(gctx, -depleteHpBy);
    game::sound::EmitFormSpecificSoundForPlayer(game::sound::EffectId::NA_SE_VO_LI_FALL_L);
  }

  void IceTrap_Push() {
    if (gPendingFreezes < 0xFF)
      gPendingFreezes += 1;
  }

  void IceTrap_Give() {
    game::GlobalContext* gctx = GetContext().gctx;
    game::act::Player* link = gctx->GetPlayerActor();
    if (link->flags1.IsOneSet(game::act::Player::Flag1::FreezeLink, game::act::Player::Flag1::FreezeWorld)) {
      return;
    }
    if (!gPendingFreezes || link->timer != 0 || IceTrap_IsTimedActive()) {
      return;
    }
    gPendingFreezes -= 1;
    if (gSettingsContext.enableMoreIceTraps != 1) {
      util::GetPointer<void(game::GlobalContext*, game::act::Player*, int, int16_t, char, int)>(0x239794)(
          gctx, link, static_cast<int>(IceDamageType::Ice), 1, 0, 1);
    } else {
      switch ((IceTrapKind)IceTrap_Rand(0, (int)IceTrapKind::Count)) {
      case IceTrapKind::Freeze:
        util::GetPointer<void(game::GlobalContext*, game::act::Player*, int, int16_t, char, int)>(0x239794)(
            gctx, link, static_cast<int>(IceDamageType::Ice), 1, 0, 1);
        break;
      case IceTrapKind::Jinx:
        game::sound::EmitFormSpecificSoundForPlayer(game::sound::EffectId::NA_SE_VO_LI_DAMAGE_S);
        game::GetCommonData().save.jinxTimer = rTimedTrapTicks;
        break;
      case IceTrapKind::Fire:
        IceTrap_Burn(gctx, link);
        break;
      case IceTrapKind::InvertedControls:
        game::sound::EmitFormSpecificSoundForPlayer(game::sound::EffectId::NA_SE_VO_LI_DAMAGE_S);
        // Creates an interface timer to show how long effect will last for.
        util::GetPointer<void(int, int)>(0x3B7EB0)(4, rTimerTotal);
        rInvertTicks = rTimedTrapTicks;
        break;
      case IceTrapKind::Shock:
        IceTrap_Shock(gctx, link);
        break;
      default:
        // Freeze on default.
        util::GetPointer<void(game::GlobalContext*, game::act::Player*, int, int16_t, char, int)>(0x239794)(
            gctx, link, static_cast<int>(IceDamageType::Ice), 1, 0, 1);
        break;
      }
    }
  }

  bool IceTrap_IsPending() {
    return gPendingFreezes > 0;
  }

  bool IceTrap_IsTimedActive() {
    return rInvertTicks > 0 || game::GetCommonData().save.jinxTimer > 0;
  }

  // Swaps two stick direction bits so the digital view of the stick agrees with the flipped axes.
  static void IceTrap_SwapButtons(rnd::Flags<game::pad::Button>& buttons, game::pad::Button a, game::pad::Button b) {
    const bool hasA = buttons.IsSet(a);
    const bool hasB = buttons.IsSet(b);
    buttons.Set(b, hasA);
    buttons.Set(a, hasB);
  }

  void IceTrap_Update() {
    if (rInvertTicks == 0)
      return;
    game::GlobalContext* gctx = GetContext().gctx;
    if (gctx->GetPlayerActor()->flags1.IsSet(game::act::Player::Flag1::Dead)) {
      rInvertTicks = 0;
      return;
    }
    rInvertTicks--;
    auto& pad = gctx->pad_state;
    pad.input.main_stick_x = -pad.input.main_stick_x;
    pad.input.main_stick_y = -pad.input.main_stick_y;
    pad.main_stick.x = -pad.main_stick.x;
    pad.main_stick.y = -pad.main_stick.y;
    pad.main_stick.x_raw = -pad.main_stick.x_raw;
    pad.main_stick.y_raw = -pad.main_stick.y_raw;
    pad.main_stick.x_raw_last = -pad.main_stick.x_raw_last;
    pad.main_stick.y_raw_last = -pad.main_stick.y_raw_last;
    for (auto* buttons : {&pad.input.buttons, &pad.input.new_buttons, &pad.input.released_buttons}) {
      IceTrap_SwapButtons(*buttons, game::pad::Button::MainStickLeft, game::pad::Button::MainStickRight);
      IceTrap_SwapButtons(*buttons, game::pad::Button::MainStickUp, game::pad::Button::MainStickDown);
    }
  }

  void IceTrap_Reset() {
    rInvertTicks = 0;
  }
}  // namespace rnd
