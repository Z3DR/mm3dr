.arm

.section .patch_ApplyDamageMultiplier
.global patch_ApplyDamageMultiplier
patch_ApplyDamageMultiplier:
    bl hook_ApplyDamageMultiplier