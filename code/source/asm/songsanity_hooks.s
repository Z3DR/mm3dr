.arm
.text

.global rActiveItemRow
.rActiveItemRow_addr:
    .word rActiveItemRow

.global hook_CheckExtForSongOfTime
hook_CheckExtForSongOfTime:
    push {r0-r12, lr}
    mov r0,#0x67
    bl ItemOverride_ReceivedSongOverride
    cmp r0,#0x0
    pop {r0-r12,lr}
    bx lr

.global hook_SongOfTimeCheckExtData
hook_SongOfTimeCheckExtData:
    push {r0-r12, lr}
    bl ItemOverride_CheckIfSongOfTimeAwarded
    cmp r0,#0x4C
    pop {r0-r12,lr}
    bx lr

.global hook_EnOsnCheckSoHExtData
hook_EnOsnCheckSoHExtData:
    push {r0-r12, lr}
    mov r0,#0x69
    bl ItemOverride_ReceivedSongOverride
    cmp r0,#0x0
    pop {r0-r12,lr}
    bx lr

.global hook_EnMnkSongOverride
hook_EnMnkSongOverride:
    push {r0-r12,lr}
    mov r0,#0x61
    bl ItemOverride_ReceivedSongOverride
    cmp r0,#0x0
    pop {r0-r12, lr}
    beq noOverrideSonataItemID
    b 0x325D5C @Item was already given, so basically noop this call.
noOverrideSonataItemID:
    cpy r0,r5
    bl 0x233BEC
    b 0x325D5C

.global hook_RemoveSoSCheckKaepora
hook_RemoveSoSCheckKaepora:
    push {r0-r12, lr}
    mov r0,#0x6A
    bl ItemOverride_ReceivedSongOverride
    cmp r0,#0x0
    pop {r0-r12,lr}
    bx lr

.global hook_EnMkNWBNOverride
hook_EnMkNWBNOverride:
    push {r0-r12,lr}
    cpy r2,r1
    mov r1,#0xFF
    bl ItemOverride_GetSoHOrSongItem
    ldr r5,.rActiveItemRow_addr
    ldr r5,[r5]
    cmp r5,#0x0
    pop {r0-r12, lr}
    beq noOverrideNWBNItemID
    push {r0-r12, lr}
    ldr r0, [r8,#0xDC] @Load player actor.
    bl ItemOverride_GetItemTextAndItemID
    pop {r0-r12, lr}
    cpy r0,r8
    b 0x587230
noOverrideNWBNItemID:
    cpy r0,r8
    nop
    nop
    bl 0x233BEC
    b 0x587230