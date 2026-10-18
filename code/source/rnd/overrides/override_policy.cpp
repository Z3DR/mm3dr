#include "rnd/overrides/override_policy.h"
#include "rnd/item_table.h"
#include "rnd/savefile.h"

namespace rnd {
  struct GetItemRange {
    u16 first;
    u16 last;
  };

  // Overrides that are never paid a second time. They are either used up when picked up (rupees, ammo, hearts),
  // applied in place (upgrades, keys, maps), or can't be tested for in the inventory at all (ice trap, refills).
  // Anything that is not listed here is repeated once the player no longer has it.
  static const GetItemRange rOnceOnly[] = {
      {0x01, 0x07},  // rupees
      {0x08, 0x09},  // wallets
      {0x0A, 0x13},  // recovery hearts, heart piece and container, magic jars, ice trap
      {0x14, 0x1A},  // bombs, deku sticks, bombchus
      {0x1B, 0x1D},  // bomb bags
      {0x1E, 0x22},  // arrows and the bow
      {0x23, 0x24},  // quivers
      {0x28, 0x31},  // deku nuts, magic, bombchus, boss remains
      {0x32, 0x33},  // shields
      {0x36, 0x40},  // bombchus, swords, small and boss keys, maps, compasses
      {0x44, 0x4A},  // gold skulltula token and the progressive items
      {0x4B, 0x58},  // songs, ocarina, notebook, boss remains
      {0x5A, 0x5A},  // empty bottle
      {0x6C, 0x6D},  // Epona's Song, gold skulltula token
      {0x72, 0x77},  // songs, magic beans pack, small keys
      {0x90, 0x95},  // bottle refills
      {0x9B, 0x9F},  // stolen sword and shield text rows
      {0xA2, 0xA9},  // keys, compasses, empty bottle
      {0xAC, 0xB9},  // compasses, maps, Tingle maps
      {0xBA, 0xBF},  // fishing hole pass, stray fairies
  };

  // Vanilla get items that the game lets the player earn again, so a completed check hands them out as usual instead
  // of a rupee. Both the bottle and the refill variant are listed, since either can be what the actor asks for.
  static const GetItemRange rRestock[] = {
      {0x35, 0x35},  // magic bean
      {0x6A, 0x6A},  // gold dust
      {0x6E, 0x6E},  // seahorse
      {0x70, 0x70},  // mystery milk
      {0x93, 0x95},  // gold dust, mystery milk and seahorse refills
                     // TODO: Zora eggs, once each egg has its own key.
  };

  static bool InRanges(const GetItemRange* ranges, u32 count, u16 getItemId) {
    for (u32 i = 0; i < count; ++i) {
      if (getItemId >= ranges[i].first && getItemId <= ranges[i].last) {
        return true;
      }
    }
    return false;
  }

  bool ItemOverride_IsOnceOnly(u16 getItemId) {
    return InRanges(rOnceOnly, ARR_SIZE(rOnceOnly), getItemId);
  }

  bool ItemOverride_IsRestockable(s16 vanillaGetItemId) {
    return vanillaGetItemId > 0 && InRanges(rRestock, ARR_SIZE(rRestock), (u16)vanillaGetItemId);
  }

  bool ItemOverride_OwnsItem(const ItemOverride& override) {
    const ItemRow* row = ItemTable_GetItemRow(override.value.getItemId);
    if (row == NULL) {
      return true;
    }
    const game::ItemId item = (game::ItemId)row->itemId;
    // Trade items are never taken away; the patch records every one that was found.
    if (item >= game::ItemId::MoonTear && item <= game::ItemId::PendantOfMemories) {
      return gExtSaveData.collectedTradeItems[(u32)item - (u32)game::ItemId::MoonTear] != game::ItemId::None;
    }
    // Spoiled Mystery Milk is the same milk gone off.
    if (item == game::ItemId::MysteryMilk && game::HasBottle(game::ItemId::MysteryMilkSpoiled)) {
      return true;
    }
    return game::HasMask(item) || game::HasItem(item) || game::HasBottle(item);
  }

