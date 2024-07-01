#include "rnd/actors/dmchar05.h"

namespace rnd {
  extern "C" {
  void DMChar05_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6CD8)(actor, gctx);
    // TODO: DETERMINE WHICH MASK AND WHATNOT FROM PARAMS.
    if (actor->params != 0x03)
      Model_SpawnByActor(actor, GetContext().gctx, 0x79);
  }

  u8 DmChar05_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    void* saModel = Model_GetOverrideSaModel(actor);
    if (saModel != NULL) {
      static_cast<Dm_Char05*>(actor)->skelAnimeModel = saModel;
      // static_cast<Dm_Char05*>(actor)->actor_util->field_34 = saModel;
      return true;
    } else
      return false;
  }

  void DMChar05_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    if (self->params != 0x03)
      Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3C6F90)(self);
  }

  void DmChar05_Goron_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x79);
  }
  
  }

}  // namespace rnd