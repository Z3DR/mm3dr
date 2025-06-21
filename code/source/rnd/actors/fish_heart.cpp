#include "rnd/actors/fish_heart.h"

namespace rnd {
  void Fish_Heart_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3F7808)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0x0C);
  }

  extern "C" s32 Fish_Heart_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  void Fish_Heart_Destroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3F7AB4)(self);
  }

}  // namespace rnd