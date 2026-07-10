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

.global hook_EnElfOrgChestCollect
hook_EnElfOrgChestCollect:
    push {r0-r12, lr}
    cpy r0,r4
    bl ItemOverride_OverrideStrayFairy
    cmp r0, #0x0
    pop {r0-r12, lr}
    bne enElfOrgChestCollectSkip
    ldrh r0,[r9, #0x54]
    b 0x3CDE10
enElfOrgChestCollectSkip:
    ldr r1,=0x670
    ldrh r0,[r4, r1]
    orr r0,r0, #0x10
    strh r0,[r4, r1]
    b 0x3CDE3C

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

.global hook_StrayFairyCollectOverride
hook_StrayFairyCollectOverride:
    push {r0-r12, lr}
    cpy r0,r4
    bl ItemOverride_OverrideStrayFairy
    cmp r0, #0x0
    pop {r0-r12, lr}
    bne strayFairyCounterSkip
    ldr r0, =0x25A820
    ldr r0, [r0]
    b 0x25A6EC
strayFairyCounterSkip:
    ldr r1,=0x670
    ldrh r0,[r4, r1]
    orr r0,r0, #0x10
    strh r0,[r4, r1]
    b 0x25A71C

.global hook_StrayFairyClockTownCollect
hook_StrayFairyClockTownCollect:
    push {r0-r12, lr}
    cpy r0,r4
    bl ItemOverride_OverrideStrayFairy
    cmp r0, #0x0
    pop {r0-r12, lr}
    beq strayFairyClockTownVanilla
    @ This falls back to the CirclePlayer call.
    @ This ensures that behaviour is kept the same
    @ amongst all fairies, instead of doing the classic
    @ freeze on this fairy.
    ldr r1,=0x670
    ldrh r0,[r4, r1]
    orr r0,r0, #0x10
    strh r0,[r4, r1]
    b 0x25A71C
strayFairyClockTownVanilla:
    mov r2,#0x11c
    b 0x25A7A4

.global hook_ClockTownFairyCheck
hook_ClockTownFairyCheck:
    push {r1-r12, lr}
    bl ItemOverride_GetClockTownFairyGiven
    cmp r0, #0x0
    movne r0,#0x80
    pop {r1-r12, lr}
    bx lr

.global hook_StrayFairyCollectMessage
hook_StrayFairyCollectMessage:
    ldr r1,=0x670
    ldrh r0,[r4, r1]
    tst r0,#0x10
    bne strayFairyMessageOverride
    vpop {d8}
    b 0x5410C4
strayFairyMessageOverride:
    push {r0,r2,r3,r12,lr}
    cpy r0,r4
    bl ItemOverride_GetStrayFairyMessageId
    cpy r1,r0
    bl ItemOverride_RemoveTextId
    pop {r0,r2,r3,r12,lr}
    vpop {d8}
    cpy r0,r5
    ldmia sp!,{r4,r5,r6,lr}
    mov r2,#0x0
    b 0x21BAFC
