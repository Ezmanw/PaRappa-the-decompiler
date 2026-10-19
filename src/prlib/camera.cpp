#include "camera.h"

#ifdef NON_MATCHING
#include <math.h>
#include <float.h>
#include "vu0mat.h"
#endif

void SpcFileHeader::Initialize() {
    ChangePointer();
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/camera", GetCamera__C13SpcFileHeaderf);
#else
NaMATRIX<float, 4, 4>* func_00149270(NaMATRIX<float, 4, 4>* self, const float* axis, const float* angle) asm("func_00149270");

PrPERSPECTIVE_CAMERA* SpcFileHeader::GetCamera(float time) const {
    static PrPERSPECTIVE_CAMERA cam;
    const char *self = (const char*)this;
    float *pos = (float*)&cam.position;
    float *interest = (float*)&cam.interest;
    float *up = (float*)&cam.up;

    if (this->unk88 != NULL) {
        cam.position = *this->unk88->GetValue(time);
    } else {
        cam.position = *(const NaVECTOR<float, 4>*)(self + 0x40);
    }

    if (this->unk8C != NULL) {
        cam.interest = *this->unk8C->GetValue(time);
    } else {
        cam.interest = *(const NaVECTOR<float, 4>*)(self + 0x50);
    }

    float roll;
    if (this->unk90 != NULL) {
        roll = *this->unk90->GetValue(time);
    } else {
        roll = *(const float*)(self + 0x60);
    }

    if (this->unk94 != NULL) {
        cam.field_of_view = *this->unk94->GetValue(time);
    } else {
        cam.field_of_view = *(const float*)(self + 0x64);
    }

    cam.near_clip = *(const float*)(self + 0x6c);
    cam.far_clip = *(const float*)(self + 0x70);
    cam.aspect = *(const float*)(self + 0x68);

    /* dir = interest - position */
    float dir[4] __attribute__((aligned(16)));
    for (int i = 0; i < 4; i++) {
        dir[i] = interest[i] - pos[i];
    }

    /* c1 = dir x (0,1,0,0) ; c2 = c1 x dir (w = 0) */
    float c1[4], c2[4];
    c1[0] = dir[1] * 0.0f - 1.0f * dir[2];
    c1[1] = dir[2] * 0.0f - 0.0f * dir[0];
    c1[2] = dir[0] * 1.0f - 0.0f * dir[1];
    c1[3] = 1.0f;
    c2[0] = c1[1] * dir[2] - dir[1] * c1[2];
    c2[1] = c1[2] * dir[0] - dir[2] * c1[0];
    c2[2] = c1[0] * dir[1] - dir[0] * c1[1];
    c2[3] = 0.0f;

    /* normalize xyz, w = 1 */
    float len = sqrtf(c2[0] * c2[0] + c2[1] * c2[1] + c2[2] * c2[2]);
    float q = (len != 0.0f) ? 1.0f / len : 3.4028235e+38f;
    up[0] = c2[0] * q;
    up[1] = c2[1] * q;
    up[2] = c2[2] * q;
    up[3] = 1.0f;

    if (roll != 0.0f) {
        float m[16] __attribute__((aligned(16)));
        func_00149270((NaMATRIX<float, 4, 4>*)m, dir, &roll);
        float x = up[0], y = up[1], z = up[2], w = up[3];
        for (int j = 0; j < 4; j++) {
            up[j] = m[j] * x + m[4 + j] * y + m[8 + j] * z + m[12 + j] * w;
        }
    }

    return &cam;
}
#endif

/* nalib/navector.h */
float* func_00153AD0(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) asm("func_00153AD0");

float* func_00153AD0(
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

float* func_00153B28(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) asm("func_00153B28");

float* func_00153B28(
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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/camera", func_00153BD8);
#else
extern NaMATRIX<float, 4, 4>* SetMat44Raw(NaMATRIX<float, 4, 4>* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15) asm("func_00145DB0");

/* Axis-angle (Rodrigues) rotation matrix: self = rot(axis, *angle) (same as func_00149270) */
NaMATRIX<float, 4, 4>* func_00153BD8(NaMATRIX<float, 4, 4>* self, const float* axis, const float* angle) asm("func_00153BD8");
NaMATRIX<float, 4, 4>* func_00153BD8(NaMATRIX<float, 4, 4>* self, const float* axis, const float* angle) {
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

    PR_VU0_WAIT();
    SetMat44Raw(self,
        a * t + 1.0f, sy * b * t + cz * a * s, cz * b * t - sy * a * s, 0.0f,
        sy * b * t - cz * a * s, -sy * sy * a * t + c, -sy * cz * a * t - b * s, 0.0f,
        cz * b * t + sy * a * s, -sy * cz * a * t + b * s, -cz * cz * a * t + c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return self;
}
#endif
