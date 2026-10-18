#pragma once

#include "rnd/item_override.h"

// Shared between the files in source/rnd/overrides/. Everything else should go through rnd/item_override.h.
namespace rnd {
  struct ItemOverride_Source {
    game::act::Id actorId;
    GetItemID getItemId;
  };
  extern ItemOverride_Source gItemOverrideSource;
  // Set once a get item was overridden; the Song of Healing text swap consumes it.
  extern bool gGivenItemOverride;

  // override_lookup.cpp
  void ItemOverride_InitLookup(void);
  void ItemOverride_ClearPendingShop(void);

  // override_active.cpp
  void ItemOverride_InitActive(void);
  void ItemOverride_Activate(ItemOverride);
  void ItemOverride_Clear(void);
  // Keeps a touch pickup (stray fairy) active while its get item plays out.
  void ItemOverride_HoldTouchPickup(void);
  void ItemOverride_ReleaseTouchPickup(void);
  bool ItemOverride_IsTouchPickupPending(void);

  // override_completion.cpp
  void ItemOverride_AfterItemReceived(void);
  void SetExtData(void);

  // override_gate.cpp
  s16 ItemOverride_CheckNpc(game::act::Id actorId, s16 originalGetItemId, s32 incomingNegative);

  // override_songs.cpp
  bool isGIDSongOrSoHItem(GetItemID);
}  // namespace rnd
