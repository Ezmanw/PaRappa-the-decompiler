#!/usr/bin/env python3
"""Align prototype symbols to PAL addresses using fingerprint hits + link order."""
import json, re, sys, bisect

PROTO_SYMS = 'config/p3.jul12.symbol_addrs.txt'
hits = json.load(open(sys.argv[1]))

proto = []  # (addr, name, file)
cur = None
for line in open(PROTO_SYMS, encoding='utf-8'):
    m = re.match(r'//\s*(\S+)', line)
    if m: cur = m.group(1); continue
    m = re.match(r'(\S+)\s*=\s*(0x[0-9a-fA-F]+);.*type:func', line)
    if m: proto.append((int(m.group(2), 16), m.group(1), cur))
proto.sort()

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
