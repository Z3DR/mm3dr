#include "rnd/actors/fish_heart.h"

namespace rnd {
  void Fish_Heart_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    #if defined ENABLE_DEBUG || defined DEBUG_PRINT
    game::SaveData& saveData = game::GetCommonData().save;
    rnd::util::Print("%s: SPAWNING param is %#04x save data 111 & 2 is %u\n", __func__, actor->params,
                     saveData.anonymous_111 & 2);
#endif
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3F7808)(actor, gctx);
    Model_SpawnByActor(actor, gctx, 0x0C);
  }

  extern "C" s32 Fish_Heart_OverrideModelDraw(game::act::sa_unk_d4* saModel, game::act::Actor* actor) {
    s32 res = Model_DrawByActor(actor, &saModel->mtx);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: DRAWING!!!! Res is %i\n", __func__, res);	
#endif
    
    return res;
  }

  void Fish_Heart_Destroy(game::act::Actor* self, game::GlobalContext*) {
    #if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: DESTROYING\n", __func__);	
    #endif
    Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3F7AB4)(self);
  }

}  // namespace rnd