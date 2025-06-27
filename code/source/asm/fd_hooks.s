.arm
.text

.global hook_UseFDAnywhere
hook_UseFDAnywhere:
  push {r0-r12, lr}
  bl UseFDAnywhere @ found in link.cpp
  cmp r0, #0x0
  pop {r0-r12,lr}
  beq doNotUseFdAnywhere
  bx lr
doNotUseFdAnywhere:
  b 0x1B1354