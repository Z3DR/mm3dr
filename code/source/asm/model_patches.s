.arm

.section .patch_ExtendedObjectClear
.global patch_ExtendedObjectClear
patch_ExtendedObjectClear:
  bl ExtendedObject_Clear

.section .patch_ModelSpawnGetObjectStatus
.global patch_ModelSpawnGetObjectStatus
patch_ModelSpawnGetObjectStatus:
  bl hook_ModelSpawnGetObjectStatus

.section .patch_storeObjectIdForExtendedObject
.global patch_storeObjectIdForExtendedObject
patch_storeObjectIdForExtendedObject:
  bl hook_storeObjectIdForExtendedObject

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

.section .patch_RemainsModelDraw
.global RemainsModelDraw_patch
RemainsModelDraw_patch:
    bl hook_RemainsModelDraw

.section .patch_ObjMoonStoneDraw
.global patch_ObjMoonStoneDraw
patch_ObjMoonStoneDraw:
    bl hook_ObjMoonStoneDraw

.section .patch_FishHeartDraw
.global patch_FishHeartDraw
patch_FishHeartDraw:
    b hook_FishHeartDraw

.section .patch_DmChar03ModelDraw
.global DmChar03ModelDraw
DmChar03ModelDraw:
    bl hook_DmChar03ModelDraw

.section .patch_GoronMaskModelDraw
.global GoronMaskModelDraw_patch
GoronMaskModelDraw_patch:
    bl hook_DmChar05ModelDraw

.section .patch_ZoraMaskModelDraw
.global ZoraMaskModelDraw_patch
ZoraMaskModelDraw_patch:
    bl hook_DmChar05ModelDraw

.section .patch_GibdoMaskModelDraw
.global GibdoMaskModelDraw_patch
GibdoMaskModelDraw_patch:
    bl hook_DmChar05ModelDraw

.section .patch_CoupleMaskModelDraw
.global CoupleMaskModelDraw_patch
CoupleMaskModelDraw_patch:
    bl hook_DmChar05ModelDraw

.section .patch_BombersNotebookModelDraw
.global BombersNotebookModelDraw_patch
BombersNotebookModelDraw_patch:
    bl hook_DmChar05ModelDraw
