#include "rnd/actors/en_giant.h"

namespace rnd {
  extern "C" {
  void En_Giant_ShouldDrawGiant(game::act::Actor* giant) {
    u8 giantId = giant->params & 0xF;
    game::SaveData save = game::GetCommonData().save;
    game::InventoryData::CollectRegister& collect_register = save.inventory.collect_register;
    if (Settings_MetMoonRequirements())
      return;  // Draw all if it's been completed.

    /* XXX: This is honestly a very redunant function call, but it's here to branch
    / out to a new function when we can implement different checks to get to the moon,
    / like skulltulas collected, heart pieces gained, etc. */
    switch (giantId) {
    case 0:  // Snowhead
      if (collect_register.gohts_remains == 0)
        giant->draw_fn = (game::act::MainFunc*)0x0;
      return;
    case 1:  // Ikana
      if (collect_register.twinmolds_remains == 0)
        giant->draw_fn = (game::act::MainFunc*)0x0;
      return;
    case 2:  // Woodfall
      if (collect_register.odolwas_remains == 0)
        giant->draw_fn = (game::act::MainFunc*)0x0;
      return;
    case 3:  // Great Bay
      if (collect_register.gyorgs_remains == 0)
        giant->draw_fn = (game::act::MainFunc*)0x0;
      return;
    }
    giant->draw_fn = (game::act::MainFunc*)0x0;
    return;
  }

  bool En_Giant_KillAfterCutscene(game::act::Actor* giant) {
    game::SaveData save = game::GetCommonData().save;
    game::InventoryData::CollectRegister& collect_register = save.inventory.collect_register;
    if (save.week_event_reg_25.WEEKEVENTREG_OATH_CUTSCENE_SUCCEEDED == 0) {
      return false;
    }
    u8 giantId = giant->params & 0xF;
    switch (giantId) {
    case 4:  // Snowhead
      if (collect_register.gohts_remains == 0)
        return false;
      else
        return true;
    case 5:  // Ikana
      if (collect_register.twinmolds_remains == 0)
        return false;
      else
        return true;
    case 6:  // Woodfall
      if (collect_register.odolwas_remains == 0)
        return false;
      else
        return true;
    case 7:  // Great Bay
      if (collect_register.gyorgs_remains == 0)
        return false;
      else
        return true;
    }
    return true;
  }
  }
}  // namespace rnd
