# Argon Agent Status (`Communication/STATUS.md`)

Last updated: 2026-10-18 10:45 BST (Argon paused cleanly at 9% quota; all work verified, committed & pushed)

## Newly Completed in This Pass (`0 differing` in `tools/pal/check.py`)
- **`src/main/main.c`**: **17 matching, 0 differing** (100% matched!)
- **`src/main/effect.c`**: **14 matching, 0 differing** (100% matched!)
- **`src/main/wipe.c`**: **36 matching, 0 differing** (100% matched!)
- **`src/main/cdctrl.c`**: **48 matching, 0 differing** (100% matched!)
- **`src/menu/p3mc.c`**: **45 matching, 0 differing** (100% of compiled functions matched!)
- **`src/main/drawctrl.c`**: **95 matching, 1 differing** (+13 PAL diffs matched; `DrawObjStrDisp` is down to **11/472 instructions differing**, saved in `docs/pal_notes/DrawObjStrDisp_attempt.c`!)
- **`src/menu/menusub.c`**: **138 matching, 1 differing** (+17 functions matched in this session; `TSJukeCDObj_Draw` attempt saved in `docs/pal_notes/TSJukeCDObj_Draw_attempt.c` and `TsSaveMenu_Draw` in `docs/pal_notes/TsSaveMenu_Draw_attempt.c`!)
- **`src/prlib/menderer.cpp`**: **21 matching, 0 differing** (+6 functions matched!)
- **`src/prlib/vram.cpp`**: **13 matching, 0 differing** (+12 functions matched, including `PrAddDrawAreaDefinition`, `PrAddTextureAreaDefinition`, and all 10 GS register helpers!)
- **`src/prlib/spram.cpp`**: **7 matching, 0 differing** (+5 functions matched!)
- **`src/prlib/spadata.cpp`**: **15 matching, 0 differing** (+2 functions matched!)
- **`src/prlib/depthfield.cpp`**: **2 matching, 0 differing**
- **`src/pal/movie.c`**: Expanded from 429 lines to **2,024 lines** (**98 functions** decompiled and compiling cleanly with `ee-gcc` covering `audioDec*`, `videoDec*`, `viBuf*`, `voBuf*`, `readBuf*`, `strFile*`, and top-level movie playback!).

## Remaining `INCLUDE_ASM` Across Entire Repo: **89** (73 named + 16 weak template stubs)
- All Argon subagents have been cleanly stopped at 9% quota so Claude can take over any file without conflicts!
