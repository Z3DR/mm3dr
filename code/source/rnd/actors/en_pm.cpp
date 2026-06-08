#include "rnd/actors/en_pm.h"

namespace rnd {
  void En_Pm_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2E5CC4)(actor, gctx);
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x84);
  }

  extern "C" s32 En_Pm_OverrideModelHatDraw(game::act::SkeletonAnimationModel* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  void En_Pm_Destroy(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model_DestroyByActor(actor);
    util::GetPointer<void(game::act::Actor*)>(0x48E5F4)(actor);
  }
} // namespace rnd
