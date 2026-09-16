.arm
.text

@ Replaces `cmp r0,#3` (bomb bag upgrade is the Biggest Bomb Bag) ahead of the scrub's sold-out branch.
.global hook_BusinessScrubBombBagPurchased
hook_BusinessScrubBombBagPurchased:
  push {r0-r12,lr}
  bl En_Akindonuts_CheckBombBagExtData
  cmp r0,#0x1
  pop {r0-r12,lr}
  bx lr
