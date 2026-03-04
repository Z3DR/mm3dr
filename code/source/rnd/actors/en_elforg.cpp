#include "rnd/actors/en_elforg.h"

namespace rnd {
  void En_Elforg_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3CD9D0)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0xBB);
  }

  extern "C" s32 En_Elforg_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  void En_Elforg_Destroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3CDF20)(self);
  }

}  // namespace rnd