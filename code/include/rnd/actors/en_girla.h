#pragma once

#include "common/advanced_context.h"
#include "game/context.h"
#include "rnd/custom_models.h"
#include "rnd/item_override.h"
#include "rnd/item_table.h"
#include "rnd/models.h"
#include "rnd/objects.h"
#include "rnd/settings.h"
#include "rnd/shops.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif

namespace rnd {
  class En_GirlA;

  using BuyFunc = void(game::GlobalContext* gctx, En_GirlA* actor);
  using CanBuyFunc = s32(game::GlobalContext* gctx);
  using ActorFrameFunc = void(En_GirlA* actor, game::GlobalContext* gctx);

  struct En_GirlA : public game::act::Actor {
    u8 gap_1f8[64];
    ActorFrameFunc* field_238;
    s32 objectId;
    void* next_fn;
    s32 field_244;
    s16 choice_text_id;
    u8 gap_24a[2];
    rnd::GetItemID get_item_id;
    int field_250;
    void* field_254;
    void* field_258;
    s16 field_25c;
    s16 field_25e;
    s16 y_rot;
    u8 gap_262[2];
    CanBuyFunc* can_buy_function;
    BuyFunc* buy_function;
    void* buy_fanfare_function;
    s16 field_270;
    s16 item_count;
    s16 object_table_index;
    u16 field_276;
    game::act::SkeletonAnimationModel* skelAnimeModel;
    u8 gap_27c[8];
  };
  static_assert(sizeof(En_GirlA) == 0x284);
  static_assert(offsetof(En_GirlA, field_244) == 0x244);
  static_assert(offsetof(En_GirlA, buy_function) == 0x268);
  static_assert(offsetof(En_GirlA, field_276) == 0x276);

  void EnGirlA_Init(game::act::Actor*, game::GlobalContext*);
  void EnGirlA_Draw(game::act::Actor*, game::GlobalContext*);
  void EnGirlA_Destroy(game::act::Actor*, game::GlobalContext*);
  
  extern "C" {
  void EnGirlA_Randomize(En_GirlA*, game::GlobalContext*);
  void EnGirlA_BuyOverriddenItem(game::GlobalContext*, En_GirlA*);
  s32 EnGirlA_CanBuyOverriddenItem(game::GlobalContext*);
  s32 EnGirlA_CanBuySoldOut(game::GlobalContext*);
  }

}  // namespace rnd