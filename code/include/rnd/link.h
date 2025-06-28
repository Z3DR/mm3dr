#ifndef _RND_LINK_
#define _RND_LINK_

#include <optional>
#include "common/advanced_context.h"
#include "game/common_data.h"
#include "game/pad.h"
#include "game/ui.h"
#include "game/ui/layouts/play_hud.h"
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
  u8 UseFDAnywhere();
  game::act::Player::Form FierceDeityArcheryFix(game::act::Player::Form);
  }
  
}  // namespace rnd::link

#endif