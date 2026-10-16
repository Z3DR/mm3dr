#include "rnd/extdata.h"
#include "rnd/item_override.h"
#include "rnd/overrides/override_state.h"
#include "rnd/savefile.h"

namespace rnd {
  void ItemOverride_PushPendingFairyRewardItem(game::GlobalContext* gctx, game::act::GreatFairy* fromActor,
                                               s16 incomingItemId) {
    ItemOverride override = {0};
    s32 incomingNegative = incomingItemId < 0;
    if (fromActor != NULL && incomingItemId != 0) {
      s16 getItemId = incomingNegative ? -incomingItemId : incomingItemId;
      override = ItemOverride_Lookup(fromActor, (u16)gctx->scene, getItemId);
    }

    if (override.key.all == 0) {
      ItemOverride_Clear();
      return;
    }

    ItemOverride_PushPendingOverride(override);
    // if (override.value.getItemId == 0x12) {
    //   rActiveItemRow->effectArg1 = override.key.all >> 16;
    //   rActiveItemRow->effectArg2 = override.key.all & 0xFFFF;
    // }
    return;
  }

  void ItemOverride_RevealMapBasedOnId(u8 getItemMapId) {
    game::SaveData& saveData = game::GetCommonData().save;
    switch (getItemMapId) {
    case 0xB4:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_CLOCK_TOWN = 1;
      util::GetPointer<void(u8)>(0x548260)(0x0);
      break;
    case 0xB5:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_WOODFALL = 1;
      util::GetPointer<void(u8)>(0x548260)(0x1);
      break;
    case 0xB6:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_SNOWHEAD = 1;
      util::GetPointer<void(u8)>(0x548260)(0x2);
      break;
    case 0xB7:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_ROMANI_RANCH = 1;
      util::GetPointer<void(u8)>(0x548260)(0x3);
      break;
    case 0xB8:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_GREAT_BAY = 1;
      util::GetPointer<void(u8)>(0x548260)(0x4);
      break;
    case 0xB9:
      saveData.week_event_reg_35.WEEKEVENTREG_TINGLE_MAP_BOUGHT_STONE_TOWER = 1;
      util::GetPointer<void(u8)>(0x548260)(0x5);
      break;
    default:
      break;
    }
    return;
  }

  extern "C" {
  void ItemOverride_GetFairyRewardItem(game::GlobalContext* gctx, game::act::GreatFairy* fromActor,
                                       s16 incomingItemId) {
    int fairyEntrance = game::GetCommonData().sub1.entrance;
    if (fairyEntrance == 0x4600) {
      if (gExtSaveData.fairyRewards.nct == 0) {
        ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0x86);
        gExtSaveData.fairyRewards.nct = 1;
        return;
      } else if (gExtSaveData.fairyRewards.nct == 1 && game::HasMask(game::ItemId::DekuMask)) {
        ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0x0E);
        gExtSaveData.fairyRewards.nct = 2;
        return;
      }
      return;
    } else if (fairyEntrance == 0x4610 && gExtSaveData.fairyRewards.woodfall != 1) {
      gExtSaveData.fairyRewards.woodfall = 1;
      ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0x2C);
      return;
    } else if (fairyEntrance == 0x4620 && gExtSaveData.fairyRewards.snowhead != 1) {
      gExtSaveData.fairyRewards.snowhead = 1;
      ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0x2B);
      return;
    } else if (fairyEntrance == 0x4630 && gExtSaveData.fairyRewards.great_bay != 1) {
      gExtSaveData.fairyRewards.great_bay = 1;
      ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0xB2);
      return;
    } else if (fairyEntrance == 0x4640 && gExtSaveData.fairyRewards.ikana != 1) {
      gExtSaveData.fairyRewards.ikana = 1;
      ItemOverride_PushPendingFairyRewardItem(gctx, fromActor, 0x3B);
      return;
    }
  }

  bool ItemOverride_CheckTingleMaps(u16 mapId, game::GlobalContext* gctx) {
    switch (mapId) {
    case 0x1:
      if (gExtSaveData.tingleMaps.clock_town_map_get == 0) {
        gExtSaveData.tingleMaps.clock_town_map_get = 1;
        return true;
      }
      break;
    case 0x2:
      if (gExtSaveData.tingleMaps.woodfall_map_get == 0) {
        gExtSaveData.tingleMaps.woodfall_map_get = 1;
        return true;
      }
      break;
    case 0x4:
      if (gExtSaveData.tingleMaps.snowhead_map_get == 0) {
        gExtSaveData.tingleMaps.snowhead_map_get = 1;
        return true;
      }
      break;
    case 0x8:
      if (gExtSaveData.tingleMaps.romani_map_get == 0) {
        gExtSaveData.tingleMaps.romani_map_get = 1;
        return true;
      }
      break;
    case 0x10:
      if (gExtSaveData.tingleMaps.great_bay_map_get == 0) {
        gExtSaveData.tingleMaps.great_bay_map_get = 1;
        return true;
      }
      break;
    case 0x20:
      if (gExtSaveData.tingleMaps.stone_tower_map_get == 0) {
        gExtSaveData.tingleMaps.stone_tower_map_get = 1;
        return true;
      }
      break;
    default:
      return false;
    }
    return false;
  }

  u8 ItemOverride_CheckBossStatus() {
    game::SceneId scene = GetContext().gctx->scene;
    switch (scene) {
    case game::SceneId::OdolwaLair:
      return gExtSaveData.givenItemChecks.odolwaDefeated == 0 ? 1 : 0;
    case game::SceneId::GohtLair:
      return gExtSaveData.givenItemChecks.gohtDefeated == 0 ? 2 : 0;
    case game::SceneId::GyorgLair:
      return gExtSaveData.givenItemChecks.gyorgDefeated == 0 ? 3 : 0;
    case game::SceneId::TwinmoldLair:
      return gExtSaveData.givenItemChecks.twinmoldDefeated == 0 ? 4 : 0;
    default:
      return 0;
    }
  }
  }
}  // namespace rnd
