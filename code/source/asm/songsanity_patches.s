.arm

.section .patch_EnOsnCheckSoHExtData
.global patch_EnOsnCheckSoHExtData
patch_EnOsnCheckSoHExtData:
    bl hook_EnOsnCheckSoHExtData

.section .patch_RemoveSoSCheckKaepora
.global patch_RemoveSoSCheckKaepora
patch_RemoveSoSCheckKaepora:
    bl hook_RemoveSoSCheckKaepora

.section .patch_EnMkNWBNOverride
.global patch_EnMkNWBNOverride
patch_EnMkNWBNOverride:
    b hook_EnMkNWBNOverride