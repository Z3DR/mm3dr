.arm
.text  

.global hook_SwapStoredTradeItems
hook_SwapStoredTradeItems:
  push {r0-r12, lr}
  bl GearScreen_GetStoredTradeItem
  pop {r0-r12, lr}
  cpy r4, r0
  bx lr

.global hook_DoNotDeleteKafeiGearItem
hook_DoNotDeleteKafeiGearItem:
  push {r0-r12,lr}
  cmp r0, #0x31
  bleq 0x4AD1B4
  pop {r0-r12,lr}
  bx lr