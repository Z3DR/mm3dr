#include "rnd/actors/dm_char05.h"

namespace rnd {
  void Dm_Char05_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6CD8)(actor, gctx);
    if (actor->params == DMCHAR05_GORON_MASK)
      Model_SpawnByActor(actor, GetContext().gctx, 0x79);
    else if (actor->params == DMCHAR05_ZORA_MASK)
      Model_SpawnByActor(actor, GetContext().gctx, 0x7A);
    else if (actor->params == DMCHAR05_GIBDO_MASK)
      Model_SpawnByActor(actor, GetContext().gctx, 0x87);
    else if (actor->params == DMCHAR05_COUPLE_MASK)
      Model_SpawnByActor(actor, GetContext().gctx, 0x85);
    else if (actor->params == DMCHAR05_BOMBERS_NOTEBOOK)
      Model_SpawnByActor(actor, GetContext().gctx, 0x50);
  }

  void Dm_Char05_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    Model_ClearPoseBase(self);
    util::GetPointer<void(game::act::Actor*)>(0x3C6F90)(self);
  }

  // Used for all models drawn by this actor
  extern "C" s32 Dm_Char05_OverrideModelDraw(game::act::Actor* actor, z3d_nn_math_MTX34* modelMtx) {
    return Model_DrawByActor(actor, modelMtx);
  }

  extern "C" s32 Dm_Char05_OverrideAnimatedModelDraw(Dm_Char05* actor) {
    return Model_DrawByActorWithPose(actor, &actor->actor_util, 0.2f);
  }
}  // namespace rnd