.arm

.section .patch_OverrideCutsceneNextEntrance
.global patch_OverrideCutsceneNextEntrance
patch_OverrideCutsceneNextEntrance:
    bl hook_OverrideCutsceneNextEntrance

.section .patch_AdjustMoonEntryRequirements
.global patch_AdjustMoonEntryRequirements
patch_AdjustMoonEntryRequirements:
    b hook_AdjustMoonEntryRequirements
