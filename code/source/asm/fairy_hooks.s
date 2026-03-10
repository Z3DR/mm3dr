.arm
.text

.global rActiveItemRow
.rActiveItemRow_addr:
    .word rActiveItemRow

.global hook_EnElforgUpdateFairyBits
hook_EnElforgUpdateFairyBits:
    push {r0-r12, lr}
    cpy r0, r4 @actor
    cpy r1,r8 @gctx
    bl En_Elforg_UpdateExtFairyBits
    pop {r0-r12, lr}
    ldrsh r0,[r4,#0x1C]
    bx lr

.global hook_OverrideFairyItem
hook_OverrideFairyItem:
    push {r0-r12, lr}
    cpy r0,r5
    cpy r1,r4
    mov r2,#0x0
    bl ItemOverride_GetFairyRewardItem
    ldr r5,.rActiveItemRow_addr
    ldr r5,[r5]
    cmp r5,#0x0
    pop {r0-r12, lr}
    beq noOverrideFairyItemID
    push {r0-r12, lr}
    cpy r0,r5
    cpy r1,r4
    bl ItemOverride_GetItemTextAndItemID
    pop {r0-r12, lr}
    cpy r0,r4
    b 0x3becf4
noOverrideFairyItemID:
    cpy r0,r4
    b 0x3becf4

.global hook_EnElforgUpdateFairyBitsTwo
hook_EnElforgUpdateFairyBitsTwo:
    push {r0-r12, lr}
    cpy r0, r4 @actor
    cpy r1,r8 @gctx
    bl En_Elforg_UpdateExtFairyBits
    pop {r0-r12, lr}
    cpy r0,r8
    bx lr

.global hook_EnBoxCheckIfFairyObtained
hook_EnBoxCheckIfFairyObtained:
    orr r1,r3,r1, lsl #0x9
    push {r0-r12, lr}
    cpy r0,r1 @param
    cpy r1,r5 @gctx
    bl En_Elforg_Chest_IsFairyObtained
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq 0x575908
    bx lr