#include "rnd/actors/en_giant.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
#endif

namespace rnd {
  extern "C" {
  void En_Giant_ShouldDrawGiant(game::act::Actor* giant) {
    u8 giantId = giant->params & 0xF;
    game::GlobalContext* gctx = GetContext().gctx;
    game::SaveData save = game::GetCommonData().save;
    game::InventoryData::CollectRegister& collect_register = save.inventory.collect_register;
    if (Settings_MetMoonRequirements() || gctx->scene != game::SceneId::ClockTowerRooftop)
      return;  // Draw all if it's been completed, or if we are not on the rooftop.

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

  // The blue warp out of a boss lair picks the Giant's Chamber cutscene from defeated_bosses, one byte
  // per boss (read at 0x345D78): 1 plays the first giant, who teaches Oath to Order, and 2 and 3 the
  // later ones. In the randomizer that byte can come back as 1 for more than one boss, replaying the
  // first giant, so the Oath check decides instead: the first giant plays until it has been given, and
  // never again after.
  u32 En_Giant_ChamberCutsceneOrder(u32 boss) {
    u32 order = (u32)game::GetCommonData().save.defeated_bosses;
    const u32 shift = boss * 8;
    u32 visit = (order >> shift) & 0xFF;
    if (gExtSaveData.givenSongChecks.oathToOrderGiven == 0) {
      visit = 1;
    } else if (visit < 2) {
      visit = 2;
    }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: boss %u stored %u, oath given %u, playing chamber cutscene for visit %u\n", __func__, boss,
                (order >> shift) & 0xFF, (u32)gExtSaveData.givenSongChecks.oathToOrderGiven.Value(), visit);
#endif
    return (order & ~(0xFFu << shift)) | (visit << shift);
  }
  }
}  // namespace rnd
