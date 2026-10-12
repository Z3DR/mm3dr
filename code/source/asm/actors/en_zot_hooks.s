.arm
.text

.global hook_EnZotJarGameVanillaTextId
hook_EnZotJarGameVanillaTextId:
    push {r0-r12, lr}
    ldr r0, [r0, #0x3f4]
    bl EnZot_JarGameVanillaTextId
    str r0, [sp]
    pop {r0-r12, lr}
    bx lr

hook_EnZotJarGameRewardText:
    push {r0-r12, lr}
    cpy r0, r1
    bl EnZot_JarGameRewardText
    str r0, [sp, #0x4]
    pop {r0-r12, lr}
    b 0x1CEE24

hook_EnZotJarGameReward:
    push {r0-r12, lr}
    bl EnZot_GiveJarGameReward
    cmp r0, #0x0
    pop {r0-r12, lr}
    bxne lr
    b 0x2C1634

hook_EnZotJarGameRewardDoneText:
    push {r0-r12, lr}
    ldr r0, =0x1270
    bl EnZot_JarGameRewardText
    str r0, [sp, #0x4]
    pop {r0-r12, lr}
    bx lr
