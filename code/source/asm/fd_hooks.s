.arm
.text

.global hook_UseFDAnywhere
hook_UseFDAnywhere:
  push {r0-r12, lr}
  bl UseFDAnywhere @ found in link.cpp
  cmp r0, #0x35
  pop {r0-r12,lr}
  beq 0x1b1354
  cmp r3, #0x00
  bx lr

.global hook_FierceDeityArcheryFix
hook_FierceDeityArcheryFix:
  push {r0-r12, lr}
  bl FierceDeityArcheryFix @ found in link.cpp
  cmp r0, #0x3
  pop {r0-r12,lr}
  bx lr
