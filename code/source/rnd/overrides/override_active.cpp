#include "rnd/actors/en_elforg.h"
#include "rnd/custom_models.h"
#include "rnd/icetrap.h"
#include "rnd/item_override.h"
#include "rnd/item_table.h"
#include "rnd/overrides/override_policy.h"
#include "rnd/overrides/override_state.h"
#include "rnd/rheap.h"
#include "rnd/spoiler_data.h"

namespace rnd {
  // Amount of frames that ItemOverride_Update cannot clear the active get item.
  // This is used as the stray fairies circling don't set Link into a get item state with
  // flags and the flags are stored on En_Elforg.
  // So let's put 120 frames of delay during collection to ensure the
  // full get item can conclude.
  static u16 sTouchPickupHold = 0;
  static constexpr u16 rTouchPickupHoldFrames = 120;
  // The active item came from a touch pickup (a stray fairy), so there is no get-item animation to wait for.
  static bool rActiveIsTouchPickup = false;

  static ItemOverride rDeferredTouchPickups[3] = {0};
  static u16 rDeferredWaitFrames = 0;
  static constexpr u16 rDeferredMaxWaitFrames = 600;

  ItemOverride_Source gItemOverrideSource = {game::act::Id::Player, GetItemID::GI_NONE};
  static game::act::Actor* rDummyActor = NULL;
  static ItemOverride rPendingOverrideQueue[3] = {0};
  ItemOverride rActiveItemOverride = {0};
  // Accessed via hooks.
  ItemRow* rActiveItemRow = NULL;
  u32 rActiveItemGraphicId = 0x0;
  // Split active_item_row into variables for convenience in ASM
  u32 rActiveItemActionId = 0;
  u32 rActiveItemObjectModelIdx = 0x0;
  u32 rActiveItemTextId = 0;
  u32 rActiveItemObjectId = 0;
  u16 rStoredTextId = 0;
  u32 rCustomDungeonItemRetrieved = 0;
  u8 rActiveItemChestType = 0;
  bool isItemOverrideActive = false;

  static u8 rSatisfiedPendingFrames = 0;

  void ItemOverride_InitActive(void) {
    // Create an actor satisfying the minimum requirements to give the player an item
    rDummyActor = (game::act::Actor*)rHeap_Alloc(sizeof(game::act::Actor));
    rDummyActor->calc_fn = (game::act::MainFunc*)1;
    rDummyActor->parent_actor = NULL;
  }

  void ItemOverride_HoldTouchPickup(void) {
    sTouchPickupHold = rTouchPickupHoldFrames;
    rActiveIsTouchPickup = true;
  }

  bool ItemOverride_DeferTouchPickup(ItemOverride override) {
    for (u32 i = 0; i < ARR_SIZE(rDeferredTouchPickups); ++i) {
      if (rDeferredTouchPickups[i].key.all == override.key.all) {
        return true;
      }
      if (rDeferredTouchPickups[i].key.all == 0) {
        rDeferredTouchPickups[i] = override;
        return true;
      }
    }
    return false;
  }

  bool ItemOverride_IsTouchPickupPending(void) {
    return sTouchPickupHold > 0 && rActiveItemRow != NULL;
  }

  void ItemOverride_ReleaseTouchPickup(void) {
    sTouchPickupHold = 0;
  }

  void ItemOverride_Activate(ItemOverride override) {
    u16 resolvedGetItemId = ItemTable_ResolveUpgrades(override.value.getItemId);

    ItemRow* itemRow = ItemTable_GetItemRow(resolvedGetItemId);
    if (itemRow == NULL) {
      return;
    }
    u8 looksLikeItemId = ItemOverride_SetProgressiveItemDraw(override);
    ItemRow* looksLikeRow = ItemTable_GetItemRow(looksLikeItemId);
    if (looksLikeRow == NULL) {
      looksLikeRow = itemRow;
    }

    rActiveIsTouchPickup = false;
    rActiveItemOverride = override;
    rActiveItemRow = itemRow;
    rActiveItemActionId = itemRow->itemId;
    rActiveItemTextId = itemRow->textId;
    rActiveItemObjectId = itemRow->objectId;
    rActiveItemGraphicId = override.value.getItemId == 0x12 ? 0x77 : (u32)looksLikeRow->graphicId;
    rActiveItemChestType = (u8)itemRow->chestType;
    isItemOverrideActive = true;
  }

