.arm
.text

.global hook_EnKujiyaSetLotteryBitRevealed
hook_EnKujiyaSetLotteryBitRevealed:
    push {r0-r12,lr}
    bl EnKujiya_SetLotteryRevealed
    pop {r0-r12, lr}
    ldr r1,[r5,#0x14]
    bx lr
