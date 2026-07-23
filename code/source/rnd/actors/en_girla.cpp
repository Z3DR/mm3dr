#include "rnd/actors/en_girla.h"

namespace rnd {

  using GetItemHandlerFn = void(game::GlobalContext*, u32);
  using SubtractRupeesFn = void(int);
  using SetModelScaleFn = void(game::act::Actor*, f32);

  // Resolve the shopsanity override for this shop actor (scene + EnGirlA param). Returns {} if
  // this slot is not a shuffled shop item. `slot` receives the price/override slot (= param).
  ItemOverride GetShopOverride(En_GirlA* actor, game::GlobalContext* gctx, s32& slot) {
    slot = -1;
    const u8 scene = static_cast<u8>(gctx->scene);

    slot = Shopsanity_GetSlot(gctx->scene, actor->params);
    if (slot < 0)
      return ItemOverride{};

    ItemOverride_Key key = {};
    key.scene = scene;
    key.type = ItemOverride_Type::OVR_SHOP;
    key.flag = static_cast<u8>(slot);
    ItemOverride ovr = ItemOverride_LookupByKey(key);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    if (scene == 52 && actor->params == 10) {
      ovr.key = key;
      ovr.value.getItemId = 0xBD;
      ovr.value.looksLikeItemId = 0xBD;
      rShopsanityPrices[slot] = 20;  // TEST price (debug only)
    }
#endif
    return ovr;
  }

  ItemRow* GetShopItemRow(En_GirlA* actor, game::GlobalContext* gctx) {
    s32 slot = -1;
    const ItemOverride ovr = GetShopOverride(actor, gctx, slot);
    if (ovr.key.all == 0)
      return nullptr;
    const u16 displayId = (ovr.value.looksLikeItemId != 0) ? ovr.value.looksLikeItemId : ovr.value.getItemId;
    return ItemTable_GetItemRow(ItemTable_ResolveUpgrades(displayId));
  }

