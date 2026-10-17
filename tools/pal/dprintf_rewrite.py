#!/usr/bin/env python3
"""Rewrite printf(...) calls whose format string does not exist in the PAL
executable into DPRINTF(...), which compiles out for VERSION_PAL only."""
import re, glob, sys
pal = open('iso/SCES_504.08', 'rb').read()
pat = re.compile(r'(?<![\w.>])printf(\s*\(\s*"((?:[^"\\]|\\.)*)")')
changed = 0
for f in sorted(glob.glob('src/**/*.c', recursive=True) + glob.glob('src/**/*.cpp', recursive=True)):
    if '/iop/' in f: continue
    s = open(f, encoding='utf-8').read()
    def repl(m):
        global changed
        lit = m.group(2).encode('utf-8').decode('unicode_escape').encode('latin1', 'ignore')
        try:
            enc = m.group(2).encode('euc-jp').decode('unicode_escape', errors='ignore').encode('latin1', 'ignore')
        except Exception:
            enc = lit
        if lit in pal or enc in pal:
            return m.group(0)
        changed += 1
        return 'DPRINTF' + m.group(1)
    n = pat.sub(repl, s)
    if n != s:
        open(f, 'w', encoding='utf-8').write(n)
        print(f)
print(changed, 'calls rewritten')
