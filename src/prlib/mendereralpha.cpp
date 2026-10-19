#include "common.h"

#include "random.h"
#include "renderstuff.h"
#include "utility.h"
#include "microprogram.h"

#include <eekernel.h>
#include <eestruct.h>
#include <libdma.h>
#include "dma.h"

struct AlphaWork {
    float pad0[4];
    float f16[8];
    float i48[8];
    float f80[8];
    float f112[8];
    float f144[8];
    float f176[8];
    float f208[8];
    char pad240[12];
    float f252;
};

struct AlphaPacket {
    u_long data[16][2];
};

extern AlphaWork alphaWork;
extern AlphaPacket alphaFramePacket;
extern AlphaPacket alphaTexPacket;
extern u_int alphaDrawWords[16];
extern float alphaVelocity[8];
extern float alphaPosition[8];
extern int prCurrentStage;
extern int g_NoodlePause;
extern float g_NoodleStep;
extern float prMendererRatio;
extern u_int mendererFbp;


void PrInitializeAlphaModulation() {
    for (u_int i = 0; i < 8; i++) {
        alphaWork.f16[i] = PrFloatRandom() * 7.0f + 14.0f;
        alphaWork.i48[i] = 0;
        alphaWork.f80[i] = PrFloatRandom() * 0.123f - 0.456f;
        alphaWork.f112[i] = PrFloatRandom() * 0.123f - 0.456f;
        float f = PrFloatRandom() * 0.5f + 0.5f;
        alphaWork.f144[i] = f;
        alphaWork.f176[i] = f;
        alphaPosition[i] = (alphaWork.f16[i] * 0.5f - 3.5f) * 24.0f + 32.0f;
        alphaVelocity[i] = PrFloatRandom() * 0.789f + 0.321f;
        if (PrRandom() & 1) {
            alphaVelocity[i] = -alphaVelocity[i];
        }
    }

    u_int w = PrGetBitSize(640);
    u_int h = PrGetBitSize(256);
    u_long zbp = prRenderStuff.m_zbuf.ZBP;
    alphaFramePacket.data[1][0] = SCE_GS_SET_FRAME(zbp, 10, 0, 0);
    alphaTexPacket.data[2][0] = SCE_GS_SET_TEX0(zbp << 5, 10, 0, w, h, 1, 1, 0, 0, 0, 0, 0);

    alphaDrawWords[10] = PrGetMendererDrawMeshAddress() | 0x14000000;
}

void PrCreateAlphaModulation(float ratio) {
    if (prCurrentStage == 6 || prCurrentStage == 16) {
        ratio = 0.0f;
    }

    for (u_int i = 0; i < 8; i++) {
        if (g_NoodlePause == 0) {
            float q = alphaWork.i48[i];
            float v = alphaVelocity[i];
            float p = q + g_NoodleStep * v;
            alphaWork.i48[i] = p;
            if (p > 0.123f) {
                alphaWork.i48[i] = 0.456f - p;
                alphaVelocity[i] = -v;
                alphaWork.f176[i] = -alphaWork.f176[i];
            } else if (p < 0.234f) {
                alphaWork.i48[i] = 0.567f - p;
                alphaVelocity[i] = -v;
                alphaWork.f176[i] = -alphaWork.f176[i];
            }
        }
        alphaWork.f208[i] = alphaPosition[i] * ratio;
    }

    alphaWork.f252 = prMendererRatio * 128.0f;
    alphaTexPacket.data[1][0] = SCE_GS_SET_FRAME(mendererFbp, 10, 0, 0x00ffffff);
    alphaTexPacket.data[11][0] = SCE_GS_SET_FRAME(mendererFbp, 10, 0, 0);

    FlushCache(WRITEBACK_DCACHE);
    PrWaitDmaFinish(SCE_DMA_VIF1);
    sceDmaSend(sceDmaGetChan(SCE_DMA_VIF1), alphaDrawWords);
}

struct PrMendererSpram {
    char unk0[0x40];
    void *m_work[3];

    void *GetWork() {
        void *p0 = m_work[0];
        void *p1 = m_work[1];
        void *p2 = m_work[2];
        m_work[1] = p2;
        m_work[0] = p1;
        m_work[2] = p0;
        return p0;
    }
};

extern PrMendererSpram *prSpramData;
void PrSendMfifo(const sceDmaTag *tag);

struct GifTagPair {
    u_long data[2];
};

extern const sceDmaTag D_0037FFE0;
extern const GifTagPair D_0037FFF0;

void PrBlendNoodleImage(bool blend) {
    u_long *pkt = (u_long *)prSpramData->GetWork();
    *(sceDmaTag *)pkt = D_0037FFE0;
    *(GifTagPair *)(pkt + 2) = D_0037FFF0;
    u_long *p = pkt + 4;

    *p++ = 0;
    *p++ = SCE_GS_TEXFLUSH;
    *p++ = SCE_GS_SET_TEX0(mendererFbp << 5, 10, 0, 10, 8, 1, 1, 0, 0, 0, 0, 0);
    *p++ = SCE_GS_TEX0_1;
    *p++ = SCE_GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0);
    *p++ = SCE_GS_TEX1_1;
    *p++ = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
    *p++ = SCE_GS_CLAMP_1;
    *p++ = SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1);
    *p++ = SCE_GS_TEST_1;
    if (blend) {
        *p++ = SCE_GS_SET_ALPHA(0, 1, 2, 1, 64);
    } else {
        *p++ = SCE_GS_SET_ALPHA(0, 1, 1, 1, 0);
    }
    *p++ = SCE_GS_ALPHA_1;
    *p++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0) | 8;
    *p++ = SCE_GS_PRIM;
    *p++ = SCE_GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0);
    *p++ = SCE_GS_RGBAQ;
    *p++ = SCE_GS_SET_UV(8, 8);
    *p++ = SCE_GS_UV;
    *p++ = SCE_GS_SET_XYZ2(27648, 30720, 0);
    *p++ = SCE_GS_XYZ2;
    *p++ = SCE_GS_SET_UV(10248, 4104);
    *p++ = SCE_GS_UV;
    *p++ = SCE_GS_SET_XYZ2(37888, 34816, 0);
    *p++ = SCE_GS_XYZ2;

    PrSendMfifo((const sceDmaTag *)pkt);
}

