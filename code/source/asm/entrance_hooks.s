.arm
.text

.global hook_OverrideCutsceneNextEntrance
hook_OverrideCutsceneNextEntrance:
    push {r0-r12, lr}
    bl SceneEntranceOverride
    cmp r0,#0x1
    pop {r0-r12, lr}
    bne doNotOverrideCutscene
    bx lr
doNotOverrideCutscene:
    bl 0x22A7F8
    b 0x1B1838

.global hook_AdjustMoonEntryRequirements
hook_AdjustMoonEntryRequirements:
    ldr r2,[r3,#0x0]
    push {r0-r12, lr}
    bl EnFall_CheckMoonRequirements
    cmp r0,#0x1
    pop {r0-r12, lr}
    bne 0x576DFC @Did not meet requirements for moon.
    beq 0x576D6C @Met moon requirements.