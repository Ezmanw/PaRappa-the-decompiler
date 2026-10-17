#!/usr/bin/env python3
"""Locate an object's PROGBITS data section (.data/.rodata/.sdata/.lit4) inside the PAL ELF by content."""
import numpy as np
from functools import lru_cache
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ISO = 'iso/SCES_504.08'

@lru_cache(None)
def _target(sec):
    s = ELFFile(open(ISO, 'rb')).get_section_by_name(sec)
    d = s.data(); d = d[:len(d) // 4 * 4]
    return s['sh_addr'], np.frombuffer(d, dtype='<u4')

def obj_section(objp, sec):
    e = ELFFile(open(objp, 'rb'))
    for i, s in enumerate(e.iter_sections()):
        if s.name == sec and s['sh_type'] == 'SHT_PROGBITS' and s['sh_size']:
            d = s.data(); n = len(d) // 4
            words = np.frombuffer(d[:n * 4], dtype='<u4').copy()
            mask = np.full(n, 0xFFFFFFFF, dtype=np.uint32)
            for rs in e.iter_sections():
                if isinstance(rs, RelocationSection) and rs['sh_info'] == i:
                    for r in rs.iter_relocations():
                        if r['r_offset'] // 4 < n: mask[r['r_offset'] // 4] = 0
            return words & mask, mask
    return None, None

def find(objp, sec, candidates=()):
    words, mask = obj_section(objp, sec)
    if words is None: return None
    base, t = _target(sec)
    L = len(words)
    def ok(a):
        i = (a - base) // 4
        return 0 <= i and i + L <= len(t) and (a - base) % 4 == 0 and np.array_equal(t[i:i+L] & mask, words)
    good = [a for a in candidates if ok(a)]
    if len(good) == 1: return good[0]
    if good: return None
    # full search, anchored on the most distinctive (non-zero, unmasked) word
    nz = [j for j in range(L) if mask[j] and words[j] not in (0, 0xFFFFFFFF)]
    if not nz: return None
    j = nz[0]
    cand = np.nonzero(t == words[j])[0] - j
    hits = [int(base + 4 * c) for c in cand if c >= 0 and ok(int(base + 4 * c))]
    return hits[0] if len(hits) == 1 else None
