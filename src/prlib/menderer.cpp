#include "common.h"

#if defined(PRD_SYORI)
#include "dbug/syori.h"
#endif

#include "renderstuff.h"
#include "utility.h"

#include <eeregs.h>

/* sdata */
extern float prMendererRatio;
extern int prMendererDirection;
extern int prMendererColorModulation;
extern u_int mendererTbp;
extern u_int mendererFbp;
extern int mendererInitialized;

void PrInitializeTextureCreation(u_int arg0, u_int arg1, u_int arg2, u_int arg3);
void PrInitializeAlphaModulation();
void PrInitializeAwfulBackground(void *picture);
void PrInitializeNoodlePolygonPosition();

void InitializeNoodleStripRendering(u_int arg0, u_int arg1, u_int arg2, u_int arg3);
void StartNoodleRotation();

extern int prCurrentStage;
extern int g_NoodlePause;
extern float g_NoodleStep;
extern u_int noodleRandomSeed;
extern float noodleAngle;
extern int noodleRotationState;
extern u_int noodleChangeTimer;
extern float noodleRotation;
extern "C" void PrSendMfifo(const sceDmaTag *tag);
void PrSendMfifo(const sceDmaTag *tag);

struct PrMendererSpram {
    char unk0[0x40];
    void *m_work[3];

    void *GetWork() {
        void *p0 = m_work[0];
        void *p1 = m_work[1];
        void *p2 = m_work[2];
        m_work[0] = p1;
        m_work[1] = p2;
        m_work[2] = p0;
        return p0;
    }
};

struct NoodleStripHeader {
    u_long data[16][2];
};

extern PrMendererSpram *prSpramData;
extern u_int D_003836A4;
extern NoodleStripHeader noodleStripHeader;

void InitializeNoodleStripRendering(u_int tbp, u_int fbp, u_int width, u_int height) {
    noodleStripHeader.data[6][0] = SCE_GS_SET_FRAME(fbp, 10, 0, 0);
    noodleStripHeader.data[10][0] = SCE_GS_SET_TEX0(tbp, 4, 0, width, height, 1, 0, 0, 0, 0, 0, 0);
}

float GetRandom() {
    u_int seed = noodleRandomSeed * 0x19660d + 0x3c6ef35f;
    float ret = (float)((seed >> 8) & 0xffff) / 65536.0f;
    noodleRandomSeed = seed;
    return ret;
}

u_int StageIndexForColor() {
    if (prCurrentStage == 19) {
        return 0;
    }
    return (u_int)prCurrentStage % 10;
}

extern float D_00383684;
extern float D_00383688;
extern float D_00371AF0[10][2];
extern float D_00371B40[10][3];
extern float D_003719B0[4];

void PushNoodleColor(u_long *ptr) {
    if (!prMendererColorModulation || prCurrentStage == 6) {
        *ptr = SCE_GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0);
        return;
    }

    u_int stageIndex = StageIndexForColor();
    float *range = D_00371AF0[stageIndex];
    float f20 = range[0] + (range[1] - range[0]) * GetRandom();

    float f4 = GetRandom() * 3.0f + D_00383684;
    if (f4 > 3.0f) {
        f4 -= 3.0f;
    }

    float r;
    if (f4 < 1.0f) {
        r = f4;
    } else if (f4 < 2.0f) {
        r = 2.0f - f4;
    } else {
        r = 0.0f;
    }

    float g;
    if (f4 < 1.0f) {
        g = 0.0f;
    } else if (f4 < 2.0f) {
        g = f4 - 1.0f;
    } else {
        g = 3.0f - f4;
    }

    float b;
    if (f4 < 1.0f) {
        b = 1.0f - f4;
    } else if (f4 < 2.0f) {
        b = 0.0f;
    } else {
        b = f4 - 2.0f;
    }

    r = D_00371B40[stageIndex][0] + r * f20;
    g = D_00371B40[stageIndex][1] + g * f20;
    b = D_00371B40[stageIndex][2] + b * f20;

    float ratio;
    if (prCurrentStage == 19 || prMendererRatio <= 1.0f) {
        ratio = 0.0f;
    } else if (prMendererRatio <= 1.4f) {
        ratio = (prMendererRatio - 1.0f) * 0.3f / 0.4f;
    } else if (prMendererRatio <= 1.6f) {
        ratio = 0.3f;
    } else if (prMendererRatio <= 2.0f) {
        ratio = (prMendererRatio - 1.6f) * 0.7f / 0.4f + 0.3f;
    } else {
        ratio = 1.0f;
    }

    float f3 = D_00383688 + GetRandom() * 2.0f;
    if (f3 >= 2.0f) {
        f3 -= 2.0f;
    }
    float f1 = (1.0f - f3 >= 0.0f) ? (1.0f - f3) : -(1.0f - f3);

    f3 = f1 * (0.9f - 0.6f) + 0.6f;
    r += (f3 * D_003719B0[0] - r) * ratio;
    g += (f3 * D_003719B0[1] - g) * ratio;
    b += (f3 * D_003719B0[2] - b) * ratio;

    if (r > 1.0f) {
        r = 1.0f;
    }
    if (g > 1.0f) {
        g = 1.0f;
    }
    if (b > 1.0f) {
        b = 1.0f;
    }

    u_int ri = (u_int)(r * 255.99f);
    u_int gi = (u_int)(g * 255.99f);
    u_int bi = (u_int)(b * 255.99f);
    *ptr = SCE_GS_SET_RGBAQ(ri, gi, bi, 0x80, 0);
}

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawNoodleStripChunk__FRCt8NaMATRIX3Zfi4i4);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", SetNoodleRotationMatrix__FRt8NaMATRIX3Zfi4i4f);
void SetNoodleRotationMatrix(NaMATRIX<float, 4, 4> &mat, float angle) asm("SetNoodleRotationMatrix__FRt8NaMATRIX3Zfi4i4f");
void DrawNoodleStripChunk(const NaMATRIX<float, 4, 4> &mat) asm("DrawNoodleStripChunk__FRCt8NaMATRIX3Zfi4i4");

