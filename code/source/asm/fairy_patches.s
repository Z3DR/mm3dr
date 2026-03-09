.arm

.section .patch_EnElforgUpdateFairyBits
.global patch_EnElforgUpdateFairyBits
patch_EnElforgUpdateFairyBits:
    bl hook_EnElforgUpdateFairyBits

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

.section .patch_EnElfOrgTreasureSwitchSet
.global patch_EnElfOrgTreasureSwitchSet
patch_EnElfOrgTreasureSwitchSet:
    bl hook_EnElforgUpdateFairyBitsTwo


@TODO: Break out Chest patches into separate chest file.
.section .patch_EnBoxCheckIfFairyObtained
.global patch_EnBoxCheckIfFairyObtained
patch_EnBoxCheckIfFairyObtained:
    bl hook_EnBoxCheckIfFairyObtained