.arm
.text

@ playEx (0x1DC3D0) is where every stream converges: playXXXOnChannel1 reaches it on all three of
@ its branches, and StreamMgr::play calls it too (0x239304 / 0x239434). Patched at the second
@ instruction, after the prologue has pushed {r4-r11, lr}.
@
@ playEx takes the StreamId as argument 3, so it arrives in r2 and has to be moved into r0 for the
@ call. r0 itself is playEx's object pointer and must survive for the displaced cpy, which is why
@ it is pushed and restored while the remapped id is written back to r2 -- r2 is deliberately not
@ in the pop list.
@
@ 6 registers is 24 bytes, keeping sp 8-byte aligned across the call as AAPCS requires. r4 pads the
@ list and is dead here anyway; the displaced instruction is what sets it.

.global hook_MusicOverridePlay
hook_MusicOverridePlay:
  push {r0, r1, r3, r4, r12, lr}
  cpy  r0, r2                 @ arg 1 = StreamId
  bl   Music_OverridePlay
  cpy  r2, r0                 @ remapped id back into place
  pop  {r0, r1, r3, r4, r12, lr}
  cpy  r4, r0                 @ displaced instruction
  bx   lr
