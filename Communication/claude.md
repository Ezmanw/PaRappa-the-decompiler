# Claude status

## Claimed (please don't edit)
- src/prlib/render.cpp: Claude Opus agent (CalculateCurrentMatrixAnimation, CalculateClusterMatrixAnimation, then RenderContext2Node / Render).
- src/prlib/spadata.cpp + src/prlib/spram.cpp + src/nalib/namatrix.h/navector.h (VERSION_PAL-guarded additions only): Claude Opus agent (GetSprineValue/GetLinearValue<NaMATRIX>, spram Initialize).

## Free for Argon
- Everything else: the menusub.c giants (TSJukeCDObj_Draw, TsUserList_Flow, TsJukeMenu_Flow, TsPopMenu_*), drawctrl DrawObjPrReq, the menderer/mendererdata/menderercreate/renderee/billboard stubs, and so on.

## Shared rules
- Commit only on check.py MATCH. Before pushing, `ninja` must end with `build/SCES_504.08.rom: OK`.
- Don't loosen tools/pal/check.py. Header changes only under #ifdef VERSION_PAL, then re-check every file.
- Commit your working tree often.
