#!/usr/bin/env bash
# Rebuild reference objects (VERSION_PAL, INCLUDE_ASM stubbed out) and regenerate
# the PAL function map, relocation symbol map, boundaries and symbol_addrs.
set -eu
cd "$(dirname "$0")/../.."
export PATH=$PWD/.venv/bin:$PATH
I="-Iinclude -Isrc -Iinclude/rtl/common -Iinclude/rtl/ee -Iinclude/rtl/ee_gcc -Iinclude/rtl/ee_gcc/gcc-lib"
mkdir -p pal_work/obj pal_work/tmp
for f in $(find src -path src/iop_mdl/wp2cd/iop -prune -o -path src/prlib/old -prune -o \( -name '*.c' -o -name '*.cpp' \) -print); do
    o=pal_work/obj/$(echo $f | sed 's|^src/||; s|/|__|g').o
    if [[ $f == *.cpp ]]; then fl="-O2 -G8 -x c++ -fno-exceptions -fno-strict-aliasing"; else fl="-O2 -G8 -g"; fi
    t=pal_work/tmp/$(basename $f); iconv -f UTF-8 -t EUC-JP $f > $t
    tools/toolchain/ee-gcc29/bin/ee-gcc -c $I -I$(dirname $f) $fl -DPERMUTER -DVERSION_PAL -include include/version.h -Wa,-Iinclude $t -o $o 2>pal_work/tmp/err.txt || echo "compile failed: $f"
done
python -I tools/pal/fingerprint.py iso/SCES_504.08 pal_work/obj pal_work/matches.json
python -I tools/pal/align.py pal_work/matches.json pal_work/align.json > /dev/null
cp pal_work/align.json config/p3.pal.func_map.json
python -I tools/pal/funcstarts.py iso/SCES_504.08 pal_work/funcstarts.json
python -I tools/pal/boundaries.py > /dev/null
python -I tools/pal/symmap.py pal_work/obj
python -I tools/pal/gen_syms.py
