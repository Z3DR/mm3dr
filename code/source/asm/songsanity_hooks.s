.arm
.text

.global hook_RemoveSoSCheckKaepora
hook_RemoveSoSCheckKaepora:
  push {r0-r12, lr}
  mov r0,#0x6A
  bl ItemOverride_ReceivedSongOverride
  cmp r0,#0x0
  pop {r0-r12,lr}
  bx lr