#include "rnd/boss.h"
/**
 * @file boss.cpp
 * @author leoetlino (https://github.com/leoetlino/)
 * @brief
 * @date 2021-09-15
 *
 * Brought in from the Project Restoration libraries. Edited to adjust for the randomizer.
 */
namespace rnd {
  
  struct TwinmoldFixState {
    s8 blue_prev_life;
    s8 red_prev_life;
    game::act::BossTwinmold::Status red_prev_status;
    u16 red_prev_hit_counter;
    bool is_hit_counter_sane;
  };

  void FixTwinmold() {
    static std::optional<TwinmoldFixState> state{};
    const game::GlobalContext* gctx = GetContext().gctx;

    auto* red_twinmold =
        gctx->FindActorWithId<game::act::BossTwinmold>(game::act::Id::BossTwinmold, game::act::Type::Boss);
    if (!red_twinmold) {
      state.reset();
      return;
    }

    auto* blue_twinmold = red_twinmold->other_twinmold_actor;

    if (state) {
      // Red Twinmold has 12 HP (after killing their blue friend).
      //
      // Spinning it deals 3-5 damage points based on lin_vel_xxx:
      //     boss->life -= 3 + (5 - 2) * player->lin_vel_xxx;
      // It is possible to deal 5 damage to the boss by spinning the main stick,
      // but that's not obvious at all...
      //
      // If the player spins the main stick (which is not an obvious thing to do...),
      // killing Red Twinmold takes 3 identical cycles.
      // If not, 4 cycles (!) are required.
      //
      // Let's make that less tedious and less boring by reducing the number of required cycles
      // (1 if the player touches the stick, 2 otherwise).
      if (state->red_prev_life > red_twinmold->life) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        util::Print("%s: dealing more damage to Red Twinmold\n", __func__);
#endif
        red_twinmold->life -= 8;
      }

      // Only update the hit counter if it is sane. One way of ensuring that condition is satisfied
      // is to only consider the counter to be sane after the player has hit Twinmold once.
      if (red_twinmold->hit_counter == 8)
        state->is_hit_counter_sane = true;

      // 10 hits are required to stun Red or Blue Twinmold. This would have been acceptable
      // if it weren't for the fact that Red Twinmold regularly burrows back into sand during phase
      // 2 and the hit counter is reset every time that happens. This makes for a confusing
      // experience the first time the player fights Twinmold, as there is nothing in the game that
      // indicates that the hit counter resets every time (and it's still frustrating on subsequent
      // playthroughs).
      //
      // Fix that by restoring the previous hit counter after it's been reset by the game.
      const bool was_reset = red_twinmold->hit_counter == 9 && state->red_prev_hit_counter != 9;
      const bool is_legit_reset = red_twinmold->status == game::act::BossTwinmold::Status::Stunned;
      if (state->is_hit_counter_sane && was_reset && !is_legit_reset) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        util::Print("%s: restoring hit counter (%u)\n", __func__, state->red_prev_hit_counter);
#endif
        
        red_twinmold->hit_counter = state->red_prev_hit_counter;
      }
    } else {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
      util::Print("%s: initialising state\n", __func__);
#endif
      state.emplace();
      state->is_hit_counter_sane = false;
    }

    state->blue_prev_life = blue_twinmold->life;
    state->red_prev_life = red_twinmold->life;
    state->red_prev_status = red_twinmold->status;
    if (state->is_hit_counter_sane)
      state->red_prev_hit_counter = red_twinmold->hit_counter;
  }
  void FixBosses() {
    FixTwinmold();
  }
} // namespace rnd
