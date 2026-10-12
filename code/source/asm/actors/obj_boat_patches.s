.arm

.section .patch_ObjBoatTimeSpeedOne
.global patch_ObjBoatTimeSpeedOne
patch_ObjBoatTimeSpeedOne:
    bl hook_ObjBoatTimeSpeed

.section .patch_ObjBoatRideFinished
.global patch_ObjBoatRideFinished
patch_ObjBoatRideFinished:
    bl hook_ObjBoatRideFinished

.section .patch_ObjBoatTimeSpeedTwo
.global patch_ObjBoatTimeSpeedTwo
patch_ObjBoatTimeSpeedTwo:
    bl hook_ObjBoatTimeSpeed
