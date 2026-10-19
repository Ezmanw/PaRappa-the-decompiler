// Near-miss attempt for CreateMendererTexture__Ff (145/147 matching, 2/147 instructions differ)
// Only 2 instruction scheduling differences at the end of the inner j loop:
// Target:
//   120 addu v0,a1,t0
//   121 move a2,v1
//   122 addu v0,v0,t4
//   123 addu a0,t1,a1
//   124 swc1 $f1,0(v0)
// Mine:
//   120 addu v0,a1,t0
//   121 addu a0,t1,a1
//   122 addu v0,v0,t4
//   123 move a2,v1
//   124 swc1 $f1,0(v0)

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
            mendererPhase[i][j] = p;
            asm("" : "+f"(p));
            param->f4[j] = p;
        }
    }

    FlushCache(WRITEBACK_DCACHE);
    PrWaitDmaFinish(SCE_DMA_VIF1);

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_VIF1);
    chan->chcr.TTE = 1;
    sceDmaSend(chan, mendererDrawWords);
}
