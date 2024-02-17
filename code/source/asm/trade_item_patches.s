.arm

.section .patch_RemoveTradeItemFromSlot
.global patch_RemoveTradeItemFromSlot
patch_RemoveTradeItemFromSlot:
  bleq hook_RemoveTradeItemFromExtSlot

.section .patch_DoNotRemoveTradeItems
.global patch_DoNotRemoveTradeItems
patch_DoNotRemoveTradeItems:
  b 0x1C9AD0

.section .patch_TradeItemDeleteMoonsTear
.global patch_TradeItemDeleteMoonsTear
patch_TradeItemDeleteMoonsTear:
  mov r2,#0x96
  bl hook_RemoveTradeItemFromInventory
  mov r2,#0x97

.section .patch_TradeItemDeleteLandTitleDeed
.global patch_TradeItemDeleteLandTitleDeed
patch_TradeItemDeleteLandTitleDeed:
  bleq hook_RemoveTradeItemFromInventory
  
.section .patch_TradeItemDeleteSwampTitleDeed
.global patch_TradeItemDeleteSwampTitleDeed
patch_TradeItemDeleteSwampTitleDeed:
  bleq hook_RemoveTradeItemFromInventory
  
.section .patch_TradeItemDeleteMountainTitleDeed
.global patch_TradeItemDeleteMountainTitleDeed
patch_TradeItemDeleteMountainTitleDeed:
  bleq hook_RemoveTradeItemFromInventory


.section .patch_SwapStoredTradeItems
.global patch_SwapStoredTradeItems
patch_SwapStoredTradeItems:
  bl hook_SwapStoredTradeItems