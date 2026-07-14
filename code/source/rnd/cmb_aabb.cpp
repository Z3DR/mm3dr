#include "rnd/cmb_aabb.h"
#include "common/utils.h"

namespace rnd {
  namespace {
    using namespace game::cmb;

    constexpr u32 kMaxBones = 32;
    constexpr u32 kMaxBoneCandidates = 8;
    constexpr u32 kMaxCmbSize = 0x800000;

    bool InBounds(u32 cmbSize, u32 offset, u32 len) {
      return offset <= cmbSize && len <= cmbSize - offset;
    }

    void GameMemcpy(void* dst, const void* src, u32 len) {
      util::GetPointer<void*(void*, const void*, u32)>(0x1F28E8)(dst, src, len);
    }

    // 4-byte chunk magic comparison; the game has no discoverable memcmp and this is the only
    // comparison the scanner needs.
    bool MagicMatches(const char* magic, const char* expected) {
      return magic[0] == expected[0] && magic[1] == expected[1] && magic[2] == expected[2] && magic[3] == expected[3];
    }

    u32 DataTypeSize(DataTypes type) {
      switch (type) {
      case DataTypes::Byte:
      case DataTypes::UByte:
        return 1;
      case DataTypes::Short:
      case DataTypes::UShort:
        return 2;
      case DataTypes::Int:
      case DataTypes::UInt:
      case DataTypes::Float:
        return 4;
      default:
        return 0;  // caller treats as malformed
      }
    }

    f32 ReadComponent(const u8* p, DataTypes type) {
      switch (type) {
      case DataTypes::Byte:
        return (f32)(s8)p[0];
      case DataTypes::UByte:
        return (f32)p[0];
      case DataTypes::Short: {
        s16 v;
        GameMemcpy(&v, p, sizeof(v));
        return (f32)v;
      }
      case DataTypes::UShort: {
        u16 v;
        GameMemcpy(&v, p, sizeof(v));
        return (f32)v;
      }
      case DataTypes::Int: {
        s32 v;
        GameMemcpy(&v, p, sizeof(v));
        return (f32)v;
      }
      case DataTypes::UInt: {
        u32 v;
        GameMemcpy(&v, p, sizeof(v));
        return (f32)v;
      }
      default: {  // Float
        f32 v;
        GameMemcpy(&v, p, sizeof(v));
        return v;
      }
      }
    }

    constexpr f32 kRadToBinang = 10430.378f;  // 32768 / pi

    f32 Trig_Sin(f32 rad) {
      return util::GetPointer<f32(s32)>(0x2294A8)((s32)(rad * kRadToBinang));
    }

    f32 Trig_Cos(f32 rad) {
      return util::GetPointer<f32(s32)>(0x229470)((s32)(rad * kRadToBinang));
    }

    // Local T * Rz*Ry*Rx * S from a CMB bone's rest pose (rotation in radians, XYZ Euler).
    void Mtx34_FromBoneTrs(z3d_nn_math_MTX34* m, const Bone& b) {
      f32 sx = Trig_Sin(b.rotation.x), cx = Trig_Cos(b.rotation.x);
      f32 sy = Trig_Sin(b.rotation.y), cy = Trig_Cos(b.rotation.y);
      f32 sz = Trig_Sin(b.rotation.z), cz = Trig_Cos(b.rotation.z);
      m->data[0][0] = b.scale.x * (cy * cz);
      m->data[0][1] = b.scale.y * (sx * sy * cz - cx * sz);
      m->data[0][2] = b.scale.z * (cx * sy * cz + sx * sz);
      m->data[0][3] = b.translation.x;
      m->data[1][0] = b.scale.x * (cy * sz);
      m->data[1][1] = b.scale.y * (sx * sy * sz + cx * cz);
      m->data[1][2] = b.scale.z * (cx * sy * sz - sx * cz);
      m->data[1][3] = b.translation.y;
      m->data[2][0] = b.scale.x * (-sy);
      m->data[2][1] = b.scale.y * (sx * cy);
      m->data[2][2] = b.scale.z * (cx * cy);
      m->data[2][3] = b.translation.z;
    }    

    void Mtx34_Identity(z3d_nn_math_MTX34* m) {
      for (u32 i = 0; i < 3; ++i)
        for (u32 j = 0; j < 4; ++j)
          m->data[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }  // namespace

  bool CmbAabb_Compute(const void* cmbData, z3dVec3f* outMin, z3dVec3f* outMax) {
    const u8* cmb = (const u8*)cmbData;
    if (cmb == NULL)
      return false;

    const CMB_HEAD* head = (const CMB_HEAD*)cmb;
    if (!MagicMatches(head->magic, "cmb "))
      return false;
    u32 cmbSize = head->size;
    if (cmbSize < sizeof(CMB_HEAD) || cmbSize > kMaxCmbSize)
      return false;
    if (head->version < 10)
      return false;

    if (!InBounds(cmbSize, head->vatrOffset, sizeof(CMB_VATR)) ||
        !InBounds(cmbSize, head->sklmOffset, sizeof(CMB_SKLM)))
      return false;
    const CMB_VATR* vatr = (const CMB_VATR*)(cmb + head->vatrOffset);
    if (!MagicMatches(vatr->magic, "vatr") || vatr->position.size == 0)
      return false;
    u32 posSliceOff = head->vatrOffset + vatr->position.offset;
    if (posSliceOff < head->vatrOffset || !InBounds(cmbSize, posSliceOff, vatr->position.size))
      return false;
    const u8* posSlice = cmb + posSliceOff;

    const CMB_SKLM* sklm = (const CMB_SKLM*)(cmb + head->sklmOffset);
    if (!MagicMatches(sklm->magic, "sklm"))
      return false;
    u32 shpOff = head->sklmOffset + sklm->shpOffset;
    if (shpOff < head->sklmOffset || !InBounds(cmbSize, shpOff, sizeof(CMB_SHP)))
      return false;
    const CMB_SHP* shp = (const CMB_SHP*)(cmb + shpOff);
    if (!MagicMatches(shp->magic, "shp "))
      return false;
    u32 sepdCount = shp->sepdCount;
    if (sepdCount == 0 || !InBounds(cmbSize, shpOff + sizeof(CMB_SHP), sepdCount * sizeof(u16)))
      return false;

    // Bind-pose world matrix per bone. Skeletons past the cap fall back to raw vertices.
    z3d_nn_math_MTX34 boneWorld[kMaxBones];
    u32 boneCount = 0;
    if (head->sklOffset != 0 && InBounds(cmbSize, head->sklOffset, sizeof(Skeleton))) {
      const Skeleton* skl = (const Skeleton*)(cmb + head->sklOffset);
      if (MagicMatches(skl->magic, "skl ") && skl->boneCount <= kMaxBones &&
          InBounds(cmbSize, head->sklOffset + sizeof(Skeleton), skl->boneCount * sizeof(Bone))) {
        boneCount = skl->boneCount;
        for (u32 i = 0; i < boneCount; ++i) {
          const Bone& b = skl->bone[i];
          z3d_nn_math_MTX34 local;
          Mtx34_FromBoneTrs(&local, b);
          s32 parent = b.parentIndex;
          if (parent >= 0 && (u32)parent < i)
            Model_MultiplyMatrix34(&boneWorld[i], &boneWorld[parent], &local);
          else
            boneWorld[i] = local;
        }
      }
    }

    bool any = false;
    z3dVec3f mn = {0, 0, 0};
    z3dVec3f mx = {0, 0, 0};
    for (u32 i = 0; i < sepdCount; ++i) {
      u32 sepdOff = shpOff + shp->sepdOffsets[i];
      if (!InBounds(cmbSize, sepdOff, sizeof(SEPD)))
        return false;
      const SEPD* sepd = (const SEPD*)(cmb + sepdOff);
      if (!MagicMatches(sepd->magic, "sepd"))
        return false;
      if (!sepd->HasAttribute(0) || sepd->position.mode != Modes::Array || sepd->prmsCount == 0)
        continue;
      u32 compSize = DataTypeSize(sepd->position.dataType);
      if (compSize == 0)
        return false;
      u32 start = (u32)sepd->position.start;
      if (start >= vatr->position.size)
        continue;

      // Each SEPD's vertices are a contiguous run of the position slice beginning at
      // position.start; the run ends where the next SEPD's run begins (or at the slice end).
      u32 end = vatr->position.size;
      for (u32 j = 0; j < sepdCount; ++j) {
        u32 otherOff = shpOff + shp->sepdOffsets[j];
        if (!InBounds(cmbSize, otherOff, sizeof(SEPD)))
          return false;
        const SEPD* other = (const SEPD*)(cmb + otherOff);
        if (other->HasAttribute(0) && other->position.mode == Modes::Array) {
          u32 otherStart = (u32)other->position.start;
          if (otherStart > start && otherStart < end)
            end = otherStart;
        }
      }

      // Gather the bones this SEPD's prms chunks can bind (single-bone rigid on every retail
      // item model, so this is exact there; multiple bones union conservatively - may lift a
      // touch high, never clips). Smooth-skinned vertices are already stored in model space.
      z3d_nn_math_MTX34 candidates[kMaxBoneCandidates];
      u32 candidateCount = 0;
      bool identityOnly = boneCount == 0;
      if (!identityOnly && InBounds(cmbSize, sepdOff + sizeof(SEPD), sepd->prmsCount * sizeof(u16))) {
        for (u32 k = 0; k < sepd->prmsCount && candidateCount < kMaxBoneCandidates; ++k) {
          u32 prmsOff = sepdOff + sepd->prmsOffsets[k];
          if (!InBounds(cmbSize, prmsOff, sizeof(PRMS) + sizeof(PrmSet)))
            return false;
          const PRMS* prms = (const PRMS*)(cmb + prmsOff);
          if (!MagicMatches(prms->magic, "prms"))
            return false;
          const PrmSet* set = prms->PRMs;
          if (set->skinningMode == SkinningModes::Smooth) {
            identityOnly = true;  // model-space vertices
            break;
          }
          u32 tableOff = prmsOff + (u32)set->boneTableOffset;
          if (!InBounds(cmbSize, tableOff, set->boneTableCount * sizeof(u16)))
            return false;
          const u16* table = (const u16*)(cmb + tableOff);
          for (u32 t = 0; t < set->boneTableCount && candidateCount < kMaxBoneCandidates; ++t) {
            u32 boneId = table[t] & 0xFFF;
            if (boneId < boneCount)
              candidates[candidateCount++] = boneWorld[boneId];
          }
        }
      }
      if (identityOnly || candidateCount == 0) {
        Mtx34_Identity(&candidates[0]);
        candidateCount = 1;
      }

      u32 stride = compSize * 3;
      f32 scale = sepd->position.scale;
      for (u32 v = start; v + stride <= end; v += stride) {
        f32 x = ReadComponent(posSlice + v, sepd->position.dataType) * scale;
        f32 y = ReadComponent(posSlice + v + compSize, sepd->position.dataType) * scale;
        f32 z = ReadComponent(posSlice + v + compSize * 2, sepd->position.dataType) * scale;
        for (u32 c = 0; c < candidateCount; ++c) {
          z3dVec3f p = Model_Matrix34_MulVec(&candidates[c], x, y, z);
          if (!any) {
            mn = p;
            mx = p;
            any = true;
          } else {
            mn.x = p.x < mn.x ? p.x : mn.x;
            mn.y = p.y < mn.y ? p.y : mn.y;
            mn.z = p.z < mn.z ? p.z : mn.z;
            mx.x = p.x > mx.x ? p.x : mx.x;
            mx.y = p.y > mx.y ? p.y : mx.y;
            mx.z = p.z > mx.z ? p.z : mx.z;
          }
        }
      }
    }

    if (any) {
      *outMin = mn;
      *outMax = mx;
    }
    return any;
  }
}  // namespace rnd
