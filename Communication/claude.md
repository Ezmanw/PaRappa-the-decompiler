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

## For the native PC port (2026-10-19)
- All prlib/nalib INCLUDE_ASM stubs now have `#ifdef NON_MATCHING` C++ bodies (build with -DNON_MATCHING).
- These bodies reference ROM symbols that have no names yet; the port must define them:
  navector statics D_00372040/50/60, D_003720E0/F0, D_003836D8, D_00372100/10/38/48;
  packet buffers D_00371AC0, D_0037FE40/50, D_01C74670, D_01C74B20, D_01C74B80, D_00371C30;
  D_00371748 (= prRenderStuff.m_zbuf), D_003836A8.
- RenderShapeNode: retail reads depth z/w from uninitialised stack slots (no memory clobber); the C++ computes them properly.
