.arm

.section .patch_DoNotRemoveSkulltulaTokens
.global patch_DoNotRemoveSkulltulaTokens
patch_DoNotRemoveSkulltulaTokens:
    str r4,[r5,#0x1F0]

.section .patch_UpdateBossLairExtData
.global patch_UpdateBossLairExtData
patch_UpdateBossLairExtData:
    bl hook_UpdateBossLairExtData

.section .patch_LoadExtData
.global patch_LoadExtData
patch_LoadExtData:
    bl hook_SaveFile_Load

.section .patch_SaveFile_init
.global patch_SaveFile_init
patch_SaveFile_init:
    bl hook_SaveFile_Init

.section .patch_SaveExtDataOnOwl
.global patch_SaveExtDataOnOwl
patch_SaveExtDataOnOwl:
    b hook_OwlExtDataSave