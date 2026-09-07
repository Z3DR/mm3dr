#include "rnd/actors/en_girla.h"

namespace rnd {

  using ChargeItemCostFn = void(game::GlobalContext*);
  using ActorOverlayFn = void(game::act::Actor*, game::GlobalContext*);

  static bool EnGirlA_HasAmmoContainer(const ItemRow* row) {
    const auto& counts = game::GetCommonData().save.inventory.inventory_count_register;
    switch ((game::ItemId)row->itemId) {
    case game::ItemId::Bomb:
    case game::ItemId::FiveBombs:
    case game::ItemId::TenBombs:
    case game::ItemId::TwentyBombs:
    case game::ItemId::ThirtyBombs:
    case game::ItemId::Bombchu:
    case game::ItemId::OneBombchu:
    case game::ItemId::OneBombchuAgain:
    case game::ItemId::FiveBombchu:
    case game::ItemId::TenBombchus:
      return counts.bomb_bag_upgrade.Value() != game::BombBag::NoBag;
    case game::ItemId::Arrow:
    case game::ItemId::TenArrows:
    case game::ItemId::ThirtyArrows:
    case game::ItemId::FortyArrows:
    case game::ItemId::FiftyArrows:
      return counts.quiver_upgrade.Value() != game::Quiver::NoQuiver;
    default:
      return true;
    }
  }

  // Consumables restock forever; everything else is a one-time purchase.
  static bool EnGirlA_IsRestockable(const ItemRow* row) {
    switch ((game::ItemId)row->itemId) {
    case game::ItemId::Arrow:
    case game::ItemId::TenArrows:
    case game::ItemId::ThirtyArrows:
    case game::ItemId::FortyArrows:
    case game::ItemId::FiftyArrows:
    case game::ItemId::Bomb:
    case game::ItemId::FiveBombs:
    case game::ItemId::TenBombs:
    case game::ItemId::TwentyBombs:
    case game::ItemId::ThirtyBombs:
    case game::ItemId::Bombchu:
    case game::ItemId::OneBombchu:
    case game::ItemId::OneBombchuAgain:
    case game::ItemId::FiveBombchu:
    case game::ItemId::TenBombchus:
    case game::ItemId::DekuStick:
    case game::ItemId::TenSticks:
    case game::ItemId::TenSticksAgain:
    case game::ItemId::TwentySticks:
    case game::ItemId::ThirtySticks:
    case game::ItemId::DekuNuts:
    case game::ItemId::FiveNuts:
    case game::ItemId::TenNuts:
    case game::ItemId::ThirtyNuts:
    case game::ItemId::FortyNuts:
    case game::ItemId::MagicBean:
    case game::ItemId::PowderKeg:
    case game::ItemId::ChateauRomaniFill:
    case game::ItemId::MilkFill:
    case game::ItemId::GoldDustFill:
    case game::ItemId::SeahorseFill:
    case game::ItemId::RecoveryHeart:
    case game::ItemId::OneRupee:
    case game::ItemId::FiveRupees:
    case game::ItemId::TenRupees:
    case game::ItemId::TwentyRupees:
    case game::ItemId::FiftyRupees:
    case game::ItemId::OneHundredRupees:
    case game::ItemId::TwoHundredRupees:
      return true;
    default:
      return false;
    }
  }

  bool EnGirlA_IsSoldOut(En_GirlA* actor, game::GlobalContext* gctx, const ItemOverride& ovr) {
    if (actor == nullptr || gctx == nullptr || ovr.key.all == 0)
      return false;
    // No resolve as we don't want bombs to convert to rupees here.
    const ItemRow* row = ItemTable_GetItemRow(ovr.value.getItemId);
    if (row == nullptr)
      return false;

    if (EnGirlA_IsRestockable(row))
      return false;

    return Shopsanity_IsSlotPurchased(Shopsanity_GetSlot(gctx->scene, actor->params));
  }

  // Bought out: draw nothing at all, leaving the shelf empty.
  void EnGirlA_DrawSoldOut(game::act::Actor*, game::GlobalContext*) {}

  void EnGirlA_ShowOverridden(game::GlobalContext* gctx, En_GirlA* actor) {
    (void)gctx;
    if (actor == nullptr)
      return;
    actor->field_250 = 0;
    actor->draw_fn = &EnGirlA_Draw;
  }

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

    if (EnGirlA_IsSoldOut(static_cast<En_GirlA*>(actor), gctx, ovr))
      return;  // nothing to spawn; EnGirlA_Randomize suppresses the vanilla model too

    Model_SpawnByActorFromOverride(actor, gctx, ovr, ovr.value.getItemId);
  }
  void EnGirlA_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    const ItemOverride drawOvr = ItemOverride_LookupShopItem(actor, gctx);
    const bool drawSoldOut = EnGirlA_IsSoldOut(static_cast<En_GirlA*>(actor), gctx, drawOvr);
    if (drawSoldOut) {
      return;
    }

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
    // The shop hides the shelf on purchase and shows it again when the follow-up
    // textbox closes.
    actor->field_258 = reinterpret_cast<void*>(&EnGirlA_ShowOverridden);

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
    if (EnGirlA_IsSoldOut(actor, gctx, ovr))
      return 2;  // sold out -- error jingle and the "you already have that" message

    // Ammo still sits on the shelf without a bomb bag or quiver, it just cannot be bought.
    const ItemRow* buyRow = ItemTable_GetItemRow(ovr.value.getItemId);
    if (buyRow != nullptr && !EnGirlA_HasAmmoContainer(buyRow))
      return 2;  // nothing to carry it in

    ItemOverride_SetPendingShopItem(ovr.key);

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: REACHED cost=%d\n", __func__, (int)gctx->msg_context.item_cost);
#endif
    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    Shopsanity_SetSlotPurchased(Shopsanity_GetSlot(gctx->scene, actor->params));

    return 0;
  }

  s32 EnGirlA_CanBuySoldOut(game::GlobalContext* gctx, En_GirlA* actor) {
    (void)gctx;
    (void)actor;
    return 2;  // vanilla "you already have that" -- blocks the purchase
  }
  }

}  // namespace rnd