void PreDrawNoodleStrip() {
    NoodleStripHeader *pkt = (NoodleStripHeader *)prSpramData->GetWork();
    *pkt = noodleStripHeader;
    pkt->data[2][0] = SCE_GS_SET_BITBLTBUF(D_003836A4 << 5, 10, 0, mendererFbp << 5, 10, 0);
    PrSendMfifo((const sceDmaTag *)pkt);
}

extern float D_0038366C;
extern float D_00383670;
extern float D_00383674;

void DrawNoodleStrip(float arg0, float arg1) {
    PreDrawNoodleStrip();

    NaMATRIX<float, 4, 4> mat;
    SetNoodleRotationMatrix(mat, arg1);

    float d, e, c;
    if (arg0 <= 1.0f) {
        c = arg0 * -0.28540003f + 0.60540003f;
        e = 0.9375f;
        d = 0.7f;
    } else if (arg0 <= 2.0f) {
        float t = arg0 - 1.0f;
        e = t * 0.66410005f + 0.9375f;
        c = t * -0.3786f + 0.32f;
        d = t * 0.3f + 0.7f;
    } else {
        float t = arg0 - 2.0f;
        e = t * -0.66400003f + 1.6016001f;
        c = t * 0.5086f + -0.0586f;
        d = t * -0.3f + 1.0f;
    }

    D_00383670 = d;
    D_0038366C = c;
    D_00383674 = e;

    DrawNoodleStripChunk(mat);
}

void StartNoodleRotation() {
    noodleRotationState = 0;
    noodleChangeTimer = 900;
    noodleRotation = 0.001f;
    noodleAngle = 0.0f;
}

extern float g_NoodleStepB;
extern int g_NoodleDecelerating;
extern float g_NoodleDecel;

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", UpdateNoodleRotation__Fv);
#else
void UpdateNoodleRotation() {
    if (g_NoodlePause != 0) {
        return;
    }

    if (prCurrentStage == 6) {
        noodleAngle += g_NoodleStep * 0.001f;
        return;
    }

    int state = noodleRotationState;
    if (state == 0 || state == 2) {
        if (--noodleChangeTimer == 0) {
            noodleChangeTimer = 900;
            state = (state + 1) % 4;
            noodleRotationState = state;
        }
    }

    float rot;
    if (state == 1) {
        rot = noodleRotation - 1.6666668e-05f;
        if (rot < -0.001f) {
            rot = -0.001f;
            noodleRotationState = 2;
        }
        noodleRotation = rot;
    } else if (state == 3) {
        rot = noodleRotation + 1.6666668e-05f;
        if (0.001f < rot) {
            rot = 0.001f;
            noodleRotationState = 0;
        }
        noodleRotation = rot;
    } else {
        rot = noodleRotation;
    }

    noodleAngle += g_NoodleStep * rot;
}
#endif

void PrUpdateMendererSpeed() {
    if (g_NoodlePause != 0) {
        return;
    }
    if (g_NoodleDecelerating == 0) {
        return;
    }
    float s;
    if (prCurrentStage == 6 || prCurrentStage == 16) {
        s = g_NoodleStepB - g_NoodleDecel;
        if (s < 0.0f) {
            g_NoodleStepB = 0.0f;
            return;
        }
        goto store_b;
    } else {
        s = g_NoodleStep - g_NoodleDecel;
        if (s < 0.0f) {
            g_NoodleStep = 0.0f;
            return;
        }
        goto store_a;
    }
store_b:
    g_NoodleStepB = s;
    return;
store_a:
    g_NoodleStep = s;
}

