.arm

.section .patch_FastChestOne
.global patch_FastChestOne
patch_FastChestOne:
  bl hook_FastChests

.section .patch_changeChestTypeToMatchContents
.global patch_changeChestTypeToMatchContents
patch_changeChestTypeToMatchContents:
    bl hook_changeChestTypeToMatchContents

@ Removes a check to see if the GID is a small key.
.section .patch_AdjustMapAndCompassChests
.global patch_AdjustMapAndCompassChests
patch_AdjustMapAndCompassChests:
    bl hook_checkChestContentSetting

.section .patch_FastChestTwo
.global patch_FastChestTwo
patch_FastChestTwo:
  bl hook_FastChests
  bne 0x588078

.section .patch_RemoveChestGlowFromNonOrnateChests
.global patch_RemoveChestGlowFromNonOrnateChests
patch_RemoveChestGlowFromNonOrnateChests:
  bl hook_RemoveChestGlowFromNonOrnateChests