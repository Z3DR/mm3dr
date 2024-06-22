#include "rnd/actors/item00.h"

namespace rnd {
  extern "C" {
  void SpawnItem00Model(game::act::Actor* actor) {
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0);
  }

  u8 DrawItem00Model(game::act::Actor* actor) {
    // XXX: Be cautious about this, since we're only rendering over heart pieces for now.
    // This field 1FC is a check to see if a heart container was collected. 0xA = not collected.
    // 0x00 = collected.
    if (static_cast<En_Item00*>(actor)->field_1FC != 0x00)
      return Model_DrawByActor(actor);
    else return 0;
  }
  void EnItem00_rDestroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3D0930)(self);
  }
  }

}  // namespace rnd