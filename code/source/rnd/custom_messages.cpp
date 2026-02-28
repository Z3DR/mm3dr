#include "rnd/custom_messages.h"
#include "game/common_data.h"
#include "game/message.h"

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif

#define MAX_CUSTOM_MSG_SIZE 512
#define MAX_CUSTOM_MSGS 512
#define LINE_WIDTH 260
#define ICON_WIDTH 16
#define MAJORA_ICON_WIDTH 32
#define MAX_CHAR 0x220
#define DEFAULT_WIDTH 8
#define INSTANT_FLAG 0x8000
#define REPEAT_FLAG 0x4000
#define LINE_PADDING(msg) ((((msg).flags & 0x00FF0000) == 0x00FF0000) ? 0 : 20)

// Pixel widths of printable characters (many are replaced by * which is 8 pixels)
const char width[MAX_CHAR] = {
    0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 00x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 01x
    4,  4,  6,  10, 9,  12, 10, 4,  5,  5,  8,  9,  5,  7,  4,  7,   // 02x
    9,  6,  9,  9,  10, 9,  9,  9,  9,  9,  4,  5,  8,  8,  8,  8,   // 03x
    11, 10, 10, 10, 11, 9,  9,  10, 10, 4,  6,  10, 8,  13, 11, 10,  // 04x
    10, 10, 9,  9,  9,  10, 10, 13, 9,  9,  10, 5,  7,  5,  8,  8,   // 05x
    5,  8,  9,  8,  9,  9,  5,  8,  9,  4,  5,  8,  4,  12, 9,  9,   // 06x
    9,  8,  6,  7,  5,  9,  8,  11, 8,  8,  8,  5,  4,  5,  6,  0,   // 07x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 08x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 09x
    0,  4,  8,  10, 8,  9,  4,  9,  6,  10, 5,  8,  7,  7,  10, 7,   // 0Ax
    4,  10, 6,  5,  5,  10, 8,  4,  4,  4,  5,  8,  9,  9,  10, 8,   // 0Bx
    10, 10, 10, 10, 11, 10, 12, 9,  9,  9,  9,  10, 5,  5,  5,  5,   // 0Cx
    11, 10, 10, 10, 10, 10, 10, 10, 10, 9,  9,  9,  9,  9,  8,  8,   // 0Dx
    8,  8,  9,  9,  8,  9,  11, 8,  9,  9,  9,  9,  5,  5,  5,  5,   // 0Ex
    9,  8,  9,  9,  9,  9,  9,  11, 9,  9,  9,  9,  9,  8,  9,  9,   // 0Fx
    10, 8,  10, 8,  11, 10, 10, 8,  8,  8,  10, 8,  10, 8,  11, 12,  // 10x
    8,  8,  9,  9,  8,  8,  9,  9,  9,  9,  9,  9,  8,  8,  10, 8,   // 11x
    10, 8,  10, 8,  8,  8,  11, 9,  8,  8,  6,  6,  8,  8,  5,  5,   // 12x
    4,  4,  10, 8,  8,  8,  10, 8,  8,  9,  6,  8,  5,  8,  8,  8,   // 13x
    8,  9,  5,  11, 9,  11, 9,  11, 9,  8,  8,  8,  8,  8,  8,  8,   // 14x
    10, 9,  11, 12, 10, 6,  8,  8,  10, 6,  9,  7,  8,  8,  9,  7,   // 15x
    9,  7,  8,  8,  9,  7,  8,  8,  8,  8,  10, 9,  8,  8,  10, 9,   // 16x
    10, 9,  10, 10, 8,  8,  8,  8,  8,  10, 8,  10, 8,  10, 8,  8,   // 17x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 18x
    8,  8,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 19x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Ax
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Bx
    8,  8,  8,  8,  8,  15, 14, 8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Cx
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Dx
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Ex
    8,  8,  15, 14, 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 1Fx
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,   // 20x
    8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9,  5,  8,  8,  8,  8,   // 21x
};

