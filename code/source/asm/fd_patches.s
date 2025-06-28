.arm

.section .patch_UseFDAnywhere
.global patch_UseFDAnywhere
patch_UseFDAnywhere:
  bleq hook_UseFDAnywhere

.section .patch_FierceDeityArcheryFix
.global patch_FierceDeityArcheryFix
patch_FierceDeityArcheryFix:
  bl hook_FierceDeityArcheryFix

.section .patch_FDClimbingAnimationFixOne
.global patch_FDClimbingAnimationFixOne
patch_FDClimbingAnimationFixOne:
  .word 0x338
  .word 0x1F9
  .word 0x1FA
  .word 0x1FB
  .word 0x1FC


.section .patch_FDClimbingAnimationFixTwo
.global patch_FDClimbingAnimationFixTwo
patch_FDClimbingAnimationFixTwo:
  .word 0x1F5
  .word 0x1F5
  .word 0x1F8

.section .patch_FDDoorCrashOne
.global patch_FDDoorCrashOne
patch_FDDoorCrashOne:
  .word 0x2BD

.section .patch_FDDoorCrashTwo
.global patch_FDDoorCrashTwo
patch_FDDoorCrashTwo:
  .word 0x2BD

.section .patch_FDDoorCrashThree
.global patch_FDDoorCrashThree
patch_FDDoorCrashThree:
  .word 0x2BE

.section .patch_FDDoorCrashFour
.global patch_FDDoorCrashFour
patch_FDDoorCrashFour:
  .word 0x2BE