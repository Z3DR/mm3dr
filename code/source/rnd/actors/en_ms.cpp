#include "rnd/actors/en_ms.h"

namespace rnd {
  extern "C" {
    bool En_Ms_CheckBeanExtData() {
      #if defined ENABLE_DEBUG || defined DEBUG_PRINT
      rnd::util::Print("%s: CHECKING BEAN DADDY BEFORE FREE ITEM. %u\n", __func__,
                       gExtSaveData.givenItemChecks.beanDaddyGivenFreeBean.Value());
      
#endif
      return gExtSaveData.givenItemChecks.beanDaddyGivenFreeBean.Value();
    }

    bool En_Ms_CheckBeanInventory() {
      return game::HasItem(game::ItemId::MagicBean);
    }
  }
} // namespace rnd 