#include "rnd/actors/en_zot.h"

namespace rnd {
  namespace {
    constexpr u16 rewardTextiD = 0x126F;  // "Well, here's 100 Rupees..."
    constexpr u16 doneTextID = 0x1270;    // "You get 90 Rupees!"
    constexpr u16 customRewardTextID = 0x614B;
    constexpr u16 customRewardDoneTextID = 0x614C;

    ItemOverride EnZot_LookupJarGameReward() {
      ItemOverride_Key key = {.all = 0};
      key.scene = (u8)game::SceneId::ZoraCape;
      key.type = ItemOverride_Type::OVR_MINI_GAME;
      key.flag = 1;
      return ItemOverride_LookupByKey(key);
    }
  }  // namespace

  extern "C" {
  // The shuffled reward replaces the rupees only for the first win as Zora with all five jars; other forms reach the
  // same reward text, so the form is checked here too.
  bool EnZot_JarGameOverrideActive() {
    return !gExtSaveData.miniGameCompletion.zoraJarGameCompleted &&
           game::GetCommonData().save.player_form == game::act::Player::Form::Zora &&
           EnZot_LookupJarGameReward().key.all != 0;
  }

  bool EnZot_GiveJarGameReward() {
    if (!EnZot_JarGameOverrideActive())
      return false;
    ItemOverride_PushPendingOverride(EnZot_LookupJarGameReward());
    return true;
  }

  // The reward texts become custom text while the shuffled reward is being given.
  u16 EnZot_JarGameRewardText(u16 textId) {
    if (!EnZot_JarGameOverrideActive())
      return textId;
    if (textId == rewardTextiD)
      return customRewardTextID;
    if (textId == doneTextID)
      return customRewardDoneTextID;
    return textId;
  }

  // The Zora decides what to do next from the ID of the message on screen; read the custom texts as the messages they
  // stand in for.
  u32 EnZot_JarGameVanillaTextId(u32 textId) {
    if (textId == customRewardTextID)
      return rewardTextiD;
    if (textId == customRewardDoneTextID)
      return doneTextID;
    return textId;
  }
  }
}  // namespace rnd
