#include "rnd/actors/obj_warpstone.h"

namespace rnd {
  extern "C" void Obj_Warpstone_ExitMoon(game::GlobalContext* gctx, u16 sceneNum) {
    if (sceneNum == 0xC810) {
      game::CommonData& cdata = game::GetCommonData();

      if (game::HasItem(game::ItemId::SongOfTime) || Settings_MetVictoryRequirements())
        return;
      gctx->next_entrance = 0xD8B0;
      cdata.sub13s[0].entrance_index = 0xD8B0;
      cdata.sub1.field_1C = static_cast<int>(game::SceneId::SouthClockTown);
      cdata.sub1.save_entrance = 0xD8B0;
      gctx->field_C529_one_to_clear_input = 0x14;
      return;
    }
  }
}  // namespace rnd