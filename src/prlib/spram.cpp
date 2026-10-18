#include <nalib/namatrix.h>

#include "spram.h"

#include "prpriv.h"

#include "model.h"
#include "vu0mat.h"
#include "random.h"

#include "vu1/vucommon.h"
#include "vu1/vumem.h"

#include <eestruct.h>

INCLUDE_ASM("asm/nonmatchings/prlib/spram", Initialize__12PrSPRAM_DATAP13PrSceneObject);

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

INCLUDE_ASM("asm/nonmatchings/prlib/spram", func_00147FB8);

INCLUDE_ASM("asm/nonmatchings/prlib/spram", func_00148140);

Mat44 func_00148248(const float& x, const float& y, const float& z) asm("func_00148248");
Mat44 func_00148248(const float& x, const float& y, const float& z) {
    return Mat44(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, z, 1.0f
    );
}

INCLUDE_RODATA("asm/nonmatchings/prlib/spram", D_00396790);
