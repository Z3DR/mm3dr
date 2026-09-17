.arm
.text

@ Replaces the seller's empty bottle check on both of his milk choices, so a sold out check takes the
@ same path as having no bottle to fill.
.global hook_EnInGormanMilkCanBuy
hook_EnInGormanMilkCanBuy:
  push {r1-r12,lr}
  bl En_In_CanBuyMilk
  pop {r1-r12,lr}
  bx lr
