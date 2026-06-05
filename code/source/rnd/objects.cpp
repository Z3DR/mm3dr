#include "rnd/objects.h"

namespace rnd {
  ExtendedObjectContext rExtendedObjectCtx = {0};
  s32 rStoredObjId = -1;

  // void TexAnim_Spawn(void* model, void* cmabMan) {
  //   return util::GetPointer<void(void*, void*)>(0x609c3c)(model, cmabMan);
  // }

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

  void Object_Free(void* object) {
    u8* allocInstance = (u8*)0x72F7F0;
    return util::GetPointer<void(void*, void*)>(0x1DAF18)(allocInstance, &object);
  }

  void* GAR_GetCMABByIndex(game::ObjectBank::ObjectBankArchive* objBankArchive, u32 objectAnimIdx) {
    return util::GetPointer<void*(void*, u32)>(0x1F28AC)(objBankArchive, objectAnimIdx);
  }

  s32 ExtendedObject_Spawn(game::ActorResource::ObjectContext* objectCtx, s16 objectId) {
    return Object_SpawnPersistent(&rExtendedObjectCtx, objectId) + OBJECT_EXCHANGE_BANK_MAX;
  }

  game::ActorResource::ActorResource* Object_GetEntry(s16 slot) {
    game::GlobalContext* gctx = GetContext().gctx;
    if (slot >= OBJECT_EXCHANGE_BANK_MAX)
      return &rExtendedObjectCtx.status[slot - OBJECT_EXCHANGE_BANK_MAX];
    else if (slot >= 0)
      return &gctx->object_context.status[slot];

    return NULL;
  }

  s32 Object_FindSlotOrSpawn(s16 objectId) {
    game::GlobalContext* gctx = GetContext().gctx;
    s32 objectSlot = Object_GetSlot(&gctx->object_context, objectId);
    if (objectSlot < 0) {
      objectSlot = Object_SpawnPersistent((game::ActorResource::ObjectContext*)&rExtendedObjectCtx, objectId) +
                   OBJECT_EXCHANGE_BANK_MAX;
    }
    return objectSlot;
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

  void* ExtendedObject_GetCMABByIndex(s16 objectId, u32 objectAnimIdx) {
    game::GlobalContext* gctx = GetContext().gctx;
    s16 objectBankIdx = ExtendedObject_GetIndex(&gctx->object_context, objectId);
    void* cmabMan;

    if (objectBankIdx < OBJECT_EXCHANGE_BANK_MAX) {
      cmabMan = GAR_GetCMABByIndex(&gctx->object_context.status[objectBankIdx].archive, objectAnimIdx);
    } else {
      cmabMan = GAR_GetCMABByIndex(&rExtendedObjectCtx.status[objectBankIdx - OBJECT_EXCHANGE_BANK_MAX].archive,
                                   objectAnimIdx);
    }
    return cmabMan;
  }

  void ExtendedObject_ClearNonPersistent() {
    // CitraPrint("ExtendedObject_ClearNonPersistent enter %d", rExtendedObjectCtx.numPersistentEntries);
    for (s32 i = rExtendedObjectCtx.numPersistent; i < OBJECT_EXCHANGE_BANK_MAX; i++) {
      game::ActorResource::ActorResource* entry = &rExtendedObjectCtx.status[i];
      if (entry->object_id > 0) {
        // CitraPrint("deleting object %X", entry->id);
        if (entry->file_size != 0) {
          game::ObjectBank::freeAndCleanup(&entry->archive);
          entry->file_size = 0;
        }
      }
    }
    rExtendedObjectCtx.num = rExtendedObjectCtx.numPersistent;
  }

  extern "C" {
  game::ActorResource::ActorResource* ExtendedObject_GetStatus() {
    s32 i;
    for (i = 0; i < rExtendedObjectCtx.num; ++i) {
      s32 id = rExtendedObjectCtx.status[i].object_id;
      id = (id < 0 ? -id : id);
      if (id == rStoredObjId) {
        return &rExtendedObjectCtx.status[i];
      }
    }
    return NULL;
  }

  void ExtendedObject_Clear(game::ActorResource::ObjectContext* objectCtx) {
    Object_Clear(objectCtx);
    Object_Clear(&rExtendedObjectCtx);
  }

  void ExtendedObject_AfterObjectListCommand() {
    game::GlobalContext* gctx = GetContext().gctx;
    if (gctx->status == game::State::Status::Initialising) {
      Object_FindSlotOrSpawn(static_cast<s16>(ObjectId::OBJECT_CUSTOM_ASSETS));
      Object_FindSlotOrSpawn(3);  // zelda_dangeon_keep (main dungeon object)
      rExtendedObjectCtx.numPersistent = rExtendedObjectCtx.num;
    }
  }

  void ExtendedObject_InvalidateRoomObjects() {
    game::GlobalContext* gctx = GetContext().gctx;
    if (gctx->status == game::State::Status::Running) {  // Loading room
      ExtendedObject_ClearNonPersistent();
      game::KillAllActorsWithMissingObjects(gctx);
      Model_DestroyAll();
    }
  }
  }

}  // namespace rnd