#ifndef _RND_MODELS_H_
#define _RND_MODELS_H_
#include "game/actor.h"
#include "game/actorresource.h"
#include "rnd/item_table.h"
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

  s32 Object_SpawnPersistent(void* objectCtx, s16 objectId);
  s32 Object_GetSlot(void* objectCtx, s16 objectId);
  // TODO: IsLoaded, UpdateBank, and Clear

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

  // Extended Object to deal with spawning multiple actors without running out of standard game space.
  typedef game::ActorResource::ObjectContext ExtendedObjectContext;
  extern ExtendedObjectContext rExtentdedObjectCtx;

  s32 ExtendedObject_Spawn(game::ActorResource::ObjectContext* objectCtx, s16 objectId);      // TODO: Implement
  s32 ExtendedObject_GetIndex(game::ActorResource::ObjectContext* objectCtx, s16 objectId);   // TODO: Implement
  game::ActorResource::ActorResource* ExtendedObject_GetStatus(s16 objectId);                 // TODO: Implement
  s32 ExtendedObject_IsLoaded(game::ActorResource::ObjectContext* objectCtx, s16 bankIndex);  // TODO: Implement
  void ExtendedObject_Clear(game::GlobalContext* globalCtx,
                            game::ActorResource::ObjectContext* objectCtx);  // TODO: Implement
  void* ExtendedObject_GetCMABByIndex(s16 objectId, u32 objectAnimIdx);      // TODO: Implement
}  // namespace rnd
#endif  //_RND_MODELS_H_