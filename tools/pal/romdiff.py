#!/usr/bin/env python3
"""Report differing ranges between built and target ROM, labelled by section."""
import sys
from elftools.elf.elffile import ELFFile
a = open(sys.argv[1], 'rb').read(); b = open(sys.argv[2], 'rb').read()
secs = [(s['sh_addr'] - 0x100000, s['sh_size'], s.name) for s in ELFFile(open(sys.argv[3], 'rb')).iter_sections() if s['sh_addr']]
print(f"built={len(a):#x} target={len(b):#x}")
def sec(o):
    for st, sz, n in secs:
        if st <= o < st + sz: return f"{n}+{o-st:#x}"
    return "?"
n = min(len(a), len(b)); i = 0; shown = 0
while i < n and shown < 25:
    if a[i] != b[i]:
        j = i
        while j < n and a[j] != b[j]: j += 1
        print(f"diff {i:#x}..{j:#x} ({j-i:#x} bytes) @ {sec(i)}  built={a[i:i+8].hex()} target={b[i:i+8].hex()}")
        shown += 1; i = j
    else: i += 1
