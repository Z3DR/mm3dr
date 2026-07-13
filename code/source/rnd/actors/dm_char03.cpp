#include "rnd/actors/dm_char03.h"

namespace rnd {

  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6A38)(actor, gctx);
    Model_SpawnByActor(actor, GetContext().gctx, 0x78);
  }

  extern "C" s32 Dm_Char03_OverrideModelDraw(game::act::SkeletonAnimationModel* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  extern "C" s32 Dm_Char03_OverrideAnimatedModelDraw(Dm_Char03* actor) {
    return Model_DrawByActorWithPose(actor, &actor->actor_util, 0.2f);
  }

  void Dm_Char03_Destroy(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model_DestroyByActor(actor);
    Model_ClearPoseBase(actor);
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2043cc)(actor, gctx);
  }

}  // namespace rnd