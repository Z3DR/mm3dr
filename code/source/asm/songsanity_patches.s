.arm

.section .patch_CheckExtForSongOfTime
.global patch_CheckExtForSongOfTime
patch_CheckExtForSongOfTime:
    bl hook_CheckExtForSongOfTime

.section .patch_SongOfTimeCheckExtData
.global patch_SongOfTimeCheckExtData
patch_SongOfTimeCheckExtData:
    bl hook_SongOfTimeCheckExtData

.section .patch_LullabyIntroCheckEnJg
.global patch_LullabyIntroCheckEnJg
patch_LullabyIntroCheckEnJg:
    bl hook_LullabyIntroCheckEnJg

.section .patch_LullabyCheckEnJg
.global patch_LullabyCheckEnJg
patch_LullabyCheckEnJg:
    bl hook_LullabyCheckEnJg

.section .patch_EnOsnCheckSoHExtData
.global patch_EnOsnCheckSoHExtData
patch_EnOsnCheckSoHExtData:
    bl hook_EnOsnCheckSoHExtData

.section .patch_EnMa4ExtDataCheckOne
.global patch_EnMa4ExtDataCheckOne
patch_EnMa4ExtDataCheckOne:
    bl hook_EnMa4ExtDataCheckOne

.section .patch_EnMa4ExtDataCheckThree
.global patch_EnMa4ExtDataCheckThree
patch_EnMa4ExtDataCheckThree:
    bl hook_EnMa4ExtDataCheckThree

.section .patch_EnMa4ExtDataCheckTwo
.global patch_EnMa4ExtDataCheckTwo
patch_EnMa4ExtDataCheckTwo:
    bl hook_EnMa4ExtDataCheckTwo

.section .patch_EnMnkSongOverride
.global patch_EnMnkSongOverride
patch_EnMnkSongOverride:
    b hook_EnMnkSongOverride

.section .patch_RemoveSoSCheckKaepora
.global patch_RemoveSoSCheckKaepora
patch_RemoveSoSCheckKaepora:
    bl hook_RemoveSoSCheckKaepora

.section .patch_OverrideSoSGiveItem
.global patch_OverrideSoSGiveItem
patch_OverrideSoSGiveItem:
    b hook_OverrideSoSGiveItem

.section .patch_EnGkCheckLullabyRewardGiven
.global patch_EnGkCheckLullabyRewardGiven
patch_EnGkCheckLullabyRewardGiven:
    bleq hook_EnGkCheckLullabyRewardGiven


.section .patch_EnMkNWBNOverride
.global patch_EnMkNWBNOverride
patch_EnMkNWBNOverride:
    b hook_EnMkNWBNOverride