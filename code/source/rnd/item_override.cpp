#include "rnd/item_override.h"
#include "rnd/extdata.h"
#include "rnd/item_table.h"
#include "rnd/overrides/override_policy.h"
#include "rnd/overrides/override_state.h"
#include "rnd/savefile.h"

namespace rnd {
  void ItemOverride_Init(void) {
    ItemOverride_InitLookup();
    ItemOverride_InitActive();
  }

  extern "C" {
  void ItemOverride_GetItemTextAndItemID(game::act::Player* actor) {
    if (rActiveItemRow != NULL) {
      if (rActiveItemOverride.key.type == ItemOverride_Type::OVR_CHEST) {
        gExtSaveData.chestRewarded[rActiveItemOverride.key.scene][rActiveItemOverride.key.flag] = 1;
      }
      game::GlobalContext* gctx = GetContext().gctx;
      u16 textId = rActiveItemRow->textId;
      u8 itemId = rActiveItemRow->itemId;
      ItemOverride_ReleaseTouchPickup();
      ItemTable_CallEffect(rActiveItemRow);
      // Only check if we have the ID set, that means text is displayed elsewhere.
      if (rStoredTextId == 0) {
        gctx->ShowMessage(textId, actor);
      }

      // Get_Item_Handler. Don't give ice traps, since it may cause UB. Swords are given in full by their item effects:
      // the handler would put them on the current form's B button, Fierce Deity's sword included.
      const bool isSword = itemId >= (u8)game::ItemId::KokiriSword && itemId <= (u8)game::ItemId::GildedSword;
      if (itemId != (u8)game::ItemId::None && !isSword) {
        util::GetPointer<int(game::GlobalContext*, game::ItemId)>(0x233BEC)(gctx, (game::ItemId)itemId);
        // Since the regular get item handler does not take care of this situation, we need to do it manually.
        if (rActiveItemOverride.value.getItemId > 0xB3 && rActiveItemOverride.value.getItemId < 0xBA)
          ItemOverride_RevealMapBasedOnId(rActiveItemOverride.value.getItemId);
      }
      ItemOverride_AfterItemReceived();
    }
  }

  void ItemOverride_GetItem(game::GlobalContext* gctx, game::act::Actor* fromActor, game::act::Player* player,
                            s16 incomingGetItemId) {
    ItemOverride override = {0};
    s32 incomingNegative = incomingGetItemId < 0;
    if (fromActor != NULL && incomingGetItemId != 0) {
      s16 getItemId = ItemOverride_CheckNpc(fromActor->id, incomingGetItemId, incomingNegative);
#if defined ENABLE_DEBUG || DEBUG_PRINT
      util::Print(
          "%s: Our actor ID is %#06x\nScene is %#04x\nIncoming item id is %#04x\ngetItemId %#04x\nParams %#04x\n",
          __func__, fromActor->id, gctx->scene, incomingGetItemId, getItemId, fromActor->params);
#endif
      gItemOverrideSource.actorId = fromActor->id;
      gItemOverrideSource.getItemId = incomingNegative ? (GetItemID)-incomingGetItemId : (GetItemID)incomingGetItemId;
      override = ItemOverride_Lookup(fromActor, (u16)gctx->scene, getItemId);
      if (override.key.all != 0) {
        // Override the stored get item if we are a bottled item.
        // If we're in the default spot to retrieve these items, set stored to NONE to avoid
        // This should avoid ext data being set before getting an actual bottle.
        if ((GetItemID)incomingGetItemId == GetItemID::GI_BOTTLE_POTION_RED ||
            (GetItemID)incomingGetItemId == GetItemID::GI_BOTTLE_MILK ||
            (GetItemID)incomingGetItemId == GetItemID::GI_BOTTLE_GOLD_DUST ||
            (GetItemID)incomingGetItemId == GetItemID::GI_BOTTLE_CHATEAU_ROMANI) {
          gItemOverrideSource.getItemId = GetItemID::GI_NONE;
        } else if (override.value.getItemId == 0x59 || override.value.getItemId == 0x60 ||
                   override.value.getItemId == 0x6A || override.value.getItemId == 0x6F) {
          gItemOverrideSource.getItemId = (GetItemID) override.value.getItemId;
        }
      }
    }
    if (override.key.all == 0) {
      // No override, use base game's item code. A touch pickup that is still waiting (a stray fairy) must keep its
      // override: opening a chest asks for its own vanilla item while the fairy is still on its way.
      if (!ItemOverride_IsTouchPickupPending()) {
        ItemOverride_Clear();
      }
      player->get_item_id = incomingGetItemId;
      return;
    }

    RepeatOutcome resolveOutcome =
        ItemOverride_ResolveRepeat(ItemOverride_IsCheckComplete(fromActor, override), override,
                                   incomingNegative ? -incomingGetItemId : incomingGetItemId);
    switch (resolveOutcome) {
    case RepeatOutcome::Payout:
      ItemOverride_ApplyRepeatPayout(override);
      break;
    case RepeatOutcome::Vanilla:
      ItemOverride_Clear();
      player->get_item_id = incomingGetItemId;
      return;
    case RepeatOutcome::GiveOverride:
      break;
    }

    ItemOverride_ApplyBottleRules(override);
    ItemOverride_Activate(override);
    s16 baseItemId = rActiveItemRow->baseItemId;

    if (override.value.getItemId == 0x12) {
      rActiveItemRow->effectArg1 = override.key.all >> 16;
      rActiveItemRow->effectArg2 = override.key.all & 0xFFFF;
    }

    if (incomingGetItemId != 0x44 && incomingGetItemId != 0x6D && incomingGetItemId != 0x52 &&
        (incomingGetItemId < (s16)GetItemID::GI_STRAY_FAIRY_CLOCK_TOWN ||
         incomingGetItemId > (s16)GetItemID::GI_STRAY_FAIRY_STONE_TOWER))
      player->get_item_id = incomingNegative ? -baseItemId : baseItemId;
    // Edge case with Song of healing items. Override their show text in their own functions
    // to ensure that we have the same 'feel' as the base game.
    // This also ensures that if there is no override the default text still works.
    if (isGIDSongOrSoHItem(static_cast<GetItemID>(incomingGetItemId))) {
      rStoredTextId = rActiveItemRow->textId;
    }
    gGivenItemOverride = true;
    return;
  }

  void ItemOverride_RemoveTextId() {
    rStoredTextId = 0;
  }
  }
}  // namespace rnd
