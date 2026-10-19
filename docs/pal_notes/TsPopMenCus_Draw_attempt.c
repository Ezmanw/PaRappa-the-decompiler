extern PATPOS PopMenuSel_Pat[5];
extern PATPOS VSComMenuSel_Pat[4];
extern PATPOS VSComMenuSelH_Pat[4];
extern int POPSel2Btn[5];

void TsPopMenCus_Draw(SPR_PKT pk, SPR_PRM *spr, POPUP_MENU *pfw, int px, int py, u_int hicol, u_int nmcol, int dflg) {
    int     i, next_i;
    int     bPut, bState, bState2;
    float   oy0, fSin;
    PATPOS *ppat;

    oy0 = spr->ofsy;

    for (i = 0; i < 5; i = next_i) {
        pfw->cani.nabgr = nmcol;
        bPut = 0;
        pfw->cani.habgr = hicol;

        spr->ofsy = oy0 + sinf((float)(MNSceneGetMusicFitTimer() % 480) * 6.2831855f * (1.0f / 480.0f) + (float)POPSel2Btn[i] * 2.5132742f) * 3.3f;

        if (i == pfw->selno && pfw->cani.okTim != 0) {
            bState = 2;
            bPut = 2;
        } else {
            bState = 0;
            if (POPSel2Btn[i] == pfw->btnNo) {
                bState = 1;
            }
            if (i == pfw->selno) {
                bPut = 1;
                if (pfw->cani.onTim != 0) {
                    bState = 3;
                }
            }
        }

        if (pfw->isSelLev != 0) {
            bState = (i == 2) ? 5 : 0;
        }

        switch (bPut) {
        case 0:
            spr->rgba0 = pfw->cani.nabgr;
            break;
        case 1:
            spr->rgba0 = pfw->cani.habgr;
            break;
        }

        TsPopCusPut(pk, spr, dflg, &pfw->cani, bState, i, &PopMenuSel_Pat[i], px, py);

        next_i = i + 1;
        if (i == 2) {
            pfw->cani.habgr = nmcol;
            pfw->cani.nabgr = nmcol;
            for (i = 0; i < pfw->levMax; i++) {
                spr->rgba0 = nmcol;
                ppat = &VSComMenuSel_Pat[i];
                bState2 = bState;
                if (pfw->isSelLev != 0) {
                    if (i == pfw->selLev) {
                        bState2 = 6;
                        fSin = sinf((float)(MNSceneGetMusicFitTimer() % 30) * 3.1415927f / 30.0f);
                        spr->rgba0 = GetDToneColor(0x80ffffff, 0x80606060, (int)(fSin * 256.0f));
                        ppat = &VSComMenuSelH_Pat[i];
                    } else {
                        bState2 = 6;
                        spr->rgba0 = 0x80707070;
                    }
                }
                TsPopCusPut(pk, spr, dflg, &pfw->cani, bState2, 2, ppat, px, py);
            }
            next_i = 3;
        }
    }

    spr->ofsy = oy0;
}
