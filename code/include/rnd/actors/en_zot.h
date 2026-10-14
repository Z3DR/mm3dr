#pragma once

#include "game/actor.h"
#include "game/common_data.h"
#include "rnd/item_override.h"

namespace rnd {
  extern "C" {
  bool EnZot_JarGameOverrideActive();
  bool EnZot_GiveJarGameReward();
  u16 EnZot_JarGameRewardText(u16);
  u32 EnZot_JarGameVanillaTextId(u32);
  }

  ItemOverride_Key EnZot_GetZoraHallKey(game::act::Actor* actor, s16 getItemId);
}  // namespace rnd
