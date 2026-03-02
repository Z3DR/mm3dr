.arm

.section .patch_OverrideCutsceneNextEntrance
.global patch_OverrideCutsceneNextEntrance
patch_OverrideCutsceneNextEntrance:
    bl hook_OverrideCutsceneNextEntrance

@ Skip past all the fairy and 
@ door resetting if we are the temples
@ as we don't want to softlock users
@ if they have already used their keys.
.section .patch_DoNotResetTempleFlags
.global patch_DoNotResetTempleFlags
patch_DoNotResetTempleFlags:
    bl hook_DoNotResetTempleFlags

.section .patch_AdjustMoonEntryRequirements
.global patch_AdjustMoonEntryRequirements
patch_AdjustMoonEntryRequirements:
    b hook_AdjustMoonEntryRequirements
