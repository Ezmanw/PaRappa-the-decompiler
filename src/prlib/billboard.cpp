#define NALIB_NAVECTOR_H
#define NALIB_NAMATRIX_H

template <typename T, int t0>
class NaVECTOR {
public:
    NaVECTOR() {}
    NaVECTOR(const NaVECTOR<float, 4>& rhs) {
        Copy(*this, rhs);
    }

    NaVECTOR<T, t0>& Set(const T& x, const T& y, const T& z, const T& w);

public:
    T& operator[](int arg0) {
        return v[arg0];
    }

    T operator[](int arg0) const {
        return v[arg0];
    }

    static NaVECTOR<float, 4>& Copy(NaVECTOR<float, 4>& lhs, const NaVECTOR<float, 4>& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(&lhs), "r"(&rhs)
        : "$6");
        return lhs;
    }

    NaVECTOR<float, 4>& operator=(const NaVECTOR<float, 4>& rhs) {
        return Copy(*this, rhs);
    }

private:
    T v[t0];
};

template <typename T, int t0, int t1>
class NaMATRIX {
public:
    NaMATRIX() {}
    NaMATRIX(const NaMATRIX<float, 4, 4>& rhs) {
        Copy(*this, rhs);
    }

    const NaVECTOR<T, t0>& operator[](int arg0) const {
        return m[arg0];
    }

    static NaMATRIX<float, 4, 4>& Copy(NaMATRIX<float, 4, 4>& lhs, const NaMATRIX<float, 4, 4>& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(&lhs), "r"(&rhs)
        : "$6", "$7", "$8", "$9");
        return lhs;
    }

    NaMATRIX<float, 4, 4>& operator=(const NaMATRIX<float, 4, 4>& rhs) {
        return Copy(*this, rhs);
    }

private:
    NaVECTOR<T, t0> m[t1];

public:
    static NaMATRIX<float, 4, 4> IDENT;
};

#include "common.h"

#include "model.h"
#include "spram.h"
#include "vu0mat.h"

#include <math.h>
#include <libvu0.h>

extern PrSPRAM_DATA *D_00383638;

extern "C" void func_00165FA8(sceVu0FMATRIX m0, const sceVu0FMATRIX m1);
NaMATRIX<float, 4, 4> func_0014ACE8(int axis, const float& angle) asm("func_0014ACE8");

struct PrRawMat { float m[4][4]; };

static inline NaMATRIX<float, 4, 4> *MatCopy(NaMATRIX<float, 4, 4> *lhs, const NaMATRIX<float, 4, 4> *rhs) {
    return &NaMATRIX<float, 4, 4>::Copy(*lhs, *rhs);
}

#define LDA(A) asm volatile( \
        "lqc2     $vf4,   0x0(%0)       \n\t" \
        "lqc2     $vf5,  0x10(%0)       \n\t" \
        "lqc2     $vf6,  0x20(%0)       \n\t" \
        "lqc2     $vf7,  0x30(%0)       \n\t" \
    : : "r"(A))

#define ROW(o) \
        "lqc2     $vf8,  " #o "(%1)       \n\t" \
        "vmulax   ACC,    $vf4,   $vf8  \n\t" \
        "vmadday  ACC,    $vf5,   $vf8  \n\t" \
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t" \
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t" \
        "sqc2     $vf9,   " #o "(%0)       \n\t"

#define MUL(R, B) asm volatile(ROW(0x0) ROW(0x10) ROW(0x20) ROW(0x30) : : "r"(R), "r"(B))

static inline void InverseMat(PrRawMat& tmp_mat, const NaMATRIX<float, 4, 4>& mat) {
    func_00165FA8(tmp_mat.m, *(const sceVu0FMATRIX*)&mat);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
}

const NaMATRIX<float, 4, 4>& CreateBillboardMatrix(const NaMATRIX<float, 4, 4>& mat) {
    NaVECTOR<float, 4> cam_pos = D_00383638->unk50;
    cam_pos[3] = 1.0f;

    NaVECTOR<float, 4> local_pos;
    {
        PrRawMat inv_mat;
        {
            PrRawMat tmp_mat;
            {
                int w = 3;
                do { w--; } while (w != -1);
            }
            InverseMat(tmp_mat, mat);
            asm volatile("
                lq $6, 0(%0)
                lq $7, 0x10(%0)
                lq $8, 0x20(%0)
                lq $9, 0x30(%0)
                sq $6, 0(%1)
                sq $7, 0x10(%1)
                sq $8, 0x20(%1)
                sq $9, 0x30(%1)
            " : : "r"(&tmp_mat), "r"(&inv_mat)
            : "$6", "$7", "$8", "$9");
        }
        {
            NaVECTOR<float, 4> tmp_vec;
            asm volatile(
                "lqc2     $vf4,   0x0(%1)       \n\t"
                "lqc2     $vf5,  0x10(%1)       \n\t"
                "lqc2     $vf6,  0x20(%1)       \n\t"
                "lqc2     $vf7,  0x30(%1)       \n\t"
                "lqc2     $vf8,   0x0(%2)       \n\t"
                "vmulax.xyzw   ACC,    $vf4,   $vf8  \n\t"
                "vmadday.xyzw  ACC,    $vf5,   $vf8  \n\t"
                "vmaddaz.xyzw  ACC,    $vf6,   $vf8  \n\t"
                "vmaddw.xyzw   $vf9,   $vf7,   $vf8  \n\t"
                "sqc2     $vf9,   0x0(%0)       \n\t"
            : : "r"(&tmp_vec), "r"(&inv_mat), "r"(&cam_pos));
            local_pos = tmp_vec;
        }
    }

    if (local_pos[0] == 0.0f && local_pos[2] == 0.0f) {
        return NaMATRIX<float, 4, 4>::IDENT;
    }

    float angle = atan2f(local_pos[0], local_pos[2]);
    static NaMATRIX<float, 4, 4> return_matrix;
    return_matrix = func_0014ACE8(1, angle);
    return return_matrix;
}

void SpmNode::ApplyBillboardMatrix() {
    const NaMATRIX<float, 4, 4>& bb = CreateBillboardMatrix(unk40);
    PrRawMat t;
    PrRawMat r;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    LDA(&unk40);
    MUL((NaMATRIX<float, 4, 4>*)&r, &bb);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((NaMATRIX<float, 4, 4>*)&t, (NaMATRIX<float, 4, 4>*)&r);
    MatCopy(&unk40, (NaMATRIX<float, 4, 4>*)&t);
}

/* nalib/navector.h */
float* func_0014C4E8(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) asm("func_0014C4E8");

float* func_0014C4E8(
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

float* func_0014C540(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) asm("func_0014C540");

float* func_0014C540(
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
INCLUDE_ASM("asm/nonmatchings/prlib/billboard", func_0014C5F0);
#else
extern NaMATRIX<float, 4, 4>* SetMat44Raw(NaMATRIX<float, 4, 4>* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15) asm("func_00145DB0");

/* Axis rotation matrix (0 = X, 1 = Y, otherwise Z), same as func_0014ACE8 */
NaMATRIX<float, 4, 4>* func_0014C5F0(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) asm("func_0014C5F0");
NaMATRIX<float, 4, 4>* func_0014C5F0(NaMATRIX<float, 4, 4>* self, int axis, const float* angle) {
    float c = cosf(*angle);
    float s = sinf(*angle);

    if (axis == 0) {
        SetMat44Raw(self,
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    } else if (axis == 1) {
        SetMat44Raw(self,
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        SetMat44Raw(self,
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }
    return self;
}
#endif
