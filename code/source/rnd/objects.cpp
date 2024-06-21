#include "rnd/objects.h"

namespace rnd {
  ExtendedObjectContext rExtendedObjectCtx = {0};
  s32 storedObjId = -1;

  void TexAnim_Spawn(void* model, void* cmabMan) {
    return util::GetPointer<void(void*, void*)>(0x609c3c)(model, cmabMan);
  }

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

  void* GAR_GetCMBByIndex(game::ObjectBank::ObjectBankArchive* objBankArchive, u32 objectAnimIdx) {
    return util::GetPointer<void*(void*, u32)>(0x1F28AC)(objBankArchive, objectAnimIdx);
  }

  s32 ExtendedObject_Spawn(game::ActorResource::ObjectContext* objectCtx, s16 objectId) {
    return Object_SpawnPersistent(&rExtendedObjectCtx, objectId) + OBJECT_EXCHANGE_BANK_MAX;
  }

  extern "C" void ExtendedObject_Clear(game::ActorResource::ObjectContext* objectCtx) {
    Object_Clear(objectCtx);
    Object_Clear(&rExtendedObjectCtx);
  }

  s32 ExtendedObject_GetIndex(game::ActorResource::ObjectContext* objectCtx, s16 objectId) {
    // XXX: There seems to be a race condition issue. If we include one or two more function
    // calls, it appears that gold dust and it's bottle will willingly spawn in.
    if (objectId == 0x01E9 || objectId == 0x01E8)
      rnd::util::Print("%s: %#06x\n", __func__, objectId);
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
    if (storedObjId <= -1)
      return NULL;
    s32 i;
    for (i = 0; i < rExtendedObjectCtx.num; ++i) {
      s32 id = rExtendedObjectCtx.status[i].object_id;
      id = (id < 0 ? -id : id);
      if (id == storedObjId) {
        util::GetPointer<void(void*, s16)>(0x1F15B4)(&rExtendedObjectCtx, id);
        return &rExtendedObjectCtx.status[i];
      }
    }
    return NULL;
  }

  void* ExtendedObject_GetCMABByIndex(s16 objectId, u32 objectAnimIdx) {
    game::GlobalContext* gctx = GetContext().gctx;
    s16 objectBankIdx = ExtendedObject_GetIndex(&gctx->object_context, objectId);
    void* cmabMan;

    if (objectBankIdx < OBJECT_EXCHANGE_BANK_MAX) {
      cmabMan = GAR_GetCMBByIndex(&gctx->object_context.status[objectBankIdx].archive, objectAnimIdx);
    } else {
      cmabMan = GAR_GetCMBByIndex(&rExtendedObjectCtx.status[objectBankIdx - OBJECT_EXCHANGE_BANK_MAX].archive,
                                  objectAnimIdx);
    }
    return cmabMan;
  }

}  // namespace rnd