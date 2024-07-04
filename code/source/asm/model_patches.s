.arm

.section .patch_ExtendedObjectClear
.global patch_ExtendedObjectClear
patch_ExtendedObjectClear:
  bl ExtendedObject_Clear

.section .patch_ModelSpawnGetObjectStatus
.global patch_ModelSpawnGetObjectStatus
patch_ModelSpawnGetObjectStatus:
  bl hook_ModelSpawnGetObjectStatus

.section .patch_GetExtendedObjectStatus
.global patch_GetExtendedObjectStatus
patch_GetExtendedObjectStatus:
  bl hook_ModelSpawnGetObjectStatus

@ .section .patch_DmChar05GoronInit
@ .global patch_DmChar05GoronInit
@ patch_DmChar05GoronInit:
@   bl hook_DmChar05GoronInit

.section .patch_OverrideItem00Init
.global patch_OverrideItem00Init
patch_OverrideItem00Init:
  bl hook_OverrideItem00Init

@ .section .patch_DmChar05GoronDraw
@ .global patch_DmChar05GoronDraw
@ patch_DmChar05GoronDraw:
@   bl hook_DmChar05Draw

.section .patch_OverrideItem00Draw
.global patch_OverrideItem00Draw
patch_OverrideItem00Draw:
  bl hook_OverrideItem00Draw

.section .patch_DmChar05GetObjectStatus
.global patch_DmChar05GetObjectStatus
patch_DmChar05GetObjectStatus:
  bl hook_ModelSpawnGetObjectStatus