#include "rnd/models.h"
#include "rnd/actors/dm_char03.h"
#include "rnd/actors/dm_char05.h"
#include "rnd/actors/dm_hina.h"
#include "rnd/actors/en_elforg.h"
#include "rnd/actors/en_mag.h"
#include "rnd/actors/en_pm.h"
#include "rnd/actors/en_si.h"
#include "rnd/actors/fish_heart.h"
#include "rnd/actors/item00.h"
#include "rnd/actors/item_b_heart.h"
#include "rnd/actors/obj_moon_stone.h"
#include "rnd/custom_models.h"
#include "rnd/item_table.h"
#define LOADEDMODELS_MAX 16

namespace rnd {
  Model ModelContext[LOADEDMODELS_MAX] = {0};

  game::act::SkeletonAnimationModel* SkeletonAnimationModel_Spawn(game::act::Actor* actor, game::GlobalContext* gctx,
                                                                  s16 objectId, s32 objectModelIndex) {
    return util::GetPointer<game::act::SkeletonAnimationModel*(
        game::act::Actor * actor, game::GlobalContext * globalCtx, s16 objId, s32 objModelIdx)>(0x203C40)(
        actor, gctx, objectId, objectModelIndex);
  }

  void Actor_SetModelMatrix(float x, float y, float z, z3d_nn_math_MTX34* mtx, game::act::ActorShape* shape) {
    util::GetPointer<void(float x, float y, float z, z3d_nn_math_MTX34* mtx, game::act::ActorShape* shape)>(0x1F0948)(
        x, y, z, mtx, shape);
  }

  void SkeletonAnimationModel_CopyMtx(z3d_nn_math_MTX34* dst, z3d_nn_math_MTX34* src) {
    util::GetPointer<void(z3d_nn_math_MTX34*, z3d_nn_math_MTX34*)>(0x1FEAB0)(dst, src);
  }

  void TexAnim_Spawn(game::act::SA_TextureAnimation* texAnim, void* cmab) {
    // This pointer was extracted at runtime from the Skulltula Token init function (En_Si::init),
    // but it comes from a virtual method, so it might not work in all cases.
    util::GetPointer<void(void*, void*)>(0x229DA4)(texAnim, cmab);
  }

