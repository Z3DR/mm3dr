.arm
.text

.global hook_CheckMasksOnMoon
hook_CheckMasksOnMoon:
    push {r5-r12,lr}
    bl En_Js_CurrentMasksInInventory
    cpy r4,r0
    pop {r5-r12, lr}
    mov r0,#0x0
    bx lr

.global hook_EnJsVictoryCheck
hook_EnJsVictoryCheck:
    push {r0, r2-r12, lr}
    bl En_Js_CheckVictoryRequirements
    mov r1,r0
    pop {r0, r2-r12, lr}
    bx lr

.global hook_EnJsSoftlockOnCustomText
hook_EnJsSoftlockOnCustomText:
    push {r0-r12, lr}
    cpy r0,r6
    bl En_Js_CheckTextValues
    pop {r0-r12,lr}
    cmp r0,#0x5
    bx lr

.global hook_EnJsAutoPressAOnEmptyText
hook_EnJsAutoPressAOnEmptyText:
    push {r0-r12,lr}
    cpy r0,r6
    bl En_Js_AutoPressEmptyText
    cmp r0,#0x1
    pop {r0-r12,lr}
    bne doNormalCmp
    cmp r0,#0x1
    bx lr
doNormalCmp:
    cmp r0,#0x0
    bx lr