#pragma once

namespace rnd {
  enum class RepeatOutcome : unsigned char {
    GiveOverride,  // pay the randomized item (again)
    Payout,        // the check was already paid and the item is still there: a Blue Rupee (milk for cows)
    Vanilla,       // let the game hand out its own item (a resource the game lets you earn again)
  };

  struct RepeatQuery {
    bool complete;
    bool onceOnly;
    bool owned;
    bool restock;
  };

  constexpr RepeatOutcome ItemOverride_Decide(RepeatQuery q) {
    if (!q.complete) {
      return RepeatOutcome::GiveOverride;
    }
    // A resource the game lets you earn again always stays available, whatever the check paid.
    if (q.restock) {
      return RepeatOutcome::Vanilla;
    }
    if (q.onceOnly || q.owned) {
      return RepeatOutcome::Payout;
    }
    // The item was lost, so the check can be completed again.
    return RepeatOutcome::GiveOverride;
  }
}  // namespace rnd
