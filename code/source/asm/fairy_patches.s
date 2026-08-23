.arm

.section .patch_AdjustFairyHeartText
.global patch_AdjustFairyHeartText
patch_AdjustFairyHeartText:
    bl hook_AdjustFairyHeartText

.section .patch_OverrideFairyGiveItem
.global OverrideFairyItemID_patch
OverrideFairyItemID_patch:
    b hook_OverrideFairyItem

.section .patch_OverrideGreatFairySpawn
.global OverrideGreatFairySpawn_patch
OverrideGreatFairySpawn_patch:
    nop
    @ldmia sp!,{r4 - r6, pc}
    @ldmia sp !, {r4 - r6, lr}
    @ bx lr

.section .patch_EnElfOrgChestCollect
.global patch_EnElfOrgChestCollect
patch_EnElfOrgChestCollect:
    bl hook_EnElfOrgChestCollect

@TODO: Break out Chest patches into separate chest file.
.section .patch_EnBoxCheckIfFairyObtained
.global patch_EnBoxCheckIfFairyObtained
patch_EnBoxCheckIfFairyObtained:
    bl hook_EnBoxCheckIfFairyObtained

.section .patch_StrayFairyCollectOverride
.global patch_StrayFairyCollectOverride
patch_StrayFairyCollectOverride:
    bl hook_StrayFairyCollectOverride

.section .patch_StrayFairyClockTownCollect
.global patch_StrayFairyClockTownCollect
patch_StrayFairyClockTownCollect:
    bl hook_StrayFairyClockTownCollect

.section .patch_ClockTownFairyFountainCheck
.global patch_ClockTownFairyFountainCheck
patch_ClockTownFairyFountainCheck:
    bl hook_ClockTownFairyCheck

.section .patch_StrayFairyCollectMessage
.global patch_StrayFairyCollectMessage
patch_StrayFairyCollectMessage:
    bl hook_StrayFairyCollectMessage