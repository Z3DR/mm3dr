#include "rnd/actors/en_rz.h"
#include "rnd/overrides/override_quests.h"

namespace rnd {
  extern "C" {
  // The Rosa Sisters pay their Heart Piece only while RECEIVED_ROSA_SISTERS_HEART_PIECE is clear, and the game sets
  // that flag for good. Report it as clear unless the check has paid its randomized item and the player still has it,
  // so the sisters take the reward path again when the item was lost. The save's own flag is left alone.
  u8 En_Rz_GetHeartPieceFlagByte() {
    u8 flags = game::GetCommonData().save.week_event_reg_75.raw;
    constexpr u8 heartPieceReceived = 0x80;
    if ((flags & heartPieceReceived) &&
        !ItemOverride_IsQuestClosedAt((u8)game::SceneId::WestClockTown, GetItemID::GI_HEART_PIECE)) {
      flags &= ~heartPieceReceived;
    }
    return flags;
  }
  }
}  // namespace rnd
