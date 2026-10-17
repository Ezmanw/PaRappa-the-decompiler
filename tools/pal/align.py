#!/usr/bin/env python3
"""Align prototype symbols to PAL addresses using fingerprint hits + link order."""
import json, re, sys, bisect

PROTO_SYMS = 'config/p3.jul12.symbol_addrs.txt'
hits = json.load(open(sys.argv[1]))

import glob
proto_by_name = {}
cur = None
for line in open(PROTO_SYMS, encoding='utf-8'):
    m = re.match(r'//\s*(\S+)', line)
    if m: cur = m.group(1); continue
    m = re.match(r'(\S+)\s*=\s*(0x[0-9a-fA-F]+);.*type:func', line)
    if m: proto_by_name[m.group(1)] = (int(m.group(2), 16), m.group(1), cur)

for f in sorted(glob.glob('src/**/*.c', recursive=True) + glob.glob('src/**/*.cpp', recursive=True)):
    if 'src/prlib/old' in f or 'src/iop_mdl/wp2cd/iop' in f: continue
    rel = f[4:]
    txt = open(f, encoding='utf-8', errors='ignore').read()
    for m in re.finditer(r'^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)', txt, re.M):
        fn = m.group(1)
        if fn not in proto_by_name:
            m2 = re.match(r'func_([0-9A-Fa-f]{8})$', fn)
            if m2:
                proto_by_name[fn] = (int(m2.group(1), 16), fn, rel)

proto = sorted(proto_by_name.values())

# candidates in proto order
seq = [(pa, n, f, [int(h, 16) for h in hits.get(n, {}).get('hits', [])]) for pa, n, f in proto]

# LIS on unique hits to drop spurious ones
uniq = [(i, s[3][0]) for i, s in enumerate(seq) if len(s[3]) == 1]
tails, tidx, prev = [], [], [-1]*len(uniq)
for k, (i, a) in enumerate(uniq):
    j = bisect.bisect_left(tails, a)
    if j == len(tails): tails.append(a); tidx.append(k)
    else: tails[j] = a; tidx[j] = k
    prev[k] = tidx[j-1] if j else -1
k = tidx[-1]; keep = {}
while k != -1:
    keep[uniq[k][0]] = uniq[k][1]; k = prev[k]

# resolve ambiguous using neighbouring anchors
idx = sorted(keep)
for i, s in enumerate(seq):
    if i in keep or len(s[3]) < 2: continue
    j = bisect.bisect_left(idx, i)
    lo = keep[idx[j-1]] if j > 0 else 0
    hi = keep[idx[j]] if j < len(idx) else 1 << 32
    c = [a for a in s[3] if lo < a < hi]
    if len(c) == 1: keep[i] = c[0]
    elif len(c) > 1:  # pick closest to predicted delta
        d = (lo - seq[idx[j-1]][0]) if j > 0 else 0
        keep[i] = min(c, key=lambda a: abs(a - (s[0] + d)))

out = []
for i, (pa, n, f, _) in enumerate(seq):
    out.append({'name': n, 'file': f, 'proto': pa, 'pal': keep.get(i)})
json.dump(out, open(sys.argv[2], 'w'), indent=1)
print(f"proto funcs={len(seq)} anchored={len(keep)} dropped_uniq={len(uniq)-len([u for u in uniq if u[0] in keep])}")
# per-file summary of delta
from collections import OrderedDict
fs = OrderedDict()
for o in out:
    fs.setdefault(o['file'], []).append(o)
for f, L in fs.items():
    a = [o for o in L if o['pal']]
    ds = sorted(set(o['pal'] - o['proto'] for o in a))
    print(f"{str(f):34s} n={len(L):4d} anchored={len(a):4d} deltas={[hex(d) for d in ds[:4]]}{'...' if len(ds)>4 else ''}")

