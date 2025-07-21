.arm
.text

.global hook_CheckOcarinaDive
hook_CheckOcarinaDive:
    push {r0-r12, lr}
    bl SettingsEnableOcarinaDive
    cmp r0, #0x0
    pop {r0-r12, lr}
    bne 0x1e1f10
    tst r0,#0x1
    beq 0x1E1FB4
    b 0x1e1f10

.global hook_FixRemovingOcarinaFromInventory
hook_FixRemovingOcarinaFromInventory:
    cmp r0,#0x0
    beq ocarinaAlwaysInInventory
    add r0,r0,r1 @ original instruction
    b 0x201068
ocarinaAlwaysInInventory:
    mov r0, #0x0 @ Force the ocarina to always be in inventory
    b 0x20106C

.global hook_DmSktOcarinaAnimationPatchOne
hook_DmSktOcarinaAnimationPatchOne:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq ocarinaAnimationPatchOne
    cmp r0,r0
    bx lr
ocarinaAnimationPatchOne:
    cmp r0,#0x2
    bx lr

.global hook_DmSktOcarinaAnimationPatchTwo
hook_DmSktOcarinaAnimationPatchTwo:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq ocarinaAnimationPatchTwo
    cmp r0,r0
    bx lr
ocarinaAnimationPatchTwo:
    cmp r0,#0x2
    bx lr

.global hook_DmSktOcarinaAnimationPatchThree
hook_DmSktOcarinaAnimationPatchThree:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq ocarinaAnimationPatchThree
    cmp r0,r0
    bx lr
ocarinaAnimationPatchThree:
    cmp r0,#0x2
    bx lr


.global hook_DmSktOcarinaAnimationPatchFour
hook_DmSktOcarinaAnimationPatchFour:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq ocarinaAnimationPatchFour
    cmp r0,r0
    bx lr
ocarinaAnimationPatchFour:
    cmp r0,#0x2
    bx lr
  
.global hook_DmStkInitOcarinaPatch
hook_DmStkInitOcarinaPatch:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq initOcarinaReceivedFromSkt
    cmp r0,r0
    bx lr
initOcarinaReceivedFromSkt:
    cmp r0,#0x2
    bx lr

.global hook_UpdateOcarinaVisibility
hook_UpdateOcarinaVisibility:
    push {r0-r5, r7-r12,lr}
    bl CheckIfOcarinaIsInInventory
    subs r6,r0,#0xFF
    pop {r0-r5, r7-r12,lr}
    bx lr

.global hook_DmSktOcarinaAnimationPatchFive
hook_DmSktOcarinaAnimationPatchFive:
    push {r0-r12,lr}
    bl ItemOverride_ReceivedOcarinaFromSkt
    cmp r0, #0x1
    pop {r0-r12,lr}
    beq ocarinaAnimationPatchFive
    cmp r0,r0
    bx lr
ocarinaAnimationPatchFive:
    cmp r0,#0x2
    bx lr

.global hook_FixOcarinaGearScreenDefault
hook_FixOcarinaGearScreenDefault:
    push {r1-r12, lr}
    bl CheckIfOcarinaIsInInventory
    pop {r1-r12, lr}
    bx lr

.global hook_FixOcarinaGearScreenDeku
hook_FixOcarinaGearScreenDeku:
    push {r1-r12, lr}
    bl CheckIfOcarinaIsInInventory
    pop {r1-r12, lr}
    bx lr

.global hook_FixOcarinaGearScreenGoron
hook_FixOcarinaGearScreenGoron:
    push {r1-r12, lr}
    bl CheckIfOcarinaIsInInventory
    pop {r1-r12, lr}
    bx lr

.global hook_FixOcarinaGearScreenZora
hook_FixOcarinaGearScreenZora:
    push {r1-r12, lr}
    bl CheckIfOcarinaIsInInventory
    pop {r1-r12, lr}
    bx lr

.global hook_FixOcarinaGearScreenZoraTwo
hook_FixOcarinaGearScreenZoraTwo:
    push {r1-r12, lr}
    bl ZoraMaskCheck
    pop {r1-r12, lr}
    bx lr

.global hook_HandleOcarina
hook_HandleOcarina:
    push {r0-r12, lr}
    mov r1, r0 @ song
    mov r0, r4 @ MessageWindow* this
    bl HandleOcarinaSong
    cmp r0, #0
    pop {r0-r12, lr}
    bne 0x606424
    cmp r0, #0x16 @ original instruction
    b 0x604d90