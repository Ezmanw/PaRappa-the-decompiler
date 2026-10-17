#!/usr/bin/env python3
"""Locate compiled functions from src/ inside a retail ELF by masked instruction matching."""
import sys, glob, struct, json
import numpy as np
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

def load_target(path):
    e = ELFFile(open(path, 'rb'))
    t = e.get_section_by_name('.text')
    return t['sh_addr'], np.frombuffer(t.data(), dtype='<u4')

# Masks for relocated instructions: keep opcode + register fields
def reloc_mask(word, rtype):
    op = word >> 26
    if rtype == 4:          # R_MIPS_26 (j/jal)
        return 0xFC000000
    return 0xFFFF0000       # HI16/LO16/GPREL16/LITERAL etc.

def obj_funcs(path):
    e = ELFFile(open(path, 'rb'))
    secs = {i: s for i, s in enumerate(e.iter_sections())}
    symtab = e.get_section_by_name('.symtab')
    out = []
    for idx, sec in secs.items():
        if sec.name != '.text' or sec['sh_size'] == 0:
            continue
        data = np.frombuffer(sec.data(), dtype='<u4').copy()
        mask = np.full(len(data), 0xFFFFFFFF, dtype=np.uint32)
        for rs in e.iter_sections():
            if isinstance(rs, RelocationSection) and rs['sh_info'] == idx:
                for r in rs.iter_relocations():
                    w = r['r_offset'] // 4
                    mask[w] = reloc_mask(int(data[w]), r['r_info_type'])
        syms = sorted([(s['st_value'], s['st_size'], s.name) for s in symtab.iter_symbols()
                       if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] == idx])
        for i, (v, sz, name) in enumerate(syms):
            if sz == 0:
                end = syms[i+1][0] if i+1 < len(syms) else sec['sh_size']
                sz = end - v
            a, b = v//4, (v+sz)//4
            out.append((name, data[a:b] & mask[a:b], mask[a:b]))
    return out

def main(target, objs, outp):
    base, text = load_target(target)
    n = len(text)
    results = {}
    for o in sorted(objs):
        for name, words, mask in obj_funcs(o):
            L = len(words)
            if L < 2:
                continue
            # candidate positions from the first instruction, then filter
            cand = np.nonzero((text[:n-L+1] & mask[0]) == words[0])[0]
            for j in range(1, L):
                if len(cand) == 0: break
                cand = cand[(text[cand+j] & mask[j]) == words[j]]
            hits = [hex(base + int(c)*4) for c in cand]
            results[name] = {'obj': o, 'size': L*4, 'hits': hits}
    json.dump(results, open(outp, 'w'), indent=1)
    uniq = sum(1 for r in results.values() if len(r['hits']) == 1)
    none = sum(1 for r in results.values() if not r['hits'])
    print(f"functions={len(results)} unique={uniq} ambiguous={len(results)-uniq-none} missing={none}")

if __name__ == '__main__':
    main(sys.argv[1], glob.glob(sys.argv[2] + '/*.o'), sys.argv[3])
