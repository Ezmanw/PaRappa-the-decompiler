#include "common.h"

#include "dma.h"
#include "random.h"
#include "wave.h"

#include <eekernel.h>
#include <eetypes.h>
#include <libdma.h>
#include <libgraph.h>

/* data */
extern u_long mendererFadeData[7][2];

/* sdata */
extern int prAwfulMendererPause;
extern float prAwfulMendererSpeed;
extern float awfulBackgroundAngle;

/* sbss */
extern u_int mendererAwfulColor;
extern int awfulRotationState;
extern u_int awfulChangeTimer;
extern float awfulRotation;

/* bss */
extern WAVE_STR awfulWave;

void SetNextSwitchRotationTimer() {
    awfulChangeTimer = (u_int)((PrFloatRandom() * 4.0f + 3.0f) * 60.0f);
}

#include "tim2.h"

extern TIM2_PICTUREHEADER *awfulPictureHeader;
extern float D_003719B0[4];

float GetAwfulRotation() {
    if (awfulRotationState == 0 || awfulRotationState == 2) {
        u_int t = awfulChangeTimer - 1;
        awfulChangeTimer = t;
        if (t == 0) {
            awfulChangeTimer = 0;
            awfulRotationState = (awfulRotationState + 1) % 4;
            SetNextSwitchRotationTimer();
        }
    }

    if (awfulRotationState == 1) {
        awfulRotation -= 0.016f / 180.0f;
        if (awfulRotation < -0.016f) {
            awfulRotation = -0.016f;
            awfulRotationState = 2;
        }
    } else if (awfulRotationState == 3) {
        awfulRotation += 0.016f / 180.0f;
        if (awfulRotation > 0.016f) {
            awfulRotation = 0.016f;
            awfulRotationState = 0;
        }
    }

    return awfulRotation;
}

void PrStartAwfulRotation() {
    awfulRotationState = 0;
    awfulRotation = 0.016f;
    SetNextSwitchRotationTimer();
}

void PrFadeFrameImage(float arg0) {
    if (arg0 == 0.0f) {
        return;
    }

    u_int alp = (u_int)(arg0 * 128.0f + 0.5f);
    mendererFadeData[3][0] =
        SCE_GS_SET_RGBAQ(
            (mendererAwfulColor >> 0 ) & 255,
            (mendererAwfulColor >> 8 ) & 255,
            (mendererAwfulColor >> 16) & 255,
            alp & 255,
            0
        );

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_GIF);
    chan->chcr.TTE = 0;

    FlushCache(WRITEBACK_DCACHE);
    PrWaitDmaFinish(SCE_DMA_GIF);
    sceDmaSendN(chan, &mendererFadeData, 7);
}

void PrInitializeAwfulBackground(void *picture) {
    if (picture != NULL) {
        awfulPictureHeader = Tim2GetPictureHeader(picture, 0);
        u_int color = Tim2GetClutColor(awfulPictureHeader, 0, 0);
        mendererAwfulColor = color;
        D_003719B0[0] = (float)((color >> 0) & 255) / 255.0f;
        D_003719B0[1] = (float)((color >> 8) & 255) / 255.0f;
        D_003719B0[2] = (float)((color >> 16) & 255) / 255.0f;
        D_003719B0[3] = 1.0f;
    } else {
        awfulPictureHeader = NULL;
    }

    WaveCtrlInit(&awfulWave, 640, 256, WM_WSLICE);
}

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", PrDrawAwfulBackground__FG10sceGsFrame);

void PrUpdateAwfulMenderer() {
    if (prAwfulMendererPause != 0) {
        return;
    }
    float angle = awfulBackgroundAngle + prAwfulMendererSpeed * GetAwfulRotation();
    if (angle >= 6.2831855f) {
        angle -= 6.2831855f;
    }
    if (angle < 0.0f) {
        angle += 6.2831855f;
    }
    awfulBackgroundAngle = angle;
    WaveCtrlUpdate(&awfulWave, prAwfulMendererSpeed);
}

/* nalib/navector.h */
float* func_00150CE8(float* self, const float& a0, const float& a1, const float& a2, const float& a3) asm("func_00150CE8");
float* func_00150CE8(float* self, const float& a0, const float& a1, const float& a2, const float& a3) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}

float* func_00150D10(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8
) asm("func_00150D10");
float* func_00150D10(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8
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

