.arm
.text

.global hook_EnRzHeartPieceFlag
hook_EnRzHeartPieceFlag:
    push {r0,r2,r3,r4,r12,lr}
    bl En_Rz_GetHeartPieceFlagByte
    mov r1,r0
    pop {r0,r2,r3,r4,r12,lr}
    bx lr
