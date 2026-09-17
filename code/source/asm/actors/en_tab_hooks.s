.arm
.text

.global hook_EnTabGetSaleScript
hook_EnTabGetSaleScript:
  push {r0-r1,r3,lr}
  cpy r0,r2
  bl En_Tab_GetSaleScript
  cpy r2,r0
  pop {r0-r1,r3,lr}
  b 0x2B9A80
