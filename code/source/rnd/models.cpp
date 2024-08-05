#include "rnd/models.h"
#include "rnd/actors/dm_char03.h"
#include "rnd/actors/dm_char05.h"
#include "rnd/actors/dm_hina.h"
#include "rnd/actors/en_si.h"
#include "rnd/actors/item00.h"
#include "rnd/actors/item_b_heart.h"
#define LOADEDMODELS_MAX 16

namespace rnd {
  Model ModelContext[LOADEDMODELS_MAX] = {0};

  void* SkeletonAnimationModel_Spawn(game::act::Actor* actor, game::GlobalContext* gctx, s16 objectId,
                                     s32 objectModelIndex) {
    return util::GetPointer<void*(game::act::Actor * actor, game::GlobalContext * globalCtx, s16 objId,
                                  s32 objModelIdx)>(0x203C40)(actor, gctx, objectId, objectModelIndex);
  }

  void SkeletonAnimationModel_CopyMtx(void* fromMtx, void* toMtx) {
    util::GetPointer<void(void*, void*)>(0x1FEAB0)(fromMtx, toMtx);
  }

  /** Used to set the mesh for rupees and stray fairies.
   * @param drawItemId Value from DrawGraphicItemID enum, but decreased by one for some reason.
   */
  void SkeletonAnimationModel_SetMeshByDrawItemID(void* model, s32 drawItemId) {
    return util::GetPointer<void(void*, s32)>(0x12A3DC)(model, drawItemId);
  }

  void SkeletonAnimationModel_Draw(void* model, int unk) {
    util::GetPointer<void(void*, int)>(0x20AAA8)(model, unk);
  }

  void Model_SetScale(game::act::Actor* actor, float scale) {
    util::GetPointer<void(game::act::Actor*, float)>(0x21E30C)(actor, scale);
  }

  void Model_SetMtxAndModel(void* model, void* mtx) {
    util::GetPointer<void(void*, void*)>(0x1FEAA8)(model, mtx);
  }

  void Model_InvertMatrix(void* mtx) {
    // Inverse model if model is upside down.
    util::GetPointer<void(void*, float, int)>(0x22B038)(mtx, 3.14159, 1);
  }

  void Model_InvertMatrixByScale(void* mtx, float scale) {
    // Inverse model if model is upside down.
    util::GetPointer<void(void*, float, int)>(0x22B038)(mtx, scale, 1);
  }

  void Model_UpdateMatrixPosition(void* mtx, void* mtxTwo, void* vec3) {
    util::GetPointer<void(void*, void*, void*)>(0x20BDB4)(mtx, mtxTwo, vec3);
  }

  void Model_MultiplyMatrix(void* mtx, void* mtxTwo, void* scaleMtx) {
    util::GetPointer<void(void*, void*, void*)>(0x21B850)(mtx, mtxTwo, scaleMtx);
  }

  void Model_GetObjectBankIndex(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx) {
    s32 objectBankIdx = ExtendedObject_GetIndex(&globalCtx->object_context, model->itemRow->objectId);
    if (objectBankIdx < 0) {
      objectBankIdx = ExtendedObject_Spawn(&globalCtx->object_context, model->itemRow->objectId);
    }
    storedObjId = model->itemRow->objectId;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: storedObjId is %#04x\n", __func__, storedObjId);
#endif
    model->objectBankIdx = objectBankIdx;
  }

  void Model_SetAnim(void* model, s16 objectId, u32 objectAnimIdx) {
    // void* cmabMan = ExtendedObject_GetCMABByIndex(objectId, objectAnimIdx);
    // TexAnim_Spawn((model+0x0C), cmabMan);
  }

  void Model_SetMatrix(Model* model) {
    float tmpMtx[3][4] = {0};
    float scaleMtx[4][4] = {0};
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    z3dVec3f tmpPos = {0.0f, 0.0f, 0.0f};
#endif

    SkeletonAnimationModel_CopyMtx(&tmpMtx, &model->actor->mtx);
    // Base case - if we're a free-standing heart piece then set scale and use built-in scaling call.
    if (model->baseItemId == 0x00 && model->itemRow->objectId == 0x01) {
      if (model->itemRow->objectModelIdx == 0x05)
        model->scale = 0.010f;
      else
        model->scale = 0.015f;
      Model_SetScale(model->actor, model->scale);
    } else {
      if (model->baseItemId == 0x00) {
        if (model->itemRow->objectId == 0x0157)
          model->scale = 1.00f;
        else
          model->scale = 10.00f;
      }
      scaleMtx[0][0] = model->scale;
      scaleMtx[1][1] = model->scale;
      scaleMtx[2][2] = model->scale;
      scaleMtx[3][3] = 1.0f;
      Model_MultiplyMatrix(&tmpMtx, &tmpMtx, &scaleMtx);
    }
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    tmpPos.y = yPos;
    tmpPos.x = xPos;
    tmpPos.z = zPos;

    Model_UpdateMatrixPosition(&tmpMtx, &tmpMtx, &tmpPos);
#endif

    if (model->saModel != NULL)
      Model_SetMtxAndModel(model->saModel, &tmpMtx);

    if (model->saModel2 != NULL)
      Model_SetMtxAndModel(model->saModel2, &tmpMtx);
  }

