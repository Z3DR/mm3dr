.arm

.section .patch_EnTabGetSaleScript
.global patch_EnTabGetSaleScript
patch_EnTabGetSaleScript:
  bl hook_EnTabGetSaleScript
