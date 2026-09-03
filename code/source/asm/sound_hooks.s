.arm
.text

@ The two vanilla sound-effect entry points. Both are patched at their second instruction so the
@ prologue has already pushed lr, letting us return straight to PlayEffect's caller when muted.

.global hook_MuteSoundEffectOne
hook_MuteSoundEffectOne:
    push {r0-r3, r12, lr}
    bl SoundEffectsMuted
    cmp r0, #0x0
    pop {r0-r3, r12, lr}
    bne muteSoundEffectOne
    cpy r4, r0
    bx lr
muteSoundEffectOne:
    mov r0, #0x0
    ldmia sp!, {r4, pc}

.global hook_MuteSoundEffectTwo
hook_MuteSoundEffectTwo:
    push {r0-r3, r12, lr}
    bl SoundEffectsMuted
    cmp r0, #0x0
    pop {r0-r3, r12, lr}
    bne muteSoundEffectTwo
    cpy r4, r0
    bx lr
muteSoundEffectTwo:
    mov r0, #0x0
    ldmia sp!, {r4, r5, pc}
