#pragma once

#include "common/types.h"
#include "game/scene.h"
#include "rnd/item_override.h"
#include "rnd/item_table.h"

namespace rnd {

  constexpr u32 SHOPSANITY_SLOT_COUNT = 21;

  struct ShopSlot {
    game::SceneId scene;
    s16 param;
    u8 shopItemIndex;
  };

  constexpr ShopSlot kShopSlots[] = {
      // Southern Swamp - Potion Shop (3)   [global 0..2]
      {game::SceneId::PotionShop, 0, 1},
      {game::SceneId::PotionShop, 1, 2},
      {game::SceneId::PotionShop, 2, 3},
      // West Clock Town - General Store / Trading Post (8)   [global 3..10]
      // Shelf layout:  1 2   5 6
      //                3 4   7 8
      {game::SceneId::TradingPost, 10, 1},
      {game::SceneId::TradingPost, 5, 2},
      {game::SceneId::TradingPost, 6, 3},
      {game::SceneId::TradingPost, 3, 4},
      {game::SceneId::TradingPost, 7, 5},
      {game::SceneId::TradingPost, 8, 6},
      {game::SceneId::TradingPost, 9, 7},
      {game::SceneId::TradingPost, 43, 8},
      // West Clock Town - Bomb Shop (4)   [global 11..14]
      {game::SceneId::BombShop, 23, 1},
      {game::SceneId::BombShop, 25, 2},
      {game::SceneId::BombShop, 26, 3},
      {game::SceneId::BombShop, 24, 4},
      // Zora Hall (3)   [global 15..17]
      {game::SceneId::ZoraHallRooms, 27, 1},
      {game::SceneId::ZoraHallRooms, 28, 2},
      {game::SceneId::ZoraHallRooms, 29, 3},
      // Goron Village (3)   [global 18..20]
      {game::SceneId::GoronShop, 30, 1},
      {game::SceneId::GoronShop, 31, 2},
      {game::SceneId::GoronShop, 32, 3},
  };
  static_assert(sizeof(kShopSlots) / sizeof(kShopSlots[0]) == SHOPSANITY_SLOT_COUNT);

  // Purchases tracked alongside the shelves in shopSlotsPurchased, for sellers that are not En_GirlA.
  constexpr s32 SHOPSANITY_MILK_BAR_MILK = SHOPSANITY_SLOT_COUNT;
  constexpr s32 SHOPSANITY_MILK_BAR_CHATEAU = SHOPSANITY_SLOT_COUNT + 1;
  constexpr s32 SHOPSANITY_GORMAN_MILK = SHOPSANITY_SLOT_COUNT + 2;
  // The four Deku merchants (En_Akindonuts). Each one's sale is a single check wherever he sells it.
  constexpr s32 SHOPSANITY_SCRUB_SWAMP = SHOPSANITY_SLOT_COUNT + 3;
  constexpr s32 SHOPSANITY_SCRUB_GORON = SHOPSANITY_SLOT_COUNT + 4;
  constexpr s32 SHOPSANITY_SCRUB_ZORA = SHOPSANITY_SLOT_COUNT + 5;
  constexpr s32 SHOPSANITY_SCRUB_IKANA = SHOPSANITY_SLOT_COUNT + 6;
  constexpr u32 SHOPSANITY_PURCHASE_COUNT = SHOPSANITY_SLOT_COUNT + 7;
  static_assert(SHOPSANITY_PURCHASE_COUNT <= 32);

  struct ShopShelf {
    game::SceneId scene;
    s16 param;
  };

  struct ShopShelfAlias {
    ShopShelf shelf;      // what the shopkeeper actually spawned
    ShopShelf canonical;  // the kShopSlots entry it stands in for
  };

