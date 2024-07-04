#include "rnd/actors/dm_char03.h"

namespace rnd {

  void Dm_Char03_Init(game::act::Actor* actor, game::GlobalContext* gctx) {
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x3C6A38)(actor, gctx);
    Model_SpawnByActor(actor, GetContext().gctx, 0x78);
    Model* model = Model_GetOverrideSaModel(actor);
    
    // static_cast<Dm_Char03*>(actor)->skelAnimeModel = model->saModel;
    if (model != NULL) {
      static_cast<Dm_Char03*>(actor)->actor_util->field_34 = model->saModel;
      if (model->objectId == 0x01) {
        auto* scale = util::GetPointer<f32>(0x3B9298);
        util::Write(scale, 0x00, 0.005f);
      }
    }
  }

  void Dm_Char03_Draw(game::act::Actor* actor, game::GlobalContext* gctx) {
    Model* model = Model_GetOverrideSaModel(actor);
    static_cast<Dm_Char03*>(actor)->skelAnimeModel = model->saModel;
    static_cast<Dm_Char03*>(actor)->actor_util->field_34 = model->saModel;
    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x41CE70)(actor, gctx);
  }

}  // namespace rnd