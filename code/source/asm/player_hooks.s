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