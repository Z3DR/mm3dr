.arm

.section .patch_AfterInvalidatingRoomObjects
patch_AfterInvalidatingRoomObjects:
  bl hook_AfterInvalidatingRoomObjects

.section .patch_AfterObjectListCommand
patch_AfterObjectListCommand:
  bl hook_AfterObjectListCommand