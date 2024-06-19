#ifndef _RND_MODELS_H_
#define _RND_MODELS_H_
#include "game/actor.h"
#include "rnd/item_table.h"
#include "rnd/objects.h"
#include "z3d/z3DVec.h"
namespace rnd {

  typedef struct {
    game::act::Actor* actor;
    ItemRow* itemRow;
    s16 objectId;
    s32 objectBankIdx;
    u8 loaded;
    void* saModel;
    void* saModel2;
    f32 scale;
  } Model;

  void* SkeletonAnimationModel_Spawn(game::act::Actor*, game::GlobalContext*, s16, s32);
  void SkeletonAnimationModel_CopyMtx(void*, void*);
  void SkeletonAnimationModel_SetMeshByDrawItemID(void* model, s32 drawItemId);
  void SkeletonAnimationModel_Draw(void*, int);

  void Model_SetScale(game::act::Actor*, float);
  void Model_SetMtxAndModel(void*, void*);
  void Model_InvertMatrix(void* mtx);
  void Model_InvertMatrixByScale(void* mtx, float scale);
  void Model_UpdateMatrixPosition(void* mtx, void* mtxTwo, void* scaleMtx);

  void Model_GetObjectBankIndex(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx);
  void Model_SetAnim(void* model, s16 objectId, u32 objectAnimIndex);

  void Model_Init(Model* model, game::GlobalContext* globalCtx);
  void Model_Destroy(Model* model);
  void Model_UpdateAll(game::GlobalContext* globalCtx);
  void Model_Draw(Model* model);

  void Model_LookupByOverride(Model* model, ItemOverride override);
  void Model_InfoLookup(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId);
  void Model_Create(Model* model, game::GlobalContext* globalCtx);
  void Model_SpawnByActor(game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId);
  void Model_DestroyByActor(game::act::Actor* actor);
  void Model_DestroyAll(void);
  s32 Model_DrawByActor(game::act::Actor* actor);
  void Actor_Init();
}  // namespace rnd
#endif  //_RND_MODELS_H_