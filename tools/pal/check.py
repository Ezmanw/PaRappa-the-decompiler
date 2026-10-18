#!/usr/bin/env python3
"""Compile one source file and check which of its functions match the PAL executable.

usage: tools/pal/check.py src/prlib/prlib.cpp [FUNC ...]
Functions still under INCLUDE_ASM are skipped (compiled with -DPERMUTER).
A function "matches" when its instructions (relocated fields masked) are found
at its expected PAL address.
"""
import sys, os, subprocess, json, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fingerprint as fp
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ISO = os.environ.get('PAL_ISO', os.path.join(ROOT, 'iso/SCES_504.08'))
src = sys.argv[1]; want = set(sys.argv[2:])
inc = "-Iinclude -Isrc -Iinclude/rtl/common -Iinclude/rtl/ee -Iinclude/rtl/ee_gcc -Iinclude/rtl/ee_gcc/gcc-lib".split()
flags = "-O2 -G8 -x c++ -fno-exceptions -fno-strict-aliasing".split() if src.endswith('.cpp') else "-O2 -G8 -g".split()
tmp = tempfile.mkdtemp()
c = os.path.join(tmp, os.path.basename(src))
subprocess.run(f"iconv -f UTF-8 -t EUC-JP {src} > {c}", shell=True, check=True)
obj = os.path.join(tmp, 'out.o')
r = subprocess.run([os.path.join(ROOT, 'tools/toolchain/ee-gcc29/bin/ee-gcc'), '-c', *inc, f'-I{os.path.dirname(src)}', *flags,
                    '-DPERMUTER', '-DVERSION_PAL', '-include', 'include/version.h', '-Wa,-Iinclude', c, '-o', obj], cwd=ROOT, capture_output=True, text=True)
if r.returncode: print(r.stderr); sys.exit(1)
fmap = {o['name']: o for o in json.load(open(os.path.join(ROOT, 'config/p3.pal.func_map.json')))}
base, text = fp.load_target(ISO)
ok = bad = 0
for name, words, mask in fp.obj_funcs(obj):
    if want and name not in want: continue
    e = fmap.get(name, {}).get('est')
    if e is None: print(f"  ?    {name}: no PAL address"); continue
    i = (e - base) // 4; L = len(words)
    seg = text[i:i+L]
    np = fp.np
    match = len(seg) == L and np.array_equal(seg & mask, words)
    if match: ok += 1
    else:
        nd = int(((seg & mask) != words).sum()) if len(seg) == L else L + 1
        best_nd, best_e = nd, e
        exact = []
        for di in range(-256, 257, 2):
            if i + di < 0 or i + di + L > len(text): continue
            s2 = text[i+di:i+di+L]
            d2 = int(((s2 & mask) != words).sum())
            if d2 == 0: exact.append(e + di * 4)
            if d2 < best_nd:
                best_nd, best_e = d2, e + di * 4
        # A relocated match only counts if it is unambiguous: masked relocations
        # make short or repetitive functions match unrelated code nearby.
        if best_nd == 0 and len(exact) == 1 and L >= 8:
            ok += 1
            print(f"  MATCH {name} @ {best_e:#x} (map says {e:#x} -- fix config/p3.pal.func_map.json)")
        elif best_nd == 0:
            bad += 1
            print(f"  AMBIG {name}: {len(exact)} nearby exact hits, {L} instrs -- not counted; fix the map address")
        else:
            bad += 1
            loc = f"{best_e:#x} (est {e:#x})" if best_e != e and best_nd < L // 2 else f"{e:#x}"
            print(f"  DIFF {name} @ {loc}: {best_nd if best_e != e and best_nd < L // 2 else (nd if nd <= L else -1)}/{L} instructions differ")
print(f"{src}: {ok} matching, {bad} differing")
