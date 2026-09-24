#include "rnd/actors/en_tab.h"

namespace rnd {
  using MemCopyFn = void*(void* dest, const void* src, size_t size);

  static u8 sSaleScript[kEnTabSaleScriptSize];

  static bool En_Tab_IsSoldOut(GetItemID getItemId, s32 purchase) {
    ItemOverride_Key key = {.all = 0};
    key.scene = (u8)game::SceneId::MilkBar;
    key.type = ItemOverride_Type::OVR_BASE_ITEM;
    key.flag = (u8)getItemId;
    const ItemOverride override = ItemOverride_LookupByKey(key);
    if (override.key.all == 0)
      return false;
    return Shopsanity_IsSoldOut(ItemTable_GetItemRow(override.value.getItemId), purchase);
  }

  static void En_Tab_RefuseChoice(EnTabSaleScriptOffset choice) {
    const u16 skip = ENTAB_SALE_REFUSE - ENTAB_SALE_CHOICE_BASE;
    sSaleScript[choice] = skip >> 8;
    sSaleScript[choice + 1] = skip & 0xFF;
  }

  extern "C" {
  const u8* En_Tab_GetSaleScript(const u8* script) {
    const u32 address = (u32)script;
    if (address != kEnTabSaleScript && address != kEnTabSaleScriptDayThree)
      return script;

    // Always hand back the copy so the script pointer holds steady through a conversation.
    util::GetPointer<MemCopyFn>(0x300154)(sSaleScript, script, sizeof(sSaleScript));
    if (En_Tab_IsSoldOut(GetItemID::GI_BOTTLE_MILK_REFILL, SHOPSANITY_MILK_BAR_MILK))
      En_Tab_RefuseChoice(ENTAB_SALE_CHOICE_MILK);
    if (En_Tab_IsSoldOut(GetItemID::GI_BOTTLE_CHATEAU_ROMANI_REFILL, SHOPSANITY_MILK_BAR_CHATEAU))
      En_Tab_RefuseChoice(ENTAB_SALE_CHOICE_CHATEAU);
    return sSaleScript;
  }
  }
}  // namespace rnd
