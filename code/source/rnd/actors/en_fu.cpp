#include "rnd/actors/en_fu.h"

namespace rnd {
  ItemOverride_Key En_Fu_GetItemKeyBasedOnDay(game::act::Actor* actor) {
    ItemOverride_Key key = {.all = 0};
    s16 currentDay = game::GetCurrentDay();
    if (currentDay == 1 || currentDay == 2) {
      key.scene = (u8)game::SceneId::HoneyAndDarling;
      key.type = ItemOverride_Type::OVR_MINI_GAME;
      key.flag = (u8)currentDay;
    } else {
      key.scene = (u8)game::SceneId::HoneyAndDarling;
      key.type = ItemOverride_Type::OVR_COLLECTABLE;
      key.flag = actor->overlay_info->info->flags;
    }
    return key;
  }
}  // namespace rnd
