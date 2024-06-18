.arm

.section .patch_ExtendedObjectClear
.global patch_ExtendedObjectClear
patch_ExtendedObjectClear:
  bl ExtendedObject_Clear

.section .patch_ModelSpawnGetObjectStatus
.global patch_ModelSpawnGetObjectStatus
patch_ModelSpawnGetObjectStatus:
  bl hook_ModelSpawnGetObjectStatus

.section .patch_OverrideDmChar05Init
.global patch_OverrideDmChar05Init
patch_OverrideDmChar05Init:
  bl hook_OverrideDmChar05Init

.section .patch_OverrideItem00Init
.global patch_OverrideItem00Init
patch_OverrideItem00Init:
  bl hook_OverrideItem00Init

.section .patch_OverrideDmChar05Draw
.global patch_OverrideDmChar05Draw
patch_OverrideDmChar05Draw:
  bl hook_OverrideDmChar05Draw

.section .patch_OverrideItem00Draw
.global patch_OverrideItem00Draw
patch_OverrideItem00Draw:
  bl hook_OverrideItem00Draw