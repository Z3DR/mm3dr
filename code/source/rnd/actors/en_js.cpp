#include "rnd/actors/en_js.h"

namespace rnd
{
  extern "C" u16 En_Js_CheckVictoryRequirements() {
    auto& cdata = game::GetCommonData();
    if (cdata.save.player_form != game::act::Player::Form::Human)
      return 0x220B;
    
    u16 remainsCollected = 0;
    if (cdata.save.inventory.collect_register.odolwas_remains == 1)
      remainsCollected++;
    if (cdata.save.inventory.collect_register.gohts_remains == 1)
      remainsCollected++;
    if (cdata.save.inventory.collect_register.gyorgs_remains == 1)
      remainsCollected++;
    if (cdata.save.inventory.collect_register.twinmolds_remains == 1)
      remainsCollected++;

    if (remainsCollected >= gSettingsContext.masksNeededForVictory) {
      if (rnd::util::GetPointer<u16(int)>(0x2F217C)(0) < 20)
        return 0x21FC;
      else
        return 0x2202;
    }
    return 0xFFFE;
  }
} // namespace rnd
