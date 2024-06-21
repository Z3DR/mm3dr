#include "rnd/actors/item_b_heart.h"

namespace rnd {
  extern "C" {
  void ItemBHeart_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x464648)(actor, gctx);
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x0D);
  }

  void ItemBHeart_Draw(game::act::Actor* actor, game::GlobalContext*) {
    if (!Model_DrawByActor(actor)) {
      util::GetPointer<void(game::act::Actor*)>(0x4A779C)(actor);
    }
  }
  
  void ItemBHeart_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x4646F0)(self);
  }
  }

}  // namespace rnd