volatile const char rCustomMessageTextData[1 << 18] = {0};
volatile const char rCustomMessageColData[4 << (10 - 3)] = {0};
volatile const char rCustomMessageIconData[6 << (10 - 3)] = {0};
volatile const char rCustomMessageDelayData[6 << (10 - 3)] = {0};

typedef struct {
  u16 id;
  u16 size;
  char data[MAX_CUSTOM_MSG_SIZE];
} CustomMessage;

// Extract data less than 8 bits long from a char array without wasting bits
u8 unpackData(u8 size, u32 idx, volatile const char* data) {
  u8 i, unpacked = 0;
  for (i = 0; i < size; i++) {
    unpacked <<= 1;
    if ((data[(idx * size + i) / 8] << ((idx * size + i) % 8)) & 0x80)
      unpacked += 1;
  }
  return unpacked;
}

// Pack data less than 8 bits long into a char array without wasting bits
void packData(u8 size, u32 idx, char* data, u8 unpacked) {
  for (u8 i = size; i--;) {
    if (unpacked & 1)
      data[(idx * size + i) / 8] |= 0x80 >> ((idx * size + i) % 8);
    else
      data[(idx * size + i) / 8] &= ~(0x80 >> ((idx * size + i) % 8));
    unpacked >>= 1;
  }
}

typedef enum {
  VT_UNK,
  VT_INT,
  VT_STR,
  VT_WSTR,
} VarType;

typedef struct {
  VarType type;
  union {
    u32 num;
    char* text;
    char16_t* text16;
  } value;
} VarVal;

// Retrieve data to match a three letter acronym
VarVal getVarVal(volatile const char* text) {
  // Ensure prefixed with '='
  if (text[0] != '=')
    return (VarVal){VT_UNK, 0};

// Macro to help make checking the three letter acronym easier to read
#define is(TLA) text[1] == TLA[0] && text[2] == TLA[1] && text[3] == TLA[2]
  // Oceanside spider house token count
  if (is("OSH"))
    return (VarVal){VT_INT, (u32)game::GetCommonData().save.skulltulas_collected.ocean_count};

  // Swamp spider house token count
  if (is("SSH"))
    return (VarVal){VT_INT, (u32)game::GetCommonData().save.skulltulas_collected.swamp_count};

// Examples for adding a string or wstring value
// if (is("TXT")) return (VarVal){ VT_STR, { .text = someCharStar } };
// if (is("PLN")) return (VarVal){ VT_WSTR, { .text16 = game::GetCommonData().save.player.playerName } };
#undef is

  // No matches
  return (VarVal){VT_UNK, 0};
}

volatile const char* resolveTextPtr(volatile const u32* offsets) {
  // Offsets are 18 bits long packed into array
  u32 offsetUsEn = offsets[0] & 0x3FFFF;
  u32 offsetUsFr = ((offsets[1] << 14) | (offsets[0] >> 18)) & 0x3FFFF;
  u32 offsetUsEs = (offsets[1] >> 4) & 0x3FFFF;
  u32 offsetEuEn = ((offsets[2] << 10) | (offsets[1] >> 22)) & 0x3FFFF;
  u32 offsetEuFr = (offsets[2] >> 8) & 0x3FFFF;
  u32 offsetEuEs = ((offsets[3] << 6) | (offsets[2] >> 26)) & 0x3FFFF;
  u32 offsetEuDe = (offsets[3] >> 12) & 0x3FFFF;
  u32 offsetEuIt = ((offsets[4] << 2) | (offsets[3] >> 30)) & 0x3FFFF;
  u32 offsetJpJp = ((offsets[5] << 16) | (offsets[4] >> 16)) & 0x3FFFF;

  // Choose offset based on current language and if an offset is assigned
  // Offset 0 should contain a "No text" message
  game::MessageMgr mgr = game::MessageMgr::Instance();
  switch (mgr.lang) {
  case game::Language::UsFr:
    return rCustomMessageTextData + ((offsetUsFr) ? offsetUsFr : offsetUsEn);

  case game::Language::UsEs:
    return rCustomMessageTextData + ((offsetUsEs) ? offsetUsEs : offsetUsEn);

  case game::Language::EuFr:
    return rCustomMessageTextData + ((offsetEuFr) ? offsetEuFr : offsetEuEn);

  case game::Language::EuEs:
    return rCustomMessageTextData + ((offsetEuEs) ? offsetEuEs : offsetEuEn);

  case game::Language::EuDe:
    return rCustomMessageTextData + ((offsetEuDe) ? offsetEuDe : offsetEuEn);

  case game::Language::EuIt:
    return rCustomMessageTextData + ((offsetEuIt) ? offsetEuIt : offsetEuEn);

  case game::Language::JpJp:
    return rCustomMessageTextData + ((offsetJpJp) ? offsetJpJp : offsetUsEn);

  case game::Language::EuEn:
    return rCustomMessageTextData + offsetEuEn;

  case game::Language::UsEn:
  default:
    return rCustomMessageTextData + offsetUsEn;
  }
}

