#include "rnd/actors/en_akindonuts.h"
#include "rnd/actors/en_cow.h"
#include "rnd/actors/en_elforg.h"
#include "rnd/actors/en_si.h"
#include "rnd/extdata.h"
#include "rnd/item_override.h"
#include "rnd/overrides/override_policy.h"
#include "rnd/overrides/override_quests.h"
#include "rnd/overrides/override_state.h"
#include "rnd/savefile.h"
#include "rnd/shops.h"
#include "rnd/spoiler_data.h"

namespace rnd {
  void ItemOverride_AfterItemReceived(void) {
    ItemOverride_Key key = rActiveItemOverride.key;
    if (key.all == 0) {
      return;
    }
    // One bag, two owners. Track across areas.
    if ((key.type == ItemOverride_Type::OVR_SHOP && key.scene == (u8)game::SceneId::BombShop &&
         key.flag == (u8)kBigBombBagShelfParam) ||
        (key.type == ItemOverride_Type::OVR_BASE_ITEM && key.scene == kCuriosityBombBagScene &&
         key.flag == kCuriosityBombBagFlag)) {
      gExtSaveData.givenItemChecks.stolenBombBagTaken = 1;
    }

    if (key.type == ItemOverride_Type::OVR_SHOP) {
      ItemOverride_ClearPendingShop();  // hand-off complete
      Shopsanity_SetSlotPurchased(Shopsanity_GetSlot((game::SceneId)key.scene, (s16)key.flag));
    }
    if (key.type == ItemOverride_Type::OVR_BASE_ITEM && key.scene == (u8)game::SceneId::MilkBar) {
      if (key.flag == (u8)GetItemID::GI_BOTTLE_MILK_REFILL)
        Shopsanity_SetSlotPurchased(SHOPSANITY_MILK_BAR_MILK);
      else if (key.flag == (u8)GetItemID::GI_BOTTLE_CHATEAU_ROMANI_REFILL)
        Shopsanity_SetSlotPurchased(SHOPSANITY_MILK_BAR_CHATEAU);
    }
    if (key.type == ItemOverride_Type::OVR_BASE_ITEM && key.scene == (u8)game::SceneId::GormanTrack &&
        key.flag == (u8)GetItemID::GI_BOTTLE_MILK_REFILL) {
      Shopsanity_SetSlotPurchased(SHOPSANITY_GORMAN_MILK);
    }
    if (gItemOverrideSource.actorId == game::act::Id::EnAkindonuts) {
      En_Akindonuts_SetSaleBought(key);
    }
    if (key.type == ItemOverride_Type::OVR_COW) {
      En_Cow_SetMilked(key.flag);
    }
    if (key.type == ItemOverride_Type::OVR_STRAY_FAIRY) {
      if (key.scene == (u8)game::SceneId::LaundryPool || key.scene == (u8)game::SceneId::EastClockTown) {
        gExtSaveData.givenItemChecks.clockTownStrayFairyCollected = 1;
        // Set fairy collected so it doesn't spawn anymore for the cycle.
        game::GetCommonData().save.week_event_reg_08.WEEKEVENTREG_08_80 = 1;
      } else {
        En_Elforg_SetFairyCollected((game::SceneId)key.scene, key.flag);
      }
    }
    if (key.type == ItemOverride_Type::OVR_MINI_GAME) {
      if (key.scene == (u8)game::SceneId::HoneyAndDarling) {
        if (key.flag == 1)
          gExtSaveData.miniGameCompletion.dayOneHoneyAndDarlingCompleted = 1;
        else if (key.flag == 2)
          gExtSaveData.miniGameCompletion.dayTwoHoneyAndDarlingCompleted = 1;
      } else if (key.scene == (u8)game::SceneId::DekuScrubPlayground) {
        if (key.flag == 1)
          gExtSaveData.miniGameCompletion.dayOneDekuPlaygroundCompleted = 1;
        else if (key.flag == 2)
          gExtSaveData.miniGameCompletion.dayTwoDekuPlaygroundCompleted = 1;
      } else if (key.scene == (u8)game::SceneId::ZoraCape) {
        gExtSaveData.miniGameCompletion.zoraJarGameCompleted = 1;
      } else if (key.scene == (u8)game::SceneId::ZoraHall) {
        if (key.flag == 1)
          gExtSaveData.miniGameCompletion.zoraHallRehearsalCompleted = 1;
        else if (key.flag == 2)
          gExtSaveData.miniGameCompletion.zoraHallLuluBadPhotoCompleted = 1;
        else if (key.flag == 3)
          gExtSaveData.miniGameCompletion.zoraHallLuluGoodPhotoCompleted = 1;
      } else if (key.scene == (u8)game::SceneId::LotteryShop) {
        if (key.flag == 1)
          gExtSaveData.miniGameCompletion.dayOneLotteryCompleted = 1;
        else if (key.flag == 2)
          gExtSaveData.miniGameCompletion.dayTwoLotteryCompleted = 1;
        else if (key.flag == 3)
          gExtSaveData.miniGameCompletion.dayThreeLotteryCompleted = 1;
      }
    }
    SetExtData();
    SpoilerLog_UpdateIngameLog(key.type, key.scene, key.flag);
    // #if ENABLE_DEBUG || DEBUG_PRINT
    //  util::Print(
    //          "%s: Our key values:\nKey Type %#04x\nKey Scene: %#04x\nKey Flag: %#06x\n",
    //          key.type, key.scene, key.flag);
    // #endif
  }

