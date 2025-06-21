#include "rnd/actors/dm_hina.h"

namespace rnd {
  void Dm_Hina_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x34F7A4)(actor, gctx);
    // TODO - Use scene or params to determined base item ID.
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x5C + (actor->params * 2));
  }

  void Dm_Hina_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x34F864)(self);
  }

  extern "C" s32 Remains_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

}  // namespace rnd