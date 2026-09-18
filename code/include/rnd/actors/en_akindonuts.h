#pragma once

#include "game/actor.h"
#include "rnd/item_override.h"
#include "rnd/savefile.h"

namespace rnd {
  extern "C" {
  bool En_Akindonuts_CheckBombBagExtData();
  }

  bool En_Akindonuts_IsSaleBoughtAndNonRepeatable(const ItemOverride* override);
  void En_Akindonuts_SetSaleBought(ItemOverride_Key key);
}  // namespace rnd
