.arm

.section .patch_MuteSoundEffectOne
.global patch_MuteSoundEffectOne
patch_MuteSoundEffectOne:
    bl hook_MuteSoundEffectOne

.section .patch_MuteSoundEffectTwo
.global patch_MuteSoundEffectTwo
patch_MuteSoundEffectTwo:
    bl hook_MuteSoundEffectTwo
