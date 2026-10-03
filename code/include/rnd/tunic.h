#pragma once

#include "common/types.h"
#include "game/objectbankarchive.h"

namespace rnd {
  enum class TunicForm : u8 { Human, Deku, Goron, Zora, Count };

  // 0 is the seed's colour (vanilla if Custom Tunic Colors is off), the rest are the app's presets.
  constexpr u8 tunicColorCount = 32;
  extern const char* const tunicColorNames[];
  extern const char* const tunicFormNames[];

  u8 Tunic_GetChoice(TunicForm);
  void Tunic_SetChoice(TunicForm, u8);
  u32 Tunic_GetChoiceColor(u8);
  void Tunic_ApplyToObject(game::ObjectBank::ObjectBankArchive*);
}  // namespace rnd
