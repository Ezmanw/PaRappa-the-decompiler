#!/usr/bin/env python3
# usage: d.py SRC FUNC [ADDR [TARGETLEN]] [-q]  -> aligned diff
import sys, os, subprocess, json, tempfile, difflib, re
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT + '/tools/pal')
import fingerprint as fp
import numpy as np
src, func = sys.argv[1], sys.argv[2]
quiet = '-q' in sys.argv or '-m' in sys.argv
args = [a for a in sys.argv[3:] if a not in ('-q','-m')]
ISO = os.environ['PAL_ISO']
inc = "-Iinclude -Isrc -Iinclude/rtl/common -Iinclude/rtl/ee -Iinclude/rtl/ee_gcc -Iinclude/rtl/ee_gcc/gcc-lib".split()
flags = "-O2 -G8 -x c++ -fno-exceptions -fno-strict-aliasing".split() if src.endswith('.cpp') else "-O2 -G8 -g".split()
tmp = tempfile.mkdtemp()
c = os.path.join(tmp, os.path.basename(src))
subprocess.run(f"iconv -f UTF-8 -t EUC-JP {src} > {c}", shell=True, check=True)
obj = os.path.join(tmp, 'out.o')
r = subprocess.run([ROOT + '/tools/toolchain/ee-gcc29/bin/ee-gcc', '-c', *inc, f'-I{ROOT}/src/prlib', *flags,
                    '-DPERMUTER', '-DVERSION_PAL', '-include', 'include/version.h', '-Wa,-Iinclude', c, '-o', obj],
                   cwd=ROOT, capture_output=True, text=True)
if r.returncode:
    print(r.stderr); sys.exit(1)
fmap = {o['name']: o for o in json.load(open(ROOT + '/config/p3.pal.func_map.json'))}
base, text = fp.load_target(ISO)
mine = None
for name, words, mask in fp.obj_funcs(obj):
    if name == func: mine = (words, mask)
if mine is None:
    print("not found"); sys.exit(1)
words, mask = mine
e = int(args[0], 16) if args else fmap[func]['est']
TL = int(args[1]) if len(args) > 1 else len(words)
tgt = text[(e - base) // 4:(e - base) // 4 + TL]

def dis(ws):
    f = os.path.join(tmp, 'b.bin'); np.asarray(ws, dtype='<u4').tofile(f)
    out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:5900', '-EL', '-z', f],
                         capture_output=True, text=True).stdout
    res = []
    for l in out.splitlines():
        m = re.match(r'\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(.*)', l)
        if m: res.append(re.sub(r'\s+', ' ', m.group(1)).strip())
    return res

A = dis(tgt); B = dis(words)
if "-m" in sys.argv: print("\n".join(f"{i:4d} {b}" for i,b in enumerate(B)))
ta = [a.split(' ')[0] for a in A]; tb = [b.split(' ')[0] for b in B]
sm = difflib.SequenceMatcher(None, ta, tb, autojunk=False)
eq = 0; diffs = 0
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == 'equal':
        for k in range(i2 - i1):
            if (int(tgt[i1 + k]) & int(mask[j1 + k])) == int(words[j1 + k]):
                eq += 1
            else:
                diffs += 1
                if not quiet: print(f"~ {i1+k:4d} {A[i1+k]:40s} | {B[j1+k]}")
    else:
        diffs += max(i2 - i1, j2 - j1)
        if not quiet:
            for k in range(max(i2 - i1, j2 - j1)):
                a = A[i1 + k] if i1 + k < i2 else ''; b = B[j1 + k] if j1 + k < j2 else ''
                print(f"{op[0]} {i1+k:4d} {a:40s} | {b}")
pm = len(tgt) == len(words) and np.array_equal(tgt & mask, words)
print(f"target {len(A)} mine {len(B)} eq {eq} diffscore {diffs} MATCH={pm}")
