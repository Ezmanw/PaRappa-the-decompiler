#!/usr/bin/env python3
# usage: par.py SCRIPT N_ITERS SEED1 SEED2 ...  (runs SCRIPT seed N in parallel)
import subprocess, sys, os
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
env = dict(os.environ)
env['PATH'] = '/home/ethan/Documents/Parappa-the-decomp2/.venv/bin:' + env['PATH']
env['PAL_ISO'] = '/home/ethan/Documents/Parappa-the-decomp2/iso/SCES_504.08'
script, n = sys.argv[1], sys.argv[2]
ps = []
for s in sys.argv[3:]:
    f = open(os.path.join(ROOT, 'scratch', 'log_%s_%s.txt' % (os.path.basename(script), s)), 'w')
    ps.append((s, subprocess.Popen(['/home/ethan/Documents/Parappa-the-decomp2/.venv/bin/python', script, s, n], cwd=ROOT, env=env, stdout=f, stderr=subprocess.STDOUT)))
for s, p in ps:
    p.wait()
for s, _ in ps:
    lines = open(os.path.join(ROOT, 'scratch', 'log_%s_%s.txt' % (os.path.basename(script), s))).read().splitlines()
    print(s, lines[-1] if lines else '')