  void ItemOverride_Clear(void) {
    rActiveItemOverride = (ItemOverride){0};
    rActiveItemRow = NULL;
    rActiveItemActionId = 0;
    rActiveItemTextId = 0;
    rActiveItemObjectId = 0;
    rActiveItemGraphicId = 0;
    rActiveItemChestType = 0;
    isItemOverrideActive = false;
    rCustomDungeonItemRetrieved = 0;
    rActiveIsTouchPickup = false;
    gItemOverrideSource = {game::act::Id::Player, GetItemID::GI_NONE};
  }

  void ItemOverride_PushPendingOverride(ItemOverride override) {
    for (u32 i = 0; i < ARR_SIZE(rPendingOverrideQueue); ++i) {
      if (rPendingOverrideQueue[i].key.all == 0) {
        rPendingOverrideQueue[i] = override;
        break;
      }
      if (rPendingOverrideQueue[i].key.all == override.key.all) {
        // Prevent duplicate entries
        break;
      }
    }
  }

  s32 ItemOverride_IsAPendingOverride(void) {
    return (rPendingOverrideQueue[0].key.all != 0);
  }

  void ItemOverride_PushDelayedOverride(u8 flag) {
    ItemOverride_Key key;
    key.all = 0;
    key.scene = 0xFF;
    key.type = ItemOverride_Type::OVR_DELAYED;
    key.flag = flag;
    ItemOverride override = ItemOverride_LookupByKey(key);
    if (override.key.all != 0) {
      ItemOverride_PushPendingOverride(override);
    }
  }

  static void ItemOverride_PopPendingOverride(void) {
    rPendingOverrideQueue[0] = rPendingOverrideQueue[1];
    rPendingOverrideQueue[1] = rPendingOverrideQueue[2];
    rPendingOverrideQueue[2].key.all = 0;
    rPendingOverrideQueue[2].value.all = 0;
  }

  static void ItemOverride_PopIceTrap(void) {
    ItemOverride_Key key = rPendingOverrideQueue[0].key;
    ItemOverride_Value value = rPendingOverrideQueue[0].value;
    // Make Ice Trap Item IDs.
    if (value.getItemId == 0x12) {
      IceTrap_Push();
      ItemOverride_PopPendingOverride();
      SpoilerLog_UpdateIngameLog(key.type, key.scene, key.flag);
    }
  }

  static u32 ItemOverride_PlayerIsReady(void) {
    // Using MMR's can receive item call - use the animation IDs to determine whether
    // we can receive item. Adjust pending frames as some items may softlock?
    game::GlobalContext* gctx = GetContext().gctx;
    if (!gctx || gctx->type != game::StateType::Play)
      return 0;
    game::act::Player* player = gctx->GetPlayerActor();
    if (!player)
      return 0;
    u32 currentAniId = player->player_util.state.id;
    switch (currentAniId) {
    case 0x026:  // Idle with enemies nearby - Human
    case 0x07A:  // Rolling - FD
    case 0x0B9:  // Walking - FD
    case 0x0BA:  // Sidewalking - FD
    case 0x0C2:  // Idle - FD
    case 0x0C5:  // Slow Walking - FD
    case 0x0DC:  // Backwalking - Human, Deku, Zora, Goron, FD
    case 0x11F:  // Rolling - Human, Zora, Goron Rolling
    case 0x135:  // Walking - Human, Deku, Zora, Goron
    case 0x13D:  // Sidewalking - Human, Deku, Zora, Goron
    case 0x150:  // Idle - Human, Deku
    case 0x155:  // ESS Maybe - Human, Deku, Zora, Goron
    case 0x158:  // Slow Walking - Human, Deku, Zora, Goron
    case 0x183:  // Idle - Human
    case 0x225:  // Walking with Weapon/Shield Drawn - Human
    case 0x2CF:  // Idle - Zora
    case 0x32D:  // Uncurling - Goron
    case 0x339:  // Idle - Goron
      rSatisfiedPendingFrames++;
      break;
    default:
      rSatisfiedPendingFrames = 0;
      break;
    }
    if (rSatisfiedPendingFrames >= 2) {
      rSatisfiedPendingFrames = 0;
      return 1;
    }
    return 0;
  }

