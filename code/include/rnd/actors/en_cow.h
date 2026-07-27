#pragma once

#include "game/actor.h"
#include "game/common_data.h"
#include "rnd/item_override.h"
#include "rnd/savefile.h"

namespace rnd {
  static const u32 COW_ANY_ENTRANCE = 0x00;

  struct CowEntry {
    u8 gameScene;
    u8 room;
    u32 returnEntrance;
    s16 x, y, z;
    u8 keyScene;
    u8 index;
  };

  bool En_Cow_FillSearchKey(game::act::Actor* actor, game::SceneId scene, ItemOverride_Key* key);
  bool En_Cow_IsMilked(u8 cowIndex);
  void En_Cow_SetMilked(u8 cowIndex);
  bool En_Cow_IsMilkedAndNonRepeatable(ItemOverride* override);
}  // namespace rnd