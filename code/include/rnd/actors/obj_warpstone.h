#pragma once

#include "game/actor.h"
#include "game/collision.h"
#include "game/items.h"
#include "rnd/settings.h"

namespace rnd {

  typedef enum {
    /* 0 */ SEK_MODEL_CLOSED,
    /* 1 */ SEK_MODEL_OPENED
  } ObjectSekModels;

  struct Obj_Warpstone : public game::act::Actor {
    u8 gap_1F8[8];
    u32 field_200;
    u8 gap_204[8];
    game::CollisionBodyCylinder col_body;
    u8 talking;
    u8 timer;
    ObjectSekModels modelIndex;
    u8 save_status;
    void* calc_fn;
    game::act::sa_unk_d4* skelAnime;
    u32 field_270;
    u32 field_274;
    u16 field_278;
    u16 field_27A;
    u16 timer2;
    u8 gap_27E[2];
    u32 field_280;
    u32 field_284;
    u32 field_288;
    u8 activating;
    u8 ex_started_activating;
    u16 field_28E;
  };
  static_assert(sizeof(Obj_Warpstone) == 0x290);

  extern "C" void Obj_Warpstone_ExitMoon(game::GlobalContext*, u16);
}  // namespace rnd
