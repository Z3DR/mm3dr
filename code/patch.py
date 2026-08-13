import os
import struct
import subprocess
import sys
import json

elf = sys.argv[1]
result = subprocess.run([os.environ["DEVKITARM"] + r'/bin/arm-none-eabi-objdump', '--section-headers', elf], stdout=subprocess.PIPE)
lines = str(result.stdout).split('\\n')
sectionsInfo = [line.split()[1:6] for line in lines if line.split() and line.split()[0].isdigit()]
sections = ((sec[0], int(sec[2],16), int(sec[4],16), int(sec[1],16)) for sec in sectionsInfo if int(sec[2],16) != 0)

# Put here the symbols from the patch which are needed by the app
desiredSymbols = ("rItemOverrides", "gSettingsContext", "gSpoilerData", "rScrubRandomItemPrices", "rDungeonRewardOverrides", "rCustomMessageTextData", "rCustomMessageColData", "rCustomMessageIconData", "rCustomMessageDelayData", "rCustomMessages", "numCustomMessageEntries", "ptrCustomMessageEntries", "rShopsanityPrices", "rEntranceOverrides", "rBGMOverrides", "rSfxData")

nmResult = subprocess.run([os.environ["DEVKITARM"] + r'/bin/arm-none-eabi-nm', elf], stdout=subprocess.PIPE)
nmLines = str(nmResult.stdout).split('\\n')
symbolsInfo = [line.split() for line in nmLines if len(line.split()) >= 3 and line.split()[2].replace("\\r", "") in desiredSymbols]
symbols = {sym[2].replace("\\r", ""):hex(int(sym[0],16)) for sym in symbolsInfo}
symbolsJson = json.dumps(symbols, indent=4)
with open("../source/patch_symbols.hpp", 'w') as syms:
    syms.write("#pragma once\n")
    for sym in symbolsInfo:
        syms.write("#define "+sym[2].replace("\\r", "").upper()+"_ADDR"+" 0x"+sym[0]+"\n")
print("wrote desired symbols to patch_symbols.json")


IPS_RAW_MAX = 0xFFFF
IPS_RLE_MAX = 0xFFFF
IPS_RLE_MIN_RUN = 14
# A record starting at this offset would be read as the "EOF" footer and silently
# truncate the patch. Nothing currently lands here, so refuse rather than corrupt.
IPS_EOF_OFFSET = 0x454F46

def off(vaddr):
    offset = vaddr - 0x100000
    if not 0 <= offset <= 0xFFFFFF:
        raise ValueError("vaddr 0x%X is outside the 3-byte IPS offset range" % vaddr)
    if offset == IPS_EOF_OFFSET:
        raise ValueError("record at vaddr 0x%X would be read as the IPS EOF footer" % vaddr)
    return struct.pack(">I", offset)[1:]

sz = lambda size: struct.pack(">H", size)

def ips_raw(vaddr, chunk):
    return off(vaddr) + sz(len(chunk)) + chunk

def ips_rle(vaddr, length, value):
    return off(vaddr) + sz(0) + sz(length) + bytes([value])

def ips_encode(vaddr, data):
    """Encode one contiguous block, collapsing long runs of one byte into RLE records."""
    out = bytearray()
    spanStart = 0
    i = 0

    def flushRaw(end):
        start = spanStart
        while start < end:
            stop = min(start + IPS_RAW_MAX, end)
            out.extend(ips_raw(vaddr + start, data[start:stop]))
            start = stop

    while i < len(data):
        value = data[i]
        runEnd = i
        while runEnd < len(data) and data[runEnd] == value:
            runEnd += 1
        if runEnd - i >= IPS_RLE_MIN_RUN:
            flushRaw(i)
            start = i
            while start < runEnd:
                length = min(runEnd - start, IPS_RLE_MAX)
                out.extend(ips_rle(vaddr + start, length, value))
                start += length
            spanStart = runEnd
        i = runEnd
    flushRaw(len(data))
    return bytes(out)

ips = b'PATCH'
sectionBytes = 0
with open(elf, 'rb') as e:
    for name, vaddr, offset, size in sections:
        if ".eh_frame" in name:
            continue
        e.seek(offset, 0)
        data = e.read(size)
        if len(data) != 0:
            print('{:0X}'.format(vaddr), '{:0X}'.format(vaddr + size), name)
            ips += ips_encode(vaddr, data)
            sectionBytes += len(data)
ips += b'EOF'
print("encoded %d bytes of section data into a %d byte patch" % (sectionBytes, len(ips)))

with open("basecode.ips", 'wb') as patchFile:
    patchFile.write(ips)

# Write basecode1.1ips as well.
# Ignore first 5 bytes since we don't need P A T C H
ips = ips[5:]
rawDeltaBytes = b''
with open("deltapatch.ips", "rb") as f:
    # Ignore last 3 bytes that are E O F 
    rawDeltaBytes = f.read()
rawDeltaBytes = rawDeltaBytes[:-3]
rawDeltaBytes += ips

with open("basecode1.1.ips", "wb") as newPatchFile:
  newPatchFile.write(rawDeltaBytes)