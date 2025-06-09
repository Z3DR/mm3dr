#include "rnd/actors/obj_moon_stone.h"

namespace rnd {
  void Obj_Moon_Stone_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x4A95E4)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0x96);
  }

  extern "C" s32 Obj_Moon_Stone_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  void Obj_Moon_Stone_Destroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x4A9820)(self);
  }

}  // namespace rnd