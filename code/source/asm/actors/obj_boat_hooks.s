.arm
.text

.global hook_ObjBoatTimeSpeed
hook_ObjBoatTimeSpeed:
    push {r0-r12,lr}
    cpy r0,r4
    ldrsh r1,[r5,#0x18]
    bl ObjBoat_GetTimeSpeed
    str r0,[sp]
    pop {r0-r12,lr}
    bx lr

.global hook_ObjBoatRideFinished
hook_ObjBoatRideFinished:
    push {r0-r12,lr}
    cpy r0,r4
    bl ObjBoat_OnRideFinished
    pop {r0-r12,lr}
    ldrh r0,[r5,#0x10]
    bx lr
