#include "rnd/overrides/override_quests.h"
#include "rnd/overrides/override_policy.h"
#include "rnd/savefile.h"

namespace rnd {
  static constexpr game::act::Id anyActor = (game::act::Id)0xFFFF;
  static constexpr u8 anyScene = 0xFF;
  static constexpr game::ItemId noGate = game::ItemId::None;

  // One row per quest that pays a randomized item once and then remembers it in a flag.
  //  scene/getItemId: the check's override key, (scene, vanilla get item). This is what pairs the flag with the
  //                   randomized item, so a done quest can be paid again once the item is gone.
  //  actor:           only used by rows that have no key (anyScene/GI_NONE); everything else matches on the key.
  //  gateItem:        the vanilla item the NPC's own "do you own it?" test asks about, if it has one.
  // Rows are matched in order.
  struct QuestCheck {
    game::act::Id actor;
    GetItemID getItemId;
    u8 scene;
    game::ItemId gateItem;
    bool (*isDone)();
    void (*markDone)();
  };

// A flag in the patch's own save data, set when the quest's randomized item is paid.
#define QUEST_FLAG(reg, field)                                                                                         \
  []() -> bool { return gExtSaveData.reg.field != 0; }, []() { gExtSaveData.reg.field = 1; }

  // clang-format off
  static const QuestCheck rQuests[] = {
      // Masks and gifts held by an NPC.
      {anyActor, GetItemID::GI_MASK_KAFEIS,           0x12, game::ItemId::KafeiMask,        QUEST_FLAG(givenItemChecks, enAlGivenItem)},
      {anyActor, GetItemID::GI_MASK_STONE,            0x14, game::ItemId::StoneMask,        QUEST_FLAG(givenItemChecks, enStoneHeishiGivenItem)},
      {anyActor, GetItemID::GI_MASK_BREMEN,           0x70, game::ItemId::BremenMask,       QUEST_FLAG(givenItemChecks, enGuruGuruGivenItem)},
      {anyActor, GetItemID::GI_MASK_KAMARAOS,         0x2D, game::ItemId::KamaroMask,       QUEST_FLAG(givenItemChecks, enYbGivenItem)},
      {anyActor, GetItemID::GI_MASK_BLAST,            0x6E, game::ItemId::BlastMask,        QUEST_FLAG(givenItemChecks, enBabaGivenItem)},
      {anyActor, GetItemID::GI_MASK_ALL_NIGHT,        0x0D, game::ItemId::AllNightMask,     QUEST_FLAG(givenItemChecks, enFsnANMGivenItem)},
      {anyActor, GetItemID::GI_LETTER_TO_MAMA,        0x0D, game::ItemId::LetterToMama,     QUEST_FLAG(givenItemChecks, letterToMamaGiven)},
      {anyActor, GetItemID::GI_MASK_KEATON,           0x0D, game::ItemId::KeatonMask,       QUEST_FLAG(givenItemChecks, enFsnGivenItem)},
      {anyActor, GetItemID::GI_MASK_POSTMANS,         0x6C, game::ItemId::PostmanHat,       QUEST_FLAG(givenItemChecks, enPmGivenItem)},
      {anyActor, GetItemID::GI_MASK_OF_TRUTH,         0x27, game::ItemId::MaskOfTruth,      QUEST_FLAG(givenItemChecks, enSshGivenItem)},
      {anyActor, GetItemID::GI_MASK_OF_SCENTS,        0x52, game::ItemId::MaskOfScents,     QUEST_FLAG(givenItemChecks, enDnoGivenItem)},
      {anyActor, GetItemID::GI_MASK_GREAT_FAIRYS,     0x26, game::ItemId::GreatFairyMask,   QUEST_FLAG(givenItemChecks, bgDyYoseizoGivenItem)},
      {anyActor, GetItemID::GI_MASK_GARO,             0x6A, game::ItemId::GaroMask,         QUEST_FLAG(givenItemChecks, enInGivenItem)},
      {anyActor, GetItemID::GI_BOTTLE_MYSTERY_MILK,   0x6A, noGate,                         QUEST_FLAG(givenItemChecks, enInMysteryMilkGiven)},
      {anyActor, GetItemID::GI_MASK_BUNNY_HOOD,       0x42, game::ItemId::BunnyHood,        QUEST_FLAG(givenItemChecks, enHsGivenItem)},
      {anyActor, GetItemID::GI_MASK_GIBDO,            0x55, game::ItemId::GibdoMask,        QUEST_FLAG(givenItemChecks, enHgoGivenItem)},
      {anyActor, GetItemID::GI_PICTOGRAPH_BOX,        0x57, game::ItemId::PictographBox,    QUEST_FLAG(givenItemChecks, enTruGivenItem)},
      {anyActor, GetItemID::GI_MASK_TROUPE_LEADERS,   0x15, game::ItemId::CircusLeaderMask, QUEST_FLAG(givenItemChecks, enGmGivenItem)},
      {anyActor, GetItemID::GI_MASK_DON_GEROS,        0x50, game::ItemId::DonGeroMask,      QUEST_FLAG(givenItemChecks, enGegGivenItem)},
      {anyActor, GetItemID::GI_MASK_ROMANIS,          0x2D, game::ItemId::RomaniMask,       QUEST_FLAG(givenItemChecks, enMaYtoGivenItem)},
      {anyActor, GetItemID::GI_MASK_FIERCE_DEITY,     0x67, game::ItemId::FierceDeityMask,  QUEST_FLAG(givenItemChecks, enJsGivenItem)},
      // Paid through Song of Healing, which sets these flags itself; the rows are for the gate.
      {anyActor, GetItemID::GI_MASK_ZORA,             0x37, game::ItemId::ZoraMask,         QUEST_FLAG(givenItemChecks, enZogGivenItem)},
      {anyActor, GetItemID::GI_MASK_GORON,            0x4E, game::ItemId::GoronMask,        QUEST_FLAG(givenItemChecks, enGgGivenItem)},
      {anyActor, GetItemID::GI_MAGIC_BEAN,            0x07, noGate, QUEST_FLAG(givenItemChecks, beanDaddyGivenFreeBean)},
      // The Goron's Powder Keg is only a check in Goron Village; the Bomb Shop sells it.
      {anyActor, GetItemID::GI_POWDER_KEG,            0x4D, noGate, QUEST_FLAG(givenItemChecks, enGoGivenItem)},
      {anyActor, GetItemID::GI_POWDER_KEG,            0x48, noGate, QUEST_FLAG(givenItemChecks, enGoGivenItem)},
      // Trade items are paid by whichever scene holds the check, so each scene that can hold one gets its own row.
      {anyActor, GetItemID::GI_MOONS_TEAR,            0x2D, noGate, QUEST_FLAG(givenItemChecks, enObjMoonStoneGivenItem)},
      {anyActor, GetItemID::GI_TOWN_TITLE_DEED,       0x6F, noGate, QUEST_FLAG(givenItemChecks, enTownDeedGivenItem)},
      {anyActor, GetItemID::GI_SWAMP_TITLE_DEED,      0x45, noGate, QUEST_FLAG(givenItemChecks, enSwampDeedGivenItem)},
      {anyActor, GetItemID::GI_SWAMP_TITLE_DEED,      0x00, noGate, QUEST_FLAG(givenItemChecks, enSwampDeedGivenItem)},
      {anyActor, GetItemID::GI_MOUNTAIN_TITLE_DEED,   0x4D, noGate, QUEST_FLAG(givenItemChecks, enMtnDeedGivenItem)},
      {anyActor, GetItemID::GI_MOUNTAIN_TITLE_DEED,   0x48, noGate, QUEST_FLAG(givenItemChecks, enMtnDeedGivenItem)},
      {anyActor, GetItemID::GI_OCEAN_TITLE_DEED,      0x4C, noGate, QUEST_FLAG(givenItemChecks, enOcnDeedGivenItem)},
      {anyActor, GetItemID::GI_ROOM_KEY,              0x61, noGate, QUEST_FLAG(givenItemChecks, roomKeyGiven)},
      {anyActor, GetItemID::GI_LETTER_TO_KAFEI,       0x61, noGate, QUEST_FLAG(givenItemChecks, letterToKafeiGiven)},
      {anyActor, GetItemID::GI_PENDANT_OF_MEMORIES,   0x0D, noGate, QUEST_FLAG(givenItemChecks, pendantGiven)},
      {anyActor, GetItemID::GI_BOTTLE_SEAHORSE_REFILL, 0x3C, noGate, QUEST_FLAG(miscNpcGiven, seahorsePhotoGiven)},
      // The game sets the Rosa Sisters' own flag before it offers the Heart Piece, so it can't tell a first payout from a
      // paid one; this flag is set once the item has been received. See En_Rz_GetHeartPieceFlagByte.
      {anyActor, GetItemID::GI_HEART_PIECE,           0x6D, noGate, QUEST_FLAG(miscNpcGiven, rosaSistersHeartPieceGiven)},
      // Flags without a key of their own: nothing can be paid again from them, they are only remembered.
      {game::act::Id::NpcEnNb,          GetItemID::GI_NONE, anyScene, noGate, QUEST_FLAG(givenItemChecks, enNbGivenItem)},
      {game::act::Id::EnAkindonuts,     GetItemID::GI_BOMB_BAG_40, anyScene, noGate, QUEST_FLAG(givenItemChecks, enAkindonutsBombBagGiven)},
      {game::act::Id::NpcEnBjt,         GetItemID::GI_NONE, anyScene, noGate, QUEST_FLAG(miscNpcGiven, enBjtGivenItem)},
      {game::act::Id::EnOsh,            GetItemID::GI_NONE, anyScene, noGate, QUEST_FLAG(givenItemChecks, enOshGivenItem)},
      {game::act::Id::EnPst,            GetItemID::GI_NONE, anyScene, noGate, QUEST_FLAG(givenItemChecks, enPstGivenItem)},
      {game::act::Id::EnShn,            GetItemID::GI_NONE, anyScene, noGate, QUEST_FLAG(givenItemChecks, enShnGivenItem)},
      {anyActor, GetItemID::GI_MASK_GIANTS,           anyScene, noGate, QUEST_FLAG(givenItemChecks, enBoss02GivenItem)},
      {anyActor, GetItemID::GI_OCARINA_OF_TIME,       anyScene, noGate, QUEST_FLAG(givenItemChecks, ocarinaOfTimeGiven)},
      // A bottle of each kind has been handed out, so later ones only refill (see ItemOverride_GetItem).
      {anyActor, GetItemID::GI_BOTTLE_MILK,           anyScene, noGate, QUEST_FLAG(givenItemChecks, bottleMilkGiven)},
      {anyActor, GetItemID::GI_BOTTLE_GOLD_DUST,      anyScene, noGate, QUEST_FLAG(givenItemChecks, bottleGoldDustGiven)},
      {anyActor, GetItemID::GI_BOTTLE_CHATEAU_ROMANI, anyScene, noGate, QUEST_FLAG(givenItemChecks, bottleChateuGiven)},
      {anyActor, GetItemID::GI_BOTTLE_POTION_RED,     anyScene, noGate, QUEST_FLAG(givenItemChecks, bottleRedPotionGiven)},
  };
  // clang-format on
#undef QUEST_FLAG