  void SkeletonAnimationModel_Destroy(void* skelModel) {
    void* expHeap = getExpHeapPtr();
    util::GetPointer<void(void*, void*)>(0x1EE6C4)(expHeap, skelModel);
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

  void SkeletonAnimationModel_ShowMesh(void* model, s32 meshIdx) {
    util::GetPointer<void(void*, s32)>(0x21AC14)(model, meshIdx);
  }

  void SkeletonAnimationModel_HideNextMesh(void* model) {
    util::GetPointer<void(void*)>(0x21AC24)(model);
  }

  [[maybe_unused]] s32 SkeletonAnimationModel_GetMeshCount(void* model) {
    return util::GetPointer<s32(void*)>(0x21B694)(model);
  }

  static bool Model_IsStrayFairyGraphic(DrawGraphicItemID graphicId) {
    return graphicId >= DrawGraphicItemID::DI_STRAY_FAIRY_CLOCK_TOWN &&
           graphicId <= DrawGraphicItemID::DI_STRAY_FAIRY_STONE_TOWER;
  }

  // Shared spin for freestanding fairy models, advanced once per frame.
  static u16 sFairyItemSpinYaw = 0;

  void Model_SetScale(game::act::Actor* actor, float scale) {
    util::GetPointer<void(game::act::Actor*, float)>(0x21E30C)(actor, scale);
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

  void Model_MultiplyMatrix(z3d_nn_math_MTX34* dst, z3d_nn_math_MTX34* lhs, z3d_nn_math_MTX44* rhs) {
    util::GetPointer<void(z3d_nn_math_MTX34*, z3d_nn_math_MTX34*, z3d_nn_math_MTX44*)>(0x21B850)(dst, lhs, rhs);
  }

  void Model_GetObjectBankIndex(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx) {
    s32 objectBankIdx = ExtendedObject_GetIndex(&globalCtx->object_context, model->itemRow->objectId);
    if (objectBankIdx < 0) {
      objectBankIdx = ExtendedObject_Spawn(&globalCtx->object_context, model->itemRow->objectId);
    }
    // rStoredObjId = model->itemRow->objectId;
    model->objectBankIdx = objectBankIdx;
  }

  void Model_SetAnim(game::act::SkeletonAnimationModel* model, s16 objectId, u32 objectAnimIdx) {
    void* cmabMan = ExtendedObject_GetCMABByIndex(objectId, objectAnimIdx);
    TexAnim_Spawn(model->texAnim, cmabMan);
    // model->texAnim->animMode = 1;
  }

  void Matrix_UpdatePosition(void* dst, void* src, void* vec) {
    util::GetPointer<void(void*, void*, void*)>(0x19A360)(dst, src, vec);
  }

  game::SceneId Model_GetActorScene(game::act::Actor* actor, game::GlobalContext* gctx) {
    // Some models will tend to travel around, like Postman and Kafei.
    // Their overrides do not, so we need to change the scene for the
    // key lookup.
    switch (actor->id) {
    case game::act::Id::NpcEnPm:
      return game::SceneId::EastClockTown;
    case game::act::Id::ObjMoonStone:
      return game::SceneId::TerminaField;
    default:
      return gctx->scene;
    }
  }

  void Model_SetMatrix(Model* model) {
    // Init scale matrix
    z3d_nn_math_MTX44 scaleMtx = {0};
    f32 scale = model->scale;
    switch (model->actor->id) {  // Change model scale when overriding special actors
    case game::act::Id::DmChar03:
      scale *= 2;
      break;
    case game::act::Id::DmHina:
    case game::act::Id::ObjMoonStone:
    case game::act::Id::DmChar05:
      scale *= 3;
      break;
    case game::act::Id::FishHeart:
      scale *= 25;
      break;
    default:
      break;
    }
    scaleMtx.data[0][0] = scale;
    scaleMtx.data[1][1] = scale;
    scaleMtx.data[2][2] = scale;
    scaleMtx.data[3][3] = 1.0f;

    // Get model coordinates
    z3dVec3f actorPos = model->actor->pos.pos;
    f32 modelPosY = actorPos.y + (model->actor->model_scale.y * model->actor->actor_shape.y_offset);

    if (model->hardcodedMtx != NULL) {
      // Use the hardcoded matrix if present
      SkeletonAnimationModel_CopyMtx(&model->saModel->mtx, model->hardcodedMtx);
    } else if (model->useActorUtil) {
      // Freestanding fairy models slowly rotate in place like classic freestanding items (the
      // whole model spins as one, so head/body/glow always stay consistent with each other).
      f32 tempRotY = model->actor->actor_shape.rot.y;
      model->actor->actor_shape.rot.y = sFairyItemSpinYaw;
      Actor_SetModelMatrix(actorPos.x, modelPosY, actorPos.z, &model->saModel->mtx, &model->actor->actor_shape);
      model->actor->actor_shape.rot.y = tempRotY;
    } else {
      // Otherwise, compute model matrix from actor shape and given position.
      Actor_SetModelMatrix(actorPos.x, modelPosY, actorPos.z, &model->saModel->mtx, &model->actor->actor_shape);
    }

    // Apply scale
    Model_MultiplyMatrix(&model->saModel->mtx, &model->saModel->mtx, &scaleMtx);

    // Repeat for second model if present
    if (model->saModel2 != NULL) {
      if (model->hardcodedMtx != NULL) {
        SkeletonAnimationModel_CopyMtx(&model->saModel2->mtx, model->hardcodedMtx);
        // TODO: how to force facing the camera if the matrix is hardcoded?
      } else {
        f32 tempRotY = model->actor->actor_shape.rot.y;
        // The second model should always face the camera, except for Skull Token
        if (model->itemRow->objectId != 0x0020) {
          model->actor->actor_shape.rot.y = GetContext().gctx->main_camera.cam_dir.y;
        }
        Actor_SetModelMatrix(actorPos.x, modelPosY, actorPos.z, &model->saModel2->mtx, &model->actor->actor_shape);
        model->actor->actor_shape.rot.y = tempRotY;
      }
      Model_MultiplyMatrix(&model->saModel2->mtx, &model->saModel2->mtx, &scaleMtx);
    }
  }

  void Model_Init(Model* model, game::GlobalContext* globalCtx) {
    void* GARbuf = Object_GetEntry(model->objectBankIdx)->archive.archive.raw;
    s16 objectId = model->itemRow->objectId;

    // XXX: If there is ever any issues with models not being able to be edited, start here.
    // However, custom modles should always be loaded, this is an edge case in Ancient Castle
    // of Ikana that causes the Skulltula Token to be somehow loaded already, but the buffer is NULL.
    if (GARbuf != NULL)
      CustomModels_EditItemCMB(GARbuf, objectId, model->itemRow->special);

    model->useActorUtil = 0;
    if (Model_IsStrayFairyGraphic(model->itemRow->graphicId) && En_Elforg_GetItemObjectId() != 0 &&
        model->itemRow->objectId == En_Elforg_GetItemObjectId()) {
      game::ActorResource::ActorResource* objectEntry = Object_GetEntry(model->objectBankIdx);
      if (objectEntry != NULL) {
        En_Elforg_InitItemModel(model, globalCtx, objectEntry);
      }
    }

    if (!model->useActorUtil) {
      model->saModel = SkeletonAnimationModel_Spawn(model->actor, globalCtx, objectId, model->itemRow->objectModelIdx);
    }

    CustomModels_ApplyItemCMAB(model->saModel, model->itemRow->objectId, model->itemRow->special);

    if (!model->useActorUtil && model->itemRow->cmabIndex >= 0) {
      Model_SetAnim(model->saModel, model->itemRow->objectId, model->itemRow->cmabIndex);
    }

    if (!model->useActorUtil) {
      SkeletonAnimationModel_SetMeshByDrawItemID(model->saModel, (s32)model->itemRow->graphicId - 1);
    }

    if (model->itemRow->objectModelIdx2 >= 0) {
      model->saModel2 =
          SkeletonAnimationModel_Spawn(model->actor, globalCtx, objectId, model->itemRow->objectModelIdx2);
      if (model->itemRow->cmabIndex2 >= 0) {
        Model_SetAnim(model->saModel2, model->itemRow->objectId, model->itemRow->cmabIndex2);
      }
    }

    model->loaded = 1;
  }

  void Model_Destroy(Model* model) {
    if (model->useActorUtil) {
      En_Elforg_DestroyItemModel(model);
    }
    if (model->saModel != NULL) {
      SkeletonAnimationModel_Destroy(model->saModel);
      model->saModel = NULL;
    }
    if (model->saModel2 != NULL) {
      SkeletonAnimationModel_Destroy(model->saModel2);
      model->saModel2 = NULL;
    }
    model->actor = NULL;
    model->itemRow = NULL;
    model->loaded = 0;
    model->objectBankIdx = -1;
    model->objectId = -1;
    model->posOffset = {0, -10.00, 0};
  }

  void Model_UpdateAll(game::GlobalContext* globalCtx) {
    Model* model;

    sFairyItemSpinYaw += 0x200;

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

      // Advance the skeletal animation (fairy wing flutter) like vanilla actors do each frame.
      if (model->loaded && model->useActorUtil) {
        En_Elforg_TickItemModel(model);
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
      model->itemRow = ItemTable_GetItemRow(resolvedItemId);
    }
  }

  void Model_InfoLookup(Model* model, game::act::Actor* actor, game::GlobalContext* globalCtx, u16 baseItemId) {
    u16 scene = (u16)Model_GetActorScene(actor, globalCtx);
    ItemOverride override = ItemOverride_Lookup(actor, scene, baseItemId);

    if (override.key.all != 0) {
      if (override.key.type == ItemOverride_Type::OVR_SKULL &&
          En_Si_IsTokenCollectedAndNonRepeatable(actor, globalCtx->scene, &override)) {
        override.value.getItemId = 0x02;
        override.value.looksLikeItemId = 0x02;
      }
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
      newModel->useActorUtil = 0;
      newModel->scale = model->itemRow->scale;
      newModel->objectBankIdx = model->objectBankIdx;
      newModel->baseItemId = model->baseItemId;
      newModel->objectId = model->itemRow->objectId;
      // XXX: Small patch - if we are not the index of a deku nut, then we adjust scale.
      if (newModel->itemRow->objectModelIdx != 0x8D) {
        newModel->scale = 0.3f * newModel->itemRow->scale;
      }
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

  s32 Model_DrawByActor(game::act::Actor* actor, z3d_nn_math_MTX34* hardcodedMtx /*= NULL*/) {
    s32 actorDrawn = 0;
    for (s32 i = 0; i < LOADEDMODELS_MAX; ++i) {
      if (ModelContext[i].actor == actor) {
        actorDrawn = 1;
        ModelContext[i].hardcodedMtx = hardcodedMtx;
        if (actor->id == game::act::Id::NpcEnPm &&
            ModelContext[i].itemRow->itemId == static_cast<u8>(game::ItemId::PostmanHat)) {
          return 0;
        }
        Model_Draw(&ModelContext[i]);
      }
    }
    return actorDrawn;
  }

  void Actor_Init() {
    game::act::ActorOverlayInfo* overlayTable = game::act::GetActorOverlayInfoTable();
    game::ActorResource::ActorResourcePath* resourcePathTable = game::ActorResource::GetActorResourcePathTable();
    // Setup destroy and init functions at this point instead of creating a ton of ASM patches.
    // Use this only if the drwaing for the actor is not complex. Otherwise ASM patches are better.
    overlayTable[0x0E].info->deinit_fn = EnItem00_rDestroy;

    overlayTable[0xDC].info->init_fn = Dm_Hina_Init;
    overlayTable[0xDC].info->deinit_fn = Dm_Hina_Destroy;

    overlayTable[0x2F].info->init_fn = ItemBHeart_Init;
    overlayTable[0x2F].info->draw_fn = ItemBHeart_Draw;
    overlayTable[0x2F].info->deinit_fn = ItemBHeart_Destroy;

    overlayTable[0x99].info->init_fn = En_Si_Init;
    overlayTable[0x99].info->draw_fn = En_Si_Draw;
    overlayTable[0x99].info->deinit_fn = En_Si_Destroy;

    overlayTable[0x12B].info->init_fn = Dm_Char03_Init;
    overlayTable[0x12B].info->deinit_fn = Dm_Char03_Destroy;

    overlayTable[0x12D].info->init_fn = Dm_Char05_Init;
    overlayTable[0x12D].info->deinit_fn = Dm_Char05_Destroy;

    overlayTable[0x145].info->init_fn = En_Elforg_Init;
    overlayTable[0x145].info->draw_fn = En_Elforg_Draw;
    overlayTable[0x145].info->deinit_fn = En_Elforg_Destroy;

    overlayTable[0x166].info->init_fn = En_Pm_Init;
    overlayTable[0x166].info->deinit_fn = En_Pm_Destroy;

    overlayTable[0x16A].info->init_fn = Fish_Heart_Init;
    overlayTable[0x16A].info->deinit_fn = Fish_Heart_Destroy;

    overlayTable[0x212].info->init_fn = Obj_Moon_Stone_Init;
    overlayTable[0x212].info->deinit_fn = Obj_Moon_Stone_Destroy;

    // Define all the small key objects to be by default the same as object 134
    strncpy(resourcePathTable[static_cast<int>(ObjectId::OBJECT_CUSTOM_SMALL_KEY)].path, resourcePathTable[0x86].path,
            0x34);
    strncpy(resourcePathTable[static_cast<int>(ObjectId::OBJECT_CUSTOM_SONGS)].path, resourcePathTable[0xB5].path,
            0x34);
    strncpy(resourcePathTable[static_cast<int>(ObjectId::OBJECT_CUSTOM_ASSETS)].path,
            "rom:/actors/zelda2_custom_data.gar.lzs", 0x34);

    // Point the stray fairy item rows (0xBC-0xBF) at En_Elforg's object and record the object id
    // that gates the fairy item-model path.
    En_Elforg_ConfigureItemRows(overlayTable);
  }
}  // namespace rnd
