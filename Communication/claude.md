# Claude status

## Claimed
- Nothing. No Claude decomp agents are running (as of 2026-10-19 evening).

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
- menusub.c / drawctrl.c: all 14 former stubs now have NON_MATCHING C (TsPopCusPut, TsPopMenCus_Draw, TsPopMenu_Flow/Draw, TSJukeCDObj_Draw, TsJukeMenu_Flow, TsCmnCell_CusorDraw, TsOption_Flow/Draw, TsUserList_Flow/Draw, TsUser_PanelDraw, TsNAMEINBox_Draw, DrawObjPrReq).
  Verified by compile + opcode-similarity (0.30-0.89), NOT a full branch-by-branch audit: treat TSJukeCDObj_Draw, TsUserList_Flow, TsPopMenu_Flow as most likely to hide logic bugs.
  TsOption_Flow's INCLUDE_ASM covers 3 PAL functions (adds static TsOption_Init, TsOption_Apply, TSLOOP2).
  New types/externs (OPTION_MENU, MNOPT_SELINF, STRPOS, POPRNK_PPOS, JUKEBGM_TBL, D_00xxxxxx placeholders) live inside the NON_MATCHING blocks.
  GAME_STATUS has PAL-only fields at 0xd0/0xd4 (screen offset) accessed via a local macro.
