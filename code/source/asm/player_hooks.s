.arm
.text

.global hook_ApplyDamageMultiplier
hook_ApplyDamageMultiplier:
  push {r0, r2-r12, lr}
  bl Settings_ApplyDamageMultiplier @ Found in settings.cpp
  cpy r1,r0
  pop {r0, r2-r12, lr}
  subs r4,r1,#0x0
  bx lr

@ Runs right after player init sets up the face texture animations, with the player in r4.
.global hook_AfterPlayerFaceTexAnims
hook_AfterPlayerFaceTexAnims:
  push {r0-r12, lr}
  cpy r0,r4
  bl Tunic_AttachTexAnim
  cpy r0,r4
  bl ItemEffect_EquipPendingSword
  pop {r0-r12, lr}
  add r1,r4,#0x12000
  bx lr
