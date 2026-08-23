#include "rnd/actors/en_box.h"
#include "rnd/item_table.h"

namespace rnd {
  extern "C" {
  game::actors::EnBoxType En_Box_OverrideSize(game::actors::En_Box* actor, game::GlobalContext* gctx) {
    if (gSettingsContext.chestSize == 0) {
      return (game::actors::EnBoxType)0xFF;
    }
    s16 gid = (actor->dyna.params << 0x14) >> 0x19;
    game::SceneId scene = gctx->scene;

    ItemOverride override;
    if (gid == 0x11) {
      ItemOverride_Key key = {.all = 0};
      key.scene = (u8)scene;
      key.type = ItemOverride_Type::OVR_STRAY_FAIRY;
      key.flag = actor->dyna.params & 0x1F;
      override = ItemOverride_LookupByKey(key);
    } else {
      override = ItemOverride_Lookup((game::act::Actor*)&actor->dyna, (u16)scene, gid);
    }

    if (override.key.all == 0) {
      return (game::actors::EnBoxType)0xFF;
    }

    ItemRow* itemToBeGiven = ItemTable_GetItemRow(override.value.getItemId);
    if (itemToBeGiven == NULL) {
      return (game::actors::EnBoxType)0xFF;  // gap entry -- leave the chest alone
    }
    if (itemToBeGiven->chestType == ChestType::WOODEN_SMALL) {
      return game::actors::EnBoxType::ENBOX_TYPE_SMALL;
    } else if (itemToBeGiven->chestType == ChestType::WOODEN_BIG) {
      return game::actors::EnBoxType::ENBOX_TYPE_BIG;
    } else if (itemToBeGiven->chestType == ChestType::DECORATED_BIG) {
      return game::actors::EnBoxType::ENBOX_TYPE_BIG_ORNATE;
    }
    return (game::actors::EnBoxType)0xFF;
  }

  u8 En_Box_IsOverrideEnabled(game::actors::En_Box* actor, u16 chestType) {
    s16 gid = (actor->dyna.params << 0x14) >> 0x19;
    if (gid == 0x3C && gSettingsContext.chestSize == 0)
      return false;
    if (gid == 0x3C && gSettingsContext.chestSize == 1 &&
        (chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL ||
         chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL_ROOM_CLEAR ||
         chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL_SWITCH_FLAG))
      return true;
    else if (chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL ||
             chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL_ROOM_CLEAR ||
             chestType == (u16)game::actors::EnBoxType::ENBOX_TYPE_SMALL_SWITCH_FLAG)
      return true;
    else
      return false;
  }

  bool En_Box_OverrideAnimation() {
    if ((gSettingsContext.chestAnimations == (u8)ChestAnimationsSetting::CHESTANIMATIONS_ALWAYSFAST) ||
        (!isItemOverrideActive))  // The animation is always fast for unused chests that aren't randomized
      return false;

    switch ((ChestType)rActiveItemChestType) {
    case ChestType::WOODEN_BIG:
    case ChestType::DECORATED_BIG:
      return true;
    case ChestType::WOODEN_SMALL:
    case ChestType::DECORATED_SMALL:
      return false;
    }
    return false;
  }

  bool En_Box_IsOrnateChest(game::actors::En_Box* actor) {
    return actor->chest_type == game::actors::EnBoxType::ENBOX_TYPE_BIG_ORNATE;
  }
  }

}  // namespace rnd