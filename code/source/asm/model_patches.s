.arm

.section .patch_ExtendedObjectClear
.global patch_ExtendedObjectClear
patch_ExtendedObjectClear:
  bl ExtendedObject_Clear

.section .patch_ModelSpawnGetObjectStatus
.global patch_ModelSpawnGetObjectStatus
patch_ModelSpawnGetObjectStatus:
  bl hook_ModelSpawnGetObjectStatus

.section .patch_OverrideItem00Init
.global patch_OverrideItem00Init
patch_OverrideItem00Init:
  bl hook_OverrideItem00Init

.section .patch_OverrideItem00Draw
.global patch_OverrideItem00Draw
patch_OverrideItem00Draw:
  bl hook_OverrideItem00Draw