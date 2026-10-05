.arm
.text

@ Kotake's shelf offer: replaces the text ID load (`ldrh r0,[r5,#0x4a]`) so we can flag the free potion
.global hook_KotakeMushroomSale
hook_KotakeMushroomSale:
  ldrh r0,[r5,#0x4a]
  push {r0-r12, lr}
  bl En_Trt_KotakeMushroomSale
  pop {r0-r12,lr}
  bx lr