# Estimate PAL addresses for unanchored functions using DP over 8-byte aligned function starts
# between neighboring anchors, falling back to nearest-anchor delta.
import numpy as np
from elftools.elf.elffile import ELFFile
_t = ELFFile(open('iso/SCES_504.08', 'rb')).get_section_by_name('.text')
_base, _w = _t['sh_addr'], np.frombuffer(_t.data(), dtype='<u4')
def _starts_func(a):
    i = (a - _base) // 4
    return (a % 8 == 0) and 0 <= i < len(_w) and i >= 2 and (_w[i-2] == 0x03e00008 or _w[i-1] == 0x03e00008 or _w[i-1] == 0)

for i, o in enumerate(out):
    o['proto_size'] = (out[i+1]['proto'] - o['proto']) if i + 1 < len(out) else None
    if o['pal'] is not None:
        o['est'] = o['pal']; o['conf'] = 'anchored'; continue
    d = None
    for j in range(i - 1, -1, -1):
        if out[j]['file'] != o['file']: break
        if out[j]['pal'] is not None: d = out[j]['pal'] - out[j]['proto']; break
    if d is None:
        for j in range(i + 1, len(out)):
            if out[j]['file'] != o['file']: break
            if out[j]['pal'] is not None: d = out[j]['pal'] - out[j]['proto']; break
    xfile = False
    if d is None and o['file'] and '/' in o['file'] and not o['file'].startswith('sce/'):
        for j in range(i - 1, -1, -1):  # fall back to the previous file's anchor
            if out[j]['pal'] is not None: d = out[j]['pal'] - out[j]['proto']; xfile = True; break
    if d is None:
        o['est'] = None; o['conf'] = 'none'; continue
    o['est'] = o['proto'] + d
    o['conf'] = ('estimate' if _starts_func(o['est']) else 'estimate?') + ('-xfile' if xfile else '')

anchors = [i for i, o in enumerate(out) if o['pal'] is not None]
for a_idx in range(len(anchors) - 1):
    lo_i, hi_i = anchors[a_idx], anchors[a_idx + 1]
    if hi_i == lo_i + 1: continue
    lo_sz = hits.get(out[lo_i]['name'], {}).get('size') or (out[lo_i]['proto_size'] or 0x40)
    gap_lo = (out[lo_i]['pal'] + lo_sz + 7) & ~7
    gap_hi = out[hi_i]['pal']
    if gap_lo <= 0x13f8c0 < gap_hi or gap_lo >= gap_hi:
        continue
    cands = [a for a in range(gap_lo, gap_hi, 8) if a == gap_lo or _starts_func(a)]
    K = hi_i - lo_i - 1
    if len(cands) >= K and K <= 50:
        ps = [out[lo_i + 1 + m]['proto_size'] or 0x40 for m in range(K)]
        C = cands + [gap_hi]
        M = len(cands)
        INF = 10**18
        dp = [[INF] * M for _ in range(K)]
        prv = [[-1] * M for _ in range(K)]
        for j in range(M):
            dp[0][j] = (C[j] - gap_lo) * 4
        for m in range(1, K):
            for j in range(m, M):
                for p in range(m - 1, j):
                    cost = dp[m-1][p] + abs((C[j] - C[p]) - ps[m-1])
                    if cost < dp[m][j]:
                        dp[m][j] = cost; prv[m][j] = p
        best_j = min(range(K - 1, M), key=lambda j: dp[K-1][j] + abs((gap_hi - C[j]) - ps[K-1]))
        chosen = [0] * K
        cur_j = best_j
        for m in range(K - 1, -1, -1):
            chosen[m] = C[cur_j]
            cur_j = prv[m][cur_j]
        for m in range(K):
            o = out[lo_i + 1 + m]
            o['est'] = chosen[m]
            o['conf'] = 'estimate' if _starts_func(chosen[m]) else 'estimate?'

json.dump(out, open(sys.argv[2], 'w'), indent=1)
