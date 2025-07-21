.arm

.section .patch_OcarinaDive
.global patch_OcarinaDive
patch_OcarinaDive:
    bl hook_CheckOcarinaDive

.section .patch_EnsureTerminaIsNotFirstCycle
.global patch_EnsureTerminaIsNotFirstCycle
patch_EnsureTerminaIsNotFirstCycle:
    cmp r0,r0

.section .patch_FixRemovingOcarinaFromInventory
.global patch_FixRemovingOcarinaFromInventory
patch_FixRemovingOcarinaFromInventory:
    b hook_FixRemovingOcarinaFromInventory

.section .patch_DmSktOcarinaAnimationPatchOne
.global patch_DmSktOcarinaAnimationPatchOne
patch_DmSktOcarinaAnimationPatchOne:
  bl hook_DmSktOcarinaAnimationPatchOne

.section .patch_DmSktOcarinaAnimationPatchTwo
.global patch_DmSktOcarinaAnimationPatchTwo
patch_DmSktOcarinaAnimationPatchTwo:
  bl hook_DmSktOcarinaAnimationPatchTwo

.section .patch_DmSktOcarinaAnimationPatchThree
.global patch_DmSktOcarinaAnimationPatchThree
patch_DmSktOcarinaAnimationPatchThree:
  bl hook_DmSktOcarinaAnimationPatchThree

.section .patch_DmSktOcarinaAnimationPatchFour
.global patch_DmSktOcarinaAnimationPatchFour
patch_DmSktOcarinaAnimationPatchFour:
  bl hook_DmSktOcarinaAnimationPatchFour

.section .patch_DmStkInitOcarinaPatch
.global patch_DmStkInitOcarinaPatch
patch_DmStkInitOcarinaPatch:
    bl hook_DmStkInitOcarinaPatch

.section .patch_UpdateOcarinaVisibility
.global patch_UpdateOcarinaVisibility
patch_UpdateOcarinaVisibility:
    bl hook_UpdateOcarinaVisibility

.section .patch_DmSktOcarinaAnimationPatchFive
.global patch_DmSktOcarinaAnimationPatchFive
patch_DmSktOcarinaAnimationPatchFive:
  bl hook_DmSktOcarinaAnimationPatchFive

.section .patch_FixOcarinaGearScreenDefault
.global patch_FixOcarinaGearScreenDefault
patch_FixOcarinaGearScreenDefault:
    bl hook_FixOcarinaGearScreenDefault

.section .patch_FixOcarinaGearScreenDeku
.global patch_FixOcarinaGearScreenDeku
patch_FixOcarinaGearScreenDeku:
    bl hook_FixOcarinaGearScreenDeku

.section .patch_FixOcarinaGearScreenGoron
.global patch_FixOcarinaGearScreenGoron
patch_FixOcarinaGearScreenGoron:
    bl hook_FixOcarinaGearScreenGoron

.section .patch_FixOcarinaGearScreenZora
.global patch_FixOcarinaGearScreenZora
patch_FixOcarinaGearScreenZora:
    bl hook_FixOcarinaGearScreenZora

.section .patch_FixOcarinaGearScreenZoraTwo
.global patch_FixOcarinaGearScreenZoraTwo
patch_FixOcarinaGearScreenZoraTwo:
    bl hook_FixOcarinaGearScreenZoraTwo

@ Adjusts ocarina songs to be in-line with restoration
.section .patch_HandleOcarinaHooks
.global patch_HandleOcarinaHooks
patch_HandleOcarinaHooks:
    b hook_HandleOcarina