class MsgBuilder {
public:
  char* data;
  u16* size;

  MsgBuilder* set(CustomMessage* msg) {
    data = &msg->data[0];
    size = &msg->size;
    return this;
  }

  MsgBuilder* addChr(char chr) {
    if (*size < MAX_CUSTOM_MSG_SIZE)
      data[(*size)++] = chr;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    else
      rnd::util::Print("Error building message: MAX_MSG_SIZE exceeded\n");
#endif
    return this;
  }

  u16 addNum(u32 num) {
    u16 len = 0, chars = 1;
    char txtNum[10] = "0";
    for (u32 i = 0x80000000, j; i; i >>= 1) {
      if (txtNum[chars - 1] > '4')
        txtNum[chars++] = '0';
      for (j = chars; --j;) {
        txtNum[j] = (((txtNum[j] - '0') << 1) % 10) + ((txtNum[j - 1] > '4') ? '1' : '0');
      }
      txtNum[0] = (((txtNum[0] - '0') << 1) % 10) + ((num & i) ? '1' : '0');
    }
    for (u32 j = chars; j--;) {
      addChr(txtNum[j]);
      len += width[(u16)txtNum[j]];
    }
    return len;
  }

  u16 addText(char* txt) {
    u16 len = 0, resolved;
    for (u16 idx = 0; txt[idx]; idx++) {
      // Abort if unsupported character
      if (txt[idx] > 0xDF)
        return len;
      if (txt[idx] > 0x7F) {
        resolved = ((txt[idx] & 0x1F) << 6) | (txt[idx + 1] & 0x3F);
        addChr(txt[idx++]);
      } else
        resolved = txt[idx];
      addChr(txt[idx]);
      len += (resolved < MAX_CHAR) ? width[resolved] : DEFAULT_WIDTH;
    }
    return len;
  }

  u16 addText16(char16_t* txt) {
    u16 len = 0;
    for (u16 idx = 0; txt[idx]; idx++) {
      // Convert UTF16 to UTF8
      if (txt[idx] < 0x80) {
        addChr(txt[idx]);
      } else if (txt[idx] < 0x0800) {
        addChr(0xC0 | ((txt[idx] >> 6) & 0x1F));
        addChr(0x80 | (txt[idx] & 0x3F));
      } else {
        // Unsupported char, abort
        return len;
      }
      len += (txt[idx] < MAX_CHAR) ? width[txt[idx]] : DEFAULT_WIDTH;
    }
    return len;
  }

  MsgBuilder* text(const char* txt) {
    // UTF8 encoding excluding 0x00 and 0x7F
    // Many characters, especially control characters, print as `*`
    for (u16 idx = 0; txt[idx]; idx++)
      addChr(txt[idx]);
    return this;
  }

  MsgBuilder* pad(bool fourByte) {
    if (fourByte) {
      while (((u32)data + *size) & 3)
        addChr(0x00);
    } else if (((u32)data + *size) & 1)
      addChr(0x00);
    return this;
  }

  MsgBuilder* addCom(char com) {
    addChr(0x7f);
    pad(false);
    addChr(com);
    return addChr(0x00);
  }

  MsgBuilder* addCom(char com, char arg) {
    addCom(com);
    addChr(arg);
    return addChr(0x00);
  }

