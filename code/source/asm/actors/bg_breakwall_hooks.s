.arm
.text

.global hook_DontSetNextCsIdInGreatBayUntilMikauSaved
hook_DontSetNextCsIdInGreatBayUntilMikauSaved:
    push {r0-r12,lr}
    bl Breakwall_CheckIfMikauSaved
    cmp r0,#0x0
    pop {r0-r12, lr}
    bx lr