  // Records the quest flags that belong to the item that was just received.
  void SetExtData() {
    const game::act::Id actor = gItemOverrideSource.actorId;
    // These two pay a reward per visit, so they count the rewards instead of keeping a flag.
    if (actor == game::act::Id::EnKgy) {
      gExtSaveData.givenItemChecks.enKgyGivenItem = gExtSaveData.givenItemChecks.enKgyGivenItem == 1 ? 2 : 1;
      return;
    }
    if (actor == game::act::Id::EnGinkoMan) {
      if (gExtSaveData.givenItemChecks.enGinkoManGivenItem < 2) {
        gExtSaveData.givenItemChecks.enGinkoManGivenItem = gExtSaveData.givenItemChecks.enGinkoManGivenItem + 1;
      }
      return;
    }
    ItemOverride_MarkQuestDone(actor, gItemOverrideSource.getItemId, (u8)GetContext().gctx->scene);
  }

  // Whether the check behind this override has paid its randomized item before. Checks the patch can't tell are never
  // complete, so they keep paying as they always did.
  bool ItemOverride_IsCheckComplete(game::act::Actor* actor, const ItemOverride& override) {
    const ItemOverride_Key key = override.key;
    switch (key.type) {
    case ItemOverride_Type::OVR_CHEST:
      return gExtSaveData.chestRewarded[key.scene][key.flag] == 1;
    case ItemOverride_Type::OVR_SKULL:
      return actor != NULL && En_Si_IsTokenCollected(actor, (game::SceneId)key.scene);
    case ItemOverride_Type::OVR_COW:
      return En_Cow_IsMilked(key.flag);
    case ItemOverride_Type::OVR_STRAY_FAIRY:
      return En_Elforg_IsFairyCollected(override);
    default:
      break;
    }
    if (actor != NULL && actor->id == game::act::Id::EnAkindonuts && En_Akindonuts_IsSaleBought(override)) {
      return true;
    }
    // One bag, two owners.
    if (key.type == ItemOverride_Type::OVR_BASE_ITEM && key.scene == kCuriosityBombBagScene &&
        key.flag == kCuriosityBombBagFlag) {
      return gExtSaveData.givenItemChecks.stolenBombBagTaken != 0;
    }
    return ItemOverride_IsQuestDone(override);
  }

  bool ItemOverride_IsRepeatBlocked(game::act::Actor* actor, const ItemOverride& override) {
    return ItemOverride_ResolveRepeat(ItemOverride_IsCheckComplete(actor, override), override, 0) ==
           RepeatOutcome::Payout;
  }
}  // namespace rnd