  extern "C" {
  void EnGirlA_Randomize(En_GirlA* actor, game::GlobalContext* gctx) {
    if (actor == nullptr || gctx == nullptr)
      return;

    const u8 scene = static_cast<u8>(gctx->scene);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: id=0x%X scene=%u param=%d shopsanity=%u\n", __func__, (unsigned)actor->id, (unsigned)scene,
                (int)actor->params, (unsigned)gSettingsContext.shopsanity);
#endif

    const bool shopsanityOn = gSettingsContext.shopsanity != static_cast<u8>(ShopsanitySetting::SHOPSANITY_OFF);
#if !defined ENABLE_DEBUG && !defined DEBUG_PRINT
    if (!shopsanityOn)
      return;
#else
    (void)shopsanityOn;
#endif

    s32 slot = -1;
    const ItemOverride ovr = GetShopOverride(actor, gctx, slot);
    if (ovr.key.all == 0)
      return;  // this slot is not a shuffled shop item

    // Swap the sold item and redirect the buy handler so the placed item is granted on purchase.
    actor->get_item_id = static_cast<GetItemID>(ovr.value.getItemId);
    actor->buy_function = &EnGirlA_BuyOverriddenItem;
    actor->can_buy_function = &EnGirlA_CanBuyOverriddenItem;
    actor->field_238 = &EnGirlA_ApplyItemScale;

    const u16 displayId = (ovr.value.looksLikeItemId != 0) ? ovr.value.looksLikeItemId : ovr.value.getItemId;
    ItemRow* row = ItemTable_GetItemRow(ItemTable_ResolveUpgrades(displayId));
    if (row != nullptr) {
      s32 objSlot = Object_GetSlot(&gctx->object_context, static_cast<s16>(row->objectId));
      if (objSlot < 0)
        objSlot = Object_SpawnPersistent(&gctx->object_context, static_cast<s16>(row->objectId));

      ShopObjectTableEntry* entry = Shopsanity_GetObjectTableEntry(actor->object_table_index);

      entry->objectId = static_cast<s16>(row->objectId);
      entry->models[0].cmbIndex = static_cast<u8>(row->objectModelIdx);
      entry->models[0].cmabIndex = static_cast<u8>(row->cmabIndex);
      entry->models[1].cmbIndex = static_cast<u8>(row->objectModelIdx2);
      entry->models[1].cmabIndex = static_cast<u8>(row->cmabIndex2);
      entry->models[2].cmbIndex = 0xFF;
      entry->models[2].cmabIndex = 0xFF;
      actor->choice_text_id = row->textId;

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      util::Print("%s: model objTblIdx=%d -> objectId=0x%X slot=%d cmb=0x%X cmab=0x%X\n", __func__,
                  (int)actor->object_table_index, (unsigned)row->objectId, (int)objSlot, (unsigned)row->objectModelIdx,
                  (unsigned)(u8)row->cmabIndex);
#endif
    }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: slot=%d -> getItemId=0x%X price=%d\n", __func__, (int)slot, (unsigned)ovr.value.getItemId,
                (int)Shopsanity_GetPrice((u32)slot));
#endif
  }

  void EnGirlA_BuyOverriddenItem(game::GlobalContext* gctx, En_GirlA* actor) {
    if (actor == nullptr || gctx == nullptr)
      return;

    s32 slot = -1;
    const ItemOverride ovr = GetShopOverride(actor, gctx, slot);
    const bool alreadyOwned = ItemOverride_IsItemObtainedOrEmptyBottle(ovr);
    if (alreadyOwned)
      actor->can_buy_function = &EnGirlA_CanBuySoldOut;

    // Grant the placed item (resolving progressive upgrades, like ItemOverride_Activate does).
    if (ovr.key.all != 0 && ovr.value.getItemId != 0) {
      const u16 resolved = ItemTable_ResolveUpgrades(ovr.value.getItemId);
      if (resolved != 0)
        util::GetPointer<GetItemHandlerFn>(0x233BEC)(gctx, resolved);
    }

    actor->can_buy_function = &EnGirlA_CanBuySoldOut;

    // Charge the shopsanity price.
    const s32 price = (slot >= 0) ? Shopsanity_GetPrice(static_cast<u32>(slot)) : 0;
    gctx->msg_context.item_cost = price;
    util::GetPointer<SubtractRupeesFn>(0x2C1634)(-20);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: granted getItemId=0x%X, charged %d ovr key flag is %u\n", __func__,
                (unsigned)(ovr.key.all != 0 ? ovr.value.getItemId : 0), (int)price, ovr.key.flag);
#endif
  }

  extern "C" s32 EnGirlA_CanBuyOverriddenItem(game::GlobalContext* gctx) {
    if (gctx == nullptr)
      return 0;

    // Mirrors EnGirlA::CanBuyShieldMirror's affordability branch, minus the already-own gate.
    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    return 0;  // can buy
  }

  s32 EnGirlA_CanBuySoldOut(game::GlobalContext* gctx) {
    (void)gctx;
    return 2;  // vanilla "you already have that" -- blocks the purchase
  }

  void EnGirlA_ApplyItemScale(En_GirlA* actor, game::GlobalContext* gctx) {
    if (actor == nullptr || gctx == nullptr)
      return;
    const ItemRow* row = GetShopItemRow(actor, gctx);
    if (row == nullptr)
      return;

    util::GetPointer<SetModelScaleFn>(0x21E30C)(actor, 0.25f);
  }

  void EnGirlA_AfterModelLoad(En_GirlA* actor, game::GlobalContext* gctx) {
    if (actor == nullptr || gctx == nullptr || actor->skelAnimeModel == nullptr)
      return;
    const ItemRow* row = GetShopItemRow(actor, gctx);
    if (row == nullptr)
      return;

    // Items whose colour is data-driven (songs/ocarinas pick a hue via the custom TEXANIM_SONG
    // CMAB frame, small keys via material edits) can't be expressed by the object-table's plain
    // cmabIndex, so apply the randomizer's own CMAB pass -- the same call models.cpp uses.
    CustomModels_ApplyItemCMAB(actor->skelAnimeModel, row->objectId, row->special);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: applied CMAB objectId=0x%X special=%d scale=%d/1000\n", __func__, (unsigned)row->objectId,
                (int)row->special, (int)(row->scale * 1000.0f));
#endif
  }
  }

}  // namespace rnd
