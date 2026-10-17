#include "common.h"

#include "random.h"

struct PrNoodlePositionData {
    float phase;
    float x, y, z;
    float vx, vy, vz;
    unsigned int count;
};

extern int   g_NoodlePause;
extern float g_NoodleStep;

void SetNextTarget(PrNoodlePositionData *data);

static PrNoodlePositionData noodlePositionData[115];

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", SetNextTarget__FP20PrNoodlePositionData);

void InitializeNoodlePositionData() {
    for (unsigned int i = 0; i < 115; i++) {
        PrNoodlePositionData *d = &noodlePositionData[i];

        d->x = (PrFloatRandom() - 0.5f) * 2.0f * 0.1f;
        d->y = (PrFloatRandom() - 0.5f) * 2.0f * 0.04f;
        d->z = (PrFloatRandom() - 0.5f) * 2.0f * 0.1f;
        d->phase = (float)((i * 2) % 115) / 115.0f;
        SetNextTarget(d);
    }
}

void UpdateNoodlePositionData(PrNoodlePositionData *data) {
    if (g_NoodlePause == 0) {
        if (--data->count == -1) {
            SetNextTarget(data);
        }
        data->x += g_NoodleStep * data->vx;
        data->y += g_NoodleStep * data->vy;
        data->z += g_NoodleStep * data->vz;
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", GetSynchronizeRatio__FPC20PrNoodlePositionData);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", PrInitializeNoodlePolygonPosition__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", PrGetNoodlePolygonPosition__FPt8NaVECTOR2Zfi4Ui);

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", func_00151D78);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", func_00151DA0);

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", func_00151DF8);
