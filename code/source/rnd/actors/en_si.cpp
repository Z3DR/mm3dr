#include "rnd/actors/en_si.h"

namespace rnd {
  extern "C" {
  void En_Si_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2E7FCC)(actor, gctx);
    Model_SpawnByActor(actor, GetContext().gctx, 0x20);
  }

  void En_Si_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (!Model_DrawByActor(actor)) {
      util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x367C14)(actor, gctx);
    }
  }

  void En_Si_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x51159C)(self);
  }
  }

}  // namespace rnd