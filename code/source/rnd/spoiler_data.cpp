#include "rnd/spoiler_data.h"
#include "game/common_data.h"
#include "rnd/item_override.h"
#include "rnd/settings.h"
#include "z3d/z3DVec.h"

namespace rnd {
  SpoilerData gSpoilerData = {0};
  SpoilerDataLocs gSpoilerDataLocs[SPOILER_LOCDATS] = {0};

  typedef struct {
    ItemOverride_Type type;
    u8                flag;
    const u8*         scenes;
    u8                numScenes;
  } MultiLocGroup;
  #define MULTI_LOC(t, f, ...) (MultiLocGroup){ \
    .type = t, \
    .flag = f, \
    .scenes = (const u8[])__VA_ARGS__, \
    .numScenes = sizeof((u8[])__VA_ARGS__) / sizeof(u8) \
  }
  const MultiLocGroup multiLocs[] = {
    // Koume
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x59, { 0x0A, 0x64 }),
    // Postboxes
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xBA, { 0x6C, 0x6E, 0x6F }),
    // Tingle Clocktown Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB4, { 0x13, 0x6E }),
    // Tingle Woodfall Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB5, { 0x40, 0x6E }),
    // Tingle Snowhead Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB6, { 0x40, 0x5D, 0x5E }),
    // Tingle Ranch Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB7, { 0x22, 0x5D, 0x5E }),
    // Tingle Great Bay Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB8, { 0x22, 0x37 }),
    // Tingle Stone Tower Map
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0xB9, { 0x13, 0x37 }),
    // Keaton
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x03, { 0x22, 0x5A, 0x6E }),
    // GV Powder Keg Challenge
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x34, { 0x48, 0x4D }),
    // GV Deku Merchant Purchase
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x1D, { 0x48, 0x4D }),
    // GV Deku Merchant Trade
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x99, { 0x48, 0x4D }),
    // GV Ledge Heart Piece
    MULTI_LOC(ItemOverride_Type::OVR_COLLECTABLE, 0x00, { 0x48, 0x4D }),
    // SS Deku Merchant Purchase
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x35, { 0x00, 0x45 }),
    // SS Deku Merchant Trade
    MULTI_LOC(ItemOverride_Type::OVR_BASE_ITEM, 0x98, { 0x00, 0x45 }),
    // SS Tourist Center Roof HP
    MULTI_LOC(ItemOverride_Type::OVR_COLLECTABLE, 0x00, { 0x00, 0x45 }),
    // STT Death Armos Chest
    MULTI_LOC(ItemOverride_Type::OVR_CHEST, 0x05, { 0x16, 0x18 }),
  };
  #undef MULTI_LOC

  SpoilerItemLocation* SpoilerData_ItemLoc(u16 itemIndex) {
    return &gSpoilerDataLocs[itemIndex / SPOILER_ITEMS_MAX].ItemLocations[itemIndex % SPOILER_ITEMS_MAX];
  }

  SpoilerItemCollectType SpoilerData_CollectType(u16 itemIndex) {
    return gSpoilerData.ItemLocations[itemIndex].CollectType;
  }

  bool SpoilerData_IndexIs(int itemIndex, ItemOverride_Type type, u8 scene, u8 flag) {
    return gSpoilerData.ItemLocations[itemIndex].LocationScene == scene &&
           gSpoilerData.ItemLocations[itemIndex].OverrideType == type &&
           gSpoilerData.ItemLocations[itemIndex].LocationFlag == flag;
  }

  bool SpoilerData_IsUniqueLocation(u16 itemIndex) {
    // Ensure itemIndex does not match any of the multi locations' alternate scenes
    for (size_t idx = 0; idx < sizeof(multiLocs) / sizeof(MultiLocGroup); idx++) {
      if (gSpoilerData.ItemLocations[itemIndex].OverrideType == multiLocs[idx].type &&
          gSpoilerData.ItemLocations[itemIndex].LocationFlag == multiLocs[idx].flag
      ) {
        // Treat first scene in multi group as unique -> start checking from index 1
        for (u8 sceneIdx = 1; sceneIdx < multiLocs[idx].numScenes; sceneIdx++) {
          if (gSpoilerData.ItemLocations[itemIndex].LocationScene == multiLocs[idx].scenes[sceneIdx]) {
            return false;
          }
        }
      }
    }

    // Didn't match any, location is unique
    return true;
  }

  char* SpoilerData_StringData(u16 itemIndex) {
    return gSpoilerDataLocs[itemIndex / SPOILER_ITEMS_MAX].StringData;
  }

  char* SpoilerData_GetItemLocationString(u16 itemIndex) {
    return &gSpoilerData.StringData[gSpoilerData.ItemLocations[itemIndex].LocationStrOffset];
  }

  char* SpoilerData_GetItemNameString(u16 itemIndex) {
    return &gSpoilerData.StringData[gSpoilerData.ItemLocations[itemIndex].ItemStrOffset];
  }

  SpoilerItemLocation GetSpoilerItemLocation(u8 sphere, u16 itemIndex) {
    return gSpoilerData
        .ItemLocations[gSpoilerData.SphereItemLocations[gSpoilerData.Spheres[sphere].ItemLocationsOffset]];
  }

  u8 SpoilerData_ChestCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  u8 SpoilerLog_UpdateIngameLog(ItemOverride_Type type, u8 scene, u8 flag) {
    // SpoilerData currentCheck = {0};
    for (int i = 0; i < gSpoilerData.ItemLocationsCount; i++) {
      if (SpoilerData_IndexIs(i, type, scene, flag)) {
        gSpoilerData.ItemLocations[i].Collected = true;
        // Since it's not saved here, we need to return
        gExtSaveData.itemCollected[i] = 1;
        break;  // stop searching
      }
    }
    SpoilerLog_CheckMultiLocationItems(type, scene, flag);
    return -1;
  }
  u8 SpoilerLog_CheckMultiLocationItems(ItemOverride_Type type, u8 scene, u8 flag) {
    //#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    //    rnd::util::Print(
    //        "%s: CheckMultiLocationItems passed values:\nActor Type %d\nScene: %d\nFlag: %d\n",
    //        __func__, type, scene, flag);
    //#endif
    //#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    //    rnd::util::Print("%s: passed the scene check for %d\n", __func__,scene);
    //#endif
    for (size_t idx = 0; idx < sizeof(multiLocs) / sizeof(MultiLocGroup); idx++) {
      if (type == multiLocs[idx].type && flag == multiLocs[idx].flag) {
        bool match = false;
        for (u8 sceneIdx = 0; sceneIdx < multiLocs[idx].numScenes; sceneIdx++) {
          if (scene == multiLocs[idx].scenes[sceneIdx]) {
            match = true;
            break;
          }
        }

        if (match) {
          for (u8 sceneIdx = 0; sceneIdx < multiLocs[idx].numScenes; sceneIdx++) {
            if (scene != multiLocs[idx].scenes[sceneIdx]) {
              SpoilerData_UpdateMultiLocations(type, multiLocs[idx].scenes[sceneIdx], flag);
            }
          }
        }
      }
    }
    return -1;
  }
  void SpoilerData_UpdateMultiLocations(ItemOverride_Type type, u8 newScene, u8 flag) {
    for (int i = 0; i < gSpoilerData.ItemLocationsCount; i++) {
      if (SpoilerData_IndexIs(i, type, newScene, flag)) {
        gSpoilerData.ItemLocations[i].Collected = true;
        // Since it's not saved here, we need to return
        gExtSaveData.itemCollected[i] = 1;
        break;
      }
    }
  }
  u8 SpoilerData_CollectableCheck(SpoilerItemLocation itemLoc) {
    // TODO: Implement Collectable Checking. no need to use bits as we have
    // builtin BitField classes.
    // Reference:
    // https://github.com/gamestabled/OoT3D_Randomizer/blob/e53be23c14090b15c6c39e08933ca7af54f747f7/code/src/spoiler_data.c#L34-L41
    return -1;
  }

  // Shop checks, will need to be decomped, most likely in common_data.h.
  u8 SpoilerData_ItemGetInfCheck(u8 slot) {
    // TODO: Reimplement GetInfCheck. no need to use bits as we have
    // builtin BitField classes.
    // Reference:
    // https://github.com/gamestabled/OoT3D_Randomizer/blob/e53be23c14090b15c6c39e08933ca7af54f747f7/code/src/spoiler_data.c#L43-L49
    return -1;
  }

  // Event flags, will need to be decomped in common_data.h.
  u8 SpoilerData_InfTableCheck(u8 offset, u8 bit) {
    // TODO: Discern all event flags.
    // TODO: Reimplement with BitField class.
    // Reference:
    // https://github.com/gamestabled/OoT3D_Randomizer/blob/e53be23c14090b15c6c39e08933ca7af54f747f7/code/src/spoiler_data.c#L51-L57
    return -1;
  }

  // Once again, most likely in SaveData, to be decomped.
  u8 SpoilerData_QuestItemCheck(u8 slot) {
    // TODO: Discern all event flags.
    // TODO: Reimplement with BitField class.
    // Reference:
    // https://github.com/gamestabled/OoT3D_Randomizer/blob/e53be23c14090b15c6c39e08933ca7af54f747f7/code/src/spoiler_data.c#L59-L62
    return -1;
  }

  // Item upgrades.
  // TODO: Implement.
  u8 SpoilerData_UpgradeCheck(u8 bit) {
    return -1;
  }

  // TODO: Decomp/Implmenet cow flags.
  u8 SpoilerData_CowCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  // TODO: Decomp/implement fishing flags.
  u8 SpoilerData_FishingCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  // TODO: Decomp/implement scrub location/checks.
  u8 SpoilerData_ScrubCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  // TODO: Decomp shop/items.
  u8 SpoilerData_ShopItemCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  // TODO: Decomp bean daddy.
  u8 SpoilerData_MagicBeansCheck(SpoilerItemLocation itemLoc) {
    return -1;
  }

  u8 SpoilerData_GetIsItemLocationCollected(u16 itemIndex) {
    if (itemIndex >= gSpoilerData.ItemLocationsCount) {
      return 0;
    }

    SpoilerItemLocation itemLoc = gSpoilerData.ItemLocations[itemIndex];
    if (itemLoc.Collected == true) {
      return 1;
    } else if (gExtSaveData.itemCollected[itemIndex] == 1) {
      return 1;
    }
    if (itemLoc.CollectionCheckType == SPOILER_CHK_ALWAYS_COLLECTED) {
      return 1;
    }
    if (itemLoc.CollectionCheckType == SPOILER_CHK_NONE) {
      return 0;
    }
    return 0;
    /*
    // game::SaveData &gSaveContext = game::GetCommonData().save;
    switch (itemLoc.CollectionCheckType) {
    case SPOILER_CHK_NONE: {  // Not ever 'collectable' (Ganon, or any item that didn't have a type
                              // set)
      return 0;
    }
    case SPOILER_CHK_ALWAYS_COLLECTED: {  // Items that are considered always collected, like Link's
                                          // Pocket
      return 1;
    }
    case SPOILER_CHK_CHEST: {  // Chest
      return rnd::SpoilerData_ChestCheck(itemLoc);
    }
    case SPOILER_CHK_COLLECTABLE: {  // Placed in world, saved in the collect flags of a scene
      return SpoilerData_CollectableCheck(itemLoc);
    }
    case SPOILER_CHK_SWAMP_SKULLTULA: {  // Swamp skulltula
      // gSaveContext.skulltulas_collected.swamp_count
      return -1;
    }
    case SPOILER_CHK_OCEAN_SKULLTULA: {  // Ocean skulltula
      // gSaveContext.skulltulas_collected.ocean_count
      return -1;
    }
    case SPOILER_CHK_STRAY_FAIRY: {
      // gSaveContext.strayfairies_collected
      return -1;
    }
    case SPOILER_CHK_ITEM_GET_INF: {  // Check a flag set in item_get_inf
      return SpoilerData_ItemGetInfCheck(itemLoc.LocationFlag);
    }
    case SPOILER_CHK_EVENT_CHK_INF: {  // Check a flag set in event_check_inf
      return -1;
      // TODO: Implement EventCheck.
      // return EventCheck(itemLoc.LocationFlag) != 0;
    }
    case SPOILER_CHK_INF_TABLE: {  // Check a flag set in event_check_inf
      return SpoilerData_InfTableCheck(itemLoc.LocationScene, itemLoc.LocationFlag);
    }
    case SPOILER_CHK_COW: {
      return SpoilerData_CowCheck(itemLoc);
    }
    case SPOILER_CHK_MINIGAME: {
      return SpoilerData_FishingCheck(itemLoc);
    }
    case SPOILER_CHK_SCRUB: {
      return SpoilerData_ScrubCheck(itemLoc);
    }

    case SPOILER_CHK_SHOP_ITEM: {
      return SpoilerData_ShopItemCheck(itemLoc);
    }
    case SPOILER_CHK_MAGIC_BEANS: {
      return SpoilerData_MagicBeansCheck(itemLoc);
    }
    default: {
      return 0;
    }
    }
    return 0;
    */
  }

  u8 SpoilerData_GetIsItemLocationRevealed(u16 itemIndex) {
    if (gSettingsContext.ingameSpoilers) {
      return 1;
    }

    SpoilerItemLocation* itemLoc = SpoilerData_ItemLoc(itemIndex);

    if (itemLoc->RevealType == REVEALTYPE_ALWAYS) {
      return 1;
    } else if (itemLoc->RevealType == REVEALTYPE_NORMAL) {
      return 0;
    }

    return SaveFile_GetIsSceneDiscovered(itemLoc->LocationScene);
  }

}  // namespace rnd