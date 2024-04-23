#include "rnd/actors/item00.h"

namespace rnd {
  extern "C" {
  void SpawnItem00Model(game::act::Actor* actor) {
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0);
  }

  u8 DrawItem00Model(game::act::Actor* actor) {
    return Model_DrawByActor(actor);
  }
  }
}  // namespace rnd