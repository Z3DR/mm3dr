#include "rnd/actors/en_girla.h"

namespace rnd {

  using ChargeItemCostFn = void(game::GlobalContext*);
  using ActorOverlayFn = void(game::act::Actor*, game::GlobalContext*);

  void EnGirlA_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<ActorOverlayFn>(0x39A7E0)(actor, gctx);  // vanilla EnGirlA::Init

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

    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
    if (ovr.key.all == 0)
      return;

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: scene=%#04x param=%#04x ovrGI=%#04x vanillaGI=%#04x\n", __func__,
                (unsigned)static_cast<u8>(gctx->scene), (unsigned)actor->params,
                (unsigned)ovr.value.getItemId, (unsigned)actor->get_item_id);
#endif
    ItemRow* row = ItemTable_GetItemRow(ItemTable_ResolveUpgrades(ovr.value.getItemId));
    actor->get_item_id = row != nullptr ? static_cast<GetItemID>(row->baseItemId)
                                        : static_cast<GetItemID>(ovr.value.getItemId);
    actor->buy_function = &EnGirlA_BuyOverriddenItem;
    actor->can_buy_function = &EnGirlA_CanBuyOverriddenItem;
    actor->draw_fn = &EnGirlA_Draw;

    // TODO: Change this to custom shopsanity text.
    // ItemRow* row = ItemTable_GetItemRow(ItemOverride_SetProgressiveItemDraw(ovr));
    // if (row != nullptr){
    //  actor->choice_text_id = 0x614A + slot;
    //  actor->text_id_maybe = 0x614A + slot + 1;
    // }

  }

  void EnGirlA_BuyOverriddenItem(game::GlobalContext* gctx, En_GirlA* actor) {
    if (actor == nullptr || gctx == nullptr)
      return;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: REACHED param=%#04x\n", __func__, (unsigned)actor->params);
#endif

    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
    if (ovr.key.all == 0)
      return;

    ItemOverride_GiveShopItem(actor, gctx);

    actor->can_buy_function = &EnGirlA_CanBuySoldOut;

    util::GetPointer<ChargeItemCostFn>(0x2C1620)(gctx);


  }

  s32 EnGirlA_CanBuyOverriddenItem(game::GlobalContext* gctx, En_GirlA* actor) {
    // TODO: More cases will need to be added such as no bottle available for liquids
    // Or shields that are already obtained.
    if (gctx == nullptr || actor == nullptr)
      return 2;  // refuse the sale rather than hand off an unresolved item

    
    const ItemOverride ovr = ItemOverride_LookupShopItem(actor, gctx);
    ItemOverride_SetPendingShopItem(ovr.key);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: REACHED cost=%d\n", __func__, (int)gctx->msg_context.item_cost);
#endif
    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    
    return 0;
  }

  s32 EnGirlA_CanBuySoldOut(game::GlobalContext* gctx, En_GirlA* actor) {
    // TODO: Add more guards to can buy sold out as well?
    (void)gctx;
    (void)actor;
    return 2;  // vanilla "you already have that" -- blocks the purchase
  }
  }

}  // namespace rnd
