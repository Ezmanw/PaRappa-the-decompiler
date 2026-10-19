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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/menderercreate", PrInitializeTextureCreation__FUiUiUiUi);
#else
u_int PrGetMendererCreateTextureAddress();

extern u_char D_01C74670[5][240]; /* per-texture GIF/VIF packets */
extern u_char D_01C74B20[96];     /* DMA chain header (mendererDrawWords) */
extern u_char D_01C74B80[224];    /* texture setup GIF packet */
extern u_long D_00371748;         /* prRenderStuff.m_zbuf */
extern int D_003836A8;

#define MC_W64(base, off) (*(u_long *)((base) + (off)))
#define MC_W32(base, off) (*(u_int *)((base) + (off)))
#define MC_W16(base, off) (*(u_short *)((base) + (off)))
#define MC_W8(base, off) (*(u_char *)((base) + (off)))

/* Rewrite NLOOP / EOP / FLG / NREG of a GIFtag (leaves the PRIM fields alone). */
static inline u_long MendererGifTag(u_long old, u_long nloop, u_int eop, u_long nreg) {
    u_long tag = (old & ~0x7fffUL) | nloop;
    if (eop) {
        tag |= 0x8000;
    } else {
        tag &= ~0x8000UL;
    }
    tag &= ~(3UL << 58);
    tag &= ~(0xfUL << 60);
    return tag | (nreg << 60);
}

void PrInitializeTextureCreation(u_int arg0, u_int arg1, u_int arg2, u_int arg3) {
    u_long zbuf = D_00371748;
    u_int zbp_blocks = arg1 << 5;
    u_long tbp_hi = (u_long)arg0 << 32;

    PrSynchronizeMendererParameter(0.0f);

    /* FRAME_1 data: FBP = arg1, FBW = 10 */
    u_long frame = (u_int)(arg1 | 0xa0000);
    /* TEX0_1 data: TBP0 = arg0, TBW = 4, TW = arg2, TH = arg3, TCC = 1 */
    u_long tex0 = (u_long)(u_int)(arg0 | 0x10000) | (1UL << 34) | ((u_long)arg3 << 30) | ((u_long)arg2 << 26);

    u_int i;
    for (i = 0; i < 5; i++) {
        u_char *p = D_01C74670[i];
        u_long regs;

        MC_W32(p, 12) = 0;
        MC_W32(p, 28) = 0;
        MC_W32(p, 44) = 0;
        MC_W32(p, 60) = 0;
        MC_W32(p, 64) = 0;
        MC_W32(p, 68) = 0;
        MC_W32(p, 72) = 0;
        *(float *)(p + 76) = (float)(i * 16);

        /* GIFtag: NLOOP 8, A+D */
        MC_W64(p, 80) = MendererGifTag(MC_W64(p, 80), 8, 0, 1);
        MC_W64(p, 88) = (MC_W64(p, 88) & ~0xfUL) | 0xe;

        MC_W64(p, 96) = frame;                                  /* FRAME_1 */
        MC_W64(p, 104) = 0x4c;
        MC_W64(p, 112) = 0;                                     /* XYOFFSET_1 */
        MC_W64(p, 120) = 0x18;
        MC_W64(p, 128) = ((u_long)(15 + i * 16) << 48)          /* SCISSOR_1 */
                       | ((u_long)(i * 16) << 32)
                       | 0x027f0000UL;
        MC_W64(p, 136) = 0x40;
        MC_W64(p, 144) = 0x3fe0000080808080UL;                  /* RGBAQ */
        MC_W64(p, 152) = 1;
        MC_W64(p, 160) = 0x1e;                                  /* PRIM: sprite, IIP, TME */
        MC_W64(p, 168) = 0;
        MC_W64(p, 176) = 0x3c003fc00aUL;                        /* CLAMP_1 */
        MC_W64(p, 184) = 8;
        MC_W64(p, 192) = tex0;                                  /* TEX0_1 */
        MC_W64(p, 200) = 6;
        MC_W64(p, 208) = 0x60;                                  /* TEX1_1 */
        MC_W64(p, 216) = 0x14;

        /* GIFtag: NLOOP 64, EOP, PACKED, NREG 6 (ST RGBAQ XYZ2 ST RGBAQ XYZ2) */
        MC_W64(p, 224) = MendererGifTag(MC_W64(p, 224), 64, 1, 6);
        regs = (MC_W64(p, 232) & ~0xffffffUL) | 0x512512UL;
        MC_W64(p, 232) = regs;
    }

    /* DMA chain header */
    u_char *hdr = D_01C74B20;
    MC_W16(hdr, 0) = 3;
    MC_W8(hdr, 3) = 0x10;                                       /* cnt */
    MC_W32(hdr, 4) = 0;
    MC_W32(hdr, 8) = 0x11000000;                                /* FLUSH */
    MC_W32(hdr, 12) = 0x50000003;                               /* DIRECT 3 */
    MC_W64(hdr, 16) = MendererGifTag(MC_W64(hdr, 16), 2, 1, 1);
    MC_W64(hdr, 24) = (MC_W64(hdr, 24) & ~0xfUL) | 0xe;
    MC_W64(hdr, 32) = 0x30000;                                  /* TEST_1: ZTE, ZTST ALWAYS */
    MC_W64(hdr, 40) = 0x47;
    MC_W64(hdr, 48) = zbuf | (1UL << 32);                       /* ZBUF_1 with ZMSK */
    MC_W64(hdr, 56) = 0x4e;
    MC_W16(hdr, 64) = 75;
    MC_W8(hdr, 67) = 0x30;                                      /* ref */
    MC_W32(hdr, 68) = (u_int)D_01C74670;
    MC_W32(hdr, 72) = 0;
    MC_W32(hdr, 76) = 0x6c4b0000;                               /* UNPACK V4-32, 75 qwords */
    MC_W16(hdr, 80) = 0;
    MC_W8(hdr, 83) = 0x70;                                      /* end */
    MC_W32(hdr, 84) = 0;
    MC_W32(hdr, 88) = 0x11000000;                               /* FLUSH */
    MC_W32(hdr, 92) = PrGetMendererCreateTextureAddress() | 0x14000000; /* MSCAL */

    /* Texture setup packet */
    u_char *setup = D_01C74B80;
    MC_W64(setup, 0) = MendererGifTag(MC_W64(setup, 0), 13, 1, 1);
    MC_W64(setup, 8) = (MC_W64(setup, 8) & ~0xfUL) | 0xe;
    MC_W64(setup, 16) = (u_long)(u_int)(zbp_blocks | 0xa0000) | tbp_hi | (1UL << 50); /* BITBLTBUF */
    MC_W64(setup, 24) = 0x50;
    MC_W64(setup, 32) = 1UL << 52;                              /* TRXPOS */
    MC_W64(setup, 40) = 0x51;
    MC_W64(setup, 48) = (80UL << 32) | 0x100;                   /* TRXREG */
    MC_W64(setup, 56) = 0x52;
    MC_W64(setup, 64) = 2;                                      /* TRXDIR */
    MC_W64(setup, 72) = 0x53;
    /* +80: SCISSOR_1 data (filled in later) */
    MC_W64(setup, 88) = 0x40;
    MC_W64(setup, 96) = 0;                                      /* TEXFLUSH */
    MC_W64(setup, 104) = 0x3f;
    MC_W64(setup, 112) = zbuf;                                  /* ZBUF_1 */
    MC_W64(setup, 120) = 0x4e;
    MC_W64(setup, 128) = 0x32001;                               /* TEST_1 */
    MC_W64(setup, 136) = 0x47;
    MC_W64(setup, 144) = 0x106;                                 /* PRIM: sprite, FST */
    MC_W64(setup, 152) = 0;
    MC_W64(setup, 160) = 0;                                     /* XYZ2 (0, 0) */
    MC_W64(setup, 168) = 5;
    MC_W64(setup, 176) = 0x6001000;                             /* XYZ2 (0x1000, 0x600) */
    MC_W64(setup, 184) = 5;
    /* +192: FRAME_1 data (filled in later) */
    MC_W64(setup, 200) = 0x4c;
    /* +208: XYOFFSET_1 data (filled in later) */
    MC_W64(setup, 216) = 0x18;

    D_003836A8 = 1;
}
#endif

