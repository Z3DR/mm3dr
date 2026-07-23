.arm

.section .patch_EnGirlA_Randomize
.global patch_EnGirlA_Randomize
patch_EnGirlA_Randomize:
    bl hook_EnGirlA_Randomize

.section .patch_EnGirlA_AfterModelLoad
.global patch_EnGirlA_AfterModelLoad
patch_EnGirlA_AfterModelLoad:
    b hook_EnGirlA_AfterModelLoad