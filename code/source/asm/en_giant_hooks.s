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

.global hook_GiantChamberCutsceneOrder
hook_GiantChamberCutsceneOrder:
    push {r0, r2-r12}
    cpy r0, r12
    bl En_Giant_ChamberCutsceneOrder
    cpy r1, r0
    pop {r0, r2-r12}
    mov lr, #0x0
    cmp r12, #0x0
    b 0x345D7C
