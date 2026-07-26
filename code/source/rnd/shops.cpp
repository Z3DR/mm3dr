#include "rnd/shops.h"

#include "rnd/settings.h"

namespace rnd {
  s32 rShopsanityPrices[20] = {0};

  s32 Shopsanity_GetPrice(u32 slot) {
    if (slot >= SHOPSANITY_SLOT_COUNT)
      return 0;

    const s32 price = rShopsanityPrices[slot];

    if (gSettingsContext.shopsanityPrices == static_cast<u8>(ShopsanityPriceSetting::SHOPSANITY_PRICE_AFFORDABLE)) {
      // Affordable mode: cap prices low so shuffled shop items are always buyable.
      constexpr s32 kAffordableCap = 20;
      if (price < 0 || price > kAffordableCap)
        return kAffordableCap;
    }
    return price;
  }

  s32 Shopsanity_GetSlot(game::SceneId scene, s16 param) {
    for (u32 i = 0; i < SHOPSANITY_SLOT_COUNT; ++i) {
      if (kShopSlots[i].scene == scene && kShopSlots[i].param == param) {
        return static_cast<s32>(i);
      }
    }
    return -1;  // not a shuffleable shop item
  }

  s32 Shopsanity_GetShopItemIndex(s32 slot) {
    if (slot < 0 || static_cast<u32>(slot) >= SHOPSANITY_SLOT_COUNT)
      return -1;
    return kShopSlots[slot].shopItemIndex;
  }

  const ShopItemEntry* Shopsanity_GetVanillaEntry(s16 param) {
    // Generous bound: the highest param in kShopSlots is 43.
    if (param < 0 || param >= 0x40)
      return nullptr;
    return &util::GetPointer<ShopItemEntry>(0x676EF4)[param];  // sShopItemEntries
  }

}  // namespace rnd