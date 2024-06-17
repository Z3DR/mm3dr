#include "rnd/item_table.h"
#include <stddef.h>
#include "rnd/actors/item00.h"
#include "rnd/item_effect.h"
#include "rnd/item_override.h"
#include "rnd/item_upgrade.h"
#include "rnd/rheap.h"

#include "z3d/z3DVec.h"

namespace rnd {

#define ITEM_ROW(baseItemId_, chestType_, itemId_, textId_, objectId_, objectModelIdx_, cmabIndex_, objectModelIdx2_,  \
                 cmabIndex2_, special_, graphicId_, upgrade_, effect_, effectArg1_, effectArg2_, scale_)               \
  {                                                                                                                    \
    .baseItemId = baseItemId_, .chestType = chestType_, .itemId = itemId_, .textId = textId_, .objectId = objectId_,   \
    .objectModelIdx = objectModelIdx_, .cmabIndex = cmabIndex_, .objectModelIdx2 = objectModelIdx2_,                   \
    .cmabIndex2 = cmabIndex2_, .special = special_, .graphicId = graphicId_, .upgrade = upgrade_, .effect = effect_,   \
    .effectArg1 = effectArg1_, .effectArg2 = effectArg2_, .scale = scale_                                              \
  }

  ItemRow rItemTable[] = {
      [0x00] =
          ITEM_ROW((u32)GetItemID::GI_NONE, ChestType::WOODEN_SMALL, 0xFF, 0x00C4, 0x0000, 0x00, (s8)0xFF, (s8)0xFF,
                   (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE, (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None,
                   (s16)-1, (s16)-1, 0.015f),  // None Item - So we don't have to do GetItemId-1 everywhere.

      [0x01] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::OneRupee, 0x00C4,
                        0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GREEN_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        0.015f),  // Green Rupee

      [0x02] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiveRupees, 0x0002,
                        0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BLUE_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.015f),  // Blue Rupee
      // BREAKS
      [0x03] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TenRupees, 0x0003,
                        0x0001, 0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RED_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.015f),  // Ten Rupees

      [0x04] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TwentyRupees, 0x0004,
                        0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RED_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.015f),  // Red Rupee

      [0x05] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiftyRupees, 0x0005,
                        0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_PURPLE_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        0.015f),  // Purple Rupee

      [0x06] =
          ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::OneHundredRupees, 0x0006,
                   0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SILVER_RUPEE,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.015f),  // Silver Rupee

      [0x07] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TwoHundredRupees,
                        0x0007, 0x0001, 0x8F, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_RUPEE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.015f),  // Gold Rupee

      [0x08] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::AdultWallet, 0x0008,
                        0x000A8, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ADULT_WALLET,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_FillWalletUpgrade, (s16)1, (s16)-1,
                        0.1f),  // Adult Wallet

      [0x09] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GiantWallet, 0x0009,
                        0x000A8, 0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GIANT_WALLET,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_FillWalletUpgrade, (s16)2, (s16)-1,
                        10.00f),  // Giant Wallet

      [0x0A] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::RecoveryHeart, 0x000A,
                        0x00090, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RECOVERY_HEART,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Recovery Heart

      [0x0B] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::RecoveryHeart, 0x000B,
                        0x00090, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RECOVERY_HEART,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Recovery Heart

      [0x0C] =
          ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::PieceOfHeartAgain, 0x000C,
                   0x00096, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HEART_PIECE,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Piece of Heart

      [0x0D] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_BIG, (u8)game::ItemId::HeartContainer, 0x000D,
                        0x00096, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HEART_CONTAINER,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Heart Container

      [0x0E] =
          ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::SmallMagicAccumulator,
                   0x00C8, 0x000A4, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_MAGIC_JAR,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Small Magic Jar

      [0x0F] =
          ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::BigMagicAccumulator,
                   0x000CC, 0x000A4, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BIG_MAGIC_JAR,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Big Magic Jar

      [0x10] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::RecoveryHeart, 0x0010,
                        0x00090, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RECOVERY_HEART,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Recovery Heart - Broken Text

      [0x11] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::None, 0x0011, 0x00000,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Get Fairy Text

      [0x12] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::None, 0x0012, 0x0000,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_IceTrap, (s16)-1, (s16)-1,
                        10.00f),  // Ice Trap
                                  // Overriding Errornous text + Recovery Heart
      [0x13] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::RecoveryHeart, 0x0013,
                        0x00090, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RECOVERY_HEART,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Recovery Heart - Broken Text

      [0x14] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::Bomb, 0x0014, 0x0001,
                        0x05, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        0.015f),  // Bomb (1)

      [0x15] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiveBombs, 0x0015,
                        0x00001, 0x05, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombs (5)

      [0x16] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TenBombs, 0x0016,
                        0x00001, 0x05, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombs (10)

      [0x17] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TwentyBombs, 0x0017,
                        0x00001, 0x05, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombs (20)

      [0x18] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::ThirtyBombs, 0x0018,
                        0x00001, 0x05, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombs (30)

      [0x19] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::DekuStick, 0x0019,
                        0x009F, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DEKU_STICK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Deku Stick

      [0x1A] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TenBombchus, 0x001A,
                        0x00B0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMBCHU,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombchus (10)

      [0x1B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BombBag, 0x001B, 0x00098,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMB_BAG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Bomb Bag

      [0x1C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BigBombBag, 0x001C,
                        0x00098, 0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BIG_BOMB_BAG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveUpgrade, (s16)2, (s16)1,
                        10.00f),  // Big Bomb Bag

      [0x1D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BiggestBombBag, 0x001D,
                        0x00098, 0x02, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BIGGEST_BOMB_BAG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveUpgrade, (s16)3, (s16)1,
                        10.00f),  // Biggest Bomb Bag

      [0x1E] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TenArrows, 0x001E,
                        0x00001, 0x83, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SINGLE_ARROW,
                        (rnd::upgradeFunc)ItemUpgrade_ArrowsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Arrows (10)

      [0x1F] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::ThirtyArrows, 0x001F,
                        0x00001, 0x83, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BUNDLE_ARROW_SMALL,
                        (rnd::upgradeFunc)ItemUpgrade_ArrowsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Arrows (30)

      [0x20] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FortyArrows, 0x0020,
                        0x00001, 0x84, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BUNDLE_ARROW_LARGE,
                        (rnd::upgradeFunc)ItemUpgrade_ArrowsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Arrows (40)

      [0x21] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiftyArrows, 0x0021,
                        0x00001, 0x85, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BUNDLE_ARROW_LARGE,
                        (rnd::upgradeFunc)ItemUpgrade_ArrowsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Arrows (50)

      [0x22] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Arrow, 0x0022, 0x00BF,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HERO_BOW_AND_ARROW,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)0, (s16)-1, 10.00f),  // Hero's Bow

      [0x23] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::LargeQuiver, 0x0023,
                        0x00097, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_LARGE_QUIVER,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveUpgrade, (s16)2, (s16)0,
                        10.00f),  // Large Quiver

      [0x24] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BiggestQuiver, 0x0024,
                        0x00097, 0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BIGGEST_QUIVER,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveUpgrade, (s16)3, (s16)0,
                        10.00f),  // Biggest Quiver

      [0x25] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::FireArrow, 0x0025, 0x00121,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_FIRE_ARROW,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)3, (s16)0, 10.00f),  // Fire Arrow

      [0x26] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::IceArrow, 0x0026, 0x00121,
                        0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ICE_ARROW,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)3, (s16)0, 10.00f),  // Ice Arrow

      [0x27] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::LightArrow, 0x0027,
                        0x00121, 0x02, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_LIGHT_ARROW,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)3, (s16)0, 10.00f),  // Light Arrow

      [0x28] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::DekuNuts, 0x0028,
                        0x00001, 0x8D, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DEKU_NUT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Deku Nuts (1)

      [0x29] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiveNuts, 0x0029,
                        0x00001, 0x8D, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DEKU_NUT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Deku Nuts (5)

      [0x2A] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::TenNuts, 0x002A,
                        0x00001, 0x8D, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DEKU_NUT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Deku Nuts (10)

      [0x2B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BigMagicAccumulator,
                        0x000CC, 0x00001, 0x8C, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDoubleMagic, (s16)-1, (s16)-1,
                        10.00f),  // Double Magic

      [0x2C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BigMagicAccumulator,
                        0x000CA, 0x00001, 0x18, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveGreatSpin, (s16)-1, (s16)-1,
                        10.00f),  // Great Spin Attack

      [0x2D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::OdolwaRemains, 0x0055,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ODOLWAS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)0, (s16)-1,
                        10.00f),  // Odolwa's Remains TODO: CHECK MODEL ID

      [0x2E] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::OneBombchu, 0x002E,
                        0x00B0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMBCHU,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombchu (1)

      [0x2F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GohtRemains, 0x0056,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOHTS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)1, (s16)-1,
                        10.00f),  // Goht's Remains TODO: CHECK MODEL ID

      [0x30] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GyorgRemains, 0x0057,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GYORGS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)2, (s16)-1,
                        10.00f),  // Gyrog's Remains TODO: CHECK MODEL ID

      [0x31] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::TwinmoldRemains, 0x0058,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TWINMOLDS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)3, (s16)-1,
                        10.00f),  // Twinmold's Remains TODO: CHECK MODEL ID

      [0x32] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::HeroShield, 0x0032,
                        0x000B3, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HERO_SHIELD,
                        (rnd::upgradeFunc)ItemUpgrade_CheckShield, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Hero Sheild

      [0x33] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MirrorShield, 0x0033,
                        0x000C3, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MIRROR_SHIELD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Mirror Sheild

      [0x34] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::PowderKeg, 0x0034, 0x001CA,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_POWDER_KEG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Powder Keg

      [0x35] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MagicBean, 0x0035, 0x000C6,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MAGIC_BEAN,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Magic Beans

      [0x36] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::OneBombchu, 0x0036,
                        0x00B0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMBCHU,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombchu (1)

      [0x37] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KokiriSword, 0x0037,
                        0x00148, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_KOKIRI_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveKokiriSword, (s16)-1, (s16)-1,
                        10.00f),  // Kokiri Sword

      [0x38] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::RazorSword, 0x0038,
                        0x001F9, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RAZOR_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveRazorSword, (s16)-1, (s16)-1,
                        10.00f),  // Razor Sword

      [0x39] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GildedSword, 0x0039,
                        0x001FA, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GILDED_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveGildedSword, (s16)-1, (s16)-1,
                        10.00f),  // Gilded Sword

      [0x3A] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FiveBombchu, 0x003A,
                        0x000B0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMBCHU,
                        (rnd::upgradeFunc)ItemUpgrade_BombsToRupee, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombchus (5)

      [0x3B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GreatFairySword, 0x003B,
                        0x001FB, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GREAT_FAIRY_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Great Fairy Sword

      [0x3C] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL, (u8)game::ItemId::SmallKey, 0x003C,
                        0x00086, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Small Key

      [0x3D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::BossKey, 0x003D,
                        0x00092, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOSS_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Boss Key

      [0x3E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Map, 0x003E, 0x000A0,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Map

      [0x3F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Compass, 0x003F,
                        0x00091, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COMPASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Compass

      [0x40] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KokiriSword, 0x009C,
                        0x00148, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveKokiriSword, (s16)-1, (s16)-1,
                        10.00f),  // Kokiri Sword

      [0x41] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Hookshot, 0x0041, 0x00BF,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HOOKSHOT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Hookshot

      [0x42] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_BIG, (u8)game::ItemId::LensOfTruth, 0x0042,
                        0x00C0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_LENS_OF_TRUTH,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Lens of Truth

      [0x43] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::PictographBox, 0x0043,
                        0x00228, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_PICTOGRAPH_BOX,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Pictograph Box

      [0x44] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_SMALL, (u8)game::ItemId::GoldSkulltula, 0x0052,
                        0x00125, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_SKULLTULA,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSkulltula, (s16)0, (s16)-1,
                        10.00f),  // Gold Skulltula - Swamp

      [0x45] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::RecoveryHeart, 0x0045,
                        0x00090, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RECOVERY_HEART,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Recovery Heart - Broken Text

      [0x46] =
          ITEM_ROW(0xFF, ChestType::WOODEN_BIG, 0xFF, 0xFF, 0x00098, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                   DrawGraphicItemID::DI_NONE, (rnd::upgradeFunc)ItemUpgrade_BombBag, ItemEffect_None, (s16)-1, (s16)-1,
                   10.00f),  // Progressive Bomb Bag

      [0x47] =
          ITEM_ROW(0xFF, ChestType::WOODEN_BIG, 0xFF, 0xFF, 0x00097, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                   DrawGraphicItemID::DI_NONE, (rnd::upgradeFunc)ItemUpgrade_Quiver, ItemEffect_None, (s16)-1, (s16)-1,
                   10.00f),  // Progressive Quiver

      [0x48] =
          ITEM_ROW(0xFF, ChestType::WOODEN_BIG, 0xFF, 0xFF, 0x000A8, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                   DrawGraphicItemID::DI_NONE, (rnd::upgradeFunc)ItemUpgrade_Wallet, ItemEffect_None, (s16)-1, (s16)-1,
                   10.00f),  // Progressive Wallet

      [0x49] = ITEM_ROW(0xFF, ChestType::WOODEN_BIG, 0xFF, 0xFF, 0x000A4, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                        (DrawGraphicItemID)0x000A4, (rnd::upgradeFunc)ItemUpgrade_Magic,
                        ItemEffect_GiveProgressiveMagic, (s16)-1, (s16)-1, 10.00f),  // Progressive Magic

      [0x4A] =
          ITEM_ROW(0xFF, ChestType::WOODEN_BIG, 0xFF, 0xFF, 0x001FA, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                   DrawGraphicItemID::DI_NONE, (rnd::upgradeFunc)ItemUpgrade_Sword, ItemEffect_None, (s16)-1, (s16)-1,
                   10.00f),  // Progressive Sword

      [0x4B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SonataOfAwakening, 0x1B9E,
                        0x00000, 0x56, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)1, (s16)-1,
                        10.00f),  // Sonata Of Awakening

      [0x4C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Ocarina, 0x004C, 0x000B5,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_OCARINA_OF_TIME,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Ocarina

      [0x4D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GoronLullaby, 0x1B9F,
                        0x00000, 0x56, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)2, (s16)-1,
                        10.00f),  // Goron Lullaby

      [0x4E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::NewWaveBossaNova, 0x1BA0,
                        0x00000, 0x56, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)3, (s16)-1,
                        10.00f),  // New Wave Bossanova

      [0x4F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::ElegyOfEmptiness, 0x1BA1,
                        0x00000, 0x56, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)4, (s16)-1,
                        10.00f),  // Elegy of Emptiness

      [0x50] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BombersNotebook, 0x0050,
                        0x00253, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOMBERS_NOTEBOOK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bombers Notebook

      [0x51] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::OathToOrder, 0x1BA2,
                        0x00000, 0x58, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)5, (s16)-1,
                        10.00f),  // Oath To Order

      [0x52] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::GoldSkulltula, 0x0052,
                        0x00125, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_SKULLTULA,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Gold Skulltula - Messed Up Item

      [0x53] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SongOfTime, 0x1BA4,
                        0x00000, 0x58, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)6, (s16)-1,
                        10.00f),  // Song Of Time

      [0x54] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SongOfHealing, 0x1BA5,
                        0x00000, 0x58, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)7, (s16)-1,
                        10.00f),  // Song Of Healing

      [0x55] = ITEM_ROW((u32)GetItemID::GI_HEART_CONTAINER, ChestType::WOODEN_BIG, (u8)game::ItemId::OdolwaRemains,
                        0x0055, 0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                        DrawGraphicItemID::DI_ODOLWAS_REMAINS, (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask,
                        (s16)0, (s16)-1, 10.00f),  // Odolwa's Remains TODO: FIND MODEL

      [0x56] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GohtRemains, 0x0056,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOHTS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)1, (s16)-1,
                        10.00f),  // Goht's Remains TODO: FIND MODEL

      [0x57] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GyorgRemains, 0x0057,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GYORGS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)2, (s16)-1,
                        10.00f),  // Gyrog's Remains TODO: FIND MODEL

      [0x58] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::TwinmoldRemains, 0x0058,
                        0x00000, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TWINMOLDS_REMAINS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMask, (s16)3, (s16)-1,
                        10.00f),  // Twinmold's Remains TODO: FIND MODEL

      [0x59] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::HookshotUnused, 0x0059,
                        0x00196, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_RED_POTION,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)3, (s16)-1, 10.00f),  // Red Potion?

      [0x5A] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Bottle, 0x005A, 0x009E,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_EMPTY_BOTTLE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Empty Bottle

      [0x5B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::RedPotion, 0x005B, 0x00C1,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RED_POTION,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Red Potion

      [0x5C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GreenPotion, 0x005C,
                        0x00C1, 0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GREEN_POTION,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Green Potion

      [0x5D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BluePotion, 0x005D, 0x00C1,
                        0x02, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BLUE_POTION,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Blue Potion

      [0x5E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Fairy, 0x005E, 0x0272,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_FAIRY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Fairy

      [0x5F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::DekuPrincess, 0x005F,
                        0x009E, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                        DrawGraphicItemID::DI_BOTTLE_DEKU_PRINCESS, (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None,
                        (s16)-1, (s16)-1, 10.00f),  // Bottle With Deku Princess

      [0x60] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Milk, 0x0060, 0x00B6, 0x00,
                        (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MILK_FULL,
                        (rnd::upgradeFunc)ItemUpgrade_RefillBottle, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Milk

      [0x61] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Milk, 0x0061, 0x00B6, 0x01,
                        (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MILK_FULL,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Half Milk

      [0x62] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Fish, 0x0062, 0x00C7, 0x00,
                        (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_FISH,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Fish

      [0x63] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Bug, 0x0063, 0x0137, 0x00,
                        (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_BUG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Bug

      [0x64] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BlueFire, 0x0064, 0x000,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Blue fire - not used

      [0x65] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Poe, 0x0065, 0x009E, 0x00,
                        (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_BIG_POE_PURPLE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Poe - not used?

      [0x66] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BigPoe, 0x0066, 0x0139,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_BIG_POE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Big Poe

      [0x67] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::Water, 0x0067, 0x0000,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_WATER_HALF,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Water TODO: FIND MODEL

      [0x68] =
          ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::HotSpringWater, 0x0068, 0x0000,
                   0x9C, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_HOT_SPRING_WATER_HALF,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                   10.00f),  // Bottle With Hot Spring Water TODO: FIND MODEL

      [0x69] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::ZoraEgg, 0x0069, 0x01AE,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_ZORA_EGG,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Zora Egg

      [0x6A] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GoldDust, 0x006A, 0x01E9,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_GOLD_DUST,
                        (rnd::upgradeFunc)ItemUpgrade_RefillBottle, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Gold Dust

      [0x6B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MagicalMushroom, 0x006B,
                        0x021D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                        DrawGraphicItemID::DI_BOTTLE_MAGIC_MUSHROOM, (rnd::upgradeFunc)ItemUpgrade_None,
                        ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Bottle With Magic Mushrooms

      [0x6C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::EponaSong, 0x1BA6, 0x00000,
                        0x8E, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)8, (s16)-1,
                        10.00f),  // Epona's Song

      [0x6D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_SMALL, (u8)game::ItemId::GoldSkulltula, 0x0052,
                        0x00125, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_SKULLTULA,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSkulltula, (s16)1, (s16)-1,
                        10.00f),  // Gold Skulltula - Ocean

      [0x6E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SeaHorse, 0x006E, 0x01E9,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_SEAHORSE,
                        (rnd::upgradeFunc)ItemUpgrade_RefillBottle, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Sea Horse - Gold Dust Object

      [0x6F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::ChateauRomani, 0x006F,
                        0x0227, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF,
                        DrawGraphicItemID::DI_BOTTLE_CHATEAU_ROMANI, (rnd::upgradeFunc)ItemUpgrade_RefillBottle,
                        ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Bottle With Chateau Romani

      // XXX: Trade Item, may have to change values? Seems rather individual and leads to fishing
      // pass.
      [0x70] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MysteryMilk, 0x00CE,
                        0x00B6, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_MYSTERY_MILK,
                        (rnd::upgradeFunc)ItemUpgrade_RefillBottle, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Mystery Milk

      [0x71] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MysteryMilk, 0x00CE,
                        0x00B6, 0x01, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_MYSTERY_MILK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bottle With Spoiled Mystery Milk

      [0x72] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SongOfSoaring, 0x1BA7,
                        0x00000, 0x8E, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)9, (s16)-1,
                        10.00f),  // Song Of Soaring

      [0x73] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SongOfStorms, 0x1BA8,
                        0x00000, 0x8E, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)10, (s16)-1,
                        10.00f),  // Song Of Storms

      [0x74] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GoronLullaby, 0x1BAC,
                        0x00000, 0x8E, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSong, (s16)11, (s16)-1,
                        10.00f),  // Goron Lullaby Intro

      [0x75] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MagicBean, 0x0035, 0x000C6,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MAGIC_BEAN,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_BeanPack, (s16)-1, (s16)-1,
                        10.00f),  // Magic Beans

      [0x76] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL, (u8)game::ItemId::SmallKey, 0x6133,
                        0x00086, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSmallKey, (s16)0, (s16)-1,
                        10.00f),  // Small Key (Woodfall)

      [0x77] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL, (u8)game::ItemId::SmallKey, 0x6134,
                        0x00086, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSmallKey, (s16)1, (s16)-1,
                        10.00f),  // Small Key (Snowhead)

      [0x78] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::DekuMask, 0x0078, 0x01BD,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DEKU_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Deku Mask

      [0x79] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GoronMask, 0x0079, 0x0119,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GORON_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Goron Mask

      [0x7A] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::ZoraMask, 0x007A, 0x011A,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ZORA_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 0.3f),  // Zora Mask

      [0x7B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::FierceDeityMask, 0x007B,
                        0x0242, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_FIERCE_DEITY_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Fierce Deity Mask

      [0x7C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::CaptainHat, 0x007C, 0x0102,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_CAPTAINS_HAT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Captain Hat

      [0x7D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GiantMask, 0x007D, 0x0226,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GIANTS_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Giant Mask

      [0x7E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::AllNightMask, 0x007E,
                        0x0265, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ALL_NIGHT_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // All Night Mask

      [0x7F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BunnyHood, 0x007F, 0x0103,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BUNNY_HOOD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Bunny Hood

      [0x80] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KeatonMask, 0x0080, 0x0100,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_KEATON_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Keaton Mask

      [0x81] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GaroMask, 0x0081, 0x0209,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GARO_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Garo Mask

      [0x82] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::RomaniMask, 0x0082, 0x021F,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ROMANIS_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Romani Mask

      [0x83] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::CircusLeaderMask, 0x0083,
                        0x0259, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_CIRCUS_LEADER_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Circus Leader Mask

      [0x84] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::PostmanHat, 0x0084, 0x0225,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_POSTMANS_HAT,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Postman Hat

      [0x85] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::CoupleMask, 0x0085, 0x0282,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COUPLES_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Couple's Mask

      [0x86] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GreatFairyMask, 0x0086,
                        0x020A, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GREAT_FAIRY_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Great Fairy's Mask

      [0x87] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GibdoMask, 0x0087, 0x020B,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GIBDO_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Gibdo's Mask

      [0x88] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::DonGeroMask, 0x0088,
                        0x0266, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DON_GERO_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Don Gero's Mask

      [0X89] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KamaroMask, 0x0089, 0x027D,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_KAMARO_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Kamaro's Mask

      [0x8A] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MaskOfTruth, 0x008A,
                        0x0104, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MASK_OF_TRUTH,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Mask of Truth

      [0x8B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::StoneMask, 0x008B, 0x0254,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_STONE_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Stone Mask

      [0x8C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BremenMask, 0x008C, 0x025A,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BREMEN_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Bremen Mask

      [0x8D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::BlastMask, 0x008D, 0x026D,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BLAST_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Blast Mask

      [0x8E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MaskOfScents, 0x008E,
                        0x027E, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MASK_OF_SCENTS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Mask Of Scents

      [0x8F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KafeiMask, 0x008F, 0x0258,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_KAFEI_MASK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Kafei Mask

      [0x90] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL, (u8)game::ItemId::SmallKey, 0x6135,
                        0x00086, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSmallKey, (s16)2, (s16)-1,
                        10.00f),  // Small Key (Great Bay)

      // The following bottle items either give one bottle if you do not have one, or refills any
      // bottle if you do have an empty one. Essentially acts as if you caught something in a
      // bottle.
      [0x91] =
          ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::ChateauRomaniFill, 0x0091,
                   0x0227, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_CHATEAU_ROMANI,
                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1, 10.00f),  // Cheateu Refill

      [0x92] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MilkFill, 0x0092, 0x00B6,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_MILK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Milk Refill

      [0x93] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GoldDustFill, 0x0093,
                        0x001E8, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_DUST,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Gold Dust Refill

      [0x94] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MysteryMilkFill, 0x00CE,
                        0x00B6, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_MYSTERY_MILK,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Mystery Milk Refill

      [0x95] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SeahorseFill, 0x0095,
                        0x001F0, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOTTLE_SEAHORSE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Seahorse Refill

      // TODO: Trade quest items
      [0x96] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MoonTear, 0x0096, 0x01B1,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MOONS_TEAR,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)0, (s16)-1,
                        10.00f),  // Moon's Tear

      [0x97] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::LandTitleDeed, 0x0097,
                        0x01B2, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TOWN_TITLE_DEED,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)1, (s16)-1,
                        10.00f),  // Land Title Deed

      [0x98] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SwampTitleDeed, 0x0098,
                        0x01B2, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SWAMP_TITLE_DEED,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)2, (s16)-1,
                        10.00f),  // Swamp Title Deed

      [0x99] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::MountainTitleDeed, 0x0099,
                        0x01B2, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MOUNTAIN_TITLE_DEED,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)3, (s16)-1,
                        10.00f),  // Mountain Title Deed

      [0x9A] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::OceanTitleDeed, 0x009A,
                        0x01B2, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_OCEAN_TITLE_DEED,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)4, (s16)-1,
                        10.00f),  // Ocean Title Deed

      [0x9B] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GreatFairySword, 0x009B,
                        0x001FB, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GREAT_FAIRY_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purchasing Great Fairy Sword

      [0x9C] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::KokiriSword, 0x009C,
                        0x00148, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_KOKIRI_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purchasing Kokiri Sword

      [0x9D] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::RazorSword, 0x009D,
                        0x001F9, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_RAZOR_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purchasing Razor Sword

      [0x9E] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::GildedSword, 0x009E,
                        0x001FA, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GILDED_SWORD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purchasing Gilded Sword

      [0x9F] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::HeroShield, 0x009F,
                        0x000B3, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_HERO_SHIELD,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purchasing Hero Shield

      [0xA0] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::RoomKey, 0x00A0, 0x020F,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_ROOM_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)5, (s16)-1,
                        10.00f),  // Room Key

      [0xA1] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::LetterToMama, 0x00A1,
                        0x0245, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_MAMAS_LETTER,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)8, (s16)-1,
                        10.00f),  // Letter To Mama

      [0xA2] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL, (u8)game::ItemId::SmallKey, 0x6136,
                        0x00086, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_SMALL_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSmallKey, (s16)3, (s16)-1,
                        10.00f),  // Small Key (Stone Tower)

      [0xA3] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::BossKey, 0x613F,
                        0x00092, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOSS_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)1, (s16)0,
                        10.00f),  // Boss Key (Woodfall)

      [0xA4] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::BossKey, 0x6140,
                        0x00092, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOSS_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)1, (s16)1,
                        10.00f),  // Boss Key (Snowhead)

      [0xA5] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::BossKey, 0x6141,
                        0x00092, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOSS_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)1, (s16)2,
                        10.00f),  // Boss Key (Great Bay)

      [0xA6] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::BossKey, 0x6142,
                        0x00092, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_BOSS_KEY,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)1, (s16)3,
                        10.00f),  // Boss Key (Stone Tower)

      [0xA7] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Compass, 0x613B,
                        0x00091, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COMPASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)2, (s16)0,
                        10.00f),  // Compass (Woodfall)

      [0xA8] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Compass, 0x613C,
                        0x00091, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COMPASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)2, (s16)1,
                        10.00f),  // Compass (Snowhead)

      [0xA9] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::Bottle, 0x0090,
                        0x0009E, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_EMPTY_BOTTLE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Purhcasing Bottle

      [0xAA] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::LetterToKafei, 0x00AA,
                        0x0210, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_LETTER_TO_KAFEI,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)6, (s16)-1,
                        10.00f),  // Letter To Kafei

      [0xAB] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::PendantOfMemories, 0x00AB,
                        0x0215, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_PENDANT_OF_MEMORIES,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveTradeItem, (s16)7, (s16)-1,
                        10.00f),  // Pendant Of Memories

      [0xAC] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Compass, 0x613D,
                        0x00091, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COMPASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)2, (s16)2,
                        10.00f),  // Compass (Great Bay)

      [0xAD] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Compass, 0x613E,
                        0x00091, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_COMPASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)2, (s16)3,
                        10.00f),  // Compass (Stone Tower)

      [0xAE] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Map, 0x6137, 0x000A0,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TOWN_MAP,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)3, (s16)0,
                        10.00f),  // Map (Woodfall)

      [0xAF] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Map, 0x6138, 0x000A0,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TOWN_MAP,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)3, (s16)1,
                        10.00f),  // Map (Snowhead)

      [0xB0] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Map, 0x6139, 0x000A0,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TOWN_MAP,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)3, (s16)2,
                        10.00f),  // Map (Great Bay)

      [0xB1] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG, (u8)game::ItemId::Map, 0x613A, 0x000A0,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_TOWN_MAP,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDungeonItem, (s16)3, (s16)3,
                        10.00f),  // Map (Stone Tower)

      [0xB2] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::None, 0x000CB, 0x00096,
                        0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveDoubleDefense, (s16)3, (s16)3,
                        10.00f),  // Double Defense.

      [0xB3] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG, (u8)game::ItemId::SmallMagicAccumulator,
                        0x000C8, 0x000A4, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_NONE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveMagic, (s16)-1, (s16)-1,
                        10.00f),  // Small Magic

      [0xB4] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B4,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Clocktown

      [0xB5] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B5,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Woodfall

      [0xB6] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B6,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Snowhead

      [0xB7] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B7,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Romani Ranch

      [0xB8] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B8,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Great Bay

      [0xB9] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::MapUnused, 0x00B9,
                        0x024D, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_DUNGEON_MAP_MAYBE,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Map of Stone Tower

      [0xBA] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::WOODEN_SMALL, (u8)game::ItemId::FishingPass, 0x00CF,
                        0x029B, 0x00, (s8)0xFF, (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_FISHING_PASS,
                        (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_None, (s16)-1, (s16)-1,
                        10.00f),  // Fishing Pass

      // Custom Items begin here
      // TODO: Custom text for sword?
      // [0xBB] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::KokiriSword, 0x009C, 0x00148, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveKokiriSword, (s16)-1, (s16)-1, 10.00f), // Kokiri Sword

      // Progressive Items
      // [0xBC] = ITEM_ROW(0xFF, ChestType::WOODEN_SMALL, 0xFF, 0xFF, 0x00098, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF,
      //                   (rnd::upgradeFunc)ItemUpgrade_BombBag, ItemEffect_None, (s16)-1,
      //                   (s16)-1, 10.00f), // Progressive Bomb Bag

      // [0xBD] = ITEM_ROW(0xFF, ChestType::WOODEN_SMALL, 0xFF, 0xFF, 0x00097, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_Quiver,
      //                   ItemEffect_None, (s16)-1, (s16)-1, 10.00f), // Progressive Quiver

      // [0xBE] = ITEM_ROW(0xFF, ChestType::WOODEN_SMALL, 0xFF, 0xFF, 0x000A8, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_Wallet,
      //                   ItemEffect_None, (s16)-1, (s16)-1, 10.00f), // Progressive Wallet

      // [0xBF] = ITEM_ROW(0xFF, ChestType::WOODEN_SMALL, 0xFF, 0xFF, 0x000A4, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_Magic,
      //                   ItemEffect_None, (s16)-1, (s16)-1, 10.00f), // Progressive Magic

      // [0xC0] = ITEM_ROW(0xFF, ChestType::WOODEN_SMALL, 0xFF, 0xFF, 0x001FA, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_Sword,
      //                   ItemEffect_None, (s16)-1, (s16)-1, 10.00f), // Progressive Sword

      // [0xC1] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SonataOfAwakening, 0x1B9E, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)1, (s16)-1, 10.00f), // Sonata Of Awakening

      // [0xC2] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::GoronLullaby, 0x1B9F, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)2, (s16)-1, 10.00f), // Goron Lullaby

      // [0xC3] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::NewWaveBossaNova, 0x1BA0, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)3, (s16)-1, 10.00f), // New Wave Bossanova

      // [0xC4] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::ElegyOfEmptiness, 0x1BA1, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)4, (s16)-1, 10.00f), // Elegy of Emptiness

      // [0xC5] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::OathToOrder, 0x1BA2, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)5, (s16)-1, 10.00f), // Oath To Order

      // [0xC6] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SongOfTime, 0x1BA4, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)6, (s16)-1, 10.00f), // Song Of Time

      // [0xC7] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SongOfHealing, 0x1BA5, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)7, (s16)-1, 10.00f), // Song Of Healing

      // [0xC8] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::EponaSong, 0x1BA6, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)8, (s16)-1, 10.00f), // Epona's Song

      // [0xC9] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SongOfSoaring, 0x1BA7, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)9, (s16)-1, 10.00f), // Song Of Soaring

      // [0xCA] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SongOfStorms, 0x1BA8, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)10, (s16)-1, 10.00f), // Song Of Storms

      // [0xCB] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::GoronLullaby, 0x1BAC, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSong, (s16)11, (s16)-1, 10.00f), // Goron Lullaby Intro

      // [0xCC] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::MagicBean, 0x0035, 0x000C6, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_BeanPack, (s16)-1, (s16)-1, 10.00f), // Magic Beans

      // Dungeon Items
      // [0xCD] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL,
      // (u8)game::ItemId::SmallKey, 0x003C, 0x00086, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSmallKey, (s16)0, (s16)-1, 10.00f), // Small Key (Woodfall)

      // [0xCE] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL,
      // (u8)game::ItemId::SmallKey, 0x003C, 0x00086, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSmallKey, (s16)1, (s16)-1, 10.00f), // Small Key (Snowhead)

      // [0xCF] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL,
      // (u8)game::ItemId::SmallKey, 0x003C, 0x00086, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSmallKey, (s16)2, (s16)-1, 10.00f), // Small Key (Great Bay)

      // [0xD0] = ITEM_ROW((u32)GetItemID::GI_RUPEE_BLUE, ChestType::DECORATED_SMALL,
      // (u8)game::ItemId::SmallKey, 0x003C, 0x00086, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveSmallKey, (s16)3, (s16)-1, 10.00f), // Small Key (Stone Tower)

      // [0xD1] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::BossKey, 0x003D, 0x00092, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)1, (s16)0, 10.00f), // Boss Key (Woodfall)

      // [0xD2] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::BossKey, 0x003D, 0x00092, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)1, (s16)1, 10.00f), // Boss Key (Snowhead)

      // [0xD3] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::BossKey, 0x003D, 0x00092, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)1, (s16)2, 10.00f), // Boss Key (Great Bay)

      // [0xD4] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::BossKey, 0x003D, 0x00092, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)1, (s16)3, 10.00f), // Boss Key (Stone Tower)

      // [0xD5] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Compass, 0x003F, 0x00091, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)2, (s16)0, 10.00f), // Compass (Woodfall)

      // [0xD6] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Compass, 0x003F, 0x00091, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)2, (s16)1, 10.00f), // Compass (Snowhead)

      // [0xD7] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Compass, 0x003F, 0x00091, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)2, (s16)2, 10.00f), // Compass (Great Bay)

      // [0xD8] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Compass, 0x003F, 0x00091, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)2, (s16)3, 10.00f), // Compass (Stone Tower)

      // [0xD9] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Map, 0x003E, 0x000A0, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)3, (s16)0, 10.00f), // Map (Woodfall)

      // [0xDA] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Map, 0x003E, 0x000A0, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)3, (s16)1, 10.00f), // Map (Snowhead)

      // [0xDB] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Map, 0x003E, 0x000A0, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)3, (s16)2, 10.00f), // Map (Great Bay)

      // [0xDC] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::DECORATED_BIG,
      // (u8)game::ItemId::Map, 0x003E, 0x000A0, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDungeonItem, (s16)3, (s16)3, 10.00f), // Map (Stone Tower)

      // [0xDD] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::None, 0x000CB, 0x00096, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDoubleDefense, (s16)3, (s16)3, 10.00f), // Double Defense.

      // [0xDE] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::SmallMagicAccumulator, 0x000CA, 0x000A4, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveMagic, (s16)-1, (s16)-1, 10.00f), // Small Magic

      // [0xDF] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::BigMagicAccumulator, 0x000CC, 0x000A4, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveDoubleMagic, (s16)-1, (s16)-1, 10.00f), // Double Magic

      // [0xE0] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::BigMagicAccumulator, 0x000CA, 0x000A4, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveGreatSpin, (s16)-1, (s16)-1, 10.00f), // Great Spin Attack

      // [0xE1] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::OdolwaRemains, 0x0055, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveMask, (s16)0, (s16)-1, 10.00f), // Odolwa's Remains

      // [0xE2] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::GohtRemains, 0x0056, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveMask, (s16)1, (s16)-1, 10.00f), // Goht's Remains

      // [0xE3] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::GyorgRemains, 0x0057, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveMask, (s16)2, (s16)-1, 10.00f), // Gyrog's Remains

      // [0xE4] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_BIG,
      // (u8)game::ItemId::TwinmoldRemains, 0x0058, 0x00000, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, 0xFF, (rnd::upgradeFunc)ItemUpgrade_None,
      //                   ItemEffect_GiveMask, (s16)3, (s16)-1, 10.00f), // Twinmold's Remains

      // [0xE5] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_SMALL,
      // (u8)game::ItemId::GoldSkulltula, 0x0052, 0x00125, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_SKULLTULA,
      //                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSkulltula, (s16)0,
      //                   (s16)-1, 10.00f), // Gold Skulltula - Swamp

      // [0xE6] = ITEM_ROW((u32)GetItemID::GI_NUTS_30, ChestType::WOODEN_SMALL,
      // (u8)game::ItemId::GoldSkulltula, 0x0052, 0x00125, (s8)0xFF, (s8)0xFF,
      //                   (s8)0xFF, (s8)0xFF, (s8)0xFF, DrawGraphicItemID::DI_GOLD_SKULLTULA,
      //                   (rnd::upgradeFunc)ItemUpgrade_None, ItemEffect_GiveSkulltula, (s16)1,
      //                   (s16)-1, 10.00f), // Gold Skulltula - Ocean
  };

  ItemRow* ItemTable_GetItemRow(u16 getItemId) {
    if (getItemId >= ARR_SIZE(rItemTable)) {
      return NULL;
    }
    ItemRow* itemRow = &rItemTable[getItemId];
    if (itemRow->baseItemId == 0) {
      return NULL;
    }
    return itemRow;
  }

  ItemRow* ItemTable_GetItemRowFromIndex(u8 rowIndex) {
    return &rItemTable[rowIndex];
  }

  void ItemTable_SetBombchusChestType(u8 type) {
    // rItemTable[0x6B].chestType = type;
  }

  u16 ItemTable_ResolveUpgrades(u16 getItemId) {
    game::SaveData& gSaveContext = game::GetCommonData().save;
    if (gSaveContext.has_completed_intro) {
      for (;;) {
        ItemRow* itemRow = ItemTable_GetItemRow(getItemId);
        u16 newItemId = (u16)itemRow->upgrade(&gSaveContext, (GetItemID)getItemId);
        if (newItemId == getItemId) {
          return getItemId;
        }
        getItemId = newItemId;
      }
    }
    // Should not reach.
    return getItemId;
  }

  void ItemTable_CallEffect(ItemRow* itemRow) {
    game::CommonData& commonData = game::GetCommonData();
    if (itemRow->itemId > 0x73 && itemRow->itemId < 0x79) {
      rCustomDungeonItemRetrieved = 1;
    }
    itemRow->effect(&commonData, itemRow->effectArg1, itemRow->effectArg2);
  }

}  // namespace rnd
