#!/usr/bin/env python3
"""Print the PAL disassembly of a function by name (uses config/p3.pal.func_map.json).

usage: tools/pal/func_asm.py NAME [--iso PATH]
"""
import argparse, json, subprocess, os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ap = argparse.ArgumentParser()
ap.add_argument('name')
ap.add_argument('--iso', default=os.environ.get('PAL_ISO', os.path.join(ROOT, 'iso/SCES_504.08')))
a = ap.parse_args()
m = json.load(open(os.path.join(ROOT, 'config/p3.pal.func_map.json')))
idx = {o['name']: i for i, o in enumerate(m)}
if a.name not in idx: raise SystemExit(f"unknown function {a.name}")
i = idx[a.name]; o = m[i]
if o['est'] is None: raise SystemExit(f"{a.name}: no PAL address known")
end = None
for j in range(i + 1, len(m)):
    if m[j]['est'] is not None and m[j]['conf'] in ('anchored', 'estimate'):
        end = m[j]['est']; break
if end is None or end - o['est'] > 4 * (o['proto_size'] or 0x4000):
    end = o['est'] + (o['proto_size'] or 0x400) + 0x40
print(f"# {o['name']} ({o['file']}) PAL {o['est']:#x}..{end:#x} [{o['conf']}] proto {o['proto']:#x} size {o['proto_size']:#x}")
out = subprocess.run(['mips-linux-gnu-objdump', '-d', '-z', '-m', 'mips:5900', '-EL', '-j', '.text',
                      f'--start-address={o["est"]:#x}', f'--stop-address={end:#x}', a.iso],
                     capture_output=True, text=True).stdout
print('\n'.join(l for l in out.splitlines() if l.startswith('  ')))
