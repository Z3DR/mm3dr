.arm

.section .patch_UseFDAnywhere
.global patch_UseFDAnywhere
patch_UseFDAnywhere:
  bl hook_UseFDAnywhere

.section .patch_FDClimbingAnimationFixOne
.global patch_FDClimbingAnimationFixOne
patch_FDClimbingAnimationFixOne:
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