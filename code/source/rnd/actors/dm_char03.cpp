#include "rnd/actors/dm_char03.h"

namespace rnd {

  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6A38)(actor, gctx);
    Model_SpawnByActor(actor, GetContext().gctx, 0x78);
  }


  extern "C" s32 Dm_Char03_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  void Dm_Char03_Destroy(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model_DestroyByActor(actor);
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2043cc)(actor, gctx);
  }

}  // namespace rnd