PR_EXTERN
void PrDecelerateMenderer(u_int frames) {
    if (prCurrentStage == 6 || prCurrentStage == 16) {
        frames = 120;
    }
    float d;
    if (prCurrentStage == 6 || prCurrentStage == 16) {
        d = g_NoodleStepB / (float)frames;
    } else {
        d = g_NoodleStep / (float)frames;
    }
    g_NoodleDecelerating = 1;
    d *= 1.2f;
    g_NoodleDecel = d;
}

PR_EXTERN
void PrRestartMenderer() {
    g_NoodleStep = 1.2f;
    g_NoodleStepB = 1.0f;
    g_NoodleDecelerating = 0;
}

#include <math.h>
#include "dma.h"
#include "mfifo.h"

extern float D_00383654;
extern float D_00383678;
extern float D_00383698;
void UpdateNoodleRotation();
void PrFadeFrameImage(float arg0);
void PrCreateAlphaModulation(float ratio);
void PrStartAwfulRotation();
void PrBlendNoodleImage(bool blend);

static inline float AbsDiffM(float x) {
    return (x < 0.0f) ? -x : x;
}

void DrawMenderer() {
    float ratio = prMendererRatio;
    if (ratio == 0.0f) {
        StartNoodleRotation();
        return;
    }

    if (g_NoodlePause == 0) {
        float t = D_00383678 + g_NoodleStep * 0.0032f;
        if (t > 1.0f) {
            t -= 1.0f;
        }
        D_00383678 = t;
    }

    if (prCurrentStage == 8 || prCurrentStage == 18) {
        return;
    }

    if (ratio != 1.0f && ratio != 2.0f) {
        noodleChangeTimer = 900;
    }

    UpdateNoodleRotation();

    float rotStep = (noodleRotation >= 0.0f) ? 0.01f : -0.01f;
    rotStep *= g_NoodleStep;
    if (g_NoodlePause != 0) {
        rotStep = 0.0f;
    }

    if (ratio <= 1.0f) {
        ratio = sqrtf(ratio) * 2.0f - 1.0f;
        noodleAngle += rotStep * (1.0f - ratio);
    } else {
        float d = AbsDiffM(ratio - 1.5f);
        noodleAngle += rotStep * (0.5f - d);
        PrFadeFrameImage(1.0f - d * 2.0f);
        PrWaitDmaFinish(SCE_DMA_GIF);
    }

    if (noodleAngle > 1.0f) {
        noodleAngle -= 1.0f;
    }
    if (noodleAngle < 0.0f) {
        register float a asm("$f0") = noodleAngle + 1.0f;
        noodleAngle = a;
    }

    float alphaRatio;
    if (ratio > 1.5f) {
        alphaRatio = (ratio - 1.5f) * 0.5f;
    } else if (ratio > 1.0f) {
        alphaRatio = (1.5f - ratio) * 2.0f;
    } else {
        alphaRatio = 1.0f;
    }

    if (ratio != 0.0f) {
        PrCreateAlphaModulation(alphaRatio);
    }

    PrStartMfifo();

    if (g_NoodlePause == 0) {
        if (D_00383698 <= 1.6f && ratio > 1.0f) {
            PrStartAwfulRotation();
        }
        D_00383698 = ratio;
    }

    if (prCurrentStage == 19 || prCurrentStage == 6) {
        if (ratio <= 1.0f) {
            DrawNoodleStrip(ratio, noodleAngle);
        } else if (ratio <= 1.4f) {
            DrawNoodleStrip((ratio - 1.0f) / 0.4f + 1.0f, noodleAngle);
        } else if (ratio <= 1.6f) {
            DrawNoodleStrip(2.0f, noodleAngle);
        } else {
            DrawNoodleStrip((ratio - 1.6f) * 0.6f / 0.4f + 2.0f, noodleAngle);
        }
    } else if (prMendererDirection != 0) {
        if (ratio <= 0.5f) {
            DrawNoodleStrip(ratio * 2.0f, noodleAngle);
        } else if (ratio <= 0.6f) {
            DrawNoodleStrip(1.0f, noodleAngle);
        } else if (ratio <= 1.0f) {
            DrawNoodleStrip(1.0f - (ratio - 0.6f) * 0.5f / 0.4f, noodleAngle);
        } else if (ratio <= 1.13f) {
            DrawNoodleStrip((ratio - 1.0f) * 0.5f / 0.13f + 0.5f, noodleAngle);
        } else if (ratio <= 1.4f) {
            DrawNoodleStrip((ratio - 1.13f) / 0.26f + 1.0f, noodleAngle);
        } else if (ratio <= 1.6f) {
            DrawNoodleStrip(2.0f, noodleAngle);
        } else {
            DrawNoodleStrip((ratio - 1.6f) / 0.4f + 2.0f, noodleAngle);
        }
    } else {
        if (ratio <= 1.0f) {
            DrawNoodleStrip(ratio * 0.5f, noodleAngle);
        } else if (ratio <= 1.13f) {
            DrawNoodleStrip((ratio - 1.0f) * 0.5f / 0.13f + 0.5f, noodleAngle);
        } else if (ratio <= 1.4f) {
            DrawNoodleStrip((ratio - 1.13f) / 0.26f + 1.0f, noodleAngle);
        } else if (ratio <= 1.6f) {
            DrawNoodleStrip(2.0f, noodleAngle);
        } else {
            DrawNoodleStrip((ratio - 1.6f) / 0.4f + 2.0f, noodleAngle);
        }
    }

    PrWaitDmaFinish(SCE_DMA_VIF1);
    while (*VIF1_STAT & 3) {
        ;
    }

    PrBlendNoodleImage(ratio == 0.0f);
    PrStopMfifo();
}

