#include "rnd/shops.h"

#include "rnd/settings.h"

namespace rnd {
  // Prices are carried in field_4 of each shop's message entry (low 10 bits), written by the
  // generator, so the patch does not keep a price table of its own.

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