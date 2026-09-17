#include "common/advanced_context.h"
#include "game/actorresource.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/message.h"
#include "game/sound.h"
#include "game/states/state.h"
#include "game/ui.h"
#include "game/ui/screens/gearscreen.h"
#include "rnd/boss.h"
#include "rnd/custom_entrances.h"
#include "rnd/extdata.h"
#include "rnd/icetrap.h"
#include "rnd/input.h"
#include "rnd/item_override.h"
#include "rnd/link.h"
#include "rnd/models.h"
#include "rnd/objects.h"
#include "rnd/rheap.h"
#include "rnd/savefile.h"
#include "rnd/settings.h"
#include "z3d/z3DVec.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif

namespace rnd {
  // What each StreamPlayer was playing, or was asked to play, while background music is muted.
  // Lets the music come back on unmute without waiting for an area change.
  static u32 sMutedStreamIds[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

  // Called from hook_MuteStreamPlay at 0x23922C. Returns true to suppress the stream, and
  // remembers the request so unmuting can start it.
  static bool StreamPlayBlockedImpl(u32 id, u32 player) {
    if (gExtSaveData.options.muteBackgroundMusic == 0) {
      return false;
    }
    if (player < ARR_SIZE(sMutedStreamIds)) {
      sMutedStreamIds[player] = id;
    }
    return true;
  }

  void Init(Context& context) {
    rHeap_Init();
    ItemOverride_Init();
    Actor_Init();
    link::FixSpeedIssues();
    ForceTempleFlags();
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    util::Print("MM3DR Initialized (" __DATE__ " " __TIME__ ")\n");
    game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_CHAT_ALLERT);
#else
    game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_CLEAR1);
#endif
    context.has_initialised = true;
  }
  extern "C" {
  void* __service_ptr = nullptr;
  char* fake_heap_start;
  char* fake_heap_end;
  extern void (*__init_array_start[])(void) __attribute__((weak));
  extern void (*__init_array_end[])(void) __attribute__((weak));
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
  static bool titlePlayed = false;
#endif
  void calc(game::State* state) {
    Context& context = GetContext();
    context.gctx = nullptr;

    if (!context.has_initialised && state->type == game::StateType::FirstGame) {
      Init(context);
    }

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    if (state->type == game::StateType::FileSelect) {
      if (!titlePlayed) {
        game::sound::ControlStream(game::sound::StreamPlayer::DEFAULT_PLAYER, 1, 1);
        game::sound::PlayStream(game::sound::StreamId::NA_BGM_MUJURA_2, game::sound::StreamPlayer::DEFAULT_PLAYER);
        titlePlayed = true;
      }

      return;
    } else if (state->type != game::StateType::Play)
      return;
#else
    if (state->type != game::StateType::Play)
      return;
#endif

    context.gctx = static_cast<game::GlobalContext*>(state);

    static bool bgmWasMuted = false;
    const bool bgmMuted = gExtSaveData.options.muteBackgroundMusic != 0;
    if (bgmMuted) {
      for (u32 p = 0; p < ARR_SIZE(sMutedStreamIds); p++) {
        const auto player = (game::sound::StreamPlayer)p;
        const auto id = game::sound::GetCurrentStreamId(player);
        if (id != game::sound::StreamId::None) {
          sMutedStreamIds[p] = (u32)id;
          game::sound::ControlStream(player, 0, 0);
        }
      }
    } else if (bgmWasMuted) {
      // Only DEFAULT_PLAYER carries looping background music; the other players hold one-shot
      // fanfares, and replaying a stale one on unmute would be wrong.
      const u32 p = (u32)game::sound::StreamPlayer::DEFAULT_PLAYER;
      if (sMutedStreamIds[p] != 0xFFFFFFFF) {
        game::sound::PlayStream((game::sound::StreamId)sMutedStreamIds[p], game::sound::StreamPlayer::DEFAULT_PLAYER);
      }
      for (u32 i = 0; i < ARR_SIZE(sMutedStreamIds); i++) {
        sMutedStreamIds[i] = 0xFFFFFFFF;
      }
    }
    bgmWasMuted = bgmMuted;

    Input_Update();
    if (context.gctx->GetPlayerActor()) {
      SaveFile_MaintainMagicBeans();
      ItemOverride_Update();
      link::HandleFastOcarina(context.gctx);
      link::HandleFastArrowSwitch(context.gctx->GetPlayerActor());
      link::FixFreeCameraReset();
      Model_UpdateAll(context.gctx);
    }

    return;
  }

  void readPadInput() {
    auto* gctx = GetContext().gctx;
    if (!gctx || gctx->type != game::StateType::Play)
      return;

    const u32 pressedButtons = gctx->pad_state.input.buttons.flags;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    auto& cdata = game::GetCommonData();
    auto& save = cdata.save;
    if (pressedButtons == (u32)game::pad::Button::ZR) {
      rnd::util::Print("%s: cdata.sub1.field_1C %u gExtSaveData.givenItemChecks.enZogGivenItem value %u \n", __func__,
                       cdata.sub1.field_1C, (u8)gExtSaveData.givenItemChecks.enZogGivenItem);
      cdata.sub1.field_1C = static_cast<int>(game::SceneId::SouthClockTown);

    } else if (pressedButtons == (u32)game::pad::Button::ZL) {
      rnd::util::Print("%s: weekeventreg value %u gExtSaveData.givenItemChecks.enZogGivenItem value %u \n", __func__,
                       save.week_event_reg_132.WEEKEVENTREG_132_40, (u8)gExtSaveData.givenItemChecks.enZogGivenItem);
      // save.week_event_reg_55.WEEKEVENTREG_132_40 = 0;
      game::GiveItem(game::ItemId::MysteryMilk);
      /*save.inventory.woodfall_temple_keys = 2;
      save.inventory.snowhead_temple_keys = 5;
      save.inventory.great_bay_temple_keys = 5;
      save.inventory.stone_tower_temple_keys = 5;
      save.inventory.stone_tower_dungeon_items.map = 1;*/
      // save.week_event_reg_01.WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE = 0;
    }
#endif
    if (gSettingsContext.customMaskButton != 0 && pressedButtons == gSettingsContext.customMaskButton) {
      game::ui::OpenScreen(game::ui::ScreenType::Masks);
    } else if (gSettingsContext.customItemButton != 0 && pressedButtons == gSettingsContext.customItemButton) {
      game::ui::OpenScreen(game::ui::ScreenType::Items);
    } else if (gSettingsContext.customNotebookButton != 0 && pressedButtons == gSettingsContext.customNotebookButton) {
      if (game::GetCommonData().save.inventory.collect_register.bombers_notebook != 0)
        game::ui::OpenScreen(game::ui::ScreenType::Schedule);
      else
        game::ui::OpenScreen(game::ui::ScreenType::Items);
    } else if (gSettingsContext.customMapButton != 0 && pressedButtons == gSettingsContext.customMapButton) {
      // Clear map screen type. (Needed because the screen could be in "soaring" mode.)
      util::Write<u8>(game::ui::GetScreen(game::ui::ScreenType::Map), 0x78E, 0);
      game::ui::OpenScreen(game::ui::ScreenType::Map);
      gctx->pad_state.input.buttons.Clear(game::pad::Button::Select);
      gctx->pad_state.input.new_buttons.Clear(game::pad::Button::Select);
    } else if ((gSettingsContext.customIngameSpoilerButton != 4 && pressedButtons == (u32)game::pad::Button::Select) ||
               (gSettingsContext.customIngameSpoilerButton != 8 && pressedButtons == (u32)game::pad::Button::Start)) {
      if (game::GetCommonData().save.inventory.collect_register.bombers_notebook != 0)
        game::ui::OpenScreen(game::ui::ScreenType::Schedule);
      else
        game::ui::OpenScreen(game::ui::ScreenType::Items);
    }
    return;
  }
  void _start(void) {
    // Just in case something needs to be dynamically allocated...
    static char s_fake_heap[0x10000];

    fake_heap_start = &s_fake_heap[0];
    fake_heap_end = &s_fake_heap[sizeof(s_fake_heap)];
    for (size_t i = 0; i < size_t(__init_array_end - __init_array_start); i++) {
      __init_array_start[i]();
    }
  }

  bool StreamPlayBlocked(u32 id, u32 player) {
    return StreamPlayBlockedImpl(id, player);
  }

  void PostActorCalc() {
    FixBosses();
  }
  }

}  // namespace rnd