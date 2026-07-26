#include "rnd/actors/en_girla.h"

namespace rnd {

  using SubtractRupeesFn = void(int);
  using ActorOverlayFn = void(game::act::Actor*, game::GlobalContext*);

  void EnGirlA_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<ActorOverlayFn>(0x39A7E0)(actor, gctx);  // vanilla EnGirlA::Init

    const bool shopsanityOn = gSettingsContext.shopsanity != static_cast<u8>(ShopsanitySetting::SHOPSANITY_OFF);
#if !defined ENABLE_DEBUG && !defined DEBUG_PRINT
    if (!shopsanityOn)
      return;
#else
    (void)shopsanityOn;
#endif

    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: RAN scene=%u param=%d slot=%d ovr.all=0x%X getItemId=0x%X\n", __func__,
                (unsigned)static_cast<u8>(gctx->scene), (int)actor->params, (int)ovr.key.flag, (unsigned)ovr.key.all,
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

    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
    if (ovr.key.all == 0)
      return;

    // Swap the sold item and redirect the buy handler so the placed item is granted on purchase.
    actor->get_item_id = static_cast<GetItemID>(ovr.value.getItemId);
    actor->buy_function = &EnGirlA_BuyOverriddenItem;
    actor->can_buy_function = &EnGirlA_CanBuyOverriddenItem;
    actor->draw_fn = &EnGirlA_Draw;

    // TODO: Change this to custom shopsanity text.
    // ItemRow* row = ItemTable_GetItemRow(ItemOverride_SetProgressiveItemDraw(ovr));
    // if (row != nullptr){
    //  actor->choice_text_id = 0x614A + slot;
    //  actor->text_id_maybe = 0x614A + slot + 1;
    // }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: slot=%d -> getItemId=0x%X price=%d\n", __func__, (int)ovr.key.flag, (unsigned)ovr.value.getItemId,
                (int)Shopsanity_GetPrice(ovr.key.flag));
#endif
  }

  void EnGirlA_BuyOverriddenItem(game::GlobalContext* gctx, En_GirlA* actor) {
    if (actor == nullptr || gctx == nullptr)
      return;

    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
    if (ovr.key.all == 0)
      return;

    ItemOverride_GiveShopItem(actor, gctx);

    actor->can_buy_function = &EnGirlA_CanBuySoldOut;

    // Charge the shopsanity price.
    const s32 price = Shopsanity_GetPrice(ovr.key.flag);
    gctx->msg_context.item_cost = price;
    util::GetPointer<SubtractRupeesFn>(0x2C1634)(-20/*price*/);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: granted getItemId=0x%X, charged %d ovr key flag is %u\n", __func__, (unsigned)ovr.value.getItemId,
                (int)price, ovr.key.flag);
#endif
  }

  s32 EnGirlA_CanBuyOverriddenItem(game::GlobalContext* gctx) {
    // TODO: More cases will need to be added such as no bottle available for liquids
    // Or shields that are already obtained.
    if (gctx == nullptr)
      return 0;

    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    return 0;  // can buy
  }

  s32 EnGirlA_CanBuySoldOut(game::GlobalContext* gctx) {
    // TODO: Add more guards to can buy sold out as well?
    (void)gctx;
    return 2;  // vanilla "you already have that" -- blocks the purchase
  }
  }

}  // namespace rnd
