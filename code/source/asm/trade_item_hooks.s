.arm
.text  

.global hook_SwapStoredTradeItems
hook_SwapStoredTradeItems:
  push {r0-r12, lr}
  bl GearScreen_GetStoredTradeItem
  pop {r0-r12, lr}
  cpy r4, r0
  bx lr