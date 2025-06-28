.arm
.text

.global hook_UseFDAnywhere
hook_UseFDAnywhere:
  push {r0-r12, lr}
  mov r0, r3
  bl UseFDAnywhere @ found in link.cpp
  cmp r0, #0x35
  pop {r0-r12,lr}
  b 0x1B1248

.global hook_CheckIfLinkIsFD
hook_CheckIfLinkIsFD:
  push {r0-r12, lr}
  bl CheckIfLinkIsFD @ found in link.cpp
  cmp r0, #0x1
  pop {r0-r12,lr}
  beq 0x2C921C
  cmp r0, #0x1
  cmpne r0, #0x2
  beq 0x2C921C
  bx lr

.global hook_FixFDObservatoryText
hook_FixFDObservatoryText:
  push {r0-r12, lr}
  bl CheckIfLinkIsFD @ found in link.cpp
  cmp r0, #0x1
  pop {r0-r12,lr}
  beq 0x30A440
  cmp r0, #0x2
  cmpne r0, #0x3
  beq 0x30A440
  bx lr

.global hook_FixFDObservatoryTextTwo
hook_FixFDObservatoryTextTwo:
  push {r0-r12, lr}
  bl CheckIfLinkIsFD @ found in link.cpp
  cmp r0, #0x1
  pop {r0-r12,lr}
  beq 0x30A4F4
  cmp r0, #0x2
  cmpne r0, #0x3
  beq 0x30A4F4
  bx lr

.global hook_FierceDeityArcheryFix
hook_FierceDeityArcheryFix:
  push {r0-r12, lr}
  bl FierceDeityArcheryFix @ found in link.cpp
  cmp r0, #0x3
  pop {r0-r12,lr}
  bx lr
