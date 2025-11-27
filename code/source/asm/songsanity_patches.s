.arm

.section .patch_CheckExtForSongOfTime
.global patch_CheckExtForSongOfTime
patch_CheckExtForSongOfTime:
    bl hook_CheckExtForSongOfTime

.section .patch_SongOfTimeCheckExtData
.global patch_SongOfTimeCheckExtData
patch_SongOfTimeCheckExtData:
    bl hook_SongOfTimeCheckExtData

.section .patch_EnOsnCheckSoHExtData
.global patch_EnOsnCheckSoHExtData
patch_EnOsnCheckSoHExtData:
    bl hook_EnOsnCheckSoHExtData

.section .patch_EnMnkSongOverride
.global patch_EnMnkSongOverride
patch_EnMnkSongOverride:
    b hook_EnMnkSongOverride

.section .patch_RemoveSoSCheckKaepora
.global patch_RemoveSoSCheckKaepora
patch_RemoveSoSCheckKaepora:
    bl hook_RemoveSoSCheckKaepora

.section .patch_EnMkNWBNOverride
.global patch_EnMkNWBNOverride
patch_EnMkNWBNOverride:
    b hook_EnMkNWBNOverride