.arm
.text

@ The two vanilla sound-effect entry points. Both are patched at their second instruction so the
@ prologue has already pushed lr, letting us return straight to PlayEffect's caller when muted.

.global hook_SoundEffectFilterOne
hook_SoundEffectFilterOne:
    push {r0, r2, r3, r4, r12, lr}
    vpush {d0}
    cpy r0, r1
    bl Sfx_Filter
    cmp r0, #0x0
    cpy r1, r0
    vpop {d0}
    pop {r0, r2, r3, r4, r12, lr}
    beq soundEffectOneSuppressed
    cpy r5, r0
    bx lr
soundEffectOneSuppressed:
    mov r0, #0x0
    add sp, sp, #0x20
    ldmia sp!, {pc}

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

@ emitSoundEx (0x1FDE30). EffectId is argument 2, in r1, so it moves into r0 for the call and is
@ written back after -- r1 is kept out of the pop list so the filtered value survives.
.global hook_SoundEffectFilterThree
hook_SoundEffectFilterThree:
    push {r0, r2, r3, r4, r12, lr}
    vpush {d0}
    cpy r0, r1
    bl Sfx_Filter
    cmp r0, #0x0
    cpy r1, r0
    vpop {d0}
    pop {r0, r2, r3, r4, r12, lr}
    beq soundEffectThreeSuppressed
    mov r6, #0x0
    bx lr
soundEffectThreeSuppressed:
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
