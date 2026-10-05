#include "rnd/actors/en_trt.h"
#include "rnd/item_override.h"
#include "rnd/shops.h"

namespace rnd {
  extern "C" {
  void En_Trt_KotakeMushroomSale(u16 textId) {
    if (textId != kKotakeFreePotionTextId) {
      return;
    }

    ItemOverride_Key key;
    key.all = 0;
    key.scene = (u8)game::SceneId::PotionShop;
    key.type = ItemOverride_Type::OVR_SHOP;
    key.flag = kKotakeFreePotionFlag;

    ItemOverride_SetPendingShopItem(key, game::SceneId::PotionShop);
  }
  }
}  // namespace rnd