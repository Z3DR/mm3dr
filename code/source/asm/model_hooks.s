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

.global hook_RemainsModelDraw
hook_RemainsModelDraw:
    mov r1,#0x0
    push {r0-r12, lr}
    @ r0 = saModel
    cpy r1,r4 @ actor
    bl Remains_OverrideModelDraw
    cmp r0,#0x0
    pop {r0-r12, lr}
    addne lr,lr,#0x4 @ skip drawing vanilla model
    bx lr

.global hook_ObjMoonStoneDraw
hook_ObjMoonStoneDraw:
    mov r1,#0x0
    push {r0-r12, lr}
    cpy r1,r4 @ actor
    bl Obj_Moon_Stone_OverrideModelDraw
    cmp r0,#0x0
    pop {r0-r12, lr}
    addne lr,lr,#0x4 @ skip drawing vanilla model
    @bne 0x4CEEDC
    bx lr

.global hook_FishHeartDraw
hook_FishHeartDraw:
    push {r0-r12, lr}
    cpy r1,r4 @ actor
    bl Fish_Heart_OverrideModelDraw
    cmp r0,#0x0
    pop {r0-r12, lr}
    bxne lr
    b 0x20AAA8 

.global hook_DmChar03ModelDraw
hook_DmChar03ModelDraw:
    mov r1,#0x0
    push {r0-r12, lr}
    @ r0 = saModel
    cpy r1,r4 @ actor
    bl Dm_Char03_OverrideModelDraw
    cmp r0,#0x0
    pop {r0-r12, lr}
    addne lr,lr,#0x4 @ skip drawing vanilla model
    bx lr

.global hook_DmChar05ModelDraw
hook_DmChar05ModelDraw:
    cpy r0,r5
    push {r0-r12, lr}
    cpy r0,r4 @ actor
    cpy r1,r2 @ model matrix
    bl Dm_Char05_OverrideModelDraw
    cmp r0,#0x0
    pop {r0-r12, lr}
    addne lr,lr,#0x4 @ skip drawing vanilla model
    bx lr
