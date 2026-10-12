#include "rnd/actors/obj_boat.h"

namespace rnd {
  namespace {
    // Number of waypoints in each schedule, which is how the two directions are told apart.
    constexpr u8 waypointsToPalace = 42;
    constexpr u8 waypointsBackToTourist = 33;
    // Times the vanilla speed.
    constexpr s32 fastRideMultiplier = 6;
    // The first part of the path runs at the vanilla speed. Link is dropped onto the moving boat and lands behind
    constexpr float fastRideStartTimer = 1.5f;

    bool IsPoisonedSwampBoat(ObjBoat* boat) {
      return boat->id == game::act::Id::ObjBoat && GetContext().gctx->scene == game::SceneId::SouthernSwamp &&
             boat->schedule != nullptr;
    }
  }  // namespace

  extern "C" {
  s32 ObjBoat_GetTimeSpeed(ObjBoat* boat, s32 timeSpeed) {
    if (!IsPoisonedSwampBoat(boat) || boat->timer < fastRideStartTimer)
      return timeSpeed;
    const u8 waypoints = boat->schedule[0];
    if (waypoints == waypointsToPalace && !gExtSaveData.boatRides.swampBoatThereFastRideUsed)
      return timeSpeed * fastRideMultiplier;
    if (waypoints == waypointsBackToTourist && !gExtSaveData.boatRides.swampBoatBackFastRideUsed)
      return timeSpeed * fastRideMultiplier;
    return timeSpeed;
  }

  void ObjBoat_OnRideFinished(ObjBoat* boat) {
    if (!IsPoisonedSwampBoat(boat))
      return;
    const u8 waypoints = boat->schedule[0];
    if (waypoints == waypointsToPalace)
      gExtSaveData.boatRides.swampBoatThereFastRideUsed = 1;
    else if (waypoints == waypointsBackToTourist)
      gExtSaveData.boatRides.swampBoatBackFastRideUsed = 1;
  }
  }
}  // namespace rnd
