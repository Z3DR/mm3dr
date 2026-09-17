#include "rnd/actors/en_akindonuts.h"

namespace rnd {
  extern "C" {
  // The Business Scrub refuses the Biggest Bomb Bag once the player owns one. His check is shuffled, so
  // refuse it only once it has been bought from him.
  bool En_Akindonuts_CheckBombBagExtData() {
    return gExtSaveData.givenItemChecks.enAkindonutsBombBagGiven.Value();
  }
  }
}  // namespace rnd
