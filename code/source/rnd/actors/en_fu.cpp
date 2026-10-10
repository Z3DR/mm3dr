#include "rnd/actors/en_fu.h"

namespace rnd {
  ItemOverride_Key En_Fu_GetItemKeyBasedOnDay(game::act::Actor* actor, s16 getItemId) {
    ItemOverride_Key key = {.all = 0};
    s16 currentDay = game::GetCurrentDay();
    if (currentDay == 1 || currentDay == 2) {
      if (currentDay == 1 && gExtSaveData.miniGameCompletion.dayOneHoneyAndDarlingCompleted == 1)
        return key;  // Key not set yet.
      else if (currentDay == 2 && gExtSaveData.miniGameCompletion.dayTwoHoneyAndDarlingCompleted == 1)
        return key;  // Key not set.
      key.scene = (u8)game::SceneId::HoneyAndDarling;
      key.type = ItemOverride_Type::OVR_MINI_GAME;
      key.flag = (u8)currentDay;
    } else {
      key.scene = (u8)game::SceneId::HoneyAndDarling;
      key.type = ItemOverride_Type::OVR_BASE_ITEM;
      key.flag = getItemId;
    }
    return key;
  }
}  // namespace rnd
