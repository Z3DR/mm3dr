#pragma once

#include "game/actor.h"
#include "rnd/savefile.h"

namespace rnd {
  extern "C" {
  bool En_Ms_CheckBeanExtData();
  bool En_Ms_CheckBeanInventory();
  }

}  // namespace rnd