  RepeatOutcome ItemOverride_ApplyRepeatRules(game::act::Actor* actor, ItemOverride& override, s16 vanillaGetItemId) {
    const RepeatOutcome outcome =
        ItemOverride_ResolveRepeat(ItemOverride_IsCheckComplete(actor, override), override, vanillaGetItemId);
    if (outcome == RepeatOutcome::Payout) {
      ItemOverride_ApplyRepeatPayout(override);
    }
    if (outcome != RepeatOutcome::Vanilla) {
      ItemOverride_ApplyBottleRules(override);
    }
    return outcome;
  }

  void ItemOverride_ApplyRepeatPayout(ItemOverride& override) {
    if (override.key.type == ItemOverride_Type::OVR_COW) {
      override.value.getItemId = (u16)GetItemID::GI_BOTTLE_MILK_REFILL;
      override.value.looksLikeItemId = (u8)GetItemID::GI_BOTTLE_MILK;
      return;
    }
    override.value.getItemId = (u16)GetItemID::GI_RUPEE_BLUE;
    override.value.looksLikeItemId = (u8)GetItemID::GI_RUPEE_BLUE;
  }

  RepeatOutcome ItemOverride_ResolveRepeat(bool complete, const ItemOverride& override, s16 vanillaGetItemId) {
    RepeatQuery query = {};
    query.complete = complete;
    if (complete) {
      query.onceOnly = ItemOverride_IsOnceOnly(override.value.getItemId);
      query.owned = ItemOverride_OwnsItem(override);
      query.restock = ItemOverride_IsRestockable(vanillaGetItemId);
    }
    return ItemOverride_Decide(query);
  }

  void ItemOverride_ApplyBottleRules(ItemOverride& override) {
    switch (override.value.getItemId) {
    case 0x59:
      if (gExtSaveData.givenItemChecks.bottleRedPotionGiven == 1) {
        if (!game::HasBottle(game::ItemId::Bottle))
          override.value.getItemId = 0x01;
        else
          override.value.getItemId = 0x5B;
        override.value.looksLikeItemId = 0x59;
      }
      break;
    case 0x60:
      if (gExtSaveData.givenItemChecks.bottleMilkGiven == 1) {
        if (!game::HasBottle(game::ItemId::Bottle))
          override.value.getItemId = 0x01;
        else
          override.value.getItemId = 0x92;
        override.value.looksLikeItemId = 0x60;
      }
      break;
    case 0x6A:
      if (gExtSaveData.givenItemChecks.bottleGoldDustGiven == 1) {
        if (!game::HasBottle(game::ItemId::Bottle))
          override.value.getItemId = 0x01;
        else
          override.value.getItemId = 0x93;
        override.value.looksLikeItemId = 0x6A;
      }
      break;
    case 0x6E:
      if (!game::HasBottle(game::ItemId::Bottle)) {
        override.value.getItemId = 0x01;
        override.value.looksLikeItemId = 0x6E;
      }
      break;
    case 0x6F:
      if (gExtSaveData.givenItemChecks.bottleChateuGiven == 1) {
        if (!game::HasBottle(game::ItemId::Bottle))
          override.value.getItemId = 0x01;
        else
          override.value.getItemId = 0x91;
        override.value.looksLikeItemId = 0x6F;
      }
      break;
    // Refills placed directly, and spoiled Mystery Milk (which only fills a bottle): without an empty bottle to fill,
    // give a green rupee instead.
    case 0x5B:
    case 0x71:
    case 0x91:
    case 0x92:
    case 0x93:
      if (!game::HasBottle(game::ItemId::Bottle))
        override.value.getItemId = 0x01;
      break;
    default:
      break;
    }
  }
}  // namespace rnd
