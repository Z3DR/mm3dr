#pragma once

#include "common/types.h"
#include "game/scene.h"
#include "rnd/item_override.h"

namespace rnd {

  // Number of shuffleable shop slots across all shops (size of rShopsanityPrices).
  constexpr u32 SHOPSANITY_SLOT_COUNT = 20;

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
      // West Clock Town - Bomb Shop (3)   [global 11..13]
      {game::SceneId::BombShop, 23, 1},
      {game::SceneId::BombShop, 25, 2},
      {game::SceneId::BombShop, 26, 3},
      // Zora Hall (3)   [global 14..16]
      {game::SceneId::ZoraHallRooms, 27, 1},
      {game::SceneId::ZoraHallRooms, 28, 2},
      {game::SceneId::ZoraHallRooms, 29, 3},
      // Goron Village (3)   [global 17..19]
      {game::SceneId::GoronShop, 30, 1},
      {game::SceneId::GoronShop, 31, 2},
      {game::SceneId::GoronShop, 32, 3},
  };
  static_assert(sizeof(kShopSlots) / sizeof(kShopSlots[0]) == SHOPSANITY_SLOT_COUNT);

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

  extern "C" s32 rShopsanityPrices[20];

  s32 Shopsanity_GetPrice(u32);

  s32 Shopsanity_GetSlot(game::SceneId, s16);

  s32 Shopsanity_GetShopItemIndex(s32);

  const ShopItemEntry* Shopsanity_GetVanillaEntry(s16);

}  // namespace rnd