PR_EXTERN
void PrSetMendererRatio(float ratio) {
    prMendererRatio = ratio;
}

PR_EXTERN
float PrGetMendererRatio() {
    return prMendererRatio;
}

PR_EXTERN
void PrSetMendererDirection(int direction) {
    prMendererDirection = direction;
}

PR_EXTERN
int PrGetMendererDirection() {
    return prMendererDirection;
}

PR_EXTERN
void PrSetMendererColorModulation(int color) {
    prMendererColorModulation = color;
}

PR_EXTERN
int PrIsMendererColorModulation() {
    return prMendererColorModulation;
}

PR_EXTERN
void PrInitializeMenderer(u_int tbp, void *noodlePicture, u_int fbp) {
    sceGsZbuf zbuf = prRenderStuff.m_zbuf;

    prMendererColorModulation = 1;
    mendererTbp = tbp;
    mendererFbp = fbp;

    u_int width = PrGetBitSize(256);
    u_int height = PrGetBitSize(16);

    PrInitializeTextureCreation(tbp, zbuf.ZBP, width, height);
    InitializeNoodleStripRendering(tbp, fbp, width, height);
    PrInitializeAlphaModulation();
    PrInitializeAwfulBackground(noodlePicture);
    PrInitializeNoodlePolygonPosition();
    StartNoodleRotation();

    mendererInitialized = 1;
}

PR_EXTERN
void PrRenderMenderer() {
    DrawMenderer();
    prRenderStuff.m_statistics.render_time8 = *T3_COUNT;
#if defined(PRD_SYORI)
    SyoriUpdateStats(&prRenderStuff.mStatistics);
#endif
}

/* nalib/navector.h */
float* func_0014F3B8(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) asm("func_0014F3B8");

float* func_0014F3B8(
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

float* func_0014F410(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) asm("func_0014F410");

float* func_0014F410(
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

/* prlib/menderer.cpp */
PR_EXTERN
void _GLOBAL_$I$prMendererRatio(void) {
}

/* nalib/navector.h */
#define PR_VU0_WAIT()                                \
    do {                                             \
        int _wait = 3;                               \
        do {                                         \
            _wait--;                                 \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
        } while (_wait != -1);                       \
    } while (0)

extern "C" void* func_00145DB0(
    void* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
);

static inline void* SetMat44(
    void* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) {
    PR_VU0_WAIT();
    return func_00145DB0(self, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F4C8);
#else
/* self = diag(x, y, z, 1) */
void* func_0014F4C8(void* self, const float& x, const float& y, const float& z) asm("func_0014F4C8");
void* func_0014F4C8(void* self, const float& x, const float& y, const float& z) {
    SetMat44(
        self,
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    return self;
}
#endif

void* func_0014F5D0(void* self, const float& a1, const float& a2, const float& a3) asm("func_0014F5D0");
void* func_0014F5D0(void* self, const float& a1, const float& a2, const float& a3) {
    SetMat44(
        self,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        a1, a2, a3, 1.0f
    );
    return self;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F6D8);
#else
/* Axis rotation matrix (0 = X, 1 = Y, otherwise Z) */
void* func_0014F6D8(void* self, int axis, const float* angle) asm("func_0014F6D8");
void* func_0014F6D8(void* self, int axis, const float* angle) {
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
#endif

/* prlib/menderer.cpp */
INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967E0);

INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967F0);
