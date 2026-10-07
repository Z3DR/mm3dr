#pragma once

#include "common/types.h"
#include "common/utils.h"
#include "game/cmb.h"
#include "game/resarchiveheader.h"

namespace game::ObjectBank {
  struct Archive {
    u8* raw;
    ResArchiveHeader* header;
    ResArchiveFileType* types;
    ResArchiveFileInfo* info;
    u32* data_offsets;
  };
  static_assert(sizeof(Archive) == 0x14);

  struct CsabMan {
    u32 field_0;
    u32 field_4;
    u8 field_8;
  };  // size == 0x09?
  static_assert(sizeof(CsabMan) == 0x0C);

  struct CmabMan {
    void* vtable;
    int field_4;
    u8 field_8;
    u8 gap_9[3];
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
  };  // size == 0x09?
  static_assert(sizeof(CmabMan) == 0x1C);

  struct CmbSkeleton {
    cmb::Skeleton* skl;
    cmb::Bone* bones;
    struct CmbMan* owner;
  };
  static_assert(sizeof(CmbSkeleton) == 0x0C);

  struct CmbMaterialEntry {
    cmb::Material* material;
    cmb::Combiner* combiners;  // the CMB's whole combiner table; index it with material->texEnvStagesIndices
    z3dVec4f* bufferColor;     // &material->BufferColor
  };
  static_assert(sizeof(CmbMaterialEntry) == 0x0C);

  // Points into the raw CMB, so material edits show without a re-upload.
  struct CmbMaterialSet {
    cmb::Mats* mats;
    cmb::Material* materials;
    CmbMaterialEntry* entries;
    struct CmbMan* owner;
    u32* workSize;
  };
  static_assert(sizeof(CmbMaterialSet) == 0x14);

  struct CmbLutSet {
    void* luts;
    u8 isAbs[16];  // per LUT index, from the materials' fragment lighting samplers
  };
  static_assert(sizeof(CmbLutSet) == 0x14);

  struct GfxTexture {
    u32 target;  // 0xDE1 (GL_TEXTURE_2D)
    s32 mipCount;
    u32 format;  // cmb::TextureFormatGL low half
    u32 dataType;
    u16 width;
    u16 height;
    u8* srcData;  // inside the raw CMB, which stays loaded
    u32 size;
    u32 glName;
    void* gpuData;  // glGetTexParameteriv(GL_TEXTURE_2D, 0x6790): where the GPU samples from
  };
  static_assert(offsetof(GfxTexture, srcData) == 0x14);
  static_assert(offsetof(GfxTexture, gpuData) == 0x20);
  static_assert(sizeof(GfxTexture) == 0x24);

  struct CmbMan {
    void** vtable;
    cmb::CMB_HEAD* cmb;
    void* vertexBuffer;
    void* indexBuffer;
    void* shapeSet;
    CmbMaterialSet* materialSet;
    void* vertexAttributes;
    CmbLutSet* lutSet;
    GfxTexture** textures;
    CmbSkeleton* skeleton;
    void* qtrsNodes;
    u8 isInitialized;
    u8 archiveType;
    u8 gap_2E[2];
    void* gpuAllocator;
    void* materialStates;
    void* meshStates;
    void* drawLists;
    u32 workSize;
    u8* workBuffer;
    u8* workCursor;
  };
  static_assert(offsetof(CmbMan, cmb) == 0x04);
  static_assert(offsetof(CmbMan, materialSet) == 0x14);
  static_assert(offsetof(CmbMan, textures) == 0x20);
  static_assert(offsetof(CmbMan, skeleton) == 0x24);
  static_assert(offsetof(CmbMan, gpuAllocator) == 0x30);
  static_assert(offsetof(CmbMan, workCursor) == 0x48);
  static_assert(sizeof(CmbMan) == 0x4C);

  struct ObjectBankArchive {
    u32 field_0;
    Archive archive;
    u32 archive_data;
    u32 file_type_indices[16];
    u8 field_5C;
    u8 gap_5D;
    u16 actor_id;
    CmbMan** cmb_files;
    CsabMan** csab_files;
    void** ctxb_files;
    void** ptxb_files;
    CmabMan** cmab_files;
    void** zsi_files;
    void** qdb_files;
    void** faceb_files;
    void** tbd_files;
    void** ccb_files;
    void** linkb_files;
    void** colb_files;
    void** gfb_files;
    void** vwx_files;
    void** other_files;
  };
  static_assert(sizeof(ObjectBankArchive) == 0x9C);

  void init(ObjectBankArchive*, u32, ResArchiveHeader*, int, char);
  void* getCMBManByIndex(ObjectBankArchive*, u32, u32);
  void free(ObjectBankArchive*);
  ObjectBankArchive* freeAndCleanup(ObjectBankArchive*);
}  // namespace game::ObjectBank
