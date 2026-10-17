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

static int noodlePolygonOrder[115];

void PrInitializeNoodlePolygonPosition() {
    for (unsigned int i = 0; i < 115; i++) {
        noodlePolygonOrder[i] = i;
    }
    for (unsigned int i = 0; i < 114; i++) {
        unsigned int j = PrRandom() % (115 - i);
        if (j != i) {
            int tmp = noodlePolygonOrder[i];
            noodlePolygonOrder[i] = noodlePolygonOrder[j];
            noodlePolygonOrder[j] = tmp;
        }
    }
    InitializeNoodlePositionData();
}

INCLUDE_ASM("asm/nonmatchings/prlib/mendererdata", PrGetNoodlePolygonPosition__FPt8NaVECTOR2Zfi4Ui);

/* nalib/navector.h */
float* func_00151D78(float* self, const float& a0, const float& a1, const float& a2, const float& a3) asm("func_00151D78");
float* func_00151D78(float* self, const float& a0, const float& a1, const float& a2, const float& a3) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}

float* func_00151DA0(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8) asm("func_00151DA0");
float* func_00151DA0(float* self, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8) {
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

float* func_00151DF8(float* self, const float& a0, const float& a1, const float& a2, const float& a3) asm("func_00151DF8");
float* func_00151DF8(float* self, const float& a0, const float& a1, const float& a2, const float& a3) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}
