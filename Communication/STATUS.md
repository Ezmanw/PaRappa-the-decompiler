# Argon Agent Status (`Communication/STATUS.md`)

Last updated: 2026-10-18 18:40 BST (Argon resumed; skipping Claude-claimed `render.cpp`, `spadata.cpp`, `spram.cpp`, `nalib` headers)

## Currently Claimed by Argon (In Progress)
- **`src/menu/menusub.c`**: Remaining 26 `INCLUDE_ASM` functions (`TSJukeCDObj_Draw`, `TsUserList_Flow`, `TsJukeMenu_Flow`, `TsPopMenu_Flow`, `TsPopMenu_Draw`, `McUserSaveFlow`, `McUserCheckFlow`, `TsPopCusPut`, `TsUserList_Draw`, `TsNAMEINBox_Draw`, `TsUser_PanelDraw`, `TsOption_Flow`, `TsNAMEINBox_Flow`, `TsRestoreSaveData`, `McUserLoadFlow`, `TsSetSaveData`, `TsSetScene_Map`, `TsBGMPoll`, `TsCmnCell_CusorDraw`, `TsPopMenCus_Draw`, `TsClearSet`, `TsSaveMenu_Flow`, `TsGetRankingList`, `TsMakeUserWork`, `TsOption_Draw`, `_TsJKMoveCus`) + `TsSaveMenu_Draw` PAL diff.
- **`src/main/drawctrl.c`**: `DrawObjPrReq` + `DrawObjStrDisp` (11/472 diff).
- **`src/main/mbar.c`**: `ExamDispOn` + remaining PAL diffs.
- **`src/prlib/*` (non-Claude)**: `menderer.cpp`, `menderercreate.cpp`, `mendererdata.cpp`, `mendereralpha.cpp`, `mendererawful.cpp`, `vram.cpp`, `depthfield.cpp`, `billboard.cpp`, `camera.cpp`, `cluster.cpp`, `contour.cpp`, `shape.cpp`, `renderee.cpp`, `scene.cpp`, `setpointer.cpp`, `transition.cpp`.

## Skipped (Claimed by Claude in `Communication/claude.md`)
- `src/prlib/render.cpp`
- `src/prlib/spadata.cpp`
- `src/prlib/spram.cpp`
- `src/nalib/namatrix.h` / `src/nalib/navector.h`
