#!/usr/bin/env python3
"""Generate config/p3.pal.yaml.

Text boundaries come from pal_work/boundaries.json (plus the fixed entries
below); per-file data section bases come from pal_work/symmap.json.
Files listed in tools/pal/convert.txt are emitted as C/C++ with their own
data sections (mirroring the jul12 config); everything else stays as
asm/data blobs.
"""
import json, re, subprocess, sys
from collections import OrderedDict

ROOT_HDR_END = 'segments:'
J = open('config/p3.jul12.yaml', encoding='utf-8').read()
P = open('config/p3.pal.yaml').read()
header = P[:P.index(ROOT_HDR_END)]
bounds = json.load(open('pal_work/boundaries.json'))
symmap = json.load(open('pal_work/symmap.json'))
convert = [l.split('#')[0].strip() for l in open('tools/pal/convert.txt') if l.split('#')[0].strip()]

ROM = lambda vram: vram - 0x100000
SECS = ['.data', '.rodata', '.lit4', '.sdata', '.sbss', '.bss']
SECTION_END = {'.data': 0x377258, '.rodata': 0x382b80, '.lit4': 0x383100, '.sdata': 0x3836e0,
               '.sbss': 0x38390c, '.bss': 0x1c825b4}

# jul12: which files provide which sections from C ('.data') vs asm ('data')
jtypes = {}
for typ, name in re.findall(r'- \[0x[0-9A-Fa-f]+,\s*(\.?\w+),\s*([\w/]+)\]', J):
    jtypes.setdefault(name, {})[typ.lstrip('.') if not typ.startswith('.') else typ] = typ
for typ, name in re.findall(r'\{ type: (\.?\w+),\s*vram: 0x[0-9a-fA-F]+, name: ([\w/]+)\s*\}', J):
    jtypes.setdefault(name, {})[typ] = typ
jtext = {name: typ for typ, name in re.findall(r'- \[0x[0-9A-Fa-f]+,\s*(c|cpp|asm),\s*([\w/]+)\]', J)}

def obj_sizes(name):
    ext = 'cpp' if jtext.get(name) == 'cpp' else 'c'
    o = f"pal_work/obj/{name.replace('/', '__')}.{ext}.o"
    out = subprocess.run(['mips-linux-gnu-readelf', '-SW', o], capture_output=True, text=True).stdout
    sz = {}
    for l in out.splitlines():
        m = re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)', l)
        if m: sz[m.group(1)] = int(m.group(2), 16)
    return sz

def base(name, sec):
    ext = 'cpp' if jtext.get(name) == 'cpp' else 'c'
    v = symmap.get(f"{name.replace('/', '__')}.{ext}.o:{sec}")
    if not v: return None
    (a, c), *rest = v.items()
    if rest: return None
    return int(a, 16)

# every uniquely-known per-object section base, used to place blob starts after alignment padding
known_bases = {}
for k, v in symmap.items():
    if ':' in k and len(v) == 1:
        known_bases.setdefault(k.split(':')[1], set()).add(int(next(iter(v)), 16))

def blob_start(sec, end):
    nb = [b for b in known_bases.get(sec, ()) if b >= end]
    if nb and min(nb) - end < 0x40:
        return min(nb)
    return end

errors = []
# ---- text
text = ["        - [0x0,      asm, sdk/crt0]", "        - [0xC8,     asm, sdk/libgcc]"]
grp = None
for f in bounds:
    n = f['name']
    if n.startswith(('sdk/', 'nalib/', 'prlib/')) or f['pal'] is None: continue
    g = n.split('/')[0]
    if g != grp: text.append(""); grp = g
    typ = jtext[n] if n in convert else 'asm'
    text.append(f"        - [{ROM(f['pal']):#x}, {typ}, {n}]")
text += ["", "        # PAL-only: MPEG movie playback (not in the prototype)",
         "        - [0x3f8c0, asm, pal/movie]", "",
         "        # TODO: split prlib/nalib/SDK per file",
         "        - [0x43200, asm, pal/prlib]"]

# ---- data-like sections
sec_lines = {}
for sec in SECS:
    lo = {'.data': 0x188200, '.rodata': 0x377280, '.lit4': 0x382d00, '.sdata': 0x383100,
          '.sbss': 0x383700, '.bss': 0x383980}[sec]
    ents = []  # (vram, type, name)
    for n in convert:
        t = jtypes.get(n, {}).get(sec)
        if t is None:
            continue
        sz = obj_sizes(n).get(sec, 0)
        b = base(n, sec)
        if b is None:
            errors.append(f"{n} {sec}: no unique PAL base"); continue
        ents.append((b, t, n, sz))
    ents.sort()
    out = []
    cur = lo
    blob = sec.lstrip('.')
    for b, t, n, sz in ents:
        if b < cur: errors.append(f"{n} {sec}: overlaps previous ({b:#x} < {cur:#x})"); continue
        if b > cur: out.append((cur, blob, f"pal/{blob}_{cur:x}"))
        out.append((b, t, n)); cur = blob_start(sec, b + sz)
    if cur < SECTION_END[sec]: out.append((cur, blob, f"pal/{blob}_{cur:x}"))
    sec_lines[sec] = out

def fmt(sec, ents):
    L = []
    for v, t, n in ents:
        if sec in ('.sbss', '.bss'):
            L.append(f"        - {{ type: {t}, vram: {v:#010x}, name: {n} }}")
        else:
            L.append(f"        - [{ROM(v):#x}, {t}, {n}]")
    return L

body = ["segments:", "    - name: main", "      type: code", "      start: 0x0", "      vram: 0x100000",
        "      bss_size: 0x18fec34", "      subsegments:", "        # .text"] + text + [
        "", "        # .vutext", "        - { start: 0x85960, type: vutext, use_src_path: True, name: prlib/vu1/vump_common }"]
for sec in ['.data', '.rodata']:
    body += ["", f"        # {sec}"] + fmt(sec, sec_lines[sec])
    if sec == '.rodata':
        body += ["", "        # .gcc_except_table", "        - [0x282b80, rodata, gcc_except_table]"]
for sec in ['.lit4', '.sdata', '.sbss', '.bss']:
    body += ["", f"        # {sec}"] + fmt(sec, sec_lines[sec])
body += ["", "    - name: mfifo", "      type: code", "      start: 0x283700", "      vram: 0x01C90000",
         "      subalign: 0x10000", "      follows_vram: main", "      subsegments:",
         "        - [0x283700, mfifo, prlib/mfifo]", "", "    - [0x293700]", ""]
open('config/p3.pal.yaml', 'w').write(header + "\n".join(body))
for e in errors: print("warning:", e)
print("wrote config/p3.pal.yaml;", len(convert), "files as C")
