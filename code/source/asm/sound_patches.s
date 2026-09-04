.arm

.section .patch_SoundEffectFilterOne
.global patch_SoundEffectFilterOne
patch_SoundEffectFilterOne:
    bl hook_SoundEffectFilterOne

.section .patch_MuteSoundEffectTwo
.global patch_MuteSoundEffectTwo
patch_MuteSoundEffectTwo:
    bl hook_MuteSoundEffectTwo

.section .patch_MuteStreamPlay
.global patch_MuteStreamPlay
patch_MuteStreamPlay:
    bl hook_MuteStreamPlay

.section .patch_SoundEffectFilterThree
.global patch_SoundEffectFilterThree
patch_SoundEffectFilterThree:
    bl hook_SoundEffectFilterThree

.section .patch_MuteOcarinaNotes
.global patch_MuteOcarinaNotes
patch_MuteOcarinaNotes:
    bl hook_MuteOcarinaNotes
