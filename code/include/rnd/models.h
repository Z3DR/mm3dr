#ifndef _RND_MODELS_H_
#define _RND_MODELS_H_
#include "rnd/item_table.h"
#include "rnd/objects.h"
#include "z3d/z3DVec.h"
namespace rnd {

  typedef struct {
    game::act::Actor* actor;
    ItemRow* itemRow;
    s16 objectId;
    s32 objectModelIndex;
    u8 loaded;
    u8 isFlipped;
    void* saModel;
    void* saModel2;
    f32 scale;
  } Model;

  void* SkeletonAnimationModel_Spawn(game::act::Actor*, game::GlobalContext*, s16, s32);
  void SkeletonAnimationModel_CopyMtx(void*, void*);
  void Model_SetScale(game::act::Actor*, float);
  void Model_SetMtxAndModel(void*, void*);
  void SkeletonAnimationModel_Draw(void*, int);

  void Model_UpdateAll(game::GlobalContext* globalCtx);
  void Model_SpawnByActor(game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId);
  void Model_DestroyByActor(game::act::Actor* actor);
  void Model_DestroyAll(void);
  s32 Model_DrawByActor(game::act::Actor* actor);

  void Model_InvertMatrix(void* mtx);

  void Model_InvertMatrixByScale(void* mtx, float scale);

}  // namespace rnd
#endif  //_RND_MODELS_H_