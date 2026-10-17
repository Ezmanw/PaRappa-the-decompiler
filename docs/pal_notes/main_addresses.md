# main/subt/effect address issues (Sonnet pass)
- Most remaining main.c/subt.c/effect.c DIFFs have bad [estimate] addresses, not code differences.
- selPlayDispType real start ~0x11fc90 (estimate 0x11fc88); PAL adds a 4th param (2048) -> header change needed.
- codeKanjiCheck, UG_FadeDisp (likely UG_FadeDisp2, UG_NoodlesDisp), dbg_select_disp, dummyPlay: mapped to wrong functions.
- euc2sjis maps to a zero-length range.
