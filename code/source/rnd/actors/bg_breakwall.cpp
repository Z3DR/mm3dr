#include "rnd/actors/bg_breakwall.h"

namespace rnd {
  extern "C" u8 Breakwall_CheckIfMikauSaved() {
    u8 clearedTemple = (u8)game::GetCommonData().save.week_event_reg_55.WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE;
    u8 mikauGivenItem = gExtSaveData.givenItemChecks.enZogGivenItem;
    return (clearedTemple && mikauGivenItem) ? 1 : 0;
  }
} // namespace rnd 