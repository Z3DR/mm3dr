#include "rnd/actors/en_elforg.h"

namespace rnd {
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
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: Fairy index is %u bit index is %#04x fairy type is %#04x and are we set? %u\n", __func__,
                     fairyIdx, bitIndex, fairyType, isFlagSet);
#endif
    if (fairyIdx != -1) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: Fairy type is %u\n", __func__, fairyType);
#endif
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
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        rnd::util::Print("%s: CASE 6, LOOKING AT FLAG SET?\n", __func__);
#endif
        if (isFlagSet) {
          util::GetPointer<void(game::GlobalContext*, int)>(0x4C6D58)(gctx, bitIndex);  // Set Treasure
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
          rnd::util::Print("%s: Set treasure bit, flag was true %u bitIdx is %#04x\n", __func__,
                           (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (bitIndex & 0x1F)), bitIndex);
#endif
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
    // Model_SpawnByActor(actor, gctx, 0xBB);
  }

  extern "C" {
  void En_Elforg_UpdateExtFairyBits(game::act::Actor* actor, game::GlobalContext* gctx) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: SETTTTTTTING\n", __func__);
#endif
    int bitIndex = ((actor->params << 0x10) >> 0x19);
    int fairyIdx = En_Elforg_getFairyIndex(gctx->scene);
    if (fairyIdx != -1) {
      gExtSaveData.dungeonFairyBitfields[fairyIdx] |= 1 << (bitIndex & 0x1F);
    }
    return;
  }
  s32 En_Elforg_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    return Model_DrawByActor(actor, &saModel->mtx);
  }

  bool En_Elforg_Chest_IsFairyObtained(u32 param, game::GlobalContext* gctx) {
    int bitIndex = ((param << 0x10) >> 0x19);
    int fairyIdx = En_Elforg_getFairyIndex(gctx->scene);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: Fair idx %u bitfield is %#08x\n", __func__, fairyIdx,
                     (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (bitIndex & 0x1F)));
#endif
    if (fairyIdx != -1)
      return (gExtSaveData.dungeonFairyBitfields[fairyIdx] & 1 << (bitIndex & 0x1F));
    return false;
  }
  }

  void En_Elforg_Destroy(game::act::Actor* self, game::GlobalContext*) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3CDF20)(self);
  }

}  // namespace rnd