  void Model_Init(Model* model, game::GlobalContext* globalCtx) {
    s16 objectId = model->itemRow->objectId;
    model->saModel = SkeletonAnimationModel_Spawn(model->actor, globalCtx, objectId, model->itemRow->objectModelIdx);
    if (model->itemRow->objectModelIdx2 >= 0) {
      model->saModel2 =
          SkeletonAnimationModel_Spawn(model->actor, globalCtx, objectId, model->itemRow->objectModelIdx2);
      // Model_SetAnim(model->saModel2, model->itemRow->objectId, model->itemRow->cmabIndex2);
    }

    SkeletonAnimationModel_SetMeshByDrawItemID(model->saModel, (s32)model->itemRow->graphicId - 1);
    if (model->itemRow->objectModelIdx != 0xFF) {
      // Model_SetAnim(model->saModel, model->itemRow->objectId, model->itemRow->cmabIndex);
      // model->saModel->unk_0C->animSpeed = 2.0f;
      // model->saModel->unk_0C->animMode = 1;
    }

    model->loaded = 1;
  }

  void Model_Destroy(Model* model) {
    if (model->saModel != NULL) {
      // TODO: figure out how to properly destroy the model, if it's needed
      // model->saModel->vtbl->destroy_function(model->saModel);
      model->saModel = NULL;
    }
    if (model->saModel2 != NULL) {
      model->saModel2 = NULL;
    }
    model->actor = NULL;
    model->itemRow = NULL;
    model->loaded = 0;
    model->objectBankIdx = -1;
    model->objectId = -1;
  }

  void Model_UpdateAll(game::GlobalContext* globalCtx) {
    Model* model;

    Object_UpdateBank(&rExtendedObjectCtx);

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
        if (ExtendedObject_IsLoaded(&globalCtx->object_context, model->objectBankIdx)) {
          Model_Init(model, globalCtx);
        }
      }
    }
  }

  void Model_Draw(Model* model) {
    if (model->loaded) {
      Model_SetMatrix(model);

      if (model->saModel != NULL) {
        SkeletonAnimationModel_Draw(model->saModel, 0);
      }

      if (model->saModel2 != NULL) {
        SkeletonAnimationModel_Draw(model->saModel2, 0);
      }
    }
  }

  void Model_LookupByOverride(Model* model, ItemOverride override) {
    if (override.key.all != 0) {
      u16 resolvedItemId = ItemOverride_SetProgressiveItemDraw(override);
      model->itemRow = ItemTable_GetItemRow(resolvedItemId == 0 ? override.value.getItemId : resolvedItemId);
    }
  }

  void Model_InfoLookup(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId) {
    ItemOverride override = ItemOverride_Lookup(actor, (u16)globalCtx->scene, baseItemId);

    if (override.key.all != 0) {
      Model_LookupByOverride(model, override);
      Model_GetObjectBankIndex(model, actor, globalCtx);
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
      newModel->scale = model->itemRow->scale;
      newModel->objectBankIdx = model->objectBankIdx;
      newModel->baseItemId = model->baseItemId;
      newModel->objectId = model->itemRow->objectId;
    }
  }

  void Model_SpawnByActor(game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId) {
    Model model = {0};

    Model_InfoLookup(&model, actor, globalCtx, baseItemId);
    if (model.itemRow != NULL) {
      model.actor = actor;
      model.baseItemId = baseItemId;
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

  Model* Model_GetOverrideSaModel(game::act::Actor* actor) {
    Model* actorDrawn = NULL;
    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      if (ModelContext[i].actor == actor) {
        actorDrawn = &ModelContext[i];
        break;
      }
    }
    return actorDrawn;
  }

  void Actor_Init() {
    game::act::ActorOverlayInfo* overlayTable = game::act::GetActorOverlayInfoTable();
    // Setup destroy and init functions at this point instead of creating a ton of ASM patches.
    overlayTable[0x0E].info->deinit_fn = EnItem00_rDestroy;

    // overlayTable[0xDC].info->init_fn = Dm_Hina_Init;
    // overlayTable[0xDC].info->draw_fn = Dm_Hina_Draw;
    // overlayTable[0xDC].info->deinit_fn = Dm_Hina_Destroy;

    overlayTable[0x2F].info->init_fn = ItemBHeart_Init;
    overlayTable[0x2F].info->draw_fn = ItemBHeart_Draw;
    overlayTable[0x2F].info->deinit_fn = ItemBHeart_Destroy;

    overlayTable[0x99].info->init_fn = En_Si_Init;
    overlayTable[0x99].info->draw_fn = En_Si_Draw;
    overlayTable[0x99].info->deinit_fn = En_Si_Destroy;

    // overlayTable[0x12B].info->init_fn = Dm_Char03_Init;
    // overlayTable[0x12B].info->draw_fn = Dm_Char03_Draw;
    // overlayTable[0x12B].info->deinit_fn = Dm_Char03_Destroy;

    // overlayTable[0x12D].info->init_fn = DMChar05_Init;
    // overlayTable[0x12D].info->draw_fn = DMChar05_Draw;
    // overlayTable[0x12D].info->deinit_fn = DMChar05_Destroy;
  }
}  // namespace rnd
