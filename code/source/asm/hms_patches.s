.arm

.section .patch_OverrideBombersNotebook
.global patch_OverrideBombersNotebook
patch_OverrideBombersNotebook:
    b hook_OverrideHMSBombers

.section .patch_HMSGiveItem
.global patch_HMSGiveItem
patch_HMSGiveItem:
    b hook_OverrideHMSDekuMask