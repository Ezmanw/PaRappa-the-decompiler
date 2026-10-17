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

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", GetAwfulRotation__Fv);
#if 0
// TODO: nonmatching (branch likely / delay slot choices)
float GetAwfulRotation() {
    if (awfulRotationState == 0 || awfulRotationState == 2) {
        if (--awfulChangeTimer == 0) {
            awfulChangeTimer = 0;
            awfulRotationState = (awfulRotationState + 1) % 4;
            SetNextSwitchRotationTimer();
        }
    }

    if (awfulRotationState == 1) {
        float rotation = awfulRotation - 0.016f / 180.0f;
        if (rotation < -0.016f) {
            rotation = -0.016f;
            awfulRotationState = 2;
        }
        awfulRotation = rotation;
    } else if (awfulRotationState == 3) {
        float rotation = awfulRotation + 0.016f / 180.0f;
        if (rotation > 0.016f) {
            rotation = 0.016f;
            awfulRotationState = 0;
        }
        awfulRotation = rotation;
    }

    return awfulRotation;
}
#endif

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

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", PrInitializeAwfulBackground__FPv);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", PrDrawAwfulBackground__FG10sceGsFrame);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", PrUpdateAwfulMenderer__Fv);
#if 0
// TODO: nonmatching (target keeps extra mov.s copies in the angle wrap)
void PrUpdateAwfulMenderer() {
    if (prAwfulMendererPause != 0) {
        return;
    }

    float angle = awfulBackgroundAngle + prAwfulMendererSpeed * GetAwfulRotation();
    angle = (angle >= 6.2831855f) ? angle - 6.2831855f : angle;
    angle = (angle < 0.0f) ? angle + 6.2831855f : angle;
    awfulBackgroundAngle = angle;

    WaveCtrlUpdate(&awfulWave, prAwfulMendererSpeed);
}
#endif

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", func_00150CE8);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererawful", func_00150D10);
