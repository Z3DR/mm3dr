#include "rnd/actors/dm_char05.h"

namespace rnd {
  static game::act::Actor* sPoseBaseActor = NULL;
  static z3d_nn_math_MTX34 sPoseBaseInv;

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
    if (sPoseBaseActor == self) {
      sPoseBaseActor = NULL;
    }
    util::GetPointer<void(game::act::Actor*)>(0x3C6F90)(self);
  }

  // Used for all models drawn by this actor
  extern "C" s32 Dm_Char05_OverrideModelDraw(game::act::Actor* actor, z3d_nn_math_MTX34* modelMtx) {
    return Model_DrawByActor(actor, modelMtx);
  }

  extern "C" s32 Dm_Char05_OverrideAnimatedModelDraw(Dm_Char05* actor) {
    z3d_nn_math_MTX34* actorMtx = reinterpret_cast<z3d_nn_math_MTX34*>(actor->mtx);

    game::as::BoneMatrix* pose = actor->actor_util.bone_mtx_0;
    if (pose != NULL &&
        (pose[0].mtx.data[0][0] != 0.0f || pose[0].mtx.data[1][1] != 0.0f || pose[0].mtx.data[2][2] != 0.0f)) {
      if (sPoseBaseActor != actor) {
        sPoseBaseActor = actor;
        for (u32 r = 0; r < 3; ++r) {
          for (u32 c = 0; c < 3; ++c) {
            sPoseBaseInv.data[r][c] = pose[0].mtx.data[c][r];
          }
          sPoseBaseInv.data[r][3] = 0.0f;
        }
      }

      z3d_nn_math_MTX34 deltaPose;
      Model_MultiplyMatrix34(&deltaPose, &pose[0].mtx, &sPoseBaseInv);

      z3d_nn_math_MTX34 composed;
      Model_MultiplyMatrix34(&composed, actorMtx, &deltaPose);

      float actorScale = actor->model_scale.x;
      if (actorScale > 0.000001f) {
        float factor = *util::GetPointer<float>(0x41D5B0) / actorScale;
        for (u32 r = 0; r < 3; ++r) {
          for (u32 c = 0; c < 3; ++c) {
            composed.data[r][c] *= factor;
          }
        }
      }

      composed.data[1][3] += 3.0f;
      return Model_DrawByActor(actor, &composed);
    }
    return Model_DrawByActor(actor, actorMtx);
  }
}  // namespace rnd