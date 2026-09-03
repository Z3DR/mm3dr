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


@ StreamMgr::play (0x239228). Every stream funnels here -- 0x21E31C tail-calls into it -- so
@ gating it stops background music from starting at all. Patched at the second instruction, so
@ the prologue's push {r0-r11, lr} has run and the mute path can unwind it.
.global hook_MuteStreamPlay
hook_MuteStreamPlay:
    push {r0-r3, r12, lr}
    cpy r0, r1
    cpy r1, r2
    bl StreamPlayBlocked
    cmp r0, #0x0
    pop {r0-r3, r12, lr}
    bne muteStreamPlay
    cpy r4, r0
    bx lr
muteStreamPlay:
    add sp, sp, #0x30
    mov r0, #0x0
    ldmia sp!, {pc}

.global hook_MuteSoundEffectThree
hook_MuteSoundEffectThree:
    push {r0-r3, r12, lr}
    vpush {d0}
    bl SoundEffectsMuted
    cmp r0, #0x0
    vpop {d0}
    pop {r0-r3, r12, lr}
    bne muteSoundEffectThree
    mov r6, #0x0
    bx lr
muteSoundEffectThree:
    add sp, sp, #0x20
    ldmia sp!, {pc}

.global hook_MuteOcarinaNotes
hook_MuteOcarinaNotes:
    push {r0-r3, r12, lr}
    bl SoundEffectsMuted
    cmp r0, #0x0
    pop {r0-r3, r12, lr}
    bne muteOcarinaNotes
    cpy r4, r0
    bx lr
muteOcarinaNotes:
    add sp, sp, #0x14
    ldmia sp!, {pc}
