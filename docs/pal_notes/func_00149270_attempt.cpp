/* Best non-matching PAL attempt. Drop into src/prlib/spadata.cpp in place of the
 * INCLUDE_ASM (spadata.cpp needs `#include <float.h>` and `#include "vu0mat.h"`).
 * func_00149270: REAL PAL address is 0x14c840 (map estimate 0x14c738 is wrong: 0x14c738 = func_00149168,
 * 0x14c790 = func_001491C0). Axis-angle (Rodrigues) rotation -> func_00145DB0 (16-float set).
 * 170/170 instrs, same count; ~67 differ (aligned). First ~70 instrs (sqrtf/epsilon/div/sinf/cosf)
 * are exact; the remaining diff is the FP block after cosf: target schedules c-1, a*t first and
 * the six 0.0f stores early (zero in $f1); ours schedules neg(sy) first and zero lands in $f0
 * late. Source order / temp locals / inline-args variants all give the same output.
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

NaMATRIX<float, 4, 4>* func_00149270(NaMATRIX<float, 4, 4>* self, const float* axis, const float* angle) asm("func_00149270");
NaMATRIX<float, 4, 4>* func_00149270(NaMATRIX<float, 4, 4>* self, const float* axis, const float* angle) {
    float yz = axis[1] * axis[1] + axis[2] * axis[2];
    float len = sqrtf(axis[0] * axis[0] + yz);
    float r = sqrtf(yz);
    float sy, cz;

    if (r < FLT_EPSILON) {
        sy = 0.0f;
        cz = 1.0f;
    } else {
        sy = axis[1] / r;
        cz = axis[2] / r;
    }

    float a = r / len;
    float b = -axis[0] / len;
    float s = sinf(*angle);
    float c = cosf(*angle);
    float t = a * (c - 1.0f);

    SetMat44(self,
        a * t + 1.0f, sy * b * t + cz * a * s, cz * b * t - sy * a * s, 0.0f,
        sy * b * t - cz * a * s, -sy * sy * a * t + c, -sy * cz * a * t - b * s, 0.0f,
        cz * b * t + sy * a * s, -sy * cz * a * t + b * s, -cz * cz * a * t + c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return self;
}

/* prlib/spadata.cpp */