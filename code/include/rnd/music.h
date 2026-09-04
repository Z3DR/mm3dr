#pragma once
#include "common/types.h"

#define BGM_BASE 0x01000000
#define BGM_COUNT 0x86
namespace rnd {
  extern "C" u32 rBGMOverrides[BGM_COUNT];
  extern "C" u32 Music_OverridePlay(u32 original);
  enum SeqType : u8 {
    SEQ_NOSHUFFLE = 0,
    SEQ_BGM_WORLD = 1 << 0,   // field, town, dungeon ambience
    SEQ_BGM_EVENT = 1 << 1,   // cutscene / character themes
    SEQ_BGM_BATTLE = 1 << 2,  // boss, mini-boss, chase
    SEQ_FANFARE = 1 << 3,     // item get, clear jingles
    SEQ_OCARINA = 1 << 4,     // the 0x7F OCA_MUS family
  };
}  // namespace rnd