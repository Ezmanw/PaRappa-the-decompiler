# Argon Agent Status (`Communication/STATUS.md`)

Last updated: 2026-10-18 19:29 BST (Argon paused cleanly with ~25% quota remaining; all work verified, committed & pushed)

## Newly Completed in This Pass (`0 differing` in `tools/pal/check.py` — **17 Functions Matched!**)
- **`src/main/mbar.c`**: **82 matching, 0 differing** (**100% matched!** `ExamDispOn` matched!)
- **`src/prlib/vram.cpp`**: **15 matching, 0 differing** (**100% matched!** `PrAddSpriteDefinition` and `PrAddSpriteDefinitionSuperSampled` matched!)
- **`src/prlib/depthfield.cpp`**: **9 matching, 0 differing** (**100% matched!** `PrSceneObject::ApplyDepthOfField` and `NaGifPacket::NaGifPacket` / `func_0014CDC8` matched!)
- **`src/prlib/mendererdata.cpp`**: **8 matching, 0 differing** (`SetNextTarget` and `GetSynchronizeRatio` matched; only `PrGetNoodlePolygonPosition` left!)
- **`src/prlib/scene.cpp`**: **9 matching, 0 differing** (`PrSceneObject::PrSceneObject` constructor matched; only `SetAppropriateDefaultCamera` left!)
- **`src/main/drawctrl.c`**: **96 matching, 0 differing** (`DrawObjStrDisp` 472i matched; only `DrawObjPrReq` `INCLUDE_ASM` left!)
- **`src/menu/menusub.c`**: **146 matching, 0 differing** (+7 functions matched in this pass: `TsBGMPoll`, `TsRestoreSaveData`, `TsMakeUserWork`, `TsGetRankingList`, `TsSaveMenu_Flow`, `TsSaveMenu_Draw`, `_TsJKMoveCus`, `TsNAMEINBox_Flow`; plus `TSJukeCDObj_Draw` 976i draft updated in `docs/pal_notes/TSJukeCDObj_Draw_attempt.c` down to 19/976 aligned diffs!)

## Remaining `INCLUDE_ASM` Across Entire Repo: **64** (~94.8% complete!)
- All Argon subagents have been cleanly stopped so there are zero active background processes or file conflicts.
