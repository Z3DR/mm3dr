.arm

.section .patch_CheckMasksOnMoon
.global patch_CheckMasksOnMoon
patch_CheckMasksOnMoon:
    bl hook_CheckMasksOnMoon

.section .patch_EnJsVictoryCheck
.global patch_EnJsVictoryCheck
patch_EnJsVictoryCheck:
    bl hook_EnJsVictoryCheck