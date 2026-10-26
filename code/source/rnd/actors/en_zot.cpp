#include "rnd/actors/en_zot.h"

namespace rnd {
  namespace {
    constexpr u16 rewardTextiD = 0x126F;  // "Well, here's 100 Rupees..."
    constexpr u16 doneTextID = 0x1270;    // "You get 90 Rupees!"
    constexpr u16 customRewardTextID = 0x614B;
    constexpr u16 bandRehearsalParams = 0xFC09;
    constexpr u16 luluFanParams = 0xFC07;
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
  bool EnZot_JarGameOverrideActive() {
    return !gExtSaveData.miniGameCompletion.zoraJarGameCompleted && EnZot_LookupJarGameReward().key.all != 0;
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

  u32 EnZot_JarGameVanillaTextId(u32 textId) {
    if (textId == customRewardTextID)
      return rewardTextiD;
    if (textId == customRewardDoneTextID)
      return doneTextID;
    return textId;
  }
  }

  ItemOverride_Key EnZot_GetZoraHallKey(game::act::Actor* actor, s16 getItemId) {
    ItemOverride_Key key = {.all = 0};
    key.scene = (u8)game::SceneId::ZoraHall;
    key.type = ItemOverride_Type::OVR_MINI_GAME;
    if (actor->params == bandRehearsalParams) {
      if (getItemId == (s16)GetItemID::GI_RUPEE_BLUE && !gExtSaveData.miniGameCompletion.zoraHallRehearsalCompleted)
        key.flag = 1;
    } else if (actor->params == luluFanParams) {
      if (getItemId == (s16)GetItemID::GI_RUPEE_BLUE && !gExtSaveData.miniGameCompletion.zoraHallLuluBadPhotoCompleted)
        key.flag = 2;
      else if (getItemId == (s16)GetItemID::GI_RUPEE_RED &&
               !gExtSaveData.miniGameCompletion.zoraHallLuluGoodPhotoCompleted)
        key.flag = 3;
    }
    if (key.flag == 0)
      key.all = 0;
    return key;
  }
}  // namespace rnd
