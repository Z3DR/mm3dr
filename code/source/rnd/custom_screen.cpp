#include "rnd/custom_screen.h"

namespace rnd {

  namespace gearscreen {

    extern "C" void GearScreen_GetStoredTradeItem(game::ui::screens::GearScreen* gearScreen) {
      if (gearScreen->cursorIndex == 0) {
        auto* gctx = rnd::GetContext().gctx;
        game::ItemId firstFoundItem = game::ItemId::None;
        const u32 newButtons = gctx->pad_state.input.new_buttons.flags;
        
        // Sixth Slot [5] of Inventory array is the Title deed slot. 17 [16] is letter to kafei?
        if (newButtons == (u32)game::pad::Button::R) {

          // Check what is in current slot.
          // After receiving value, check array from 0-4 to see what items we have obtained.
          // We always know the order of the array, MoonsTear -> Land Title -> Swamp -> Mountain -> Ocean
          for (int i = 0; i < 5; i++) {
            if (firstFoundItem == game::ItemId::None) {
              firstFoundItem = gExtSaveData.collectedTradeItems[i];
            }
            if (GearScreen_LoopTradeItemsForward(gearScreen, i, firstFoundItem)) break;
          }
        } else if (newButtons == (u32)game::pad::Button::L) {
          for (int i = 4; i >= 0; i--) {
            if (firstFoundItem == game::ItemId::None) {
              firstFoundItem = gExtSaveData.collectedTradeItems[i];
            }
            if (GearScreen_LoopTradeItemsBackward(gearScreen, i, firstFoundItem)) break;
          }
        }
      } else if (gearScreen->cursorIndex == 2) {
        #if defined ENABLE_DEBUG || defined DEBUG_PRINT
          rnd::util::Print("%s: Index 2!\n", __func__);	
        #endif
      }
      return;
    }

    bool GearScreen_LoopTradeItemsForward(game::ui::screens::GearScreen* gearScreen,int i, game::ItemId firstFoundItem) {
      auto& items = game::GetCommonData().save.inventory.items;
      int textId;
      int modelId;
      
      // Base case, if we have a none item, give the first available item.
      if (items[5] == game::ItemId::None && gExtSaveData.collectedTradeItems[i] != game::ItemId::None) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      }
      // Second case - we're not the last element and we contain an item, then swap that item.
      if (i != 4 && gExtSaveData.collectedTradeItems[i] != game::ItemId::None &&
          (int)items[5] < (int)gExtSaveData.collectedTradeItems[i]) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      } else if (i == 4 && gExtSaveData.collectedTradeItems[i] != game::ItemId::None &&
        gExtSaveData.collectedTradeItems[i] != items[5]) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      } else if (i == 4 && firstFoundItem != game::ItemId::None) {
        textId = GearScreen_GetTextIdFromItemId(firstFoundItem);
        modelId = GearScreen_GetModelIdFromItemId(firstFoundItem);
        GearScreen_DrawAndShowItem(firstFoundItem , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      }
      return false;
    }

    bool GearScreen_LoopTradeItemsBackward(game::ui::screens::GearScreen* gearScreen,int i, game::ItemId firstFoundItem) {
      auto& items = game::GetCommonData().save.inventory.items;
      int textId;
      int modelId;
      
      // Base case, if we have a none item, give the first available item.
      if (items[5] == game::ItemId::None && gExtSaveData.collectedTradeItems[i] != game::ItemId::None) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      }
      // Second case - we're not the last element and we contain an item, then swap that item.
      if (i != 0 && gExtSaveData.collectedTradeItems[i] != game::ItemId::None &&
          (int)items[5] > (int)gExtSaveData.collectedTradeItems[i]) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      } else if (i == 0 && gExtSaveData.collectedTradeItems[i] != game::ItemId::None &&
        gExtSaveData.collectedTradeItems[i] != items[5]) {
        textId = GearScreen_GetTextIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        modelId = GearScreen_GetModelIdFromItemId(gExtSaveData.collectedTradeItems[i]);
        GearScreen_DrawAndShowItem(gExtSaveData.collectedTradeItems[i] , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      } else if (i == 0 && firstFoundItem != game::ItemId::None) {
        textId = GearScreen_GetTextIdFromItemId(firstFoundItem);
        modelId = GearScreen_GetModelIdFromItemId(firstFoundItem);
        GearScreen_DrawAndShowItem(firstFoundItem , gearScreen->icon_event00_l->return_empty_idle_anim,
                                textId, modelId);
        return true;
      }
      return false;
    }

    int GearScreen_GetTextIdFromItemId(game::ItemId itemId) {
      // Since we're limited on what items we got, just hard code these.
      if (itemId == game::ItemId::MoonTear) return 0x1728;
      else if (itemId == game::ItemId::LandTitleDeed) return 0x1729; 
      else if (itemId == game::ItemId::SwampTitleDeed) return 0x172A; 
      else if (itemId == game::ItemId::MountainTitleDeed) return 0x172B; 
      else if (itemId == game::ItemId::OceanTitleDeed) return 0x172C; 
      else if (itemId == game::ItemId::LetterToKafei) return 0x172F; 
      else if (itemId == game::ItemId::PendantOfMemories) return 0x1730;
      return 0x00;
    }

    int GearScreen_GetModelIdFromItemId(game::ItemId itemId) {
      // Since we're limited on what items we got, just hard code these.
      if (itemId == game::ItemId::MoonTear) return 0x59;
      else if (itemId == game::ItemId::LandTitleDeed) return 0x5A; 
      else if (itemId == game::ItemId::SwampTitleDeed) return 0x40; 
      else if (itemId == game::ItemId::MountainTitleDeed) return 0x41; 
      else if (itemId == game::ItemId::OceanTitleDeed) return 0x43; 
      else if (itemId == game::ItemId::LetterToKafei) return 0x40; 
      else if (itemId == game::ItemId::PendantOfMemories) return 0x00;
      return 0x00;
    }


    void GearScreen_DrawAndShowItem(game::ItemId itemId, game::ui::Anim* emptyIdleAnim, u16 textId, int modelId) {
      game::MessageMgr mgr = game::MessageMgr::Instance();
      game::ui::ScreenContext& sctx = game::ui::GetScreenContext();
      
      game::GiveItem(itemId);
          
     int iconId = util::GetPointer<int(game::ItemId)>(0x601E18)(itemId);
      
      util::GetPointer<void(game::ui::Anim*, int)>(0x601a1c)(emptyIdleAnim, iconId);
      util::GetPointer<void(game::ui::MessageWindow*, u16, int)>(0x1ccfe4)(mgr.message_window, textId, 0);

      int modelType = util::GetPointer<int(game::ui::Anim*)>(0x6120e4)(emptyIdleAnim);
      modelType = modelType + 0x1700;
      util::GetPointer<void(game::ui::Context*, int)>(0x5a1aac)(sctx.ctx, modelId);
      game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_CURSOR);
      return;
    }
  }  // namespace gearscreen

}  // namespace rnd