#include "rnd/actors/en_cow.h"

namespace rnd {
  // TODO: Back entrances will need to be adjusted eventually for entrance randomization, as we will not be able to tell
  // which grotto we are coming from.
  static const CowEntry cowTable[] = {
      {(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -311, 0, -97, (u8)game::SceneId::Barn, 0},
      //{(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -82, 0, -127, (u8)game::SceneId::Barn, 1},
      {(u8)game::SceneId::Barn, 0x00, COW_ANY_ENTRANCE, -83, 0, -32, (u8)game::SceneId::Barn, 1},
      {(u8)game::SceneId::BeneathTheWell, 0x09, COW_ANY_ENTRANCE, 2890, 0, -195, (u8)game::SceneId::BeneathTheWell, 3},
      {(u8)game::SceneId::Grottos, 0x0A, 0x5400, 2394, 0, 907, (u8)game::SceneId::TerminaField, 4},
      {(u8)game::SceneId::Grottos, 0x0A, 0x5400, 2466, 0, 952, (u8)game::SceneId::TerminaField, 5},
      {(u8)game::SceneId::Grottos, 0x0A, 0x6800, 2394, 0, 907, (u8)game::SceneId::GreatBayCoast, 6},
      {(u8)game::SceneId::Grottos, 0x0A, 0x6800, 2466, 0, 952, (u8)game::SceneId::GreatBayCoast, 7},
  };
  static_assert(sizeof(cowTable) / sizeof(CowEntry) <= 7, "cowTable outgrew CowMilkedRegister");

  static const u8 cowTableCount = sizeof(cowTable) / sizeof(CowEntry);
  static const CowEntry* En_Cow_FindEntry(game::act::Actor* actor, game::SceneId scene) {
    if (actor == NULL || actor->id != game::act::Id::EnCow)
      return NULL;
    u32 returnEntrance = (u32)(u16)game::GetCommonData().sub13s[0].entrance_index;
    s16 x = (s16)actor->initial_pos.pos.x;
    s16 y = (s16)actor->initial_pos.pos.y;
    s16 z = (s16)actor->initial_pos.pos.z;
    for (u8 i = 0; i < cowTableCount; i++) {
      const CowEntry& entry = cowTable[i];
      if (entry.gameScene != (u8)scene || entry.room != actor->room_number)
        continue;
      if (entry.returnEntrance != COW_ANY_ENTRANCE && entry.returnEntrance != returnEntrance)
        continue;
      if (entry.x != x || entry.y != y || entry.z != z)
        continue;
      return &entry;
    }
    return NULL;
  }

  bool En_Cow_FillSearchKey(game::act::Actor* actor, game::SceneId scene, ItemOverride_Key* key) {
    const CowEntry* entry = En_Cow_FindEntry(actor, scene);
    if (entry == NULL)
      return false;
    #if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: Cow index is %u\n", __func__, entry->index);	
    #endif
    key->scene = entry->keyScene;
    key->type = ItemOverride_Type::OVR_COW;
    key->flag = entry->index;
    return true;
  }

  bool En_Cow_IsMilked(u8 cowIndex) {
    return cowIndex < 8 && (gExtSaveData.cowMilked.raw & 1 << (cowIndex & 0x7)) != 0;
  }

  void En_Cow_SetMilked(u8 cowIndex) {
    if (cowIndex < 8)
      gExtSaveData.cowMilked.raw |= 1 << (cowIndex & 0x7);
  }

  bool En_Cow_IsMilkedAndNonRepeatable(ItemOverride* override) {
    if (override->key.type != ItemOverride_Type::OVR_COW)
      return false;
    if (!En_Cow_IsMilked(override->key.flag))
      return false;
    // False once the item leaves the inventory, so an item the player lost is granted again.
    return ItemOverride_IsItemObtainedOrEmptyBottle(*override);
  }
}  // namespace rnd