  // Shelves that sell the same check as a kShopSlots entry under a different sShopItemEntries
  // index. The generator already mirrors their text and price onto the canonical location, so on
  // this side they only need to resolve to the same override key and the same purchased bit.
  constexpr ShopShelfAlias kShopShelfAliases[] = {
      // Trading Post, part-timer (after 10pm). Same shelf position as the day stock; the
      // shopkeeper's table at 0x67F868 lists the eight day items followed by the eight night ones.
      {{game::SceneId::TradingPost, 0x0F}, {game::SceneId::TradingPost, 0x09}},
      {{game::SceneId::TradingPost, 0x10}, {game::SceneId::TradingPost, 0x07}},
      {{game::SceneId::TradingPost, 0x2C}, {game::SceneId::TradingPost, 0x2B}},
      {{game::SceneId::TradingPost, 0x0C}, {game::SceneId::TradingPost, 0x08}},
      {{game::SceneId::TradingPost, 0x0B}, {game::SceneId::TradingPost, 0x03}},
      {{game::SceneId::TradingPost, 0x0E}, {game::SceneId::TradingPost, 0x05}},
      {{game::SceneId::TradingPost, 0x11}, {game::SceneId::TradingPost, 0x06}},
      {{game::SceneId::TradingPost, 0x12}, {game::SceneId::TradingPost, 0x0A}},
      // Goron Shop in spring (table at 0x683910).
      {{game::SceneId::GoronShop, 0x23}, {game::SceneId::GoronShop, 0x20}},
      {{game::SceneId::GoronShop, 0x22}, {game::SceneId::GoronShop, 0x1F}},
      {{game::SceneId::GoronShop, 0x21}, {game::SceneId::GoronShop, 0x1E}},
      // The stolen bomb bag Sakon fences at the Curiosity Shop (En_Fsn::init, 0x320454) is the
      // Bomb Shop's big bag. Keying it there gives both shelves one override and one purchased bit.
      {{game::SceneId::CuriosityShop, 0x15}, {game::SceneId::BombShop, 0x18}},
  };

  // Maps an aliased shelf to its canonical kShopSlots identity; anything else comes back unchanged.
  ShopShelf Shopsanity_ResolveShelf(game::SceneId scene, s16 param);

  struct ShopItemEntry {
    s16 objectId;
    s16 objectTableIndex;
    s16 itemCount;
    u16 textId;        // description shown while the item is highlighted
    u16 choiceTextId;  // "Buy / Don't buy" prompt
    u16 pad_0a;
    GetItemID getItemId;
    void* canBuyFunction;  // CanBuyFunc*, typed in en_girla.h
    void* buyFunction;     // BuyFunc*
    void* buyFanfareFunction;
  };
  static_assert(sizeof(ShopItemEntry) == 0x1C);
  static_assert(offsetof(ShopItemEntry, textId) == 0x06);
  static_assert(offsetof(ShopItemEntry, getItemId) == 0x0C);
  static_assert(offsetof(ShopItemEntry, buyFunction) == 0x14);

  struct ShopObjectModel {
    u8 cmbIndex;   // 0xFF = none
    u8 cmabIndex;  // 0xFF = none
  };
  struct ShopObjectTableEntry {
    s16 objectId;
    ShopObjectModel models[3];
  };
  static_assert(sizeof(ShopObjectTableEntry) == 8);

  // Resolves aliases first, so every alias of a shelf shares its slot.
  s32 Shopsanity_GetSlot(game::SceneId, s16);

  // The stolen bomb bag is a single item with two possible owners: Sakon fences it at the
  // Curiosity Shop when granny is not saved, otherwise it goes back to the Bomb Shop. The two
  // are separate locations with separate override keys, so collecting either has to retire
  // both -- otherwise a cycle reset hands the same bag over twice.
  constexpr s16 kBigBombBagShelfParam = 24;
  constexpr u8 kCuriosityBombBagScene = 0x0D;
  constexpr u8 kCuriosityBombBagFlag = 0x1D;

  // En_Fsn itself as a base item, so the shelf has no shopsanity slot of its own.
  constexpr s16 kAllNightMaskShelfParam = 0x13;

  bool Shopsanity_IsSlotPurchased(s32 slot);

  bool Shopsanity_IsRestockable(const ItemRow* row);

  bool Shopsanity_IsSoldOut(const ItemRow* row, s32 purchase);

  void Shopsanity_SetSlotPurchased(s32 slot);

  s32 Shopsanity_GetShopItemIndex(s32);

  const ShopItemEntry* Shopsanity_GetVanillaEntry(s16);

}  // namespace rnd