.arm
.text

.global hook_MusicOverridePlay
hook_MusicOverridePlay:
  push {r0, r1, r3, r4, r12, lr}
  cpy  r0, r6
  bl   Music_OverridePlay
  cpy  r2, r0
  pop  {r0, r1, r3, r4, r12, lr}
  bx   lr
