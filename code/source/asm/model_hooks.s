.arm
.text


.global hook_ModelSpawnGetObjectStatus
hook_ModelSpawnGetObjectStatus:
    push {r1-r12, lr}
    bl ExtendedObject_GetStatus
    pop {r1-r12, lr}
    bx lr

@ .global hook_DmChar05GoronInit
@ hook_DmChar05GoronInit:
@     push {r0-r12,lr}
@     bl DmChar05_Goron_Init
@     pop {r0-r12, lr}
@     add r0,r4,#0xC0
@     bx lr

.global hook_OverrideItem00Init
hook_OverrideItem00Init:
    cpy r4,r0
    push {r0-r12,lr}
    bl SpawnItem00Model
    pop {r0-r12, lr}
    bx lr

@ .global hook_DmChar05Draw
@ hook_DmChar05Draw:
@     push {r0-r12,lr}
@     cpy r0,r4
@     cpy r1,r5
@     bl DmChar05_Draw
@     cmp r0,#0x0
@     pop {r0-r12, lr}
@     beq drawOriginalModel
@     bx lr
@ drawOriginalModel:
@     str r0,[r4, #0x284]
@     bx lr

.global hook_OverrideItem00Draw
hook_OverrideItem00Draw:
    cpy r4,r0
    push {r0-r12,lr}
    bl DrawItem00Model
    cmp r0,#0x0
    pop {r0-r12, lr}
    bxeq lr
    pop {r4,r5,pc}