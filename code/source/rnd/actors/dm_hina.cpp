#include "rnd/actors/dm_hina.h"

namespace rnd {
  void Dm_Hina_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: %#04x\n", __func__, actor->params);
#endif
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x34F7A4)(actor, gctx);
    // TODO - Use scene or params to determined base item ID.
    // Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x55);
  }

  void Dm_Hina_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    // if (!Model_DrawByActor(actor)) {
    //   util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C7BA8)(actor, gctx);
    // }
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C7BA8)(actor, gctx);
  }

  void Dm_Hina_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    // Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x34F864)(self);
  }

}  // namespace rnd