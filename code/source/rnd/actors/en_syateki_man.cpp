#include "rnd/actors/en_syateki_man.h"

namespace rnd {
  namespace {
    constexpr s16 townPerfectScore = 50;
    constexpr s16 swampPerfectScore = 0x884;

    // Vanilla item ID of the heart piece. The gallery quiver has a patched ID of its own, but that isn't needed here.
    constexpr u8 heartPieceFlag = (u8)GetItemID::GI_HEART_PIECE;

    // Queues the heart piece for after the man's own item has been received. It can't be queued while the man is still
    // handing his out: the pending item would start while the player is mid-get-item.
    bool PushHeartPiece(game::SceneId scene) {
      ItemOverride_Key key = {.all = 0};
      key.scene = (u8)scene;
      key.type = ItemOverride_Type::OVR_BASE_ITEM;
      key.flag = heartPieceFlag;
      ItemOverride override = ItemOverride_LookupByKey(key);
      if (override.key.all == 0)
        return false;
      ItemOverride_PushPendingOverride(override);
      return true;
    }
  }  // namespace

  extern "C" {
  // Vanilla pays the quiver for the first good score and the heart piece only for a later perfect one. A perfect score
  // that pays the quiver pays the heart piece as well; the heart piece is marked received so it can't be paid twice.
  void EnSyatekiMan_TownPerfectBonus(EnSyatekiMan* man) {
    game::SaveData& save = game::GetCommonData().save;
    if (man->score < townPerfectScore ||
        save.week_event_reg_59.WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_QUIVER_UPGRADE ||
        save.week_event_reg_32.WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_HEART_PIECE)
      return;
    if (PushHeartPiece(game::SceneId::ShootingGalleryTown))
      save.week_event_reg_32.WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_HEART_PIECE = 1;
  }

  void EnSyatekiMan_SwampPerfectBonus(EnSyatekiMan* man) {
    game::SaveData& save = game::GetCommonData().save;
    if (man->score < swampPerfectScore ||
        save.week_event_reg_59.WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_QUIVER_UPGRADE ||
        save.week_event_reg_32.WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE)
      return;
    if (PushHeartPiece(game::SceneId::ShootingGallerySwamp))
      save.week_event_reg_32.WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE = 1;
  }
  }
}  // namespace rnd