  MsgBuilder* sound(u16 id) {
    // Must have a sufficient delay if used multiple times in one box
    addCom(0x32);
    pad(true);
    addChr(id & 0xFF);
    addChr(id >> 0x8);
    addChr(0x00);
    return addChr(0x01);
  }

  MsgBuilder* filename() {
    return addCom(0x03);
  }  // 1 to 8 UTF8 characters so size highly variable
  MsgBuilder* col(char c) {
    return addCom(0x3a, c);
  }  // Change text colour until used again or new box
  MsgBuilder* icon(char img) {
    return addCom(0x25, img);
  }  // Insert an icon. See: https://cdn.discordapp.com/attachments/896879784162918400/1138941632675328010/image.png
  MsgBuilder* delay(char time) {
    return addCom(0x29, time);
  }  // Time measured in 20ths of a second
  MsgBuilder* top() {
    return addCom(0x28);
  }  // Applies to all boxes in message
  MsgBuilder* instant() {
    return addCom(0x27);
  }  // Applies only to current box
  MsgBuilder* newline() {
    return addCom(0x01);
  }
  MsgBuilder* newBox() {
    addCom(0x31, 0x01);
    return addCom(0x02);
  }
  MsgBuilder* end() {
    addCom(0x31, 0x00);
    return addCom(0x00);
  }
  MsgBuilder* pseudoEnd() {
    addCom(0x31, 0x00);
    return addCom(0x02);
  }
  // MsgBuilder* furiganaOpen() {
  //   return addCom(0x3B);
  // }
  // MsgBuilder* furiganaSwitch() {
  //   return addCom(0x00);
  // }
  // MsgBuilder* furiganaClose() {
  //   return addCom(0x3C);
  // }

