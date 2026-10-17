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

void PreDrawNoodleStrip() {
    NoodleStripHeader *pkt = (NoodleStripHeader *)prSpramData->GetWork();
    *pkt = noodleStripHeader;
    pkt->data[2][0] = SCE_GS_SET_BITBLTBUF(D_003836A4 << 5, 10, 0, mendererFbp << 5, 10, 0);
    PrSendMfifo((const sceDmaTag *)pkt);
}

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawNoodleStrip__Fff);

void StartNoodleRotation() {
    noodleRotationState = 0;
    noodleChangeTimer = 900;
    noodleRotation = 0.001f;
    noodleAngle = 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", UpdateNoodleRotation__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PrUpdateMendererSpeed__Fv);

extern float g_NoodleStepB;
extern int g_NoodleDecelerating;
extern float g_NoodleDecel;

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

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawMenderer__Fv);
void DrawMenderer();

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
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F3B8);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F410);

/* prlib/menderer.cpp */
PR_EXTERN
void _GLOBAL_$I$prMendererRatio(void) {
}

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F4C8);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F5D0);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F6D8);

/* prlib/menderer.cpp */
INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967E0);

INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967F0);
