.arm
.text

.global hook_FastChests
hook_FastChests:
    push {r0-r12, lr}
    bl En_Box_OverrideAnimation
    cmp r0,#0x1
    pop {r0-r12, lr}
    bx lr

.global hook_changeChestTypeToMatchContents
hook_changeChestTypeToMatchContents:
    push {r0-r3, r5-r12, lr}
    cpy r0, r4
    cpy r1, r5
    bl En_Box_OverrideSize
    cmp r0,#0xFF
    beq doNotOverrideChestType
    strb r0,[r4,#0x3e9]
    pop {r0-r3, r5-r12, lr}
    strh r7, [r4,#0x18]
    bx lr
doNotOverrideChestType:
    pop {r0-r3, r5-r12, lr}
    strh r7, [r4,#0x18]
    bx lr


.global hook_checkChestContentSetting
hook_checkChestContentSetting:
    push {r0-r12, lr}
    cpy r0,r4
    ldrb r1,[r4,#0x3E9]
    bl En_Box_IsOverrideEnabled
    cmp r0,#0x0
    beq drawFancyMapChests
    pop {r0-r12,lr}
    bx lr
drawFancyMapChests:
    pop {r0-r12,lr}
    cmpne r0,#0x3C
    bx lr

.global hook_RemoveChestGlowFromNonOrnateChests
hook_RemoveChestGlowFromNonOrnateChests:
  push {r0-r12, lr}
  bl En_Box_IsOrnateChest
  cmp r0,#0x1
  pop {r0-r12, lr}
  beq 0x22CAAC
  bx lr