  static void ItemOverride_TryPendingItem(void) {
    ItemOverride override = rPendingOverrideQueue[0];
    game::act::Player* player = GetContext().gctx->GetPlayerActor();
    if (player) {
      if (override.key.all == 0) {
        return;
      }
      // An item is still waiting to be applied (a stray fairy's message hook hasn't run yet). Starting this one now
      // would replace it in the active slot and lose it.
      if (rActiveItemRow != NULL) {
        return;
      }
      if (rDummyActor->parent_actor == NULL) {
        ItemOverride_Activate(override);
        player->grabbable_actor = rDummyActor;
        player->get_item_id = rActiveItemRow->baseItemId;
        ItemOverride_PopPendingOverride();
      } else {
        rDummyActor->parent_actor = NULL;
        ItemOverride_PopPendingOverride();
      }
    }
  }

  // getTalkStatus returns TEXT_STATE_NONE (0) when the message window is idle.
  static bool ItemOverride_TextBoxOpen(game::GlobalContext* gctx) {
    return util::GetPointer<int(game::MessageContext*)>(0x1C5018)(&gctx->msg_context) != 0;
  }

  static void ItemOverride_TryDeferredTouchPickup(game::GlobalContext* gctx, game::act::Player* player) {
    if (rDeferredTouchPickups[0].key.all == 0) {
      rDeferredWaitFrames = 0;
      return;
    }
    if (gctx->type != game::StateType::Play || player == NULL || gctx->IsPaused() || rActiveItemRow != NULL ||
        sTouchPickupHold > 0) {
      return;
    }
    if (ItemOverride_TextBoxOpen(gctx) && rDeferredWaitFrames++ < rDeferredMaxWaitFrames) {
      return;
    }
    rDeferredWaitFrames = 0;

    ItemOverride override = rDeferredTouchPickups[0];
    for (u32 i = 1; i < ARR_SIZE(rDeferredTouchPickups); ++i) {
      rDeferredTouchPickups[i - 1] = rDeferredTouchPickups[i];
    }
    rDeferredTouchPickups[ARR_SIZE(rDeferredTouchPickups) - 1] = (ItemOverride){0};

    // The rules a lone fairy goes through, then the steps its own message hook takes.
    ItemOverride_ApplyRepeatRules(NULL, override,
                                  ItemOverride_GetFairyGetItemFromScene((game::SceneId) override.key.scene));
    ItemOverride_Activate(override);
    if (rActiveItemRow == NULL) {
      return;
    }
    ItemOverride_HoldTouchPickup();  // marks the row as a touch pickup, so Update releases it once it is applied
    rStoredTextId = rActiveItemRow->textId;
    const u16 textId = rStoredTextId;
    ItemOverride_GetItemTextAndItemID(player);
    gctx->ShowMessage(textId);
    ItemOverride_RemoveTextId();
  }

  void ItemOverride_Update(void) {
    game::GlobalContext* gctx = GetContext().gctx;
    game::act::Player* player = gctx->GetPlayerActor();
    if (ItemOverride_PlayerIsReady()) {
      ItemOverride_PopIceTrap();
      if (IceTrap_IsPending()) {
        IceTrap_Give();
      } else {
        ItemOverride_TryPendingItem();
      }
    }

    if (sTouchPickupHold > 0) {
      sTouchPickupHold--;
    } else if (rActiveItemRow != NULL) {
      // A touch pickup has no get-item to wait for. Other get items (a chest's vanilla one) can leave get_item_id set,
      // which would keep this row active and block the pending queue forever.
      if (rActiveIsTouchPickup || player->get_item_id == 0 ||
          (player->grabbable_actor == NULL && player->flags1.IsSet(game::act::Player::Flag1::Unk40000000))) {
        ItemOverride_Clear();
      }
    }

    ItemOverride_TryDeferredTouchPickup(gctx, player);
  }

  static s32 ItemOverride_IsItemVanilla() {
    game::GlobalContext* gctx = GetContext().gctx;
    return rActiveItemRow == NULL && !gctx->IsPaused();
  }

  void ItemOverride_PushDungeonReward(u8 dungeon) {
    ItemOverride_Key key = {.all = 0};
    key.scene = 0xFF;
    key.type = ItemOverride_Type::OVR_TEMPLE;
    key.flag = dungeon;
    ItemOverride override = ItemOverride_LookupByKey(key);
    ItemOverride_PushPendingOverride(override);
  }

  u8 ItemOverride_SetProgressiveItemDraw(ItemOverride override) {
    game::SaveData saveData = game::GetCommonData().save;
    if (override.value.getItemId == 0x4A) {
      game::SwordType sword = saveData.equipment.sword_shield.sword;
      if (sword == game::SwordType::NoSword)
        return (u8)GetItemID::GI_KOKIRI_SWORD;
      else if (sword == game::SwordType::KokiriSword)
        return (u8)GetItemID::GI_RAZOR_SWORD;
      else
        return (u8)GetItemID::GI_GILDED_SWORD;
    } else if (override.value.getItemId == 0x49) {
      if (saveData.player.magic_acquired == 0) {
        return 0x0E;
      } else
        return 0x0F;
    } else if (override.value.getItemId == 0x48) {
      if (saveData.inventory.inventory_count_register.wallet_upgrade == 0)
        return 0x08;
      else
        return 0x09;
    } else if (override.value.getItemId == 0x47) {
      if (saveData.inventory.inventory_count_register.quiver_upgrade == game::Quiver::NoQuiver)
        return 0x22;
      else if (saveData.inventory.inventory_count_register.quiver_upgrade == game::Quiver::Quiver30)
        return 0x23;
      else
        return 0x24;
    } else if (override.value.getItemId == 0x46) {
      if (saveData.inventory.inventory_count_register.bomb_bag_upgrade == game::BombBag::NoBag)
        return 0x1B;
      else if (saveData.inventory.inventory_count_register.bomb_bag_upgrade == game::BombBag::BombBag20)
        return 0x1C;
      else
        return 0x1D;
    } else if (override.value.getItemId == 0x45) {
      // Even though the objects are the same we could change these later on to indicate different items.
      // So return the proper progressive item.
      if (saveData.inventory.collect_register.lullaby_intro.Value() == 0)
        return 0x74;
      else
        return 0x4D;
    }
    // No override, use the base item.
    return override.value.looksLikeItemId == 0xFF || override.value.looksLikeItemId == 0x00 ?
               override.value.getItemId :
               override.value.looksLikeItemId;
  }

  void ItemOverride_EditDrawGetItemBeforeModelSpawn() {
    if (ItemOverride_IsItemVanilla()) {
      return;
    }
    game::act::Player* player = GetContext().gctx->GetPlayerActor();
    CustomModels_EditItemCMB(player->actor_resource_file->archive.archive.raw, rActiveItemObjectId,
                             rActiveItemRow->special);
  }

  void ItemOverride_EditDrawGetItemAfterModelSpawn(game::act::SkeletonAnimationModel* saModel, u8 loopIdx) {
    // This is called in a loop oddly enough.
    // I'm assuming there's layered models depending on the item?
    // We only wanna edit the base item, not additional layering I believe, so just return after the first model.
    if (loopIdx != 0 || ItemOverride_IsItemVanilla())
      return;

    CustomModels_ApplyItemCMAB(saModel, rActiveItemObjectId, rActiveItemRow->special);
  }
}  // namespace rnd
