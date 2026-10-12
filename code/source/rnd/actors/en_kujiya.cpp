#include "rnd/actors/en_kujiya.h"

namespace rnd {
  ItemOverride_Key EnKujiya_GetItemKeyBasedOnDay(s16 getItemId) {
    ItemOverride_Key key = {.all = 0};
    s16 currentDay = game::GetCurrentDay();
    if (currentDay >= 1 && currentDay <= 3) {
      if (currentDay == 1 && gExtSaveData.miniGameCompletion.dayOneLotteryCompleted == 1)
        return key;  // Already rewarded: vanilla prize.
      else if (currentDay == 2 && gExtSaveData.miniGameCompletion.dayTwoLotteryCompleted == 1)
        return key;  // Already rewarded: vanilla prize.
      else if (currentDay == 3 && gExtSaveData.miniGameCompletion.dayThreeLotteryCompleted == 1)
        return key;  // Already rewarded: vanilla prize.
      key.scene = (u8)game::SceneId::LotteryShop;
      key.type = ItemOverride_Type::OVR_MINI_GAME;
      key.flag = (u8)currentDay;
    }
    return key;
  }

  extern "C" void EnKujiya_SetLotteryRevealed() {
    s16 currentDay = game::GetCurrentDay();
    switch (currentDay) {
    case 1:
      gExtSaveData.miniGameCompletion.dayOneLotteryRevealed = 1;
      break;
    case 2:
      gExtSaveData.miniGameCompletion.dayTwoLotteryRevealed = 1;
      break;
    case 3:
      gExtSaveData.miniGameCompletion.dayThreeLotteryRevealed = 1;
      break;
    default:
      break;
    }
    return;
  }
}  // namespace rnd
