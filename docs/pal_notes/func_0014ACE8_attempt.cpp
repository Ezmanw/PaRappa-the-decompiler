/* Best non-matching PAL attempt. Drop into src/prlib/spadata.cpp in place of the
 * INCLUDE_ASM (spadata.cpp needs `#include <float.h>` and `#include "vu0mat.h"`).
 * func_0014ACE8 (0x14e2b8, address OK): axis rotation (0=X,1=Y,else Z) -> func_00145DB0.
 * 188 vs 190 instrs, ~23 differ, all in the third (Z) branch: target re-loads s from its
 * stack slot in the bnel delay slot (lwc1 $f0,104(sp)) and negates inside the branch; ours
 * shares the -s neg across branches 2/3 (in the bne delay slot). switch/if-return/array/ref
 * variants tried, none better.
 */
extern NaMATRIX<float, 4, 4>* SetMat44Raw(NaMATRIX<float, 4, 4>* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15) asm("func_00145DB0");

static inline NaMATRIX<float, 4, 4>* SetMat44(NaMATRIX<float, 4, 4>* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15) {
    PR_VU0_WAIT();
    return SetMat44Raw(self, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

NaMATRIX<float, 4, 4>* func_0014ACE8(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) asm("func_0014ACE8");
NaMATRIX<float, 4, 4>* func_0014ACE8(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) {
    float c = cosf(*angle);
    float s = sinf(*angle);

    if (axis == 0) {
        SetMat44(self,
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    } else if (axis == 1) {
        SetMat44(self,
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        SetMat44(self,
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }
    return self;
}

/* --- Update (Opus pass 2) ---
 * Root cause of the Z-branch diff found: cse2 (rerun after loop) follows the bnez/bne jumps
 * (-fcse-follow-jumps) and replaces each `s` load with $f0 in ALL three branches. In the target the
 * Z branch is NOT on a followed path, so it reloads s (lwc1 104(sp)) and reorg puts that load in a
 * bnel delay slot (NE predicted taken). cse only follows a jump when LABEL_NUSES(target)==1 and the
 * label is preceded by a BARRIER, so the original likely had a second reference to the Z label
 * (e.g. switch `case 2: default:` gives the reload but a tree dispatch, 184/190 eq).
 * Volatile-read hack below gives 186/190 aligned-equal (only the bnel/bne delay slot + nop shift differ,
 * because a volatile load can't go in a delay slot). Not a match.
 */
#if 0
NaMATRIX<float, 4, 4>* func_0014ACE8(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) asm("func_0014ACE8");
NaMATRIX<float, 4, 4>* func_0014ACE8(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) {
    float c = cosf(*angle);
    float s = sinf(*angle);
if (axis == 0) { SetMat44(self, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, c, s, 0.0f, 0.0f, -s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); } else if (axis == 1) { SetMat44(self, c, 0.0f, -s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, s, 0.0f, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); } else { float ns = -*(volatile float*)&s; SetMat44(self, c, s, 0.0f, 0.0f, ns, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
    return self;
}
#endif