  void format(volatile const UnformattedMessage* msg) {
    // @ - filename min: 4px max: 120px
    // # - colour change
    // & - newline
    // ^ - new box
    // $ - icon
    // % - delay
    // = - 3 letter acronym for variable to insert
    volatile const char* text = resolveTextPtr(msg->offsets);
    u16 colOffset = (u16)(msg->offsets[5] >> 2) & 0x3FF;
    u16 iconOffset = (u16)(msg->offsets[5] >> 12) & 0x3FF;
    u16 delayOffset = (u16)(msg->offsets[5] >> 22) & 0x3FF;
    u16 idx = 0xFFFF, lastSpaceIdx = 0;
    u16 colIdx = 0, colIdxAtLastSpace = 0;
    u16 iconIdx = 0, iconIdxAtLastSpace = 0;
    u16 delayIdx = 0, delayIdxAtLastSpace = 0;
    u16 sizeAtLastSpace = 0, resolvedChar = 0, lineLen = LINE_PADDING(*msg);
    bool inCol = false, inColAtLastSpace = false;
    //bool inFuriganaMode = false;
    bool lineWrap = true;
    u16 sfx = msg->sfxAndFlags & 0x3FFF;
    u8 resolvedCol = 0, resolvedIcon = 0, resolvedDelay = 0;
    VarVal resolvedVar;
    u16 playerNameLen = 0;
    *size = 0;

    if (sfx)
      sound(sfx);
    if (msg->sfxAndFlags & INSTANT_FLAG)
      instant();

    // Tingle Map Choices. Add 3 choices, 2 for maps and 1 for no thanks.
    if (msg->id >= 0x1D11 && msg->id <= 0x1D16) {
      addCom(0x2F, 3);
      // Disable line wrap to ensure text lines up with options
      lineWrap = false;
    }

    while (++idx < MAX_CUSTOM_MSG_SIZE && text[idx]) {
      resolvedChar = text[idx];

      switch (resolvedChar) {
      case '@':  // Player file name
        if (!lastSpaceIdx)
          sizeAtLastSpace = *size;
        filename();
        if (!playerNameLen) {
          // Calculate length of player name
          char16_t* playerName = game::GetCommonData().save.player.playerName;
          for (u32 i = 0; i < 8 && playerName[i]; i++) {
            if (playerName[i] < MAX_CHAR)
              playerNameLen += width[playerName[i]];
            else
              playerNameLen += DEFAULT_WIDTH;
          }
        }
        lineLen += playerNameLen;
        break;

      case '#':  // Colour marker
        if (inCol)
          col(QM_DEFAULT);
        else {
          // Get next colour
          resolvedCol = unpackData(4, colOffset + colIdx, rCustomMessageColData);
          if (resolvedCol != 0xF)
            colIdx++;
          else {
            resolvedCol = QM_DEFAULT;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
            rnd::util::Print("Warning formatting message, out of colours: %s\n", text);
#endif
          }

#if defined ENABLE_DEBUG || defined DEBUG_PRINT
          // Could be caused by not providing enough colours so provide debug message
          if (!resolvedCol)
            rnd::util::Print("Warning formatting message, colour %i is 0x00: %s\n", colIdx, text);
#endif

          col(resolvedCol);
        }
        inCol = !inCol;
        break;

      case '&':  // Newline
        newline();
        lastSpaceIdx = 0;
        lineLen = LINE_PADDING(*msg);
        break;

      case '^':  // Newbox
        newBox();
        // New boxes have default colours so re-apply colour
        if (inCol)
          col(resolvedCol);
        // If set to repeat sfx then add to this box
        if ((msg->sfxAndFlags & REPEAT_FLAG) && sfx)
          sound(sfx);
        // New boxes lose the instant text property so re-set it
        if (msg->sfxAndFlags & INSTANT_FLAG)
          instant();

        lastSpaceIdx = 0;
        lineLen = LINE_PADDING(*msg);
        break;

      case '$':  // Icon marker
        if (!lastSpaceIdx)
          sizeAtLastSpace = *size;

        // Get next icon
        resolvedIcon = unpackData(6, iconOffset + iconIdx, rCustomMessageIconData);
        if (resolvedIcon != 0x3F)
          iconIdx++;
        else {
          resolvedIcon = BLANK;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
          rnd::util::Print("Warning formatting message, out of icons: %s\n", text);
#endif
        }

        // Majora icon is wider than all other icons
        lineLen += (resolvedIcon == MAJORA_ICON) ? MAJORA_ICON_WIDTH : ICON_WIDTH;
        icon(resolvedIcon);
        break;

      case '%':  // Delay marker
        resolvedDelay = unpackData(6, delayOffset + delayIdx, rCustomMessageDelayData);
        if (resolvedDelay != 0x3F)
          delayIdx++;
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        else
          rnd::util::Print("Warning formatting message, out of delays: %s\n", text);
#endif
        // Don't bother inserting a delay of 0 as is a waste of characters
        if (resolvedDelay)
          delay(resolvedDelay && resolvedDelay != 0x3F);
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
        // Could be caused by not providing enough delays so provide debug message
        else if (!resolvedDelay)
          rnd::util::Print("Warning formatting message, delay %i is 0x00: %s\n", delayIdx, text);
#endif
        break;

      case '=':  // Numeric variable marker
        if (!lastSpaceIdx)
          sizeAtLastSpace = *size;
        resolvedVar = getVarVal(&text[idx]);
        switch (resolvedVar.type) {
        case VT_INT:
          idx += 3;
          lineLen += addNum(resolvedVar.value.num);
          break;
        case VT_STR:
          idx += 3;
          lineLen += addText(resolvedVar.value.text);
          break;
        case VT_WSTR:
          idx += 3;
          lineLen += addText16(resolvedVar.value.text16);
          break;
        default:
          // 3LA didn't match any variables, treat '=' as normal text
          addChr(text[idx]);
          lineLen += width[resolvedChar];
        }
        break;

      case ' ':
        // Keep track of spaces for inserting line breaks
        lastSpaceIdx = idx;
        colIdxAtLastSpace = colIdx;
        iconIdxAtLastSpace = iconIdx;
        delayIdxAtLastSpace = delayIdx;
        inColAtLastSpace = inCol;
        sizeAtLastSpace = *size;
        // Don't break so can still be processed as a normal character
      default:
        if (!lastSpaceIdx)
          sizeAtLastSpace = *size;
        // Uses UTF8 encoding so convert multi-byte representations to a single number
        if (resolvedChar > 0x7F) {
          // Abort if char is not a valid first UTF8 byte
          if (resolvedChar < 0xC0 || resolvedChar > 0xFD) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
            rnd::util::Print("Error formatting message, malformed character: %s\n", text);
#endif

            return;
          }
          // Store whether character code will fit in resolvedChar's 16 bits
          bool resolvedCharIsTooSmall = (resolvedChar > 0xEF);
          // Remove first 3 bits in case char is two bytes long
          resolvedChar &= 0x1F;
          // Iterate through UTF8 prefix's left bits
          for (char utfPrefix = (text[idx] << 1); utfPrefix > 0x7F; utfPrefix <<= 1) {
            // Abort if expected continuation byte is missing
            if ((text[idx + 1] >> 6) != 2) {
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
              rnd::util::Print("Error formatting message, malformed character: %s\n", text);
#endif
              return;
            }
            // Concat next byte's bits to resolvedChar
            resolvedChar <<= 6;
            resolvedChar |= (text[idx + 1] & 0x3F);
            // Add current byte to message and increment
            if (game::MessageMgr::Instance().lang != game::Language::JpJp) addChr(text[idx]);
            idx++;
          }
          // Reset resolvedChar to * if it couldn't fit value
          if (resolvedCharIsTooSmall) {
            resolvedChar = 0x2A;
          }
        }

       if (game::MessageMgr::Instance().lang == game::Language::JpJp) {
          addChr(resolvedChar & 0xFF);
          addChr(resolvedChar >> 8);
        } else
          addChr(text[idx]);
        // Assumes all further chars will be represented by * as many up to MAX_CHAR already are
        lineLen += (resolvedChar < MAX_CHAR) ? width[resolvedChar] : DEFAULT_WIDTH;
        break;
      }

      // Replace last space with newline if necessary
      // If no space available use start of last item with width
      if (lineWrap && lineLen > LINE_WIDTH) {
        if (lastSpaceIdx) {
          idx = lastSpaceIdx;
          colIdx = colIdxAtLastSpace;
          iconIdx = iconIdxAtLastSpace;
          delayIdx = delayIdxAtLastSpace;
          inCol = inColAtLastSpace;

          lastSpaceIdx = 0;
        } else
          idx -= (resolvedChar > 0x7F) ? 2 : 1;

        *size = sizeAtLastSpace;
        newline();
        lastSpaceIdx = 0;
        lineLen = LINE_PADDING(*msg);
      }
    }

// Add debug message if the last colour didn't have an end marker
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
    if (inCol)
      rnd::util::Print("Warning formatting message, colour not closed: %s\n", text);
#endif
    if (msg->id >= 0x1D11 && msg->id <= 0x1D16)
      addCom(0x00);
    else if ((msg->flags & 0x01000000) == 0)
      end();
    else
      pseudoEnd();
  }
};

MsgBuilder builder;
CustomMessage customMsg;

volatile const UnformattedMessage rCustomMessages[MAX_CUSTOM_MSGS] = {0};
volatile const u32 numCustomMessageEntries = {0};

bool SetCustomMessage(u16 id, game::MessageResEntry* msgResEntry) {
  volatile const UnformattedMessage* customMsgData;
  s32 start = 0, end = numCustomMessageEntries - 1, current;

  while (start <= end) {
    current = (start + end) / 2;
    customMsgData = rCustomMessages + current;

    if (customMsgData->id < id)
      start = current + 1;
    else if (customMsgData->id > id)
      end = current - 1;
    else {
      builder.set(&customMsg)->format(customMsgData);
      // Populate message entry with data from app side and the formatted message
      msgResEntry->id = customMsg.id = id;
      msgResEntry->field_2 = customMsgData->field_2;
      msgResEntry->field_4 = customMsgData->field_4;
      msgResEntry->flags = customMsgData->flags & 0x00FFFFFF;
      msgResEntry->texts[0].offset = customMsg.data;
      msgResEntry->texts[0].length = customMsg.size;
      return true;
    }
  }

  // Didn't find id in custom messages
  return false;
}
