#ifndef _RND_ACTORS_DM_HINA_H_
#define _RND_ACTORS_DM_HINA_H_

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  struct Dm_Hina : public game::act::Actor {
    void* next_fn;
    u8 gap_1fc[60];
    void* skel_anime_model;
    void* multi_effect;
    int gap_240;
  };
  void Dm_Hina_Init(game::act::Actor*, game::GlobalContext*);
  extern "C" void Dm_Hina_Draw(game::act::Actor*, game::GlobalContext*);
  // void Dm_Hina_Draw(game::act::Actor*, game::GlobalContext*);
  void Dm_Hina_Destroy(game::act::Actor*, game::GlobalContext*);
}  // namespace rnd
#endif