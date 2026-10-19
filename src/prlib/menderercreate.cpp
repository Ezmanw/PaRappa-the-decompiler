#include "common.h"

#include <eekernel.h>
#include <eestruct.h>
#include <libdma.h>
#include <libgraph.h>

extern int D_003836AC;
extern u_int D_003836A4;

struct MendererWaitPacket {
    u_long data[5][2];
    sceGsScissor scissor;
    u_long scissoraddr;
    u_long data2[6][2];
    sceGsFrame frame;
    u_long frameaddr;
    sceGsXyoffset xyoffset;
    u_long xyoffsetaddr;
};

static MendererWaitPacket waitPacket;

extern float D_003836B0;
extern float D_00383688;
extern int prCurrentStage;
extern int g_NoodlePause;
extern float g_NoodleStep;

void PrUpdateMendererSpeed();
void PrUpdateAwfulMenderer();
void CreateMendererTexture(float arg0);

struct MendererParamEntry {
    float f0[4];
    float f1[4];
    float f2[4];
    float f3[4];
    float f4[4];
    char unk80[160];
};

struct MendererRomParam {
    float a;
    float b;
    float c;
    float d;
};

static float mendererPhase[5][3];
static MendererParamEntry mendererParams[5];
extern MendererRomParam D_0037FE60[5][3];

#include <math.h>

void PrSynchronizeMendererParameter(float ratio) {
    float invRatio = 1.0f - ratio;

    for (u_int i = 0; i < 5; i++) {
        MendererParamEntry *dst = &mendererParams[i];

        for (u_int j = 0; j < 3; j++) {
            register float c asm("$f2") = D_0037FE60[i][j].c;
            asm("" : "+f"(c) : : "memory");
            dst->f0[j] = D_0037FE60[i][j].a;
            dst->f1[j] = D_0037FE60[i][j].b;
            dst->f2[j] = c * (5.0f / 6.0f);
            dst->f3[j] = D_0037FE60[i][j].d;

            if (i != 0) {
                dst->f0[j] = invRatio * dst->f0[j] + ratio * mendererParams[0].f0[j];
                dst->f1[j] = invRatio * dst->f1[j] + ratio * mendererParams[0].f1[j];
                dst->f2[j] = invRatio * dst->f2[j] + ratio * mendererParams[0].f2[j];
                dst->f3[j] = invRatio * dst->f3[j] + ratio * mendererParams[0].f3[j];
            }
        }

        if (i != 0 && ratio == 1.0f) {
            for (u_int j = 0; j < 3; j++) {
                float halfPeriod = (1.0f / dst->f2[j]) * 0.5f;
                float diff = mendererPhase[0][j] - mendererPhase[i][j];
                if (fabsf(diff) >= halfPeriod) {
                    diff = -diff;
                }
                mendererPhase[i][j] += diff / 90.0f;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/menderercreate", PrInitializeTextureCreation__FUiUiUiUi);

INCLUDE_ASM("asm/nonmatchings/prlib/menderercreate", CreateMendererTexture__Ff);

PR_EXTERN
void PrCreateMendererTexture() {
    PrUpdateMendererSpeed();
    PrUpdateAwfulMenderer();

    if (prCurrentStage == 19 || prCurrentStage == 6 || prCurrentStage == 8 || prCurrentStage == 18) {
        CreateMendererTexture(0.0f);
        return;
    }

    register float t asm("$f12") = D_003836B0;
    if (g_NoodlePause == 0) {
        t += g_NoodleStep * (1.0f / 720.0f);
        D_003836B0 = t;
    }

    if (t >= 5.0f) {
        t -= 5.0f;
        D_003836B0 = t;
    }

    register float val asm("$f0");
    if (t < 1.0f) {
        val = 0.0f;
    } else if (t < 2.0f) {
        val = t - 1.0f;
    } else if (t < 4.0f) {
        val = 1.0f;
    } else {
        val = 5.0f - t;
    }

    CreateMendererTexture(D_00383688 = val);
}

void PrWaitMendererTexture(sceGsDrawEnv1 *env, const sceGsFrame &frame, const sceGsXyoffset &xyoffset) {
    if (D_003836AC == 0) {
        return;
    }

    D_003836AC = 0;

    D_003836A4 = (waitPacket.frame = frame).FBP;
    waitPacket.xyoffset = xyoffset;
    waitPacket.scissor = env->scissor1;

    FlushCache(WRITEBACK_DCACHE);
    sceGsSyncPath(0, 0);

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_GIF);
    chan->chcr.TTE = 0;
    sceDmaSendN(chan, &waitPacket, 14);
    sceGsSyncPath(0, 0);
}

INCLUDE_RODATA("asm/nonmatchings/prlib/menderercreate", D_00396800);