  static bool QuestHasKey(const QuestCheck& quest) {
    return quest.scene != anyScene && quest.getItemId != GetItemID::GI_NONE;
  }

  void ItemOverride_MarkQuestDone(game::act::Id actor, GetItemID getItemId, u8 scene) {
    for (const QuestCheck& quest : rQuests) {
      if (quest.actor != anyActor && quest.actor != actor) {
        continue;
      }
      if (quest.getItemId != GetItemID::GI_NONE && quest.getItemId != getItemId) {
        continue;
      }
      if (quest.scene != anyScene && quest.scene != scene) {
        continue;
      }
      quest.markDone();
      return;
    }
  }

  bool ItemOverride_IsQuestDone(const ItemOverride& override) {
    if (override.key.type != ItemOverride_Type::OVR_BASE_ITEM) {
      return false;
    }
    for (const QuestCheck& quest : rQuests) {
      if (QuestHasKey(quest) && quest.scene == override.key.scene && (u8)quest.getItemId == override.key.flag) {
        return quest.isDone();
      }
    }
    return false;
  }

  // 1 when the NPC should treat the quest as done, 0 when it should offer the item again.
  static int QuestClosed(const QuestCheck& quest) {
    if (!quest.isDone()) {
      return 0;
    }
    if (!QuestHasKey(quest)) {
      return 1;
    }
    ItemOverride_Key key = {.all = 0};
    key.scene = quest.scene;
    key.type = ItemOverride_Type::OVR_BASE_ITEM;
    key.flag = (u8)quest.getItemId;
    const ItemOverride override = ItemOverride_LookupByKey(key);
    if (override.key.all == 0) {
      return 1;
    }
    // The same rule the get item uses, with no restock: closed unless the player lost the item.
    return ItemOverride_ResolveRepeat(true, override, 0) != RepeatOutcome::GiveOverride ? 1 : 0;
  }

  int ItemOverride_QuestGateClosed(game::ItemId gateItem) {
    if (gateItem == noGate) {
      return -1;
    }
    for (const QuestCheck& quest : rQuests) {
      if (quest.gateItem == gateItem) {
        return QuestClosed(quest);
      }
    }
    return -1;
  }

  bool ItemOverride_IsQuestClosedAt(u8 scene, GetItemID getItemId) {
    for (const QuestCheck& quest : rQuests) {
      if (QuestHasKey(quest) && quest.scene == scene && quest.getItemId == getItemId) {
        return QuestClosed(quest) == 1;
      }
    }
    return false;
  }
}  // namespace rnd
