#pragma once

#include "game/common_data.h"
#include "game/message.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"
namespace rnd {
  struct En_Js : public game::act::Actor {
    int field_1FC;
    game::as::ActorUtil actor_util;
    u8 gap_288[1868];
    u16 field_9D4;
    u8 gap_9D6[10];
    void* next_fn;
  };
  static_assert(sizeof(En_Js) == 0x9E4);
  extern "C" {
  u16 En_Js_CurrentMasksInInventory();
  u16 En_Js_CheckVictoryRequirements();
  bool En_Js_IsCustomTerminalText(game::GlobalContext*);
  }
}  // namespace rnd