extern float prMendererRatio;
extern u_int mendererDrawWords[];
void PrWaitDmaFinish(u_int);

void CreateMendererTexture(float arg0) {
    PrSynchronizeMendererParameter(arg0);

    float speed;
    if (prMendererRatio <= 1.0f) {
        speed = 1.0f;
    } else if (prMendererRatio <= 1.4f) {
        speed = (prMendererRatio - 1.0f) * (17.0f / 3.0f) / 0.4f + 1.0f;
    } else if (prMendererRatio <= 1.6f) {
        speed = 20.0f / 3.0f;
    } else {
        speed = (2.0f - prMendererRatio) * (17.0f / 3.0f) / 0.4f + 1.0f;
    }

    speed *= g_NoodleStep;
    if (g_NoodlePause != 0) {
        speed = 0.0f;
    }

    D_003836AC = 1;
    float step = speed / 60.0f;

    for (u_int i = 0; i < 5; i++) {
        MendererParamEntry *param = &mendererParams[i];
        for (u_int j = 0; j < 3; j++) {
            float p = mendererPhase[i][j] + step;
            if (p < 0.0f) {
                while (p < 0.0f) {
                    p += 1.0f / param->f2[j];
                }
            } else {
                while (p >= 1.0f / param->f2[j]) {
                    p -= 1.0f / param->f2[j];
                }
            }
            param->f4[j] = p;
            mendererPhase[i][j] = p;
        }
    }

    FlushCache(WRITEBACK_DCACHE);
    PrWaitDmaFinish(SCE_DMA_VIF1);

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_VIF1);
    chan->chcr.TTE = 1;
    sceDmaSend(chan, mendererDrawWords);
}

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
