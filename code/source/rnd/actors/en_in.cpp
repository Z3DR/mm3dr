#include "rnd/actors/en_in.h"

namespace rnd {
  using HasBottleFn = bool();

  extern "C" {
  // The Gorman brothers sell milk as often as the player likes, so their shuffled check would be handed
  // over on every purchase. Refuse the sale the same way the seller refuses it without an empty bottle.
  bool En_In_CanBuyMilk() {
    ItemOverride_Key key = {.all = 0};
    key.scene = (u8)game::SceneId::GormanTrack;
    key.type = ItemOverride_Type::OVR_BASE_ITEM;
    key.flag = (u8)GetItemID::GI_BOTTLE_MILK_REFILL;
    const ItemOverride override = ItemOverride_LookupByKey(key);
    if (override.key.all != 0 &&
        Shopsanity_IsSoldOut(ItemTable_GetItemRow(override.value.getItemId), SHOPSANITY_GORMAN_MILK)) {
      return false;
    }
    return util::GetPointer<HasBottleFn>(0x33362C)();  // vanilla: does Link have an empty bottle
  }
  }
}  // namespace rnd
