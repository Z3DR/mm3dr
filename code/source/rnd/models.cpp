#include "rnd/models.h"
#define LOADEDMODELS_MAX 16

namespace rnd {
  Model ModelContext[LOADEDMODELS_MAX] = {0};
  void* SkeletonAnimationModel_Spawn(game::act::Actor* actor, game::GlobalContext* gctx, s16 objectId, s32 objectModelIndex) {
    return util::GetPointer<void*(game::act::Actor * actor, game::GlobalContext * globalCtx, s16 objId,
                                  s32 objModelIdx)>(0x203c40)(actor, gctx, objectId, objectModelIndex);
  }

  void SkeletonAnimationModel_CopyMtx(void* fromMtx, void* toMtx) {
    util::GetPointer<void(void*, void*)>(0x1feab0)(fromMtx, toMtx);
  }

  void Model_SetScale(game::act::Actor* actor, float scale) {
    util::GetPointer<void(game::act::Actor*, float)>(0x21e30c)(actor, scale);
  }

  void Model_SetMtxAndModel(void* model, void* mtx) {
    util::GetPointer<void(void*, void*)>(0x1feaa8)(model, mtx);
  }

  void Model_Draw(void* model, int unk) {
    util::GetPointer<void(void*, int)>(0x20AAA8)(model, unk);
  }
} // namespace rnd
