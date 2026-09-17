#include "rnd/shops.h"

#include "rnd/savefile.h"
#include "rnd/settings.h"

namespace rnd {
  // Prices are carried in field_4 of each shop's message entry (low 10 bits), written by the
  // generator, so the patch does not keep a price table of its own.

  bool Shopsanity_IsSlotPurchased(s32 slot) {
    if (slot < 0 || static_cast<u32>(slot) >= SHOPSANITY_PURCHASE_COUNT)
      return false;
    return (gExtSaveData.shopSlotsPurchased & (1u << slot)) != 0;
  }

  void Shopsanity_SetSlotPurchased(s32 slot) {
    if (slot < 0 || static_cast<u32>(slot) >= SHOPSANITY_PURCHASE_COUNT)
      return;
    gExtSaveData.shopSlotsPurchased |= (1u << slot);
  }

  // Consumables restock forever; everything else is a one-time purchase.
  bool Shopsanity_IsRestockable(const ItemRow* row) {
    switch ((game::ItemId)row->itemId) {
    case game::ItemId::Arrow:
    case game::ItemId::TenArrows:
    case game::ItemId::ThirtyArrows:
    case game::ItemId::FortyArrows:
    case game::ItemId::FiftyArrows:
    case game::ItemId::Bomb:
    case game::ItemId::FiveBombs:
    case game::ItemId::TenBombs:
    case game::ItemId::TwentyBombs:
    case game::ItemId::ThirtyBombs:
    case game::ItemId::Bombchu:
    case game::ItemId::OneBombchu:
    case game::ItemId::OneBombchuAgain:
    case game::ItemId::FiveBombchu:
    case game::ItemId::TenBombchus:
    case game::ItemId::DekuStick:
    case game::ItemId::TenSticks:
    case game::ItemId::TenSticksAgain:
    case game::ItemId::TwentySticks:
    case game::ItemId::ThirtySticks:
    case game::ItemId::DekuNuts:
    case game::ItemId::FiveNuts:
    case game::ItemId::TenNuts:
    case game::ItemId::ThirtyNuts:
    case game::ItemId::FortyNuts:
    case game::ItemId::MagicBean:
    case game::ItemId::PowderKeg:
    case game::ItemId::ChateauRomaniFill:
    case game::ItemId::MilkFill:
    case game::ItemId::GoldDustFill:
    case game::ItemId::SeahorseFill:
    case game::ItemId::RecoveryHeart:
    case game::ItemId::HeroShield:
    // Bottled items that become refills once received (ItemUpgrade_RefillBottle), so they sell like ammo.
    case game::ItemId::GoldDust:        // 0x6A -> 0x93
    case game::ItemId::ChateauRomani:   // 0x6F -> 0x91
    case game::ItemId::Milk:            // 0x60 -> 0x92
    case game::ItemId::HookshotUnused:  // 0x59 Bottle with Red Potion -> 0x5B, see its item table row
    case game::ItemId::RedPotion:
    case game::ItemId::GreenPotion:
    case game::ItemId::BluePotion:
    case game::ItemId::Fairy:
      return true;
    default:
      return false;
    }
  }

  // Lives in the item grid, a bottle, or the mask grid, so the player can lose it again. The empty Bottle is
  // excluded since filling it would put a bought bottle back on the shelf, and trade items are kept on
  // hand-over and across cycle resets.
  static bool Shopsanity_IsLosableItem(game::ItemId id) {
    return id <= game::ItemId::GiantMask && id != game::ItemId::Bottle && !game::ItemIsGear(id) &&
           id != game::ItemId::MapUnused;
  }

  static bool Shopsanity_IsHeld(game::ItemId id) {
    if (game::ItemIsBottled(id))
      return game::HasBottle(id) ||
             (id == game::ItemId::MysteryMilk && game::HasBottle(game::ItemId::MysteryMilkSpoiled));
    if (game::ItemIsMask(id))
      return game::HasMask(id);
    return game::HasItem(id);
  }

  bool Shopsanity_IsSoldOut(const ItemRow* row, s32 purchase) {
    if (row == nullptr || Shopsanity_IsRestockable(row) || !Shopsanity_IsSlotPurchased(purchase))
      return false;
    // Bought before. Anything the inventory can lose comes back once it is gone; the rest (hearts,
    // dungeon items, trade items, equipment, progressive upgrades) stays sold out.
    const game::ItemId id = (game::ItemId)row->itemId;
    return Shopsanity_IsLosableItem(id) ? Shopsanity_IsHeld(id) : true;
  }

  ShopShelf Shopsanity_ResolveShelf(game::SceneId scene, s16 param) {
    for (const ShopShelfAlias& alias : kShopShelfAliases) {
      if (alias.shelf.scene == scene && alias.shelf.param == param) {
        return alias.canonical;
      }
    }
    return {scene, param};
  }

  s32 Shopsanity_GetSlot(game::SceneId scene, s16 param) {
    const ShopShelf shelf = Shopsanity_ResolveShelf(scene, param);
    for (u32 i = 0; i < SHOPSANITY_SLOT_COUNT; ++i) {
      if (kShopSlots[i].scene == shelf.scene && kShopSlots[i].param == shelf.param) {
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