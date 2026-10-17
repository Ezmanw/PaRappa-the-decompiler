#!/usr/bin/env python3
"""Work out PAL .text start addresses for each prototype source file."""
import json, re, bisect
m = json.load(open('config/p3.pal.func_map.json'))
hits = json.load(open('pal_work/matches.json'))
starts = sorted(int(k, 16) for k in json.load(open('pal_work/funcstarts.json')))
y = open('config/p3.jul12.yaml', encoding='utf-8').read()
segs = [(int(a, 16) + 0x100000, t, n) for a, t, n in re.findall(r'- \[(0x[0-9A-Fa-f]+),\s*(asm|c|cpp),\s*([\w/]+)\]', y)]
# keep only real files (drop header pseudo-segments with no functions)
files = []
for k, (pa, typ, name) in enumerate(segs):
    pend = segs[k+1][0] if k + 1 < len(segs) else 1 << 40
    fs = [o for o in m if pa <= o['proto'] < pend]
    files.append({'name': name, 'type': typ, 'proto': pa, 'funcs': fs})
size = lambda o: hits.get(o['name'], {}).get('size')
res = []
prev_end = None
for k, f in enumerate(files):
    fs = f['funcs']
    anc = [i for i, o in enumerate(fs) if o['conf'] == 'anchored']
    if not fs:
        res.append((f, None, 'empty')); continue
    if not anc:
        res.append((f, None, 'no-anchor')); prev_end = None; continue
    i0 = anc[0]; a0 = fs[i0]['pal']
    if i0 == 0:
        start, how = a0, 'anchored'
    else:
        # need i0 function starts before a0 and after prev_end
        cands = [s for s in starts if (prev_end or 0) <= s < a0]
        if len(cands) >= i0:
            start, how = cands[-i0], f'counted({len(cands)} cands for {i0})'
        else:
            start, how = None, f'short({len(cands)} cands for {i0})'
    la = anc[-1]; lo = fs[la]
    prev_end = lo['pal'] + size(lo)
    tail = len(fs) - 1 - la
    res.append((f, start, how + (f' tail_unanchored={tail}' if tail else '')))
for f, s, how in res:
    print(f"{f['name']:24s} {f['type']:4s} proto={f['proto']:#x} pal={s and hex(s)} {how}")
json.dump([{'name': f['name'], 'type': f['type'], 'proto': f['proto'], 'pal': s, 'how': how} for f, s, how in res],
          open('pal_work/boundaries.json', 'w'), indent=1)
