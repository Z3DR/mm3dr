#include "rnd/extdata.h"
#include "rnd/item_override.h"
#include "rnd/overrides/override_quests.h"
#include "rnd/overrides/override_state.h"
#include "rnd/savefile.h"

namespace rnd {
  s16 ItemOverride_CheckNpc(game::act::Id actorId, s16 originalGetItemId, s32 incomingNegative) {
    s16 getItemId = incomingNegative ? -originalGetItemId : originalGetItemId;
    if (actorId == game::act::Id::NpcEnNb) {
      if (gExtSaveData.givenItemChecks.enNbGivenItem > 0) {
        getItemId = incomingNegative ? -0xBA : 0xBA;
      }
    } else if (actorId == game::act::Id::NpcEnBjt) {
      // The hand pays again every cycle. Only the first payout is the check; later ones stay vanilla.
      if (!gExtSaveData.miscNpcGiven.enBjtGivenItem)
        getItemId = incomingNegative ? -0x01 : 0x01;
    } else if (actorId == game::act::Id::EnShn) {
      // Boathouse can give good (5 rupee), better (20 rupee) or best (PoH/Fishing Pass).
      // Ignore all values except for the PoH for this check.
      if (originalGetItemId == 0xC || originalGetItemId == 0xBA) {
        if (gExtSaveData.givenItemChecks.enShnGivenItem == 1)
          getItemId = incomingNegative ? -0x02 : 0x02;
        else
          getItemId = incomingNegative ? -0xBA : 0xBA;
      }
    } else if (actorId == game::act::Id::NpcInvisibleGuard) {
      if (gExtSaveData.givenItemChecks.enStoneHeishiGivenItem > 0) {
        getItemId = incomingNegative ? -0xBA : 0xBA;
      }
    } else if (actorId == game::act::Id::EnPst) {
      if (gExtSaveData.givenItemChecks.enPstGivenItem == 0)
        getItemId = incomingNegative ? -0xBA : 0xBA;
      else
        getItemId = 0x01;
    } else if (actorId == game::act::Id::EnDns) {
      // Business scrub salesmen in grotto.
      // Same scene as gossips so need to set item manually.
      getItemId = incomingNegative ? -0x01 : 0x01;
    } else if (getItemId == static_cast<s16>(GetItemID::GI_MASK_CAPTAINS_HAT)) {
      gExtSaveData.givenItemChecks.enOskGivenItem = 1;
    } else if (actorId == game::act::Id::EnKitan) {
      // The quiz is one check across Clock Town, Mountain Village and Milk Road. Only the first win is the check; later
      // wins pay vanilla's repeat reward, which must stay vanilla.
      if (!game::GetCommonData().save.week_event_reg_79.WEEKEVENTREG_RECEIVED_KEATON_HEART_PIECE)
        getItemId = incomingNegative ? -0x03 : 0x03;
    } else if (actorId == game::act::Id::EnGinkoMan) {
      // Check to see if we're not getting a repeat reward.
      u16 bankRupeeCount = game::GetCommonData().save.bank_rupee_count;
      if (gExtSaveData.givenItemChecks.enGinkoManGivenItem == 1 && bankRupeeCount >= 500) {
        getItemId = incomingNegative ? -0x03 : 0x03;
      }
    }

    return getItemId;
  }

  extern "C" {
  // Aroma, Mikau and Darmani are asked about through their own hooks instead of the inventory read.
  bool ItemOverride_CheckAromaGivenItem() {
    return ItemOverride_QuestGateClosed(game::ItemId::KafeiMask) == 1;
  }

  bool ItemOverride_CheckMikauGivenItem() {
    return ItemOverride_QuestGateClosed(game::ItemId::ZoraMask) == 1;
  }

  bool ItemOverride_CheckDarmaniGivenItem() {
    return ItemOverride_QuestGateClosed(game::ItemId::GoronMask) == 1;
  }

  // clang-format off
  int ItemOverride_CheckInventoryItemOverride(game::ItemId currentItem) {
    ExtSaveData::GivenItemRegister& givenItems = gExtSaveData.givenItemChecks;
    // The NPCs that pay a randomized item ask "do you own the mask?". Say yes only while the quest is done and the
    // item it paid is still held; if it was lost the NPC offers it again.
    if (currentItem != game::ItemId::KafeiMask && currentItem != game::ItemId::GoronMask) {
      const int closed = ItemOverride_QuestGateClosed(currentItem);
      if (closed >= 0) {
        return closed ? (int)currentItem : 0xFF;
      }
    }
    if (currentItem == game::ItemId::CaptainHat) {
      return givenItems.enOskGivenItem ? (int)currentItem : (int)0xFF;
    } else if (currentItem == game::ItemId::PowderKeg) {
      // Check scene if we want to buy from goron.
      game::GlobalContext* gctx = GetContext().gctx;

      if (gctx->scene == game::SceneId::GoronVillageWinter || gctx->scene == game::SceneId::GoronVillageSpring) {
        return ItemOverride_IsQuestClosedAt((u8)gctx->scene, GetItemID::GI_POWDER_KEG) ? (int) currentItem
        : (int)0xFF;
      }

      return game::HasItem((game::ItemId)currentItem) ? (int) currentItem
          : (int)0xFF;
    } else if (currentItem == game::ItemId::GiantMask) {
      return givenItems.enBoss02GivenItem ? (int) currentItem
        : (int)0xFF;
    } else if (currentItem == game::ItemId::LensOfTruth) {
      auto* gctx = rnd::GetContext().gctx;

      if (!gctx || gctx->type != game::StateType::Play) {
        return game::HasItem(currentItem) ? (int) currentItem : 0xFF;
      }
      if (gctx->scene == game::SceneId::GoronVillageWinter) {
        auto& saveData = game::GetCommonData().save;
        if (saveData.player.magic_acquired == 0 || !game::HasItem(currentItem)) return 0xFF;
      }
    } else if (currentItem == game::ItemId::Ocarina) {
      return (int) currentItem; // XXX: Use this to fix Termina being in Cycle 1.
    } else if (currentItem == game::ItemId::DekuMask) {
      auto* gctx = GetContext().gctx;
      if(gctx->scene == game::SceneId::ClockTowerInterior) {
        return givenItems.enOsnGivenMask ? (int) currentItem
          : (int)0xFF;
      }

    }
    // Use the standard pointer to array as this seems to mess with
    // some issues in checking items such as trade items, and Giant's Mask.
    std::array<game::ItemId, 24>& inventory = game::GetCommonData().save.inventory.items;
    u8* slotArray = (u8*)0x626cdc;
    u8 slot = slotArray[(int)currentItem];
    return (int)inventory[slot];
  }
  // clang-format on

  u32 ItemOverride_GetGaboraExtData() {
    return (u32)gExtSaveData.givenItemChecks.enKgyGivenItem;
  }

  u32 ItemOverride_GetOshExtData() {
    return (u32)gExtSaveData.givenItemChecks.enOshGivenItem;
  }
  }
}  // namespace rnd
