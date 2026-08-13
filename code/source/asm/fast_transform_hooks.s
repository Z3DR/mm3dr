.arm
.text

.global hook_FastTransformPatchOne
hook_FastTransformPatchOne:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq defaultAnimationOne
  cmp r1,#0x0
  strh r1,[r0,#0x92]
  blt 0x1EB58C
  mov r1,#0x0
  b 0x1EB3C8
defaultAnimationOne:
  cmp r1,#0xFF
  bx lr

.global hook_FastTransformPatchTwo
hook_FastTransformPatchTwo:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq defaultAnimationTwo
  add r2,r1,#0x6
  bx lr
defaultAnimationTwo:
  add r2,r1,#0x1
  bx lr

.global hook_FastTransformPatchThree
hook_FastTransformPatchThree:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq defaultAnimationThree
  tst r1,r2
  bne 0x1EB58C
  mov r2,#0xFD
  mov r1,#0xDC
  strh r2,[r0,#0x54]
  strh r1,[r0,#0x56]
  strh r1,[r0,#0x58]
  strh r1,[r0,#0x5A]
  cmp r10,#0x1
  b 0x1EB530
defaultAnimationThree:
  @ recheck the cmp value.
  tst r1,r2
  beq 0x1EB58C
  bx lr

.global hook_RemoveFastTransformSound
hook_RemoveFastTransformSound:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq playTransformSound
  b 0x1EB58C
playTransformSound:
  push {lr} 
  bl 0x1D2374
  pop {lr}
  bx lr

.global hook_FastTransformPatchFour
hook_FastTransformPatchFour:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq defaultAnimationFour
  add r1,r0,#0x3
  bx lr
defaultAnimationFour:
  add r1,r0,#0x1
  bx lr

.global hook_FastTransformPatchFive
hook_FastTransformPatchFive:
  push {r0-r12, lr}
  bl SettingsNoAnimationCheck
  cmp r0,#0x0
  pop {r0-r12, lr}
  beq defaultAnimationFive
  tst r0,r1
  bne 0x1EC53C
  bx lr
defaultAnimationFive:
  tst r0,r1
  beq 0x1EC53C
  bx lr