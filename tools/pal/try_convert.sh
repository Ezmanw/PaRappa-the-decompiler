#!/usr/bin/env bash
# Try converting each listed file to C in the PAL build; keep it if the ROM still matches.
# usage: tools/pal/try_convert.sh file1 file2 ...   (log in pal_work/convert_log.txt)
set -u
cd "$(dirname "$0")/../.."
export PATH=$PWD/.venv/bin:$PATH
log=pal_work/convert_log.txt
for f in "$@"; do
    grep -qx "$f" tools/pal/convert.txt && continue
    cp tools/pal/convert.txt /tmp/convert.bak.$$
    echo "$f" >> tools/pal/convert.txt
    python -I tools/pal/gen_yaml.py > pal_work/gen.out 2>&1
    rm -rf asm
    python configure.py --version pal > pal_work/cfg.out 2>&1
    if ninja > pal_work/ninja.out 2>&1; then
        echo "OK   $f" | tee -a $log
    else
        reason=$(grep -m1 -E "error|undefined reference|FAILED|overlaps|warning:" pal_work/gen.out pal_work/cfg.out pal_work/ninja.out | grep -v "cannot find entry" | head -1)
        diffs=$(python -I tools/pal/romdiff.py build/SCES_504.08.rom iso/SCES_504.08.rom iso/SCES_504.08 2>/dev/null | sed -n 2,3p | tr '\n' ' ')
        echo "FAIL $f :: $reason :: $diffs" | tee -a $log
        cp /tmp/convert.bak.$$ tools/pal/convert.txt
    fi
done
# leave the tree configured with the final kept set
python -I tools/pal/gen_yaml.py >/dev/null; rm -rf asm; python configure.py --version pal >/dev/null 2>&1; ninja 2>&1 | tail -1
