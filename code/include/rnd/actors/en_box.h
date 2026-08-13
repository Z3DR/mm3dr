#pragma once

#include "common/advanced_context.h"
#include "game/actors/en_box.h"
#include "game/context.h"
#include "rnd/item_override.h"
#include "rnd/settings.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  enum class ChestSize : u8 {
    VANILLA_SIZE,
    SIZE_MATCHES_CONTENT,
  };

  enum class ChestAnim : u8 {
    ALWAYS_FAST,
    ANIMATION_MATCHES_CONTENT,
  };

  enum class ChestType : u8 {
    WOODEN_BIG,
    WOODEN_SMALL,
    DECORATED_BIG,
    DECORATED_SMALL,
  };

  extern "C" {
  game::actors::EnBoxType En_Box_OverrideSize(game::actors::En_Box*, game::GlobalContext*);
  u8 En_Box_IsOverrideEnabled(game::actors::En_Box*, u16);
  bool En_Box_OverrideAnimation();
  bool En_Box_IsOrnateChest(game::actors::En_Box*);
  }
  // void EnBox_rInit(game::act::Actor* thisx, game::GlobalContext* globalCtx);
  // void EnBox_rUpdate(game::act::Actor* thisx, game::GlobalContext* globalCtx);

  // u8 En_Box_OverrideDecoration();
  // u8 En_Box_OverrideIceSmoke(game::act::Actor* thisx);
}  // namespace rnd
