#ifndef _RND_OBJECTS_H_
#define _RND_OBJECTS_H_
#define noop (void)0
#include "common/advanced_context.h"
#include "game/actor.h"
#include "game/actorresource.h"
#include "game/common_data.h"
#include "z3d/z3DVec.h"

namespace rnd {
  // Extended Object to deal with spawning multiple actors without running out of standard game space.
  typedef game::ActorResource::ObjectContext ExtendedObjectContext;
  extern "C" ExtendedObjectContext rExtendedObjectCtx;
  extern "C" s32 storedObjId;

  void TexAnim_Spawn(void*, void*);

  s32 Object_SpawnPersistent(void* objectCtx, s16 objectId);
  s32 Object_GetSlot(void* objectCtx, s16 objectId);
  void Object_Clear(void* objectCtx);
  bool Object_IsLoaded(game::ActorResource::ObjectContext* objectCtx, s16 bankIdx);
  void Object_UpdateBank(game::ActorResource::ObjectContext* objectCtx);
  void* GAR_GetCMBByIndex(game::ObjectBank::ObjectBankArchive* objBankArchive, u32 objectAnimIdx);

  s32 ExtendedObject_Spawn(game::ActorResource::ObjectContext* objectCtx, s16 objectId);
  extern "C" void ExtendedObject_Clear(game::ActorResource::ObjectContext* objectCtx);
  s32 ExtendedObject_GetIndex(game::ActorResource::ObjectContext* objectCtx, s16 objectId);
  s32 ExtendedObject_IsLoaded(game::ActorResource::ObjectContext* objectCtx, s16 bankIndex);
  extern "C" game::ActorResource::ActorResource* ExtendedObject_GetStatus();
  void* ExtendedObject_GetCMABByIndex(s16 objectId, u32 objectAnimIdx);
}  // namespace rnd
#endif  //_RND_OBJECTS_H_