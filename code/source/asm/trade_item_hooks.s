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
  cmp   r0, #0x2D
  cmpne r0, #0x2E
  cmpne r0, #0x2F
  cmpne r0, #0x30
  beq   skipItemDelete
  cmp   r0, #0x26 instruction
  bx    lr
skipItemDelete:
  b     0x3EC748