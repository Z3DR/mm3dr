#include "rnd/actors/en_cow.h"
#include "rnd/actors/en_fu.h"
#include "rnd/actors/en_kujiya.h"
#include "rnd/actors/en_lift_nuts.h"
#include "rnd/actors/en_zot.h"
#include "rnd/actors/item00.h"
#include "rnd/item_override.h"
#include "rnd/savefile.h"
#include "rnd/shops.h"

namespace rnd {
  static s32 rItemOverrides_Count = 0;
  ItemOverride rItemOverrides[640] = {0};

  static ItemOverride_Key sPendingShopKey = {0};
  static u16 sPendingShopScene = 0;
  static u8 rActiveTreasureGameForm = 0;

  void ItemOverride_InitLookup(void) {
#ifdef ENABLE_DEBUG
    // Manual overide example code
    rItemOverrides[0].key.scene = 0x6F;
    rItemOverrides[0].key.type = ItemOverride_Type::OVR_COLLECTABLE;
    rItemOverrides[0].value.getItemId = 0x56;
    rItemOverrides[0].value.looksLikeItemId = 0x56;
    rItemOverrides[1].key.scene = 0x6F;
    rItemOverrides[1].key.type = ItemOverride_Type::OVR_COLLECTABLE;
    rItemOverrides[1].value.getItemId = 0x4A;
    rItemOverrides[1].value.looksLikeItemId = 0x4A;
    rItemOverrides[2].key.scene = 0x12;
    rItemOverrides[2].key.type = ItemOverride_Type::OVR_COLLECTABLE;
    rItemOverrides[2].value.getItemId = 0x37;
    rItemOverrides[2].value.looksLikeItemId = 0x37;
#endif
    while (rItemOverrides[rItemOverrides_Count].key.all != 0) {
      rItemOverrides_Count++;
    }
  }

  static ItemOverride_Key ItemOverride_GetSearchKey(game::act::Actor* actor, u16 scene, s16 getItemId) {
    game::CommonData& cdata = game::GetCommonData();
    ItemOverride_Key retKey;
    retKey.all = 0;
    /*#if defined ENABLE_DEBUG || defined DEBUG_PRINT
            rnd::util::Print("%s: Retrieving search key for actor type %#04x and ID is %#04x, scene is %#04x\n",
    __func__, actor->actor_type, actor->id, scene); #endif*/
    if (actor->actor_type == game::act::Type::Chest) {
      retKey.scene = scene;
      retKey.type = ItemOverride_Type::OVR_CHEST;
      retKey.flag = actor->params & 0x1F;
      if (scene == (u16)game::SceneId::TreasureChestShop)
        retKey.flag = rActiveTreasureGameForm;
    } else if (actor->actor_type == game::act::Type::Misc) {  // Heart pieces are misc apparently
      // Only override heart pieces and keys
      const Item00Type collectibleType = static_cast<Item00Type>(actor->params & 0xFF);
      // XXX: AFAIK These are correct. Heart piece was checked.
      // Marine HP lab for some reason always is a fishing pass, 0xA.
      if (scene == (u16)game::SceneId::MarineResearchLaboratory &&
          cdata.save.week_event_reg_56.WEEKEVENTREG_RECEIVED_MARINE_RESEARCH_LAB_FISH_HEART_PIECE == 1) {
        return (ItemOverride_Key){.all = 0};
      } else if (scene != (u16)game::SceneId::MarineResearchLaboratory && collectibleType != Item00Type::HeartPiece &&
                 collectibleType != Item00Type::SmallKey) {
        return (ItemOverride_Key){.all = 0};
      }
      retKey.scene = scene;
      retKey.type = ItemOverride_Type::OVR_COLLECTABLE;
      retKey.flag = actor->overlay_info->info->flags;
    } else if (actor->id == game::act::Id::EnSi) {  // Gold Skulltula Token
      retKey.scene = scene;
      retKey.type = ItemOverride_Type::OVR_SKULL;
      retKey.flag = actor->params & 0xFF;
    } else if (actor->id == game::act::Id::EnElfOrg) {  // Stray Fairy
      retKey.scene = scene;
      retKey.type = ItemOverride_Type::OVR_STRAY_FAIRY;
      retKey.flag = ((actor->params << 0x10) >> 0x19);
    } else if (scene == 0x14C0 && actor->id == (game::act::Id)0x0075) {  // Grotto Salesman
      retKey.scene = cdata.sub13s[8].data;
      retKey.type = ItemOverride_Type::OVR_GROTTO_SCRUB;
      retKey.flag = getItemId;
    } else if (actor->id == game::act::Id::EnCow) {  // Cow
      if (!En_Cow_FillSearchKey(actor, (game::SceneId)scene, &retKey)) {
        return (ItemOverride_Key){.all = 0};
      }
    } else if (actor->actor_type == game::act::Type::Npc && actor->id == game::act::Id::EnFu) {
      if (scene == (u16)game::SceneId::HoneyAndDarling) {
        retKey = En_Fu_GetItemKeyBasedOnDay(actor, getItemId);
      }
    } else if (actor->id == game::act::Id::EnLiftNuts && scene == (u16)game::SceneId::DekuScrubPlayground) {
      retKey = EnLiftNuts_GetItemKeyBasedOnDay(getItemId);
    } else if (actor->id == game::act::Id::EnZot && scene == (u16)game::SceneId::ZoraHall) {
      retKey = EnZot_GetZoraHallKey(actor, getItemId);
    } else if (actor->id == game::act::Id::EnKujiya && scene == (u16)game::SceneId::LotteryShop) {
      retKey = EnKujiya_GetItemKeyBasedOnDay(getItemId);
    } else if (actor->id == game::act::Id::EnGirlA) {
      const ShopShelf shelf = Shopsanity_ResolveShelf((game::SceneId)scene, actor->params);
      if (Shopsanity_GetSlot(shelf.scene, shelf.param) < 0) {
        return (ItemOverride_Key){.all = 0};
      }
      retKey.scene = (u8)shelf.scene;
      retKey.type = ItemOverride_Type::OVR_SHOP;
      retKey.flag = (u8)shelf.param;
    } else if (sPendingShopKey.all != 0 && sPendingShopScene == scene) {
      retKey = sPendingShopKey;
    } else {
      retKey.scene = scene;
      retKey.type = ItemOverride_Type::OVR_BASE_ITEM;
      retKey.flag = getItemId;
    }
    return retKey;
  }

  ItemOverride ItemOverride_Lookup(game::act::Actor* actor, u16 scene, s16 getItemId) {
    /*#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        util::Print(
            "%s: Our param values:\nActor Type %#04x\nGet Item ID: %#04x\nActor ID: %#06x\n",
            __func__, actor->actor_type, getItemId, actor->id);
    #endif*/
    ItemOverride_Key key = ItemOverride_GetSearchKey(actor, scene, getItemId);
    if (key.all == 0) {
      return (ItemOverride){0};
    }
    return ItemOverride_LookupByKey(key);
  }

  void ItemOverride_SetTreasureGameForm(u8 form) {
    rActiveTreasureGameForm = form;
  }

  ItemOverride ItemOverride_LookupByKey(ItemOverride_Key key) {
    s32 start = 0;
    s32 end = rItemOverrides_Count - 1;
#ifdef ENABLE_DEBUG
    return rItemOverrides[1];
#endif
    while (start <= end) {
      s32 midIdx = (start + end) / 2;
      ItemOverride midOvr = rItemOverrides[midIdx];
      if (key.all < midOvr.key.all) {
        end = midIdx - 1;
      } else if (key.all > midOvr.key.all) {
        start = midIdx + 1;
      } else {
        return midOvr;
      }
    }
    return (ItemOverride){0};
  }

  void ItemOverride_SetPendingShopItem(ItemOverride_Key key, game::SceneId purchaseScene) {
    sPendingShopKey = key;
    sPendingShopScene = (u16)purchaseScene;
  }

  void ItemOverride_ClearPendingShop(void) {
    sPendingShopKey.all = 0;
  }
}  // namespace rnd
