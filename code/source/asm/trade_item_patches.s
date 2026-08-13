.arm

.section .patch_DoNotRemoveTradeItems
.global patch_DoNotRemoveTradeItems
patch_DoNotRemoveTradeItems:
  b 0x1C9AD0

.section .patch_DoNotDeleteOceanTitleDeed
.global patch_DoNotDeleteOceanTitleDeed
patch_DoNotDeleteOceanTitleDeed:
  nop

.section .patch_EnBjtDoNotRemoveTradeItem
.global patch_EnBjtDoNotRemoveTradeItem
patch_EnBjtDoNotRemoveTradeItem:
  nop

.section .patch_DoNotDeleteKafeiGearItem
.global patch_DoNotDeleteKafeiGearItem
patch_DoNotDeleteKafeiGearItem:
  nop

.section .patch_SwapStoredTradeItems
.global patch_SwapStoredTradeItems
patch_SwapStoredTradeItems:
  bl hook_SwapStoredTradeItems