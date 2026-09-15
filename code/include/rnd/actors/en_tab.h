#pragma once

#include "game/actor.h"
#include "rnd/item_override.h"
#include "rnd/savefile.h"
#include "rnd/shops.h"

namespace rnd {
  constexpr u32 kEnTabSaleScript = 0x684D48;
  constexpr u32 kEnTabSaleScriptDayThree = 0x684DB1;
  constexpr u32 kEnTabSaleScriptSize = 0x69;

  enum EnTabSaleScriptOffset : u8 {
    ENTAB_SALE_CHOICE_MILK = 0x19,     // CHECK_TEXT_CHOICE skip for the first choice
    ENTAB_SALE_CHOICE_CHATEAU = 0x1B,  // CHECK_TEXT_CHOICE skip for the second choice
    ENTAB_SALE_CHOICE_BASE = 0x1F,     // skips are counted from the end of CHECK_TEXT_CHOICE
    ENTAB_SALE_REFUSE = 0x2F,          // PLAY_CANCEL, BEGIN_TEXT(0x2B0D), AWAIT_TEXT, DONE
  };

  extern "C" {
  const u8* En_Tab_GetSaleScript(const u8* script);
  }
}  // namespace rnd
