.arm
.text

.global hook_EnPmOverrideHatModel
hook_EnPmOverrideHatModel:
  mov r1,#0x0
  push {r0-r12, lr}
  cpy r1,r4
  bl En_Pm_OverrideModelHatDraw
  cmp r0,#0x0
  pop {r0-r12,lr}
  addne lr,lr,#0x4 @ skip drawing vanilla model
  bx lr