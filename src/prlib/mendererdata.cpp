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

static inline float Abs(float x) {
    asm("" : "+f"(x));
    return (x >= 0.0f) ? x : -x;
}

void SetNextTarget(PrNoodlePositionData *data) {
    float tx = (PrFloatRandom() - 0.5f) * 2.0f * 0.1f;
    float ty = (PrFloatRandom() - 0.5f) * 2.0f * 0.04f;
    float tz = (PrFloatRandom() - 0.5f) * 2.0f * 0.1f;

    float dx = tx - data->x;
    float cx = Abs(dx) / 0.2f * 180.0f;
    float cy = Abs(ty - data->y) / 0.08f * 180.0f;
    float cz = Abs(tz - data->z) / 0.2f * 180.0f;

    float cxy = cx >= cy ? cx : cy;
    float cmax = cz >= 1.0f ? cz : 1.0f;
    if (cxy >= cmax) {
        cmax = cxy;
    }

    data->count = (u_int)cmax;
    data->vx = dx / (float)data->count;
    data->vy = (ty - data->y) / (float)data->count;
    data->vz = (tz - data->z) / (float)data->count;
}

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

extern float D_00383654;
extern float D_00383658;
extern float D_00383678;

static inline float AbsDiff(float x) {
    return (x >= 0.0f) ? x : -x;
}

float GetSynchronizeRatio(const PrNoodlePositionData *data) {
    register float d0 asm("$f0") = AbsDiff(data->phase - D_00383678);
    asm("" : "+f"(d0));
    register float diff asm("$f3") = d0;
    register float d2 asm("$f0") = 1.0f - diff;
    if (d2 >= diff) {
        d2 = diff;
    }
    float ratio = 1.0f - d2;
    ratio = ratio * 3.0f - 1.8f;
    ratio = ratio * D_00383658;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    } else if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (D_00383654 >= 1.2f && D_00383654 <= 1.8f) {
        float w = AbsDiff(1.5f - D_00383654) / 0.3f;
        ratio = ratio * w + (1.0f - w);
    }
    return ratio;
}

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
