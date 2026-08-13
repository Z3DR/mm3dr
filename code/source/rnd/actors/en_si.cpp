#include "rnd/actors/en_si.h"

namespace rnd {
  static u8 skulltulaMapSSH[30] = {0x03, 0x07, 0x0B, 0x13, 0x15, 0x1B, 0x1F, 0x23, 0x25, 0x29,
                                   0x2D, 0x31, 0x37, 0x3B, 0x3F, 0x41, 0x45, 0x49, 0x4F, 0x53,
                                   0x56, 0x5B, 0x5D, 0x62, 0x67, 0x69, 0x6D, 0x71, 0x76, 0x79};
  static u8 skulltulaMapOSH[30] = {0x07, 0x0B, 0x0F, 0x13, 0x17, 0x1B, 0x1F, 0x23, 0x27, 0x2B,
                                   0x2D, 0x33, 0x37, 0x3B, 0x3F, 0x42, 0x46, 0x4A, 0x4F, 0x53,
                                   0x57, 0x59, 0x5D, 0x61, 0x65, 0x69, 0x6D, 0x73, 0x77, 0x7B};

  bool En_Si_IsTokenCollectedAndNonRepeatable(game::act::Actor* actor, game::SceneId scene, ItemOverride* override) {
    if (actor->actor_type != game::act::Type::Item)
      return false;
    for (u8 i = 0; i < 30; i++) {
      if (scene == game::SceneId::SwampSpiderHouse) {
        if (skulltulaMapSSH[i] == (actor->params & 0xFF)) {
          if (gExtSaveData.chestRewarded[(u8)game::SceneId::SwampSpiderHouse][i] == 1)
            return ItemOverride_IsItemObtainedOrEmptyBottle(*override);
          else
            return false;
        }
      } else if (scene == game::SceneId::OceansideSpiderHouse) {
        // Special case - since OSH has one chest with param 00,
        // let's ensure that it only gets the elements after the first item in the array.
        if (skulltulaMapOSH[i] == (actor->params & 0xFF)) {
          if (gExtSaveData.chestRewarded[(u8)game::SceneId::OceansideSpiderHouse][i + 1] == 1)
            return ItemOverride_IsItemObtainedOrEmptyBottle(*override);
          else
            return false;
        }
      }
    }
    return false;
  }

  void En_Si_SetSkullCollected(u16 params, game::SceneId scene, game::act::Type actorType) {
    if (actorType != game::act::Type::Item)
      return;
    for (u8 i = 0; i < 30; i++) {
      if (scene == game::SceneId::SwampSpiderHouse) {
        if (skulltulaMapSSH[i] == (params & 0xFF)) {
          gExtSaveData.chestRewarded[(u8)game::SceneId::SwampSpiderHouse][i] = 1;
          return;
        }
      } else if (scene == game::SceneId::OceansideSpiderHouse) {
        // Special case - since OSH has one chest with param 00,
        // let's ensure that it only sets the second element onwards in the chestRewarded array.
        if (skulltulaMapOSH[i] == (params & 0xFF)) {
          gExtSaveData.chestRewarded[(u8)game::SceneId::OceansideSpiderHouse][i + 1] = 1;
          return;
        }
      }
    }
  }

  extern "C" {
  void En_Si_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x2E7FCC)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0x20);
  }

  void En_Si_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (!Model_DrawByActor(actor)) {
      util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x367C14)(actor, gctx);
    }
  }

  void En_Si_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x51159C)(self);
  }
  }

}  // namespace rnd