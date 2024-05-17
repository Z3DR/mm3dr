#include "rnd/models.h"
#include "rnd/actors/item00.h"
#define LOADEDMODELS_MAX 16

namespace rnd {
  Model ModelContext[LOADEDMODELS_MAX] = {0};
  void* SkeletonAnimationModel_Spawn(game::act::Actor* actor, game::GlobalContext* gctx, s16 objectId,
                                     s32 objectModelIndex) {
    return util::GetPointer<void*(game::act::Actor * actor, game::GlobalContext * globalCtx, s16 objId,
                                  s32 objModelIdx)>(0x203c40)(actor, gctx, objectId, objectModelIndex);
  }

  void SkeletonAnimationModel_CopyMtx(void* fromMtx, void* toMtx) {
    util::GetPointer<void(void*, void*)>(0x1feab0)(fromMtx, toMtx);
  }

  /** Used to set the mesh for rupees and stray fairies.
   * @param drawItemId Value from DrawGraphicItemID enum, but decreased by one for some reason.
   */
  void SkeletonAnimationModel_SetMeshByDrawItemID(void* model, s32 drawItemId) {
    return util::GetPointer<void(void*, s32)>(0x12A3DC)(model, drawItemId);
  }

  void Model_SetScale(game::act::Actor* actor, float scale) {
    util::GetPointer<void(game::act::Actor*, float)>(0x21e30c)(actor, scale);
  }

  void Model_SetMtxAndModel(void* model, void* mtx) {
    util::GetPointer<void(void*, void*)>(0x1feaa8)(model, mtx);
  }

  void SkeletonAnimationModel_Draw(void* model, int unk) {
    util::GetPointer<void(void*, int)>(0x20AAA8)(model, unk);
  }

  void Model_InvertMatrix(void* mtx) {
    // Inverse model if model is upside down.
    util::GetPointer<void(void*, float, int)>(0x22b038)(mtx, 3.14159, 1);
  }

  void Model_InvertMatrixByScale(void* mtx, float scale) {
    // Inverse model if model is upside down.
    util::GetPointer<void(void*, float, int)>(0x22b038)(mtx, scale, 1);
  }

  s32 Object_SpawnPersistent(void* objectCtx, s16 objectId) {
    return util::GetPointer<s32(void*, s16)>(0x4C01CC)(objectCtx, objectId);
  }

  s32 Object_GetSlot(void* objectCtx, s16 objectId) {
    return util::GetPointer<s32(void*, s16)>(0x1F57DC)(objectCtx, objectId);
  }

  void Model_Init(Model* model, game::GlobalContext* globalCtx) {
    // TODO: add the correct objectId and objectModelIdx to each ItemRow.
    // TODO: add extendedObjectContext to avoid messing up the base objectContext.

    // actors_spawn_stuff should be "objectCtx". (((u32)globalCtx) + 0x9438);
    void* objectCtx = (void*)&globalCtx->object_context;
    s16 objectId = model->itemRow->objectId;
    if (Object_GetSlot(objectCtx, objectId) <= -1) {
      Object_SpawnPersistent(objectCtx, objectId);
    }

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: model->itemRow->objectModelIdx is %u\n", __func__, model->itemRow->objectModelIdx);
#endif

    // model->actor->object_id = 0x02;
    // util::GetPointer<void(game::act::Actor*, void*, int)>(0x1f51fc)(model->actor, static_cast<En_Item00*>(model->actor)->skel_anime_model, 0.0);
    model->saModel = SkeletonAnimationModel_Spawn(model->actor, globalCtx, objectId, model->itemRow->objectModelIdx);
    SkeletonAnimationModel_SetMeshByDrawItemID(model->saModel, model->itemRow->graphicId - 1);
    model->loaded = 1;
  }

  void Model_Destroy(Model* model) {
    if (model->saModel != NULL) {
      // TODO: figure out how to properly destroy the model, if it's needed
      // model->saModel->vtbl->destroy(model->saModel);
      model->saModel = NULL;
    }

    model->actor = NULL;
    model->itemRow = NULL;
    model->loaded = 0;
  }

  void Model_UpdateAll(game::GlobalContext* globalCtx) {
    Model* model;

    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      model = &ModelContext[i];

      // No actor, considered unused
      if (model->actor == NULL) {
        continue;
      }

      // Actor has been killed, destroy the model
      if (model->actor->calc_fn == NULL) {
        Model_Destroy(model);
        continue;
      }

      // Actor is alive, model has not been loaded yet
      if ((model->actor != NULL) && (!model->loaded)) {
        Model_Init(model, globalCtx);
      }
    }
  }

  void Model_Draw(Model* model) {
    if (model->loaded) {
      float tmpMtx[3][4] = {0};
      SkeletonAnimationModel_CopyMtx(&tmpMtx, &model->actor->mtx);
      Model_SetScale(model->actor, model->scale);
      if (model->isFlipped) {
        Model_InvertMatrix(&tmpMtx);
      }
      Model_SetMtxAndModel(model->saModel, &tmpMtx);
      SkeletonAnimationModel_Draw(model->saModel, 0);
    }
  }

  void Model_LookupByOverride(Model* model, ItemOverride override) {
    if (override.key.all != 0) {
      // TODO: Edit item table for the graphic IDs.
      u16 itemId = override.value.looksLikeItemId ? override.value.looksLikeItemId : override.value.getItemId;
      u16 resolvedItemId = ItemTable_ResolveUpgrades(itemId);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: Resolved item ID is %u\n", __func__, resolvedItemId);
#endif
      model->itemRow = ItemTable_GetItemRow(resolvedItemId);
    }
  }

  void Model_InfoLookup(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId) {
    ItemOverride override = ItemOverride_Lookup(actor, (u16)globalCtx->scene, baseItemId);

    if (override.key.all != 0) {
      Model_LookupByOverride(model, override);
    }
  }

  void Model_Create(Model* model, game::GlobalContext* globalCtx) {
    Model* newModel = NULL;

    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      if ((ModelContext[i].actor == NULL) && (ModelContext[i].saModel == NULL)) {
        newModel = &ModelContext[i];
        break;
      }
    }

    if (newModel != NULL) {
      newModel->actor = model->actor;
      newModel->itemRow = model->itemRow;
      newModel->loaded = 0;
      newModel->saModel = NULL;
      newModel->saModel2 = NULL;
      // TODO: Change scale to what is in model.
      newModel->scale = model->itemRow->scale;
      newModel->isFlipped = model->itemRow->flipObj;
    }
  }

  void Model_SpawnByActor(game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId) {
    Model model = {0};

    Model_InfoLookup(&model, actor, globalCtx, baseItemId);
    if (model.itemRow != NULL) {
      model.actor = actor;
      Model_Create(&model, globalCtx);
    }
  }

  void Model_DestroyByActor(game::act::Actor* actor) {
    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      if (ModelContext[i].actor == actor) {
        Model_Destroy(&ModelContext[i]);
      }
    }
  }

  void Model_DestroyAll(void) {
    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      Model_Destroy(&ModelContext[i]);
    }
  }

  s32 Model_DrawByActor(game::act::Actor* actor) {
    s32 actorDrawn = 0;

    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      if (ModelContext[i].actor == actor) {
        actorDrawn = 1;
        Model_Draw(&ModelContext[i]);
      }
    }
    return actorDrawn;
  }
}  // namespace rnd
