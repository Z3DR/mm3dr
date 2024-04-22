.arm

.section .patch_OverrideItem00Init
.global patch_OverrideItem00Init
patch_OverrideItem00Init:
  bl hook_OverrideItem00Init

.section .patch_OverrideItem00Draw
.global patch_OverrideItem00Draw
patch_OverrideItem00Draw:
  bl hook_OverrideItem00Draw