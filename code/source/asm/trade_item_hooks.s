.arm
.text

.global hook_RemoveTradeItemFromInventory
hook_RemoveTradeItemFromInventory:
  push {r0-r12,lr}
  cpy r0,r2
  mov r1,#0x5
  sub r0,r0,#0x1
  bl SaveFile_RemoveStoredDeed
  pop {r0-r12,lr}
  bx lr

.global hook_SwapStoredTradeItems
hook_SwapStoredTradeItems:
  push {r0-r12, lr}
  bl GearScreen_GetStoredTradeItem
  pop {r0-r12, lr}
  cpy r4, r0
  bx lr