#include "rnd/actors/dmchar05.h"

namespace rnd {
  // TODO: Crashes on game start, need to adjust calls in patch side maybe.
  extern "C" {
  void SpawnDmChar05Model(game::act::Actor* actor) {
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0);
  }

  u8 DrawDmChar05Model(game::act::Actor* actor) {
    return Model_DrawByActor(actor);
  }
  void DmChar05_rDestroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3A9C50)(self);
  }
  }

}  // namespace rnd