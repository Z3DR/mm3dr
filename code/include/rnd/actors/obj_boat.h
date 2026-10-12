#pragma once

#include "game/actor.h"
#include "game/common_data.h"
#include "rnd/item_override.h"
#include "rnd/savefile.h"

namespace rnd {
  struct ObjBoat : public game::act::Actor {
    u8 gap_1F8[0x14];
    void* action_fn;
    // Bit 0: the path has started. Bit 1: the path is finished.
    u16 state_flags;
    u8 gap_212[2];
    // The schedule being followed. The first byte is its waypoint count: 42 going to the palace, 33 coming back.
    const u8* schedule;
    // Path progress per frame. 4 for the vanilla ride.
    s16 time_speed;
    u8 gap_21A[6];
    float last_pos[3];
    // Counts up to path_length / units_per_timer_step.
    float timer;
    s32 path_length;
    s32 units_per_timer_step;
    u32 waypoint_index;
    float path_distance;
    u32 field_240;
  };
  static_assert(offsetof(ObjBoat, action_fn) == 0x20C);
  static_assert(offsetof(ObjBoat, state_flags) == 0x210);
  static_assert(offsetof(ObjBoat, schedule) == 0x214);
  static_assert(offsetof(ObjBoat, time_speed) == 0x218);
  static_assert(offsetof(ObjBoat, last_pos) == 0x220);
  static_assert(offsetof(ObjBoat, timer) == 0x22C);
  static_assert(offsetof(ObjBoat, waypoint_index) == 0x238);
  static_assert(offsetof(ObjBoat, path_distance) == 0x23C);
  static_assert(sizeof(ObjBoat) == 0x244);

  extern "C" {
  s32 ObjBoat_GetTimeSpeed(ObjBoat*, s32 timeSpeed);
  void ObjBoat_OnRideFinished(ObjBoat*);
  }
}  // namespace rnd
