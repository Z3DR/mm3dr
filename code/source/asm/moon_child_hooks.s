.arm
.text

.global hook_CheckMasksOnMoon
hook_CheckMasksOnMoon:
    push {r5-r12,lr}
    bl CurrentMasksInInventory
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