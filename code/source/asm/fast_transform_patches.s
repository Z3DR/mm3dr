.arm

.section .patch_FastTransformPatchOne
.global patch_FastTransformPatchOne
patch_FastTransformPatchOne:
    bl hook_FastTransformPatchOne

.section .patch_FastTransformPatchTwo
.global patch_FastTransformPatchTwo
patch_FastTransformPatchTwo:
    bl hook_FastTransformPatchTwo

.section .patch_FastTransformPatchThree
.global patch_FastTransformPatchThree
patch_FastTransformPatchThree:
    bl hook_FastTransformPatchThree

.section .patch_RemoveFastTransformSound
.global patch_RemoveFastTransformSound
patch_RemoveFastTransformSound:
  bl hook_RemoveFastTransformSound

.section .patch_FastTransformPatchFour
.global patch_FastTransformPatchFour
patch_FastTransformPatchFour:
    bl hook_FastTransformPatchFour

.section .patch_FastTransformPatchFive
.global patch_FastTransformPatchFive
patch_FastTransformPatchFive:
    bl hook_FastTransformPatchFive
