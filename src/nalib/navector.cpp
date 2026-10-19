#include "common.h"

/* nalib/navector.h */
float* func_001544A8(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) asm("func_001544A8");

float* func_001544A8(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    self[4] = a4;
    self[5] = a5;
    self[6] = a6;
    self[7] = a7;
    self[8] = a8;
    self[9] = a9;
    self[10] = a10;
    self[11] = a11;
    self[12] = a12;
    self[13] = a13;
    self[14] = a14;
    self[15] = a15;
    return self;
}

float* func_00154558(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) asm("func_00154558");

float* func_00154558(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}

float* func_00154580(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) asm("func_00154580");

float* func_00154580(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    self[4] = a4;
    self[5] = a5;
    self[6] = a6;
    self[7] = a7;
    self[8] = a8;
    return self;
}

/* nalib/navector.cpp */
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/nalib/navector", func_001545D8);
#else
#include <nalib/namatrix.h>

/* Static data initializer for the nalib vector/matrix constants (all addresses are PAL .bss/.sbss). */
extern float D_00372040[4]; /* NaVECTOR<float, 4>: (0, 0, 0, 0) */
extern float D_00372050[4]; /* NaVECTOR<float, 4>: (0, 0, 0, 1) */
extern float D_00372060[16]; /* NaMATRIX<float, 4, 4>: all zero */
extern float D_003720E0[3]; /* (0, 0, 0) */
extern float D_003720F0[3]; /* (0, 0, 1) */
extern float D_003836D8[2]; /* (0, 0) */
extern float D_00372100[4]; /* NaVECTOR<float, 4>: (0, 0, 0, 0) */
extern float D_00372110[9]; /* 3x3 matrix: all zero */
extern float D_00372138[4]; /* 2x2 matrix: identity */
extern float D_00372148[9]; /* 3x3 matrix: identity */

void func_001545D8() asm("func_001545D8");
void func_001545D8() {
    int i;

    D_00372040[0] = 0.0f; D_00372040[1] = 0.0f; D_00372040[2] = 0.0f; D_00372040[3] = 0.0f;
    D_00372050[0] = 0.0f; D_00372050[1] = 0.0f; D_00372050[2] = 0.0f; D_00372050[3] = 1.0f;

    for (i = 0; i < 16; i++) {
        D_00372060[i] = 0.0f;
    }

    float *ident = (float*)&NaMATRIX<float, 4, 4>::IDENT;
    for (i = 0; i < 16; i++) {
        ident[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    D_003836D8[0] = 0.0f; D_003836D8[1] = 0.0f;
    D_003720E0[0] = 0.0f; D_003720E0[1] = 0.0f; D_003720E0[2] = 0.0f;
    D_003720F0[0] = 0.0f; D_003720F0[1] = 0.0f; D_003720F0[2] = 1.0f;

    func_00154558(D_00372100, 0.0f, 0.0f, 0.0f, 0.0f);

    func_00154580(D_00372110,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    func_00154558(D_00372138, 1.0f, 0.0f, 0.0f, 1.0f);

    func_00154580(D_00372148,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f);
}
#endif
