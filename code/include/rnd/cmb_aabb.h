#pragma once

#include "game/cmb.h"
#include "z3d/z3DVec.h"
#include "rnd/models.h"

namespace rnd {
  // Computes the bind-posed model-space AABB of a CMB by scanning the VATR position stream
  // and applying each SEPD's rest-pose bone transform (rigid CMB vertices are stored
  // bone-local; several item models carry real rest poses).
  // Every access is bounds-checked against the CMB's own size field; returns false (outputs
  // untouched) for anything malformed rather than reading out of bounds.
  bool CmbAabb_Compute(const void* cmbData, z3dVec3f* outMin, z3dVec3f* outMax);
}  // namespace rnd
