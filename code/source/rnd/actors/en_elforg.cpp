#include "rnd/actors/en_elforg.h"

namespace rnd {
  u16 sElforgItemObjectId = 0;
  int En_Elforg_getFairyIndex(game::SceneId scene) {
    switch (scene) {
    case game::SceneId::WoodfallTemple:
      return 0;
      break;
    case game::SceneId::SnowheadTemple:
      return 1;
      break;
    case game::SceneId::GreatBayTemple:
      return 2;
    case game::SceneId::StoneTowerTemple:
    case game::SceneId::StoneTowerTempleInverted:
      return 3;
    default:
      return -1;
    }
  }

  void En_Elforg_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    int fairyType = actor->params & 0xF;
    int bitIndex = ((actor->params << 0x10) >> 0x19);
    int fairyIdx = En_Elforg_getFairyIndex(gctx->scene);
    bool isFlagSet = (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (bitIndex & 0x1F));
    if (fairyIdx != -1) {
      switch (fairyType) {
      default:  // Regular switch flag
        if (isFlagSet)
          util::GetPointer<void(game::GlobalContext*, int)>(0x4C6D70)(gctx, bitIndex);
        break;
      case 1:
      case 2:
      case 3:
      case 8:
        break;
      case 6:  // Treasure flag
        if (isFlagSet) {
          util::GetPointer<void(game::GlobalContext*, int)>(0x4C6D58)(gctx, bitIndex);  // Set Treasure
        }

        break;
      case 7:  // Collectible flag
        if (isFlagSet) {
          util::GetPointer<void(game::GlobalContext*, int)>(0x494BD4)(gctx, bitIndex);  // Set Collectible
        }

        break;
      }
    }

    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3CD9D0)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0xBB);
  }

  void En_Elforg_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (!Model_DrawByActor(actor)) {
      util::GetPointer<void(game::act::Actor*)>(0x424BC0)(actor);
    }
  }

  void En_Elforg_Calc(game::act::Actor* actor, game::GlobalContext* gctx) {
    // Fix for TrappedByEnemy - avoids going down a hard-coded path for drawing
    // once and enemy is destroyed and the fairy spawns.
    if (actor->draw_fn != NULL && actor->draw_fn != En_Elforg_Draw) {
      actor->draw_fn = En_Elforg_Draw;
    }
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x424BF0)(actor, gctx);
  }

  extern "C" {
  void En_Elforg_UpdateExtFairyBits(game::act::Actor* actor, game::GlobalContext* gctx) {
    int bitIndex = ((actor->params << 0x10) >> 0x19);
    int fairyIdx = En_Elforg_getFairyIndex(gctx->scene);
    if (fairyIdx != -1) {
      gExtSaveData.dungeonFairyBitfields[fairyIdx] |= 1 << (bitIndex & 0x1F);
    }
    return;
  }
  s32 En_Elforg_OverrideModelDraw(game::act::SkeletonAnimationModel* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  bool En_Elforg_Chest_IsFairyObtained(u32 param, game::GlobalContext* gctx) {
    int bitIndex = ((param << 0x10) >> 0x19);
    int fairyIdx = En_Elforg_getFairyIndex(gctx->scene);
    if (fairyIdx != -1)
      return (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (bitIndex & 0x1F));
    return false;
  }
  }

  void En_Elforg_Destroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3CDF20)(self);
  }

  void En_Elforg_ConfigureItemRows(game::act::ActorOverlayInfo* overlayTable) {
    sElforgItemObjectId = overlayTable[0x145].info->object_id;
    float fairyScale = *util::GetPointer<float>(0x3CDCBC) / 0.3f;
    // Rows 0xBB (Clock Town) and 0xBC-0xBF (the four temples).
    for (u32 i = 0; i < 5; ++i) {
      ItemRow* row = ItemTable_GetItemRowFromIndex(0xBB + i);
      row->objectId = sElforgItemObjectId;
      row->objectModelIdx = 0x1E;  // fairy CMB index, hardcoded in En_Elforg::Init
      row->cmabIndex = (s8)0xFF;   // colours are applied in En_Elforg_InitItemModel
      row->objectModelIdx2 = (s8)0xFF;
      row->cmabIndex2 = (s8)0xFF;
      row->scale = fairyScale;
    }
  }

  u16 En_Elforg_GetItemObjectId() {
    return sElforgItemObjectId;
  }

  void En_Elforg_InitItemModel(Model* model, game::GlobalContext* gctx,
                               game::ActorResource::ActorResource* objectEntry) {
    game::ObjectBank::CmbMan* cmbMan =
        ObjectArchive_GetCmbManByIndex(&objectEntry->archive, model->itemRow->objectModelIdx);
    if (cmbMan == NULL) {
      return;
    }
    // Fallback to static if the skeleton would not fit our per-slot bone buffer
    u32 boneCount = cmbMan->skeleton->skl->boneCount;
    if (boneCount * sizeof(game::as::BoneMatrix) > sizeof(model->boneMtxBuf)) {
      return;
    }

    game::as::ActorUtil_Construct(&objectEntry->archive, gctx, &model->actorUtil, cmbMan, 0xD, model->boneMtxBuf);
    if (model->actorUtil.field_34 == NULL) {
      game::as::ActorUtil_Destroy(&model->actorUtil);
      return;
    }
    model->saModel = model->actorUtil.field_34;
    model->actorUtil.state.speed = *util::GetPointer<float>(0x3CDCC4);
    model->useActorUtil = 1;

    s32 dungeonIdx = model->itemRow->special;
    if (dungeonIdx >= 0 && dungeonIdx <= 4) {
      const ElforgColorData* colorData = *util::GetPointer<const ElforgColorData*>(0x3CDCD0);
      u16 area;
      if (dungeonIdx < 4) {
        area = colorData->dungeonAreas[dungeonIdx];
      } else {
        bool areaUsed[5] = {false, false, false, false, false};
        for (u32 i = 0; i < 4; ++i) {
          u16 dungeonArea = colorData->dungeonAreas[i];
          if (dungeonArea < 5) {
            areaUsed[dungeonArea] = true;
          }
        }
        area = 0;
        while (area < 4 && areaUsed[area]) {
          ++area;
        }
      }
      u32 cmabIdx = colorData->cmabIndexByArea[area];

      // Wing color - attach the CMAB through the texanim's function table and activate it
      void* colorCmab = GAR_GetCMABByIndex(&objectEntry->archive, cmabIdx);
      game::act::SA_TextureAnimation* texAnim = model->saModel->texAnim;
      if (colorCmab != NULL && texAnim != NULL) {
        texAnim->field_00->attach_cmab(texAnim, colorCmab);
        texAnim->field_00->set_active(texAnim, 1);
      }

      // Body color
      util::GetPointer<void(void*, game::as::ActorUtil*, u16, u16, u16)>(0x609C3C)(
          model->texAnimCtrl, &model->actorUtil, 0xB, 0xFFFF, 0xFFFF);
      util::GetPointer<void(void*, s32, s32)>(0x21B334)(model->texAnimCtrl, 0, area);
      util::GetPointer<void(void*, game::GlobalContext*)>(0x21B2A8)(model->texAnimCtrl, gctx);
    }

    //   mesh 0 = outer glow quad, mesh 1 = body + head + wings (one skinned mesh),
    //   mesh 2 = inner glow quad (NOT covered by GetMeshCount, which reports 2 - hiding "all but
    //   mesh 1" via a count-bound loop left it visible).
    SkeletonAnimationModel_HideNextMesh(model->saModel);  // mesh 0: outer glow
    SkeletonAnimationModel_ShowMesh(model->saModel, 1);   // mesh 1: body/head/wings
    SkeletonAnimationModel_HideNextMesh(model->saModel);  // mesh 2: inner glow
  }

  void En_Elforg_TickItemModel(Model* model) {
    util::GetPointer<void(game::as::ActorUtil*)>(0x4E77B8)(&model->actorUtil);
  }

  void En_Elforg_DestroyItemModel(Model* model) {
    // calling SkeletonAnimationModel_Destroy on saModel as well would double-free.
    game::as::ActorUtil_Destroy(&model->actorUtil);
    model->saModel = NULL;
    model->useActorUtil = 0;
    memset(&model->actorUtil, 0, sizeof(model->actorUtil));
    memset(model->texAnimCtrl, 0, sizeof(model->texAnimCtrl));
    memset(model->boneMtxBuf, 0, sizeof(model->boneMtxBuf));
  }

}  // namespace rnd