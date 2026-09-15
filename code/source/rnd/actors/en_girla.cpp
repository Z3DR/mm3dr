#include "rnd/actors/en_girla.h"

#include <algorithm>

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
    case game::ItemId::MagicBean:
      return game::HasItem(game::ItemId::MagicBean);
    default:
      return true;
    }
  }

  static bool EnGirlA_HasFreeBottleSlot() {
    const auto& bottles = game::GetCommonData().save.inventory.bottles;
    return std::any_of(bottles.begin(), bottles.end(), [](game::ItemId id) { return id == game::ItemId::None; });
  }

  static bool EnGirlA_HasBottleSpace(const ItemOverride& ovr) {
    switch (ItemTable_ResolveUpgrades(ovr.value.getItemId)) {
    case 0x59:  // Bottle with Red Potion
    case 0x5A:  // Empty Bottle
    case 0x60:  // Bottle with Milk
    case 0x65:  // Bottle with Poe
    case 0x6A:  // Bottle with Gold Dust
    case 0x6F:  // Bottle with Chateau Romani
    case 0x70:  // Bottle with Mystery Milk
      return EnGirlA_HasFreeBottleSlot();  // a new bottle
    case 0x5B:  // Red Potion
    case 0x5C:  // Green Potion
    case 0x5D:  // Blue Potion
    case 0x5E:  // Fairy
    case 0x5F:  // Deku Princess (gives a fairy)
    case 0x62:  // Fish
    case 0x63:  // Bug
    case 0x66:  // Big Poe
    case 0x67:  // Spring Water
    case 0x68:  // Hot Spring Water
    case 0x69:  // Zora Egg
    case 0x6B:  // Magic Mushroom
    case 0x6E:  // Seahorse
    case 0x91:  // Chateau Romani refill
    case 0x92:  // Milk refill
    case 0x93:  // Gold Dust refill
    case 0x94:  // Mystery Milk refill
    case 0x95:  // Seahorse refill
      return game::HasBottle(game::ItemId::Bottle);  // fills an empty one
    default:
      return true;
    }
  }

  bool EnGirlA_IsSoldOut(En_GirlA* actor, game::GlobalContext* gctx, const ItemOverride& ovr) {
    if (actor == nullptr || gctx == nullptr || ovr.key.all == 0)
      return false;
    // No resolve as we don't want bombs to convert to rupees here.
    const ItemRow* row = ItemTable_GetItemRow(ovr.value.getItemId);
    if (row == nullptr)
      return false;

    if (Shopsanity_IsRestockable(row))
      return false;

    const ShopShelf shelf = Shopsanity_ResolveShelf(gctx->scene, actor->params);
    if (shelf.scene == game::SceneId::BombShop && shelf.param == kBigBombBagShelfParam &&
        gExtSaveData.givenItemChecks.stolenBombBagTaken != 0) {
      return true;
    }

    return Shopsanity_IsSoldOut(row, Shopsanity_GetSlot(shelf.scene, shelf.param));
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

  static bool EnGirlA_IsAllNightMaskShelf(game::act::Actor* actor, game::GlobalContext* gctx) {
    return gctx->scene == game::SceneId::CuriosityShop && actor->params == kAllNightMaskShelfParam;
  }

  // The All-Night Mask shelf draws the check En_Fsn hands over instead of a shelf override.
  static ItemOverride EnGirlA_LookupShelfOverride(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (!EnGirlA_IsAllNightMaskShelf(actor, gctx))
      return ItemOverride_Lookup(actor, (u16)gctx->scene, 0);

    ItemOverride_Key key = {.all = 0};
    key.scene = (u8)game::SceneId::CuriosityShop;
    key.type = ItemOverride_Type::OVR_BASE_ITEM;
    key.flag = (u8)GetItemID::GI_MASK_ALL_NIGHT;
    return ItemOverride_LookupByKey(key);
  }

  void EnGirlA_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<ActorOverlayFn>(0x39A7E0)(actor, gctx);  // vanilla EnGirlA::Init

    const ItemOverride ovr = EnGirlA_LookupShelfOverride(actor, gctx);
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
    const ItemOverride drawOvr = EnGirlA_LookupShelfOverride(actor, gctx);
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

    if (EnGirlA_IsAllNightMaskShelf(actor, gctx)) {
      if (EnGirlA_LookupShelfOverride(actor, gctx).key.all != 0) {
        actor->draw_fn = &EnGirlA_Draw;
        actor->field_258 = reinterpret_cast<void*>(&EnGirlA_ShowOverridden);
      }
      return;  // buying stays vanilla
    }

    const ItemOverride ovr = ItemOverride_Lookup(actor, (u16)gctx->scene, 0);
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

    const ItemOverride ovr = ItemOverride_Lookup(actor, (u16)gctx->scene, 0);
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

    const ItemOverride ovr = ItemOverride_Lookup(actor, (u16)gctx->scene, 0);
    if (EnGirlA_IsSoldOut(actor, gctx, ovr))
      return 2;  // sold out -- error jingle and the "you already have that" message

    // Ammo still sits on the shelf without a bomb bag or quiver, it just cannot be bought.
    const ItemRow* buyRow = ItemTable_GetItemRow(ovr.value.getItemId);
    if (buyRow != nullptr && !EnGirlA_HasAmmoContainer(buyRow))
      return 2;  // nothing to carry it in

    if (!EnGirlA_HasBottleSpace(ovr))
      return 2;  // nowhere to put it

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("%s: REACHED cost=%d\n", __func__, (int)gctx->msg_context.item_cost);
#endif
    const game::CommonData& cdata = game::GetCommonData();
    if (static_cast<s32>(cdata.save.player.rupee_count) < gctx->msg_context.item_cost)
      return 4;  // not enough rupees

    ItemOverride_SetPendingShopItem(ovr.key, gctx->scene);

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
