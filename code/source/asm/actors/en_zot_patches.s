.arm

.section .patch_EnZotJarGameTextIdSwitch
.global patch_EnZotJarGameTextIdSwitch
patch_EnZotJarGameTextIdSwitch:
    bl hook_EnZotJarGameVanillaTextId

.section .patch_EnZotJarGameTextIdCheckOne
.global patch_EnZotJarGameTextIdCheckOne
patch_EnZotJarGameTextIdCheckOne:
    bl hook_EnZotJarGameVanillaTextId

.section .patch_EnZotJarGameTextIdCheckTwo
.global patch_EnZotJarGameTextIdCheckTwo
patch_EnZotJarGameTextIdCheckTwo:
    bl hook_EnZotJarGameVanillaTextId

.section .patch_EnZotJarGameTextIdNext
.global patch_EnZotJarGameTextIdNext
patch_EnZotJarGameTextIdNext:
    bl hook_EnZotJarGameVanillaTextId

.section .patch_EnZotJarGameRewardText
.global patch_EnZotJarGameRewardText
patch_EnZotJarGameRewardText:
    bl hook_EnZotJarGameRewardText

.section .patch_EnZotJarGameReward
.global patch_EnZotJarGameReward
patch_EnZotJarGameReward:
    bl hook_EnZotJarGameReward

.section .patch_EnZotJarGameRewardDoneText
.global patch_EnZotJarGameRewardDoneText
patch_EnZotJarGameRewardDoneText:
    bl hook_EnZotJarGameRewardDoneText
