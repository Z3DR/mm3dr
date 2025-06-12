#include "rnd/actors/en_mag.h"

namespace rnd {

  void En_Mag_rInit(game::act::Actor* self, game::GlobalContext* gctx) {
    En_Mag* enMag = static_cast<En_Mag*>(self);

    u8 objBankIdx = enMag->object_id;
    void* titleScreenZar = gctx->object_context.status[objBankIdx].archive.archive.raw;
    void* cmabMan;

    s16 exObjectBankIdx = Object_GetSlot(&rExtendedObjectCtx, 182);
    if (exObjectBankIdx < 0) {
      exObjectBankIdx = Object_SpawnPersistent(&rExtendedObjectCtx, 182);
    }

    CustomModel_EditTitleScreenLogo(titleScreenZar);

    util::GetPointer<void(game::act::Actor*, game::GlobalContext*)>(0x32433C)(self, gctx);
    
    cmabMan = GAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].archive, 2);
    TexAnim_Spawn(enMag->skelAnime->texAnim, cmabMan);
    enMag->skelAnime->texAnim->animSpeed = 0.0f;
    enMag->skelAnime->texAnim->animMode = 0;

    // cmabMan = GAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].archive, 1);
    // TexAnim_Spawn(enMag->field_220->texAnim, cmabMan);
    // enMag->field_220->texAnim->animSpeed = 0.0f;
    // enMag->field_220->texAnim->animMode = 0;

    // cmabMan = ZAR_GetCMABByIndex(&rExtendedObjectCtx.status[exObjectBankIdx].zarInfo,
    // TEXANIM_COPY_NINTENDO); TexAnim_Spawn(enmag->copyrightModel->unk_0C, cmabMan);
    // enmag->copyrightModel->unk_0C->animSpeed = 0.0f;
    // enmag->copyrightModel->unk_0C->animMode = 0;
  }
}  // namespace rnd
