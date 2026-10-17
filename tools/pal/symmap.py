#!/usr/bin/env python3
"""Recover PAL addresses of symbols (and per-object section bases) from relocations.

For every compiled function that matches PAL at its mapped address, each
relocation's target is read back out of the PAL instruction stream.
Output: pal_work/symmap.json  {symbol: {addr: count}}, {obj:section: {base: count}}
"""
import json, glob, sys, os
from collections import defaultdict
import numpy as np
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fingerprint as fp

GP = 0x38acf0
base, text = fp.load_target('iso/SCES_504.08')
fmap = {o['name']: o for o in json.load(open('config/p3.pal.func_map.json'))}
s16 = lambda v: v - 0x10000 if v & 0x8000 else v
votes = defaultdict(lambda: defaultdict(int))

for objp in sorted(glob.glob(sys.argv[1] + '/*.o')):
    tag = os.path.basename(objp)
    e = ELFFile(open(objp, 'rb'))
    secs = list(e.iter_sections())
    symtab = e.get_section_by_name('.symtab')
    syms = list(symtab.iter_symbols())
    tidx = [i for i, s in enumerate(secs) if s.name == '.text'][0]
    ri = e.get_section_by_name('.reginfo')
    gp0 = int.from_bytes(ri.data()[20:24], 'little') if ri else 0
    tdata = np.frombuffer(secs[tidx].data(), dtype='<u4')
    funcs = sorted((s['st_value'], s['st_size'], s.name) for s in syms if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] == tidx)
    # obj text offset -> PAL address, for verified-matching functions only
    owned = {}
    for name, words, mask in fp.obj_funcs(objp):
        pal = fmap.get(name, {}).get('pal')
        if pal is None: continue
        i = (pal - base) // 4
        if i + len(words) <= len(text) and np.array_equal(text[i:i+len(words)] & mask, words):
            v = [f for f in funcs if f[2] == name][0][0]
            owned[name] = (v, len(words) * 4, pal)
    def to_pal(off):
        for v, sz, pal in owned.values():
            if v <= off < v + sz: return pal + (off - v)
    rels = [rs for rs in e.iter_sections() if isinstance(rs, RelocationSection) and rs['sh_info'] == tidx]
    if not rels: continue
    rl = sorted(rels[0].iter_relocations(), key=lambda r: r['r_offset'])
    pending_hi = {}
    for r in rl:
        off = r['r_offset']; typ = r['r_info_type']; sym = syms[r['r_info_sym']]
        pa = to_pal(off)
        if pa is None: continue
        pw = int(text[(pa - base) // 4]); ow = int(tdata[off // 4])
        if sym['st_info']['type'] == 'STT_SECTION':
            key = f"{tag}:{secs[sym['st_shndx']].name}"
        else:
            key = sym.name
        if typ == 4:   # R_MIPS_26
            tgt = (pa & 0xF0000000) | ((pw & 0x3FFFFFF) << 2)
            add = (ow & 0x3FFFFFF) << 2
            votes[key][tgt - add] += 1
        elif typ == 5: # HI16
            pending_hi[key] = (pw & 0xFFFF, ow & 0xFFFF)
        elif typ == 6: # LO16
            if key in pending_hi:
                ph, oh = pending_hi[key]
                addr = (ph << 16) + s16(pw & 0xFFFF)
                add = (oh << 16) + s16(ow & 0xFFFF)
                votes[key][addr - add] += 1
        elif typ == 7: # GPREL16
            addr = GP + s16(pw & 0xFFFF); add = s16(ow & 0xFFFF)
            if sym['st_info']['type'] == 'STT_SECTION': add += gp0
            votes[key][addr - add] += 1
out = {k: {hex(a): c for a, c in sorted(v.items(), key=lambda x: -x[1])} for k, v in votes.items()}
json.dump(out, open('pal_work/symmap.json', 'w'), indent=1)
amb = [k for k, v in out.items() if len(v) > 1]
print(f"symbols/sections resolved: {len(out)}  conflicting: {len(amb)}")
for k in amb[:20]: print('  conflict', k, list(out[k].items())[:3])
