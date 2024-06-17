.arm
.text

.global hook_ModelSpawnGetObjectStatus
hook_ModelSpawnGetObjectStatus:
    push {r1-r12, lr}
    bl ExtendedObject_GetStatus
    pop {r1-r12, lr}
    bx lr

@ .global hook_DestroyItem00
@ hook_DestroyItem00:
@     push {r0-r12, lr}
@     bl EnItem00_rDestroy
@     pop {r0-r12, lr}
@     cpy r4, r0
@     bx lr

.global hook_OverrideItem00Init
hook_OverrideItem00Init:
    cpy r4,r0
    push {r0-r12,lr}
    bl SpawnItem00Model
    pop {r0-r12, lr}
    bx lr

.global hook_OverrideItem00Draw
hook_OverrideItem00Draw:
    cpy r4,r0
    push {r0-r12,lr}
    bl DrawItem00Model
    cmp r0,#0x0
    pop {r0-r12, lr}
    bxeq lr
    pop {r4,r5,pc}