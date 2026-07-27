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

  // TODO: Back entrances will need to be adjusted eventually for entrance randomization, as we will not be able to tell
  // which grotto we are coming from.
  static const CowEntry cowTable[] = {
      {(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -311, 0, -97, (u8)game::SceneId::Barn, 0},
      {(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -82, 0, -127, (u8)game::SceneId::Barn, 1},
      {(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -83, 0, -32, (u8)game::SceneId::Barn, 2},
      {(u8)game::SceneId::BeneathTheWell, 0x09, COW_ANY_ENTRANCE, 2890, 0, -195, (u8)game::SceneId::BeneathTheWell, 3},
      {(u8)game::SceneId::Grottos, 0x0A, 0x5400, 2394, 0, 907, (u8)game::SceneId::TerminaField, 4},
      {(u8)game::SceneId::Grottos, 0x0A, 0x5400, 2466, 0, 952, (u8)game::SceneId::TerminaField, 5},
      {(u8)game::SceneId::Grottos, 0x0A, 0x6800, 2394, 0, 907, (u8)game::SceneId::GreatBayCoast, 6},
      {(u8)game::SceneId::Grottos, 0x0A, 0x6800, 2466, 0, 952, (u8)game::SceneId::GreatBayCoast, 7},
  };
  static_assert(sizeof(cowTable) / sizeof(CowEntry) <= 8, "cowTable outgrew CowMilkedRegister");

  static const u8 cowTableCount = sizeof(cowTable) / sizeof(CowEntry);

  bool En_Cow_FillSearchKey(game::act::Actor* actor, game::SceneId scene, ItemOverride_Key* key);
  bool En_Cow_IsMilked(u8 cowIndex);
  void En_Cow_SetMilked(u8 cowIndex);
  bool En_Cow_IsMilkedAndNonRepeatable(ItemOverride* override);
}  // namespace rnd