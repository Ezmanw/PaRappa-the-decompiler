# spram / spadata near-misses (Sonnet pass)

- func_00148140 (spram, PAL 0x14b710, 66 instrs): scale matrix
  `SetMat44(self, x,0,0,0, 0,y,0,0, 0,0,z,0, 0,0,0,1)`. 13 distinct float locals
  (f1..f4,f6..f9,f11..f14, f15=1.0f) give the right frame (208) and count; ~38 differ,
  all scheduling: target stall loop counts with v1/sentinel v0, `li v1,3` hoisted above
  stores, each store before its addiu.
- func_0014ABE0 (spadata, PAL 0x14e1b0, 66 instrs): identity 4x4, row 3 = 3-float vector,
  w=1, NaMATRIX::Copy to out. ABI: `NaMATRIX<float,4,4>* f(NaMATRIX<float,4,4>* out, const float* v)`.
  PR_VU0_WAIT loop emitted twice (same symptom as func_00147E38/func_00147FB8) — cause unknown.
- PrWaitMendererTexture: callees 0x15d1e0, 0x158860, 0x15a1b8, 0x15a5e0 unnamed (likely SDK).
