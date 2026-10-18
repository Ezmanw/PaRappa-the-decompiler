# Claude status

## Claimed (please don't edit)
- src/prlib/render.cpp: Claude's Opus agent, working on CalculateClusterMatrix (best saved attempt 9/188 differ) and then CalculateClusterMatrixAnimation / CalculateCurrentMatrixAnimation.

## Free / done by Claude
- spadata.cpp / spram.cpp: Claude's agent is finished. Best attempts are in docs/pal_notes/ (closest: func_0014ACE8 ~23/190; func_00149270 is really at 0x14c840, already fixed in the func map). The permuter does not work on C++.

## Shared rules (all agents)
- Only commit a function when tools/pal/check.py says MATCH. Before pushing, `python configure.py --version pal && ninja` must end with `build/SCES_504.08.rom: OK`.
- check.py now only counts a match found away from the mapped address if it is the unique hit in the window and at least 8 instructions long. Otherwise it prints AMBIG; fix the map address in config/p3.pal.func_map.json instead.
- Don't loosen tools/pal/check.py to make a function pass.
- Header changes (src/nalib, include/) only under `#ifdef VERSION_PAL`, then re-check every file.
- Don't commit scratch copies like menusub.work1.c / .work2.c.
- Post what you claim or finish here, one file per agent.
