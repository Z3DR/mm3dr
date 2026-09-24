#include "rnd/actors/en_elforg.h"

#include "game/message.h"
#include "game/player.h"

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
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3CD9D0)(actor, gctx);
    if (actor->calc_fn == NULL) {
      return;
    }
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

  bool En_Elforg_IsFairyCollectedAndNonRepeatable(ItemOverride* override) {
    if (override->key.type != ItemOverride_Type::OVR_STRAY_FAIRY)
      return false;

    bool collected;
    if (override->key.scene == (u8)game::SceneId::LaundryPool ||
        override->key.scene == (u8)game::SceneId::EastClockTown) {
      collected = gExtSaveData.givenItemChecks.clockTownStrayFairyCollected != 0;
    } else {
      int fairyIdx = En_Elforg_getFairyIndex((game::SceneId) override->key.scene);
      if (fairyIdx == -1)
        return false;
      collected = (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (override->key.flag & 0x1F)) != 0;
    }

    if (!collected)
      return false;
    return ItemOverride_IsItemObtainedOrEmptyBottle(*override);
  }

  void En_Elforg_SetFairyCollected(game::SceneId scene, u32 flag) {
    int fairyIdx = En_Elforg_getFairyIndex(scene);
    if (fairyIdx != -1)
      gExtSaveData.dungeonFairyBitfields[fairyIdx] |= 1 << (flag & 0x1F);
  }

  // Item message opened by a randomized stray fairy, still waiting to be dismissed. 0 when none.
  static u16 sOpenItemTextId = 0;
  static u16 sItemTextShownFrames = 0;
  static constexpr u16 kItemTextTimeoutFrames = 90;

  // Closes that message on A, or once it times out, much as vanilla's counter box fades. Item
  // messages stop in TEXT_STATE_EVENT and wait for their owner to close them: the player's get-item
  // action, or the actor being talked to. A stray fairy has neither, since Link keeps moving while
  // it circles him, so this stands in as the owner.
  void En_Elforg_UpdateItemText(game::GlobalContext* gctx) {
    if (sOpenItemTextId == 0) {
      return;
    }
    game::ui::MessageWindow* window = game::MessageMgr::Instance().message_window;
    const auto talkStatus = util::GetPointer<u32(game::MessageContext*)>(0x1C5018)(&gctx->msg_context);
    if (talkStatus == 0 || u16(window->msgid_2) != sOpenItemTextId) {
      sOpenItemTextId = 0;
      return;
    }
    constexpr u32 kTextStateEvent = 5;
    if (talkStatus != kTextStateEvent) {
      return;
    }
    game::act::Player* player = gctx->GetPlayerActor();
    bool dismissed = false;
    if (player != nullptr) {
      const rnd::Flags<game::pad::Button>& buttons = player->controller_info.state->input.new_buttons;
      dismissed = buttons.IsSet(game::pad::Button::A) || buttons.IsSet(game::pad::Button::B);
    }
    if (!dismissed && ++sItemTextShownFrames < kItemTextTimeoutFrames) {
      return;
    }
    if (dismissed) {
      game::PlayMessagePassSound();
    }
    util::GetPointer<void(game::ui::MessageWindow*, int)>(0x1D1A18)(window, 1);
    sOpenItemTextId = 0;
  }

  extern "C" {
  // Replaces CirclePlayer's stray fairy counter (message 0x11) for randomized fairies.
  void En_Elforg_ShowItemMessage(En_Elforg* self, game::GlobalContext* gctx) {
    const u16 textId = ItemOverride_GetStrayFairyMessageId(self);
    ItemOverride_RemoveTextId();
    gctx->ShowMessage(textId);
    // Track the ID actually opened, which is not always the one asked for: the text open routine
    // swaps a Piece of Heart's 0x0C for 0xC5-0xC7 by heart piece count (see
    // En_Elforg_CheckHeartPieceCount), and would otherwise read as another message taking over.
    sOpenItemTextId = gctx->msg_context.current_text_id;
    sItemTextShownFrames = 0;
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

  u16 En_Elforg_CheckHeartPieceCount(u16 origTextId) {
    game::SaveData& save = game::GetCommonData().save;
    if (rActiveItemOverride.key.type != ItemOverride_Type::OVR_STRAY_FAIRY) {
      if (origTextId == 0xC4)
        return 0xC4 + save.inventory.collect_register.heart_container_pieces.Value();
      else
        return origTextId;
    }

    if (rActiveItemRow != NULL) {
      if (save.inventory.collect_register.heart_container_pieces.Value() == 0 &&
          rActiveItemOverride.key.type == ItemOverride_Type::OVR_STRAY_FAIRY)
        return 0xC7;
      else
        return 0xC3 + save.inventory.collect_register.heart_container_pieces.Value();
    }

    return origTextId;
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
    game::ObjectBank::CmbMan* cmbMan = reinterpret_cast<game::ObjectBank::CmbMan*>(
        getCMBManByIndex(&objectEntry->archive, model->itemRow->objectModelIdx, 1));
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