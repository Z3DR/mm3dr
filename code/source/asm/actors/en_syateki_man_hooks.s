.arm
.text

.global hook_EnSyatekiManTownPerfectBonus
hook_EnSyatekiManTownPerfectBonus:
    push {r0-r12,lr}
    cpy r0,r8
    bl EnSyatekiMan_TownPerfectBonus
    pop {r0-r12,lr}
    ldrsh r0,[r7,#0xcc]
    bx lr

.global hook_EnSyatekiManSwampPerfectBonus
hook_EnSyatekiManSwampPerfectBonus:
    push {r0-r12,lr}
    cpy r0,r7
    bl EnSyatekiMan_SwampPerfectBonus
    pop {r0-r12,lr}
    ldrb r0,[r4,#0x28b]
    bx lr
