#include "rnd/actors/en_girla.h"

namespace rnd {

  using GetItemHandlerFn = void(game::GlobalContext*, u32);
  using SubtractRupeesFn = void(int);
  using ActorOverlayFn = void(game::act::Actor*, game::GlobalContext*);

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
      ovr.value.getItemId = 0x54;
      ovr.value.looksLikeItemId = 0x54;
      rShopsanityPrices[slot] = 20;  // TEST price (debug only)
    }
#endif
    return ovr;
  }

  void EnGirlA_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<ActorOverlayFn>(0x39A7E0)(actor, gctx);  // vanilla EnGirlA::Init

    const bool shopsanityOn = gSettingsContext.shopsanity != static_cast<u8>(ShopsanitySetting::SHOPSANITY_OFF);
#if !defined ENABLE_DEBUG && !defined DEBUG_PRINT
    if (!shopsanityOn)
      return;
#else
    (void)shopsanityOn;
#endif

    s32 slot = -1;
    const ItemOverride ovr = GetShopOverride(reinterpret_cast<En_GirlA*>(actor), gctx, slot);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: RAN scene=%u param=%d slot=%d ovr.all=0x%X getItemId=0x%X\n", __func__,
                (unsigned)static_cast<u8>(gctx->scene), (int)actor->params, (int)slot, (unsigned)ovr.key.all,
                (unsigned)ovr.value.getItemId);
#endif
    if (ovr.key.all == 0)
      return;  // not shuffled: leave the vanilla shelf model in place

    Model_SpawnByActorFromOverride(actor, gctx, ovr, ovr.value.getItemId);
  }
  void EnGirlA_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    // This is kind of a weird edge case.
    // Since we control assigning the draw function in the init call,
    // we know that the model is actually present and there is no vanilla override.
    // However, we can use this as a safeguard to ensure the vanilla item is drawn instead.
    if (!Model_DrawByActor(actor)) {
      util::GetPointer<ActorOverlayFn>(0x3FAAC4)(actor, gctx);
    }
  }

  void EnGirlA_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    util::GetPointer<ActorOverlayFn>(0x39A840)(self, gctx);
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
    actor->draw_fn = &EnGirlA_Draw;

    const u16 displayId = (ovr.value.looksLikeItemId != 0) ? ovr.value.looksLikeItemId : ovr.value.getItemId;
    ItemRow* row = ItemTable_GetItemRow(ItemTable_ResolveUpgrades(displayId));
    // TODO: Change this to custom shopsanity text.
    // if (row != nullptr){
    //  actor->choice_text_id = 0x614A + slot;
    //  actor->text_id_maybe = 0x614A + slot + 1;
    // }
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

  s32 EnGirlA_CanBuyOverriddenItem(game::GlobalContext* gctx) {
    if (gctx == nullptr)
      return 0;

    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    return 0;  // can buy
  }

  s32 EnGirlA_CanBuySoldOut(game::GlobalContext* gctx) {
    const ItemOverride ovr = GetShopOverride(actor, gctx, slot);
    const bool alreadyOwned = ItemOverride_IsItemObtainedOrEmptyBottle(ovr);
    if (alreadyOwned)
      return 2;  // vanilla "you already have that" -- blocks the purchase
    return 0;
  }
  }

}  // namespace rnd
