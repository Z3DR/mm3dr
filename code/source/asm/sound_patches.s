.arm

.section .patch_MuteSoundEffectOne
.global patch_MuteSoundEffectOne
patch_MuteSoundEffectOne:
    bl hook_MuteSoundEffectOne

.section .patch_MuteSoundEffectTwo
.global patch_MuteSoundEffectTwo
patch_MuteSoundEffectTwo:
    bl hook_MuteSoundEffectTwo

.section .patch_MuteStreamPlay
.global patch_MuteStreamPlay
patch_MuteStreamPlay:
    bl hook_MuteStreamPlay

.section .patch_MuteSoundEffectThree
.global patch_MuteSoundEffectThree
patch_MuteSoundEffectThree:
    bl hook_MuteSoundEffectThree

.section .patch_MuteOcarinaNotes
.global patch_MuteOcarinaNotes
patch_MuteOcarinaNotes:
    bl hook_MuteOcarinaNotes
