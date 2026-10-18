# Argon Agent Status (`Communication/STATUS.md`)

Last updated: 2026-10-18 10:13 BST

## Claimed by Claude (Untouched by Argon)
- `src/prlib/render.cpp`

---

## Newly Completed (`0 differing` in `tools/pal/check.py`)
- **`src/main/cdctrl.c`**: **48 matching, 0 differing** (100% matched!)
- **`src/main/wipe.c`**: **36 matching, 0 differing** (100% matched!)
- **`src/main/effect.c`**: **14 matching, 0 differing** (100% matched — `UG_FadeDisp`, `UG_FadeDisp2`, `UG_NoodlesDisp` all matched!)
- **`src/menu/p3mc.c`**: **45 matching, 0 differing** (100% of compiled functions matched!)
- **`src/main/drawctrl.c`**: **95 matching, 1 differing** (+13 PAL functions matched; only unmapped `Cl2MixTrans` remains in compiled functions!)
- **`src/prlib/menderer.cpp`**: **21 matching, 0 differing** (+6 new functions matched!)
- **`src/prlib/spadata.cpp`**: **15 matching, 0 differing** (+2 new functions matched!)

---

## Active Argon Subagents (6 Running)
1. **Menusub Part 1 Decompiler** (`src/menu/menusub.c` lines 1–3400):
   - Matched 6 PAL diffs (`TSSNDPLAY`, `TSSND_SKIPSTOP`, `TSSND_SKIPPLAY`, `_P3DATA_SIZE`, `TsMenu_Draw`, `TsMenu_End`).
   - Now working through `TsMCAMes_Draw`, `TsMakeUserWork`, `TsGetRankingList`, `TsClearSet`, `TsBGMPoll`, `TsSetScene_Map`, `TsSetSaveData`, `McUser*Flow`, `Mp*Flow` (capped at 4 tries per function).
2. **Menusub Part 2 Decompiler** (`src/menu/menusub.c` lines 3401–4150):
   - Matched `_PkMCMsgPut`, `_PkMCMsgGetLine`, `_PkSubMsgPut` (`menusub.c` is at **130 matching, 4 differing**).
   - Now working through `TsOption_Draw`, `TsSaveMenu_Flow`, `TsPopMenCus_Draw`, `TsCmnCell_CusorDraw`, `TsNAMEINBox_Flow`, `TsOption_Flow`, `TsUser_PanelDraw`, `TsNAMEINBox_Draw`, `TsUserList_Draw`, `TsPopCusPut`, `TsPopMenu_Draw`, `TsPopMenu_Flow`, `TsJukeMenu_Flow`, `TsUserList_Flow`, `TSJukeCDObj_Draw` (capped at 4 tries per function).
3. **Main & Drawctrl Decompiler**:
   - Finished `effect.c` (14/14), `p3mc.c` (45/45), and `drawctrl.c` PAL diffs (95/96); now working on `src/main/main.c` PAL diffs + `DrawObjPrReq` / `DrawObjStrDisp` / `ExamDispOn`.
4. **Spadata & Scene Decompiler**:
   - Matched 2 more in `spadata.cpp` (15/15) and 3 in `spram.cpp` (5/6); continuing through remaining `spadata.cpp`, `spram.cpp`, `scene.cpp`, `setpointer.cpp`, `transition.cpp`.
5. **Menderer & Nodes Decompiler**:
   - Matched 6 more in `menderer.cpp` (21/21); continuing through `mendereralpha.cpp` (`PrCreateAlphaModulation` down to 2/101 diff), `menderercreate.cpp`, `mendererdata.cpp`, `vram.cpp`, and node renderers.
6. **PAL Movie.c Decompiler**:
   - Expanded `src/pal/movie.c` from 429 lines to **2,031 lines** (`+1,610` lines of C written for `videoDec*`, `voBuf*`, `readBuf*`, `strFile*`, and top-level movie functions), now matching `.rodata` and instruction diffs against `iso/SCES_504.08`.
