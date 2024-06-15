#include "rnd/objects.h"

namespace rnd {
  ExtendedObjectContext rExtendedObjectCtx = {0};
  s32 storedObjId = -1;
  
  s32 Object_SpawnPersistent(void* objectCtx, s16 objectId) {
    return util::GetPointer<s32(void*, s16)>(0x4C01CC)(objectCtx, objectId);
  }

  s32 Object_GetSlot(void* objectCtx, s16 objectId) {
    return util::GetPointer<s32(void*, s16)>(0x1F57DC)(objectCtx, objectId);
  }

  void Object_Clear(void* objectCtx) {
    return util::GetPointer<void(void*)>(0x14E8F4)(objectCtx);
  }

  bool Object_IsLoaded(game::ActorResource::ObjectContext* objectCtx, s16 bankIdx) {
    return (0 < objectCtx->status[bankIdx].object_id);
  }

  void Object_UpdateBank(game::ActorResource::ObjectContext* objectCtx) {
    return util::GetPointer<void(void*)>(0x175A00)(objectCtx);
  }

  void* GAR_GetCMBByIndex(game::ObjectBank::ObjectBankArchive* objBankArchive) {
    return util::GetPointer<void*(void*)>(0x1F28AC)(objBankArchive);
  }

  s32 ExtendedObject_Spawn(game::ActorResource::ObjectContext* objectCtx, s16 objectId) {
    return Object_SpawnPersistent(&rExtendedObjectCtx, objectId) + OBJECT_EXCHANGE_BANK_MAX;
  }

  void ExtendedObject_Clear(game::ActorResource::ObjectContext* objectCtx) {
    Object_Clear(objectCtx);
    Object_Clear(&rExtendedObjectCtx);
  }

  s32 ExtendedObject_GetIndex(game::ActorResource::ObjectContext* objectCtx, s16 objectId) {
    s32 index = Object_GetSlot(objectCtx, objectId);
    if (index < 0) {
      s32 i;
      for (i = 0; i < OBJECT_EXCHANGE_BANK_MAX; ++i) {
        s32 id = rExtendedObjectCtx.status[i].object_id;
        id = (id < 0 ? -id : id);
        if (id == objectId) {
          return i + OBJECT_EXCHANGE_BANK_MAX;
        }
          
      }
    }
    return index;
  }

  s32 ExtendedObject_IsLoaded(game::ActorResource::ObjectContext* objectCtx, s16 bankIndex) {
    if (bankIndex < OBJECT_EXCHANGE_BANK_MAX) {
      return Object_IsLoaded(objectCtx, bankIndex);
    } else
      return (rExtendedObjectCtx.status[bankIndex - OBJECT_EXCHANGE_BANK_MAX].object_id >= 0);
  }

  extern "C" game::ActorResource::ActorResource* ExtendedObject_GetStatus() {
    if (storedObjId <= -1) return NULL;
    s32 i;
    for (i = 0; i < rExtendedObjectCtx.num; ++i) {
      s32 id = rExtendedObjectCtx.status[i].object_id;
      id = (id < 0 ? -id : id);
      if (id == storedObjId) {
        util::GetPointer<void(void*, s16)>(0x1F15B4)(&rExtendedObjectCtx, 0xe0);
        storedObjId = -1;
        return &rExtendedObjectCtx.status[i];
      }
    }
    return NULL;
  }

  // TODO: Implmenet for shopsanity and decompile ActorResources further?
  /*void* ExtendedObject_GetCMABByIndex(s16 objectId, u32 objectAnimIdx) {
    game::GlobalContext* gctx = GetContext().gctx;
    s16 objectBankIdx = ExtendedObject_GetIndex(&gctx->object_context, objectId);
    void* cmabMan;

    if (objectBankIdx < OBJECT_EXCHANGE_BANK_MAX) {
      cmabMan = GAR_GetCMBByIndex(&gctx->object_context.status[objectBankIdx].zarInfo, objectAnimIdx);
    } else {
      cmabMan = GAR_GetCMBByIndex(&rExtendedObjectCtx.status[objectBankIdx - OBJECT_EXCHANGE_BANK_MAX].zarInfo,
                                  objectAnimIdx);
    }

    return cmabMan;
  }*/

}  // namespace rnd