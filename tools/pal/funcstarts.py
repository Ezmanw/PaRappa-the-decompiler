#!/usr/bin/env python3
"""Collect likely function start addresses in the PAL .text section."""
import numpy as np, json, sys
from elftools.elf.elffile import ELFFile
e = ELFFile(open(sys.argv[1] if len(sys.argv) > 1 else 'iso/SCES_504.08', 'rb'))
t = e.get_section_by_name('.text'); base = t['sh_addr']; w = np.frombuffer(t.data(), dtype='<u4'); end = base + 4*len(w)
starts = {}
# jal targets
op = w >> 26
for i in np.nonzero(op == 3)[0]:
    tgt = ((base + 4*int(i)) & 0xF0000000) | ((int(w[i]) & 0x3FFFFFF) << 2)
    if base <= tgt < end: starts[tgt] = starts.get(tgt, '') + 'j'
# code pointers in data sections
for name in ('.data', '.sdata'):
    s = e.get_section_by_name(name)
    d = np.frombuffer(s.data()[:len(s.data())//4*4], dtype='<u4')
    for v in d[(d >= base) & (d < end) & (d % 8 == 0)]:
        starts[int(v)] = starts.get(int(v), '') + 'p'
# after jr ra + delay slot (skipping nop padding)
for i in np.nonzero(w == 0x03e00008)[0]:
    k = int(i) + 2
    while k < len(w) and w[k] == 0: k += 1
    a = base + 4*k
    if a < end and a % 8 == 0 and (w[k] >> 16) in (0x27bd,):  # addiu sp,sp,-N prologue
        starts[a] = starts.get(a, '') + 'r'
json.dump({hex(k): v for k, v in sorted(starts.items())}, open(sys.argv[2] if len(sys.argv) > 2 else 'pal_work/funcstarts.json', 'w'), indent=0)
print(len(starts), 'function starts')
