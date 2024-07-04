#include "rnd/actors/dm_char05.h"

namespace rnd {
  void DmChar05_SetScale(game::act::Actor* actor) {
    Model* model = Model_GetOverrideSaModel(actor);

    // static_cast<Dm_Char03*>(actor)->skelAnimeModel = model->saModel;
    if (model != NULL) {
      static_cast<Dm_Char05*>(actor)->actor_util.field_34 = model->saModel;
      if (model->objectId == 0x01) {
        auto* scale = util::GetPointer<f32>(0x41D5B0);
        util::Write(scale, 0x00, 0.005f);
      }
    }
  }

  void DMChar05_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6CD8)(actor, gctx);
    if (actor->params == 0x00) // Goron
      Model_SpawnByActor(actor, GetContext().gctx, 0x79);
    else if (actor->params == 0x01) // Zora
      Model_SpawnByActor(actor, GetContext().gctx, 0x7A);
    else if (actor->params == 0x02)  // Gibdo
      Model_SpawnByActor(actor, GetContext().gctx, 0x87);
    else if (actor->params == 0x04)  // Couple's
      Model_SpawnByActor(actor, GetContext().gctx, 0x85);
    else if (actor->params == 0x0D)  // Bomber's Notebook
      Model_SpawnByActor(actor, GetContext().gctx, 0x50);
    DmChar05_SetScale(actor);
  }

  void DMChar05_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model* model = Model_GetOverrideSaModel(actor);
    if (model != NULL) {
      static_cast<Dm_Char05*>(actor)->skelAnimeModel = model->saModel;
      static_cast<Dm_Char05*>(actor)->actor_util.field_34 = model->saModel;
    }
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x41D1C8)(actor, gctx);
  }

  void DMChar05_Destroy(game::act::Actor* self, game::GlobalContext* gctx) {
    if (self->params != 0x03)
      Model_DestroyByActor(self);
    util::GetPointer<void(game::act::Actor*)>(0x3C6F90)(self);
  }

  void DmChar05_Goron_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model_SpawnByActor(actor, rnd::GetContext().gctx, 0x79);
    DmChar05_SetScale(actor);
  }


}  // namespace rnd