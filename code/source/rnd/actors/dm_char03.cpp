#include "rnd/actors/dm_char03.h"

namespace rnd {

  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6A38)(actor, gctx);
    Model_SpawnByActor(actor, GetContext().gctx, 0x78);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: Spawned actor!\n", __func__);
#endif
  }

  void Dm_Char03_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    if (static_cast<Dm_Char03*>(actor)->field_28a != 0) {
      if (!Model_DrawByActor(actor)) {
        util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x41CE70)(actor, gctx);
      }
    } else {
      util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x41CE70)(actor, gctx);
    }
  }

  extern "C" bool Dm_Char03_Draw_Asm(game::act::Actor* actor, game::GlobalContext* gctx) {
    void* saModel = Model_GetOverrideSaModel(actor);
    if (saModel != NULL) {
      static_cast<Dm_Char03*>(actor)->skelAnimeModel = saModel;
      return true;
    } else
      return false;
  }

}  // namespace rnd