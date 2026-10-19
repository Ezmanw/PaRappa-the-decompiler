#include <nalib/namatrix.h>

#include "spram.h"

#include "prpriv.h"

#include "model.h"
#include "vu0mat.h"
#include "random.h"

#include "vu1/vucommon.h"
#include "vu1/vumem.h"

#include <eestruct.h>

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/spram", Initialize__12PrSPRAM_DATAP13PrSceneObject);
#endif

extern NaMATRIX<float, 4, 4> screenClipMatrix;
extern NaMATRIX<float, 4, 4> screenPrimitiveMatrix;
extern int g_NoodlePause;

void PrSPRAM_DATA::InitializeModel(PrModelObject *model) {
    m_model_contour_blur_alpha[0] = model->m_contour_blur_alpha[0];
    m_model_contour_blur_alpha[1] = model->m_contour_blur_alpha[1];

    m_model_transaction_blend_ratio = model->m_transaction_blend_ratio;

    float disturbance = model->m_disturbance;
    float debug_disturbance = PrGetDebugParamFloat(PR_FLOAT_PARAM_DISTURBANCE);
    m_disturbance = disturbance * debug_disturbance;
}

void PrSPRAM_DATA::SendDisplayHeader() {
    static sceDmaTag dmaTagTemplate = {
        /* .qwc  */ (sizeof(PrDisplayHeader) / 16) - 1,
        /* .mark */ 0,
        /* .id   */ 0x70, /* DMAend */
        /* .next */ NULL,
        /* .p    */ {
            SCE_VIF1_SET_STCYCL(/*WL*/4, /*CL*/4, 0),
            SCE_VIF1_SET_STMOD(0, 0)
        }
    };

    m_display_header.tag = dmaTagTemplate;

    m_display_header.stmask[0] = SCE_VIF1_SET_STMASK(0);
    m_display_header.stmask[1] = 0;

    m_display_header.base      = SCE_VIF1_SET_BASE(PR_VU1_CHUNK1_START, 0);
    m_display_header.offset    = SCE_VIF1_SET_OFFSET(PR_VU1_CHUNK2_START, 0);
    m_display_header.mark      = SCE_VIF1_SET_MARK(0, 0);
    m_display_header.mskpath3  = SCE_VIF1_SET_MSKPATH3(0, 0);

    m_display_header.unpack[0] = SCE_VIF1_SET_NOP(0);
    m_display_header.unpack[1] = SCE_VIF1_SET_UNPACK(PR_VU1_DISPLAYHDR_ADDR, sizeof(PrInnerDisplayHeader) / 16, PR_VIF_UNPACK_V4_32(0), 0);

    m_display_header.inner.view_projection_matrix = m_view_projection_matrix;
    m_display_header.inner.unk70 = this->unk120;
    m_display_header.inner.camera_matrix = m_camera_matrix;
    m_display_header.inner.camera_position = this->unk50;
    m_display_header.inner.unk100 = this->unk160;
    m_display_header.inner.screen_clip_matrix = screenClipMatrix;
    m_display_header.inner.screen_primitive_matrix = screenPrimitiveMatrix;

    if (!g_NoodlePause) {
        m_disturbance_param = PrRandom();
    }
    m_display_header.inner.disturbance_param = m_disturbance_param;

    void       *tag  = PR_DMA_SPR_ADDR(&m_display_header);
    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_VIF1);
    chan->chcr.TTE = 1;
    sceDmaSend(chan, tag);
}

/* nalib/navector.h */
float* func_00147CE0(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8, const float& a9, const float& a10, const float& a11, const float& a12, const float& a13, const float& a14, const float& a15) asm("func_00147CE0");
float* func_00147CE0(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8, const float& a9, const float& a10, const float& a11, const float& a12, const float& a13, const float& a14, const float& a15) {
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

float* func_00147D90(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8) asm("func_00147D90");
float* func_00147D90(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8) {
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

extern "C" void _GLOBAL_$I$Initialize__12PrSPRAM_DATAP13PrSceneObject() {
    asm volatile(
        "li $2, 3\n\t"
        "li $3, -1\n\t"
        "1:\n\t"
        "addiu $2, $2, -1\n\t"
        "bne $2, $3, 1b\n\t"
        "li $3, 3\n\t"
        "2:\n\t"
        "addiu $3, $3, -1\n\t"
        "li $2, -1\n\t"
        "bne $3, $2, 2b\n\t"
    : : : "$2", "$3");
}

struct Mat44 {
    Mat44(
        const float& a0, const float& a1, const float& a2, const float& a3,
        const float& a4, const float& a5, const float& a6, const float& a7,
        const float& a8, const float& a9, const float& a10, const float& a11,
        const float& a12, const float& a13, const float& a14, const float& a15
    ) {
        Set(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    }

    Mat44& Set(
        const float& a0, const float& a1, const float& a2, const float& a3,
        const float& a4, const float& a5, const float& a6, const float& a7,
        const float& a8, const float& a9, const float& a10, const float& a11,
        const float& a12, const float& a13, const float& a14, const float& a15
    ) asm("func_00145DB0");

    NaVECTOR<float, 4> m[4];
};

inline Mat44 MakeTrans44(const float& x, const float& y, const float& z) {
    return Mat44(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, z, 1.0f
    );
}

Mat44* func_00147E38(Mat44* self, const float& x, const float& y, const float& z) asm("func_00147E38");
Mat44* func_00147E38(Mat44* self, const float& x, const float& y, const float& z) {
    const Mat44& tmp = MakeTrans44(x, y, z);
    asm volatile(
        "lqc2     $vf4,   0x0(%0)       \n\t"
        "lqc2     $vf5,  0x10(%0)       \n\t"
        "lqc2     $vf6,  0x20(%0)       \n\t"
        "lqc2     $vf7,  0x30(%0)       \n\t"
    : : "r"(&tmp));
    asm volatile(
        "lqc2     $vf8,   0x0(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,   0x0(%0)       \n\t"
        "lqc2     $vf8,  0x10(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x10(%0)       \n\t"
        "lqc2     $vf8,  0x20(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x20(%0)       \n\t"
        "lqc2     $vf8,  0x30(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x30(%0)       \n\t"
    : : "r"(self), "r"(self));
    return self;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/spram", func_00147FB8);
#else
/* self = Scale(x,y,z) * self */
Mat44* func_00147FB8(Mat44* self, const float& x, const float& y, const float& z) asm("func_00147FB8");
Mat44* func_00147FB8(Mat44* self, const float& x, const float& y, const float& z) {
    Mat44 tmp(
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    asm volatile(
        "lqc2     $vf4,   0x0(%0)       \n\t"
        "lqc2     $vf5,  0x10(%0)       \n\t"
        "lqc2     $vf6,  0x20(%0)       \n\t"
        "lqc2     $vf7,  0x30(%0)       \n\t"
    : : "r"(&tmp));
    asm volatile(
        "lqc2     $vf8,   0x0(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,   0x0(%0)       \n\t"
        "lqc2     $vf8,  0x10(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x10(%0)       \n\t"
        "lqc2     $vf8,  0x20(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x20(%0)       \n\t"
        "lqc2     $vf8,  0x30(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x30(%0)       \n\t"
    : : "r"(self), "r"(self));
    return self;
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/spram", func_00148140);
#else
/* self = diag(x, y, z, 1) (overwrites self) */
Mat44* func_00148140(Mat44* self, const float& x, const float& y, const float& z) asm("func_00148140");
Mat44* func_00148140(Mat44* self, const float& x, const float& y, const float& z) {
    self->Set(
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    return self;
}
#endif

Mat44 func_00148248(const float& x, const float& y, const float& z) asm("func_00148248");
Mat44 func_00148248(const float& x, const float& y, const float& z) {
    return Mat44(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, z, 1.0f
    );
}

#ifdef NON_MATCHING
#include "renderstuff.h"
#include "scene.h"
#include <math.h>

struct SprVec {
    SprVec() {}
    SprVec(const SprVec& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "memory");
    }
    SprVec& operator=(const SprVec& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "memory");
        return *this;
    }
    SprVec operator-(const SprVec& rhs) const {
        SprVec ret;
        asm volatile("
            lqc2       $vf04,  0x0(%1)
            lqc2       $vf05,  0x0(%2)
            vsub.xyzw  $vf06,  $vf04,  $vf05
            sqc2       $vf06,  0x0(%0)
        " : : "r"(&ret), "r"(this), "r"(&rhs) : "memory");
        return ret;
    }
    SprVec operator-() const {
        SprVec ret;
        for (int i = 0; i < 4; i++) {
            ret.v[i] = 0.0f - v[i];
        }
        return ret;
    }
    SprVec Normalize() const {
        SprVec ret;
        asm volatile("
            lqc2       $vf04,  0x0(%1)
            vmul.xyz   $vf05,  $vf04,  $vf04
            vaddy.x    $vf05,  $vf05,  $vf05
            vaddz.x    $vf05,  $vf05,  $vf05
            vsqrt      Q,      $vf05x
            vwaitq
            vaddq.x    $vf05,  $vf00,  Q
            vdiv       Q,      $vf00w, $vf05x
            vsub.xyzw  $vf06,  $vf00,  $vf00
            vwaitq
            vmulq.xyz  $vf06,  $vf04,  Q
            sqc2       $vf06,  0x0(%0)
        " : : "r"(&ret), "r"(this) : "memory");
        ret.v[3] = 1.0f;
        return ret;
    }
    float v[4];
} __attribute__((aligned(16)));

struct SprMat {
    SprMat() {}
    SprMat(const SprMat& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "$7", "$8", "$9");
    }
    static SprMat& Copy(SprMat& lhs, const SprMat& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(&lhs), "r"(&rhs) : "$6", "$7", "$8", "$9");
        return lhs;
    }
    SprMat& operator=(const SprMat& rhs) {
        return Copy(*this, rhs);
    }
    SprMat operator*(const SprMat& rhs) const {
        SprMat ret;
        asm volatile(
            "lqc2     $vf4,   0x0(%0)       \n\t"
            "lqc2     $vf5,  0x10(%0)       \n\t"
            "lqc2     $vf6,  0x20(%0)       \n\t"
            "lqc2     $vf7,  0x30(%0)       \n\t"
        : : "r"(&rhs));
        asm volatile(
            "lqc2     $vf8,   0x0(%1)       \n\t"
            "vmulax   ACC,    $vf4,   $vf8  \n\t"
            "vmadday  ACC,    $vf5,   $vf8  \n\t"
            "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
            "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
            "sqc2     $vf9,   0x0(%0)       \n\t"
            "lqc2     $vf8,  0x10(%1)       \n\t"
            "vmulax   ACC,    $vf4,   $vf8  \n\t"
            "vmadday  ACC,    $vf5,   $vf8  \n\t"
            "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
            "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
            "sqc2     $vf9,  0x10(%0)       \n\t"
            "lqc2     $vf8,  0x20(%1)       \n\t"
            "vmulax   ACC,    $vf4,   $vf8  \n\t"
            "vmadday  ACC,    $vf5,   $vf8  \n\t"
            "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
            "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
            "sqc2     $vf9,  0x20(%0)       \n\t"
            "lqc2     $vf8,  0x30(%1)       \n\t"
            "vmulax   ACC,    $vf4,   $vf8  \n\t"
            "vmadday  ACC,    $vf5,   $vf8  \n\t"
            "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
            "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
            "sqc2     $vf9,  0x30(%0)       \n\t"
        : : "r"(&ret), "r"(this));
        return ret;
    }
    SprVec m[4];
};

struct SprCamera {
    SprVec position;
    SprVec interest;
    SprVec up;
    float aspect;
    float field_of_view;
    float near_clip;
    float far_clip;
};

struct SprData {
    char unk0[0x40];
    u_int unk40;
    u_int unk44;
    u_int unk48;
    char unk4C[0x4];
    SprCamera camera;
    SprVec zdir;
    SprMat camera_matrix;
    SprMat view_projection_matrix;
    SprMat unk120;
    SprMat unk160;
    SprMat view_screen[3];
    char unk260[0x40];
    sceDmaTag end_dmatag;
    char unk2B0[0x3BC];
    u_int unk66C;
    u_int unk670;
    u_int unk674;
    u_int unk678;
};

extern "C" void sceVu0CameraMatrix(SprMat* m, SprVec* p, SprVec* zd, SprVec* yd);
extern "C" void sceVu0ViewScreenMatrix(SprMat* m, float scrz, float ax, float ay,
    float cx, float cy, float zmin, float zmax, float nearz, float farz);

SprMat Spr_func_00148248(const float& x, const float& y, const float& z) asm("func_00148248");
SprMat* Spr_func_00147FB8(SprMat* self, const float& x, const float& y, const float& z) asm("func_00147FB8");
SprMat* Spr_func_00148140(SprMat* self, const float& x, const float& y, const float& z) asm("func_00148140");
SprMat* Spr_func_00147E38(SprMat* self, const float& x, const float& y, const float& z) asm("func_00147E38");

void PrSPRAM_DATA::Initialize(PrSceneObject *scene) {
    static const sceDmaTag endTag = { 0, 0, 0x70, NULL, { 0, 0 } };
    SprData* d = (SprData*)this;

    d->unk40 = (u_int)this + 0x1000;
    d->unk44 = (u_int)this + 0x2000;
    d->unk48 = (u_int)this + 0x3000;
    d->end_dmatag = endTag;

    d->camera = *(SprCamera*)scene->GetCurrentCamera();
    d->zdir = (d->camera.interest - d->camera.position).Normalize();
    sceVu0CameraMatrix(&d->camera_matrix, &d->camera.position, &d->zdir, &-d->camera.up);

    u_int width = (u_int)scene->unk74;
    u_int height = (u_int)scene->unk78;
    float zmax = (float)(0xFFFFFFFFu >> (37 - prRenderStuff.GetZbufBits()));
    float zmin = zmax * 0.000002f;
    zmax = zmax * 0.999998f;
    float aspect = (float)width / (float)height * 3.0f * 0.25f;
    float scrz = 1.0f / tanf(d->camera.field_of_view * 0.5f);

    sceVu0ViewScreenMatrix(&d->view_screen[0], scrz,
        (float)height * 0.5f * aspect, (float)height * 0.5f,
        2048.0f, 2048.0f, zmin, zmax, d->camera.near_clip, d->camera.far_clip);
    zmax -= zmin;
    d->view_projection_matrix = d->camera_matrix * d->view_screen[0];

    sceVu0ViewScreenMatrix(&d->view_screen[1], scrz,
        (float)height * 1.01f * 0.5f * aspect * (1.0f / 2048.0f), (float)height * 1.01f * 0.5f * (1.0f / 2048.0f),
        0.0f, 0.0f, -1.0f, 1.0f, d->camera.near_clip, d->camera.far_clip);
    d->unk120 = d->camera_matrix * d->view_screen[1];

    sceVu0ViewScreenMatrix(&d->view_screen[2], scrz,
        4096.0f / (float)(width + 2) * 0.99f * (float)height * 0.5f * aspect * (1.0f / 2048.0f),
        4096.0f / (float)(height + 2) * 0.99f * (float)height * 0.5f * (1.0f / 2048.0f),
        0.0f, 0.0f, -1.0f, 1.0f, d->camera.near_clip, d->camera.far_clip);
    d->unk160 = d->camera_matrix * d->view_screen[2];

    SprMat m = Spr_func_00148248((float)width * 0.5f, -(float)height * 0.5f, 5010.0f);
    *(SprMat*)&screenClipMatrix = m;

    Spr_func_00147FB8(&m, 1.01f / 2048.0f, 1.01f / 2048.0f, 1.0f / 5010.0f);
    Spr_func_00148140(&m, 1.0f, -0.5f, zmax / 10020.0f);
    *(SprMat*)&screenPrimitiveMatrix = m;
    Spr_func_00147E38((SprMat*)&screenPrimitiveMatrix,
        2048.0f - (float)width * 0.5f, 2048.0f - (float)height * 0.5f,
        zmin - zmax * -10010.0f / 10020.0f);

    d->unk66C = 0;
    d->unk670 = 0;
    d->unk674 = 0;
    d->unk678 = 0;
}

#endif

INCLUDE_RODATA("asm/nonmatchings/prlib/spram", D_00396790);
