#include "rnd/extdata.h"
#include "rnd/item_override.h"
#include "rnd/overrides/override_state.h"
#include "rnd/savefile.h"

namespace rnd {
  bool gGivenItemOverride = false;

  GetItemID ItemOverride_MapSongsToGID(game::ItemId incomingItemId) {
    switch (incomingItemId) {
    case game::ItemId::SonataOfAwakening:
      gExtSaveData.givenSongChecks.sonataGiven = 1;
      return GetItemID(0x4B);
    case game::ItemId::GoronLullaby:
      gExtSaveData.givenSongChecks.goronLullabyGiven = 1;
      return GetItemID(0x4D);
    case game::ItemId::NewWaveBossaNova:
      gExtSaveData.givenSongChecks.newWaveBossaNovaGiven = 1;
      return GetItemID(0x4E);
    case game::ItemId::ElegyOfEmptiness:
      gExtSaveData.givenSongChecks.elegyOfEmptinessGiven = 1;
      return GetItemID(0x4F);
    case game::ItemId::OathToOrder:
      gExtSaveData.givenSongChecks.oathToOrderGiven = 1;
      return GetItemID(0x51);
    case game::ItemId::SongOfTime:
      gExtSaveData.givenSongChecks.songOfTimeGiven = 1;
      return GetItemID(0x53);
    case game::ItemId::SongOfHealing:
      gExtSaveData.givenSongChecks.songOfHealingGiven = 1;
      return GetItemID(0x54);
    case game::ItemId::EponaSong:
      gExtSaveData.givenSongChecks.eponasSongGiven = 1;
      return GetItemID(0x6C);
    case game::ItemId::SongOfSoaring:
      gExtSaveData.givenSongChecks.songOfSoaringGiven = 1;
      return GetItemID(0x72);
    case game::ItemId::SongOfStorms:
      gExtSaveData.givenSongChecks.songOfStormsGiven = 1;
      return GetItemID(0x73);
    case game::ItemId::GoronLullabyIntro:
      gExtSaveData.givenSongChecks.goronLullabyIntroGiven = 1;
      return GetItemID(0x74);
    default:
      return GetItemID::GI_NONE;
    }
  }

  bool isGIDSongOrSoHItem(GetItemID incomingGetItemId) {
    switch (incomingGetItemId) {
    case GetItemID::GI_SONATA_OF_AWAKENING:
    case GetItemID::GI_GORON_LULLABY:
    case GetItemID::GI_NEW_WAVE_BOSSA_NOVA:
    case GetItemID::GI_OATH_TO_ORDER:
    case GetItemID::GI_SONG_OF_TIME:
    case GetItemID::GI_EPONAS_SONG:
    case GetItemID::GI_MASK_COUPLES:
    case GetItemID::GI_MASK_GIBDO:
    case GetItemID::GI_SONG_OF_SOARING:
    case GetItemID::GI_SONG_OF_STORMS:
    case GetItemID::GI_GORON_LULLABY_INTRO:
    case GetItemID::GI_MASK_DEKU:
    case GetItemID::GI_MASK_GORON:
    case GetItemID::GI_MASK_ZORA:
    case GetItemID::GI_ELEGY_OF_EMPTINESS:
      return true;
    default:
      return false;
    }
  }

