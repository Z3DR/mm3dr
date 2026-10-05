.arm

@ No hook, completely override call.
.section .patch_LinkEditAndRetrieveCMB
.global patch_LinkEditAndRetrieveCMB
patch_LinkEditAndRetrieveCMB:
    bl Link_EditAndRetrieveCMB

.section .patch_ApplyDamageMultiplier
.global patch_ApplyDamageMultiplier
patch_ApplyDamageMultiplier:
    bl hook_ApplyDamageMultiplier
.section .patch_AttachTunicTexAnim
.global patch_AttachTunicTexAnim
patch_AttachTunicTexAnim:
    bl hook_AttachTunicTexAnim
