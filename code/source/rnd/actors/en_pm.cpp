#include "rnd/actors/en_pm.h"

namespace rnd {
  void En_Pm_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2E5CC4)(actor, gctx);
    if (gSettingsContext.showPostmanItem == 1)
      Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x84);
  }

  extern "C" s32 En_Pm_OverrideModelHatDraw(game::act::SkeletonAnimationModel* saModel, game::act::Actor* actor) {
    if (gSettingsContext.showPostmanItem == 1) {
      static z3d_nn_math_MTX34 hatMtx;
      f32 hatX = saModel->mtx.data[0][3];
      f32 hatY = saModel->mtx.data[1][3] + 20;
      f32 hatZ = saModel->mtx.data[2][3];
      Actor_SetModelMatrix(hatX, hatY, hatZ, &hatMtx, &actor->actor_shape);
      return Model_DrawByActor(actor, &hatMtx);
    } else {
      return 0;
    }
  }

  void En_Pm_Destroy(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (gSettingsContext.showPostmanItem == 1)
      Model_DestroyByActor(actor);
    util::GetPointer<void(game::act::Actor*)>(0x48E5F4)(actor);
  }
}  // namespace rnd
