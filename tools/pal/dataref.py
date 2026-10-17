#!/usr/bin/env python3
"""Attribute PAL data addresses to the source file whose code references them.

Reads .text file ranges from config/p3.pal.yaml and prints, per data section,
contiguous runs of referenced addresses grouped by referencing file.
"""
import re, sys, json
import numpy as np
from elftools.elf.elffile import ELFFile
ISO = 'iso/SCES_504.08'
e = ELFFile(open(ISO, 'rb'))
t = e.get_section_by_name('.text'); base = t['sh_addr']; w = np.frombuffer(t.data(), dtype='<u4')
GP = 0x38acf0
y = open('config/p3.pal.yaml').read()
segs = [(int(a, 16) + 0x100000, n) for a, n in re.findall(r'- \[(0x[0-9A-Fa-f]+),\s*(?:asm|c|cpp),\s*([\w/]+)\]', y)]
segs.append((base + 4 * len(w), None))
secs = [(s['sh_addr'], s['sh_addr'] + s['sh_size'], s.name) for s in e.iter_sections()
        if s['sh_addr'] and s.name not in ('.text', '.vutext', '.mfifo') and not s.name.startswith('.DVP')]
def secof(a):
    for lo, hi, n in secs:
        if lo <= a < hi: return n
s16 = lambda v: v - 0x10000 if v & 0x8000 else v
LOADSTORE = set(range(0x20, 0x40)) | {0x1e, 0x1f, 0x09, 0x0d}  # loads/stores, lq/sq, addiu, ori
refs = {}  # addr -> set(files)
for k in range(len(segs) - 1):
    a0, name = segs[k]; a1 = segs[k + 1][0]
    hi = {}
    for i in range((a0 - base) // 4, (a1 - base) // 4):
        x = int(w[i]); op = x >> 26; rs = (x >> 21) & 31; rt = (x >> 16) & 31
        if op == 0x0F: hi[rt] = (x & 0xFFFF) << 16; continue
        if op in LOADSTORE:
            if rs == 28: a = GP + s16(x & 0xFFFF)
            elif rs in hi: a = hi[rs] + (s16(x & 0xFFFF) if op != 0x0d else (x & 0xFFFF))
            else: continue
            if secof(a): refs.setdefault(a, set()).add(name)
        if x == 0x03e00008: hi = {}
out = {}
for lo, hi_, sn in secs:
    runs = []
    for a in sorted(x for x in refs if lo <= x < hi_):
        f = sorted(refs[a])[0] if len(refs[a]) == 1 else '+'.join(sorted(refs[a]))
        if runs and runs[-1][2] == f: runs[-1][1] = a
        else: runs.append([a, a, f])
    out[sn] = runs
json.dump(out, open('pal_work/datarefs.json', 'w'), indent=1)
for sn, runs in out.items():
    print(f"== {sn}")
    for a, b, f in runs: print(f"  {a:#010x}..{b:#010x} {f}")
