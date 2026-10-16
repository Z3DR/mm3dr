#pragma once

#include "rnd/item_override.h"
#include "rnd/overrides/override_decision.h"

namespace rnd {
  bool ItemOverride_IsOnceOnly(u16 getItemId);
  bool ItemOverride_IsRestockable(s16 vanillaGetItemId);
  bool ItemOverride_OwnsItem(const ItemOverride&);
  bool ItemOverride_IsCheckComplete(game::act::Actor*, const ItemOverride&);
  bool ItemOverride_IsRepeatBlocked(game::act::Actor*, const ItemOverride&);
  RepeatOutcome ItemOverride_ResolveRepeat(bool complete, const ItemOverride&, s16 vanillaGetItemId);
  void ItemOverride_ApplyRepeatPayout(ItemOverride&);
  void ItemOverride_ApplyBottleRules(ItemOverride&);
}  // namespace rnd
