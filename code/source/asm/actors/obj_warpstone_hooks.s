.arm
.text

.global hook_ObjWarpstoneExitMoon
hook_ObjWarpstoneExitMoon:
  push {r0-r12, lr}
  cpy r0,r6
  bl Obj_Warpstone_ExitMoon
  pop {r0-r12, lr}
  strb r2,[r4,#0x51e]
  bx lr