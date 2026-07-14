#pragma once

#include "game/actor.h"
#include "game/as.h"
#include "rnd/gfx.h"
#include "rnd/item_table.h"
#include "rnd/objects.h"
#include "z3d/z3DVec.h"
namespace rnd {

  typedef struct {
    game::act::Actor* actor;
    ItemRow* itemRow;
    u16 baseItemId;
    s16 objectId;
    s32 objectBankIdx;
    s32 objectBankIdx2;
    u8 loaded;
    game::act::SkeletonAnimationModel* saModel;
    game::act::SkeletonAnimationModel* saModel2;
    z3d_nn_math_MTX34* hardcodedMtx;  // used for actors that draw their models in unusual ways
    f32 scale;
    z3dVec3f posOffset;
    z3dVec3f aabbMin;  // bind-posed model-space AABB of the object's CMB, valid when hasAabb is set
    z3dVec3f aabbMax;
    u8 hasAabb;
    u8 clampGround;  // lift the final matrix so the AABB never dips below the actor's ground Y
    game::as::ActorUtil actorUtil;  // Used for actors such as fairies to control their CSAB.
    u8 useActorUtil;
    u8 texAnimCtrl[0x1F0];
    game::as::BoneMatrix boneMtxBuf[16];
  } Model;

  game::act::SkeletonAnimationModel* SkeletonAnimationModel_Spawn(game::act::Actor*, game::GlobalContext*, s16, s32);
  void Actor_SetModelMatrix(float x, float y, float z, z3d_nn_math_MTX34* mtx, game::act::ActorShape* shape);
  void SkeletonAnimationModel_CopyMtx(z3d_nn_math_MTX34* dst, z3d_nn_math_MTX34* src);
  void TexAnim_Spawn(game::act::SA_TextureAnimation*, void*);
  void SkeletonAnimationModel_SetMeshByDrawItemID(void* model, s32 drawItemId);
  void SkeletonAnimationModel_Draw(void*, int);
  void SkeletonAnimationModel_ShowMesh(void*, s32);
  void SkeletonAnimationModel_HideNextMesh(void*);
  [[maybe_unused]] s32 SkeletonAnimationModel_GetMeshCount(void*);
  void Model_SetScale(game::act::Actor*, float);
  void Model_InvertMatrix(void* mtx);
  void Model_InvertMatrixByScale(void* mtx, float scale);
  void Model_UpdateMatrixPosition(void* mtx, void* mtxTwo, void* scaleMtx);
  void Model_MultiplyMatrix(z3d_nn_math_MTX34* dst, z3d_nn_math_MTX34* lhs, z3d_nn_math_MTX44* rhs);
  void Model_MultiplyMatrix34(z3d_nn_math_MTX34* dst, z3d_nn_math_MTX34* lhs, z3d_nn_math_MTX34* rhs);
  z3dVec3f Model_Matrix34_MulVec(const z3d_nn_math_MTX34* m, f32 x, f32 y, f32 z);
  void Model_GetObjectBankIndex(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx);
  void Model_SetAnim(game::act::SkeletonAnimationModel* model, s16 objectId, u32 objectAnimIndex);

  game::SceneId Model_GetActorScene(game::act::Actor* actor, game::GlobalContext* gctx);

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
  s32 Model_DrawByActor(game::act::Actor* actor, z3d_nn_math_MTX34* hardcodedMtx = NULL, s32 clampToGround = 0);
  s32 Model_DrawByActorWithPose(game::act::Actor* actor, game::as::ActorUtil* actorUtil, f32 displayScale);
  void Model_ClearPoseBase(game::act::Actor* actor);
  void Actor_Init();
}  // namespace rnd
