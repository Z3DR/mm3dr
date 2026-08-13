#pragma once

#include <optional>
#include "common/advanced_context.h"
#include "game/actorresource.h"
#include "game/common_data.h"
#include "game/objectbankarchive.h"
#include "game/pad.h"
#include "game/ui.h"
#include "game/ui/layouts/play_hud.h"
#include "rnd/objects.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"

namespace rnd::link {
  void FixSpeedIssues();
  void HandleFastOcarina(game::GlobalContext*);
  void HandleFastArrowSwitch(game::act::Player*);
  void FixFreeCameraReset();
  extern "C" {
  bool ShouldUseZoraFastSwim();
  bool SwitchToZoraFastSwim(game::GlobalContext*, game::act::Player*, bool);
  bool CheckIfMagicAcquired();
  void HandleFastTransform();
  u8 ZoraMaskCheck();
  void SongOfTimeSwordPlacement();
  void AssignSwordForHoneyDarling();
  void RemoveSwordFromHoneyDarling();
  void ResetPlayerForm();
  game::ItemId UseFDAnywhere(game::ItemId);
  u8 CheckIfLinkIsFD();
  u8 CheckIfOcarinaIsInInventory();
  game::act::Player::Form FierceDeityArcheryFix(game::act::Player::Form);
  void* Link_GetCustomTunicCMAB(game::ObjectBank::ObjectBankArchive*, u32);
  void** Link_EditAndRetrieveCMB(game::ObjectBank::ObjectBankArchive*, u32);
  }

}  // namespace rnd::link
