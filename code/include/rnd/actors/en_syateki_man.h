#pragma once

#include "game/actor.h"
#include "game/common_data.h"
#include "rnd/item_override.h"

namespace rnd {
  // Shooting gallery owner, used by both the Town and Swamp galleries.
  struct EnSyatekiMan : public game::act::Actor {
    u8 gap_1F8[0x8C];
    game::act::MainFunc* next_fn;
    u8 gap_288[0x840];
    // Points scored in the finished game; 50 is perfect in Town and 2180 in the Swamp.
    s16 score;
    u8 gap_ACA[2];
    // Town: the result message, which also says which reward is paid.
    s16 result_text_id;
    u8 field_ACE;
    u8 gap_ACF[5];
  };
  static_assert(offsetof(EnSyatekiMan, next_fn) == 0x284);
  static_assert(offsetof(EnSyatekiMan, score) == 0xAC8);
  static_assert(offsetof(EnSyatekiMan, result_text_id) == 0xACC);
  static_assert(offsetof(EnSyatekiMan, field_ACE) == 0xACE);
  static_assert(sizeof(EnSyatekiMan) == 0xAD4);

  extern "C" {
  void EnSyatekiMan_TownPerfectBonus(EnSyatekiMan*);
  void EnSyatekiMan_SwampPerfectBonus(EnSyatekiMan*);
  }
}  // namespace rnd
