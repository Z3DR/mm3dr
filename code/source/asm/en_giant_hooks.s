.arm
.text

.global hook_DrawGiantAfterMoonCutscene
hook_DrawGiantAfterMoonCutscene:
    beq 0x39A66C @Default instruction if weekeventreg is false.
    push {r0-r12,lr}
    cpy r0,r4
    bl En_Giant_KillAfterCutscene
    cmp r0,#0x0
    pop {r0-r12,lr}
    beq 0x39A66C
    bne 0x39A544

.global hook_EnGiantDrawGiantIfMoonRequirementsMet
hook_EnGiantDrawGiantIfMoonRequirementsMet:
    push {r0-r12,lr}
    cpy r0,r4
    bl En_Giant_ShouldDrawGiant
    pop {r0-r12,lr}
    b 0x58AB30