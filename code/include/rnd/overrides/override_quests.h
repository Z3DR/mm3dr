#pragma once

#include "rnd/item_override.h"

namespace rnd {
  void ItemOverride_MarkQuestDone(game::act::Id actor, GetItemID getItemId, u8 scene);
  bool ItemOverride_IsQuestDone(const ItemOverride& override);
  int ItemOverride_QuestGateClosed(game::ItemId gateItem);
  bool ItemOverride_IsQuestClosedAt(u8 scene, GetItemID getItemId);
}  // namespace rnd
