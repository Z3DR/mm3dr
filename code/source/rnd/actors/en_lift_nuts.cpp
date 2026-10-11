#include "rnd/actors/en_lift_nuts.h"

namespace rnd {
  ItemOverride_Key EnLiftNuts_GetItemKeyBasedOnDay(s16 getItemId) {
    ItemOverride_Key key = {.all = 0};
    s16 currentDay = game::GetCurrentDay();
    const bool isDailyReward =
        getItemId == (s16)GetItemID::GI_FISHING_HOLE_PASS || getItemId == -(s16)GetItemID::GI_FISHING_HOLE_PASS;
    if (isDailyReward && (currentDay == 1 || currentDay == 2)) {
      if (currentDay == 1 && gExtSaveData.miniGameCompletion.dayOneDekuPlaygroundCompleted == 1)
        return key;  // Already rewarded: vanilla prize.
      else if (currentDay == 2 && gExtSaveData.miniGameCompletion.dayTwoDekuPlaygroundCompleted == 1)
        return key;  // Already rewarded: vanilla prize.
      key.scene = (u8)game::SceneId::DekuScrubPlayground;
      key.type = ItemOverride_Type::OVR_MINI_GAME;
      key.flag = (u8)currentDay;
    } else {
      key.scene = (u8)game::SceneId::DekuScrubPlayground;
      key.type = ItemOverride_Type::OVR_BASE_ITEM;
      key.flag = getItemId;
    }
    return key;
  }
}  // namespace rnd
