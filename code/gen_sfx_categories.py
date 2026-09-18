# Prints the kSfxCategoryRuns initializer for source/rnd/sfx.cpp from include/game/sound.h.
# Usage: python3 gen_sfx_categories.py include/game/sound.h
import re
import sys

SFX_COUNT = 0xA36
BANKS = {"PL": "Player", "IT": "Item", "FISHING": "Item", "EV": "Environment", "SC": "Environment",
         "EN": "Enemy", "NE": "Enemy", "SY": "System", "VO": "Voice"}

src = open(sys.argv[1]).read()
body = src[src.index("enum class EffectId"):]
body = body[:body.index("};")]
names = {int(value, 16) - 0x01000000: name for name, value in re.findall(r"(\w+)\s*=\s*(0x[0-9A-Fa-f]+)", body)}


def category(index):
    name = names.get(index)
    # Only the bankless NA_SE_DUMMY_* block is unassigned. The names come from Ocarina of Time, so
    # a bank-named "dummy" can be a real MM3D sound: VO_DUMMY_127-255 are Link's voice for each form.
    if name is None:
        return "Excluded"
    bank = re.match(r"NA_SE_([A-Z0-9]+)_", name)
    return BANKS.get(bank.group(1), "Excluded") if bank else "Excluded"


previous = None
for index in range(SFX_COUNT):
    current = category(index)
    if current != previous:
        print(f"{{0x{index:03X}, SfxCategory::{current}}},")
        previous = current
