#include "rnd/actors/en_akindonuts.h"
#include "rnd/item_table.h"
#include "rnd/shops.h"

namespace rnd {
  extern "C" {
  bool En_Akindonuts_CheckBombBagExtData() {
    return gExtSaveData.givenItemChecks.enAkindonutsBombBagGiven.Value();
  }
  }

  static s32 En_Akindonuts_GetSalePurchase(ItemOverride_Key key) {
    if (key.type != ItemOverride_Type::OVR_BASE_ITEM)
      return -1;
    switch ((GetItemID)key.flag) {
    case GetItemID::GI_MAGIC_BEAN:
      return SHOPSANITY_SCRUB_SWAMP;
    case GetItemID::GI_BOMB_BAG_40:
      return SHOPSANITY_SCRUB_GORON;
    case GetItemID::GI_POTION_GREEN:
      return SHOPSANITY_SCRUB_ZORA;
    case GetItemID::GI_POTION_BLUE:
      return SHOPSANITY_SCRUB_IKANA;
    default:
      return -1;
    }
  }

  bool En_Akindonuts_IsSaleBoughtAndNonRepeatable(const ItemOverride* override) {
    const s32 purchase = En_Akindonuts_GetSalePurchase(override->key);
    return purchase >= 0 && Shopsanity_IsSoldOut(ItemTable_GetItemRow(override->value.getItemId), purchase);
  }

  void En_Akindonuts_SetSaleBought(ItemOverride_Key key) {
    Shopsanity_SetSlotPurchased(En_Akindonuts_GetSalePurchase(key));
  }
}  // namespace rnd