  extern "C" {
  void ItemOverride_GetSoHOrSongItem(game::GlobalContext* gctx, game::act::Actor* fromActor, s16 incomingItemId) {
    game::act::Player* link = gctx->GetPlayerActor();
    // Run only once. Once the get item is assigned, we shouldn't have to worry about running it again.
    // This is mainly prevalent when the item override is in a calc function (Anju & Kafei).
    // Clear the item since we should have already given it.
    // We also need to check the scene as there is some UB when doing this check with Ocarina items
    // in the Goron Lullaby check.
    if (link->get_item_id != 0x00 && gctx->scene != game::SceneId::GoronShrine) {
      ItemOverride_Clear();
      return;
    }

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    rnd::util::Print("%s: Link's getitemid %#04x incoming GID is %#04x\n", __func__, link->get_item_id, incomingItemId);
#endif
    const s16 vanillaItemId = incomingItemId;
    if (incomingItemId == 0x50) {
      fromActor = link;
    } else if ((incomingItemId >= 0x61 && incomingItemId <= 0x6C) || incomingItemId == 0x73) {
      // Additional logic to map songs to getItemIds.
      incomingItemId = (s16)ItemOverride_MapSongsToGID(game::ItemId(incomingItemId));
      fromActor = link;
    }

    ItemOverride_GetItem(gctx, fromActor, link, incomingItemId);

    if (vanillaItemId == 0x7A) {
      gExtSaveData.givenItemChecks.enZogGivenItem = 1;
    } else if (vanillaItemId == 0x79) {
      gExtSaveData.givenItemChecks.enGgGivenItem = 1;
    } else if (vanillaItemId == 0x87) {
      gExtSaveData.givenItemChecks.mummyDaddyGivenItem = 1;
    } else if (vanillaItemId == 0x78) {
      gExtSaveData.givenItemChecks.enOsnGivenMask = 1;
    } else if (vanillaItemId == 0x85) {
      gExtSaveData.givenItemChecks.kafeiGivenItem = 1;
    }
    return;
  }

  void ItemOverride_SwapSoHAndSongGetItemText(game::GlobalContext* gctx, u16 textId, game::act::Actor* fromActor) {
    // Check which text ID is coming in. If it's any mask from Song of Healing, replace it with active item text.
    if (gGivenItemOverride) {
      if (textId == 0x2919 || textId == 0x291A) {
        // Edge case in the DmChar05 cutscene for Couple's mask.
        // We technically receive the item but have to clear some text
        // before we show.
        // Yes this is hacky, we can further adjust as needed during a cleanup of code.
        gctx->ShowMessage(textId);
        return;
      }
      gGivenItemOverride = false;
      if (rStoredTextId) {
        gctx->ShowMessage(rStoredTextId);
        rStoredTextId = 0;
      } else
        gctx->ShowMessage(textId);
    } else {
      gctx->ShowMessage(textId);
    }

    return;
  }

  u8 ItemOverride_ReceivedOcarinaFromSkt() {
    return gExtSaveData.givenItemChecks.ocarinaOfTimeGiven == 1 ? 1 : 0;
  }

  bool ItemOverride_EnJgHasMetGoronChild() {
    const game::SaveData& save = game::GetCommonData().save;
    return save.week_event_reg_24.WEEKEVENTREG_24_80 || save.week_event_reg_22.WEEKEVENTREG_22_04;
  }

  u8 ItemOverride_ReceivedSongOverride(s16 incomingItemId) {
    switch (incomingItemId) {
    case 0x61:
      return gExtSaveData.givenSongChecks.sonataGiven == 1 ? 1 : 0;
    case 0x62:
      return gExtSaveData.givenSongChecks.goronLullabyGiven == 1 ? 1 : 0;
    case 0x63:
      return gExtSaveData.givenSongChecks.newWaveBossaNovaGiven == 1 ? 1 : 0;
    case 0x64:
      return gExtSaveData.givenSongChecks.elegyOfEmptinessGiven == 1 ? 1 : 0;
    case 0x65:
      return gExtSaveData.givenSongChecks.oathToOrderGiven == 1 ? 1 : 0;
    case 0x67:
      return gExtSaveData.givenSongChecks.songOfTimeGiven == 1 ? 1 : 0;
    case 0x68:
      return gExtSaveData.givenSongChecks.songOfHealingGiven == 1 ? 1 : 0;
    case 0x69:
      return gExtSaveData.givenSongChecks.eponasSongGiven == 1 ? 1 : 0;
    case 0x6A:
      return gExtSaveData.givenSongChecks.songOfSoaringGiven == 1 ? 1 : 0;
    case 0x6B:
      return gExtSaveData.givenSongChecks.songOfStormsGiven == 1 ? 1 : 0;
    case 0x73:
      return gExtSaveData.givenSongChecks.goronLullabyIntroGiven == 1 ? 1 : 0;
    default:
      return 0;
    }
  }

  u8 ItemOverride_CheckIfSongOfTimeAwarded(u8 currentItem) {
    game::SceneId scene = GetContext().gctx->scene;
    if (scene == game::SceneId::ClockTowerRooftop && gExtSaveData.givenSongChecks.songOfTimeGiven == 0)
      return 0x4C;
    return currentItem;
  }

  // TODO: Break out functions like these into specific actor files or songsanity files?
  game::OcarinaSong ItemOverride_ChangeEnGkSong() {
    game::GlobalContext* gctx = GetContext().gctx;
    game::OcarinaSong lastPlayedSong = gctx->msg_context.lastPlayedSong;
    if ((lastPlayedSong == game::OcarinaSong::GoronLullaby) && gExtSaveData.givenSongChecks.goronLullabyGiven == 0) {
      gctx->msg_context.lastPlayedSong = game::OcarinaSong::GoronLullablyIntro;
      return game::OcarinaSong::GoronLullablyIntro;
    }
    return lastPlayedSong;
  }

  u8 ItemOverride_CheckIfOcarinaInClocktower(u8 someAnimationIdx) {
    if (someAnimationIdx == 0x2B) {
      game::GlobalContext* gctx = GetContext().gctx;
      if (gctx->scene == game::SceneId::ClockTowerRooftop)
        return someAnimationIdx;
      else
        return 0x5C;  // Arbitrary, nabbed from deku mask.
    } else {
      return someAnimationIdx;
    }
  }
  }
}  // namespace rnd
