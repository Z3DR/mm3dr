.arm

.section .patch_CheckMasksOnMoon
.global patch_CheckMasksOnMoon
patch_CheckMasksOnMoon:
    bl hook_CheckMasksOnMoon

.section .patch_EnJsVictoryCheck
.global patch_EnJsVictoryCheck
patch_EnJsVictoryCheck:
    bl hook_EnJsVictoryCheck

.section .patch_EnJsSoftlockOnCustomText
.global patch_EnJsSoftlockOnCustomText
patch_EnJsSoftlockOnCustomText:
    bl hook_EnJsSoftlockOnCustomText

.section .patch_EnJsAutoPressAOnEmptyText
.global patch_EnJsAutoPressAOnEmptyText
patch_EnJsAutoPressAOnEmptyText:
    bl hook_EnJsAutoPressAOnEmptyText