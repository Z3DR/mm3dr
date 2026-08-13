.arm
.text

.global hook_AfterInvalidatingRoomObjects
hook_AfterInvalidatingRoomObjects:
  push {r0-r12, lr}
  bl ExtendedObject_InvalidateRoomObjects
  pop {r0-r12, lr}
  ldr r0,[sp,#0x18]
  bx lr

.global hook_AfterObjectListCommand
hook_AfterObjectListCommand:
  push {r0-r12, lr}
    bl ExtendedObject_AfterObjectListCommand
    pop {r0-r12, lr}
    mov r0,#0x1
    bx lr