#include "rnd/actors/en_cow.h"

namespace rnd {

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