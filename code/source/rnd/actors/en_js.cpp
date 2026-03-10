#include "rnd/actors/en_js.h"

namespace rnd {
  extern "C" {
  u16 En_Js_CurrentMasksInInventory() {
    // I can see why the original devs did this, because they did not want to count specific masks (transform + FD)
    u16 count = 0;
    if (game::HasMask(game::ItemId::MaskOfTruth))
      count += 1;
    if (game::HasMask(game::ItemId::KafeiMask))
      count += 1;
    if (game::HasMask(game::ItemId::AllNightMask))
      count += 1;
    if (game::HasMask(game::ItemId::BunnyHood))
      count += 1;
    if (game::HasMask(game::ItemId::KeatonMask))
      count += 1;
    if (game::HasMask(game::ItemId::GaroMask))
      count += 1;
    if (game::HasMask(game::ItemId::RomaniMask))
      count += 1;
    if (game::HasMask(game::ItemId::CircusLeaderMask))
      count += 1;
    if (game::HasMask(game::ItemId::PostmanHat))
      count += 1;
    if (game::HasMask(game::ItemId::CoupleMask))
      count += 1;
    if (game::HasMask(game::ItemId::GreatFairyMask))
      count += 1;
    if (game::HasMask(game::ItemId::GibdoMask))
      count += 1;
    if (game::HasMask(game::ItemId::DonGeroMask))
      count += 1;
    if (game::HasMask(game::ItemId::KamaroMask))
      count += 1;
    if (game::HasMask(game::ItemId::CaptainHat))
      count += 1;
    if (game::HasMask(game::ItemId::StoneMask))
      count += 1;
    if (game::HasMask(game::ItemId::BremenMask))
      count += 1;
    if (game::HasMask(game::ItemId::BlastMask))
      count += 1;
    if (game::HasMask(game::ItemId::MaskOfScents))
      count += 1;
    if (game::HasMask(game::ItemId::GiantMask))
      count += 1;
    return count;
  }

  u16 En_Js_CheckVictoryRequirements() {
    auto& cdata = game::GetCommonData();
    if (cdata.save.player_form != game::act::Player::Form::Human)
      return 0x220B;

    u16 remainsCollected = Settings_CountRemainsCollected();

    if (remainsCollected >= 5) {
      if (rnd::util::GetPointer<u16(int)>(0x2F217C)(0) < 20)
        return 0x21FC;
      else
        return 0x2202;
    }
    return 0x6144;
  }

  void En_Js_CheckTextValues(game::GlobalContext* gctx) {
    int textChoice = util::GetPointer<int(game::GlobalContext*)>(0x1c5018)(gctx);
    // TODO: This technically doesn't softlock, but you need to input another A press in order to talk to the child
    // again.
    if (gctx->msg_context.current_text_id == 0x0000 && textChoice == 0) {
      game::MessageMgr* ptrMgr = util::GetPointer<game::MessageMgr>(0x6B36F8);
      ptrMgr->message_window->status = 0xC;
      return;
    }
    return;
  }
  }
}  // namespace rnd
