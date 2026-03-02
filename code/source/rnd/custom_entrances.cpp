#include "rnd/custom_entrances.h"

namespace rnd {
  extern "C" {
  bool SceneEntranceOverride() {
    game::CommonData& cdata = game::GetCommonData();
    game::GlobalContext* gctx = GetContext().gctx;
    bool didWarp = false;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: Next entrance is %#06x\n", __func__, gctx->next_entrance);
#endif
    if (gctx->next_entrance == 0x1C04 && gSettingsContext.skipMikauCutscene) {
      gctx->next_entrance = 0x6890;
      cdata.sub13s[0].entrance_index = 0x6890;

      didWarp = true;
    } else if (gctx->next_entrance == 0x1C05 && gSettingsContext.skipDarmaniCutscene) {
      gctx->next_entrance = 0x9610;
      cdata.sub13s[0].entrance_index = 0x9610;
      didWarp = true;
    } else if (gctx->next_entrance == 0x1C19 && gSettingsContext.skipHMSCutscenes) {
      gctx->next_entrance = 0xc060;
      cdata.sub13s[0].entrance_index = 0xc060;
      didWarp = true;
      // Re-enable the sound track.
      game::sound::ControlStream(game::sound::StreamPlayer::DEFAULT_PLAYER, 1, 1);
      game::sound::PlayStream(game::sound::StreamId::NA_BGM_CLOCK_TOWER, game::sound::StreamPlayer::DEFAULT_PLAYER);
    } else if (gctx->next_entrance == 0x1C01 && gSettingsContext.skipHMSCutscenes) {
      gctx->next_entrance = 0xC020;
      cdata.sub13s[0].entrance_index = 0xC020;
      didWarp = true;
      // Re-enable the sound track.
      game::sound::ControlStream(game::sound::StreamPlayer::DEFAULT_PLAYER, 1, 1);
      game::sound::PlayStream(game::sound::StreamId::NA_BGM_CLOCK_TOWER, game::sound::StreamPlayer::DEFAULT_PLAYER);
    } else if (gctx->next_entrance == 0x2C10 && gctx->GetPlayerActor()->active_form != game::act::Player::Form::Deku) {
      // Patch for the rooftop to ensure that link is not turned into a form that has unintended effects.
      cdata.save.player_form = gctx->GetPlayerActor()->active_form;
    } else if (gctx->next_entrance == 0x5401 && gSettingsContext.skipGiantsCutscene) {
      if (EnFall_CheckMoonRequirements()) {
        gctx->next_entrance = 0x2C02;
        cdata.sub13s[0].entrance_index = 0x2C02;
      } else {
        gctx->next_entrance = 0x2C00;
        gctx->transitionType = 2;
        gctx->field_C535 = 0;
        cdata.next_cutscene_index = 0xfff2;
        cdata.next_transition_type = 2;
      }

      didWarp = true;
    }

    if (didWarp)
      gctx->field_C529_one_to_clear_input = 0x14;
    return didWarp;
  }

  void ForceTempleFlags() {
    game::PersistentSceneCycleFlags* persistentCycleFlags = game::GetPersistentCycleStruct();
    auto& cycleFlags = game::GetCommonData().cycleSceneFlags;
// TODO: Make this smarter, define each fairy chest for now and door and set the switches to be properly set.
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s:\ncycleFlags[woodfall].switch0 0x%08x \ncycleFlags[woodfall].switch1 "
                     "0x%08x \ncycleFlags[woodfall].chest 0x%08x \ncycleFlags[woodfall].clearedRoom "
                     "0x%08x \ncycleFlags[woodfall].collectible 0x%08x \n",
                     __func__, cycleFlags[(u32)game::SceneId::WoodfallTemple].switch0,
                     cycleFlags[(u32)game::SceneId::WoodfallTemple].switch1,
                     cycleFlags[(u32)game::SceneId::WoodfallTemple].clearedRoom,
                     cycleFlags[(u32)game::SceneId::WoodfallTemple].collectible);
#endif
    persistentCycleFlags[(u32)game::SceneId::WoodfallTemple].chest = 0xFFFFFFFF;
    persistentCycleFlags[(u32)game::SceneId::SnowheadTemple].switch1 = 0xFFFFFFFF;
    persistentCycleFlags[(u32)game::SceneId::GreatBayTemple].switch1 = 0xFFFFFFFF;
    persistentCycleFlags[(u32)game::SceneId::StoneTowerTemple].switch1 = 0xFFFFFFFF;
    persistentCycleFlags[(u32)game::SceneId::StoneTowerTempleInverted].switch1 = 0xFFFFFFFF;
  }

  bool EnFall_CheckMoonRequirements() {
    u16 remainsCollected = Settings_CountRemainsCollected();

    if (remainsCollected >= gSettingsContext.masksNeededToEnterMoon) {
      return true;
    }
    return false;
  }
  }

}  // namespace rnd