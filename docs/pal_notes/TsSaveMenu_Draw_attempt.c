// TsSaveMenu_Draw attempt (42/150 instructions differ: regalloc s0..s4 shifted due to ppat in s0)
void TsSaveMenu_Draw(SPR_PKT pk, SPR_PRM *spr) {
    SAVE_MENU *pfw;
    float      fRate;
    float      rt0, rt1, rt2;
    int        ton;
    int        i;
    int        dmy;
    int        bPut, bState;
    PATPOS    *ppat;

    pfw = &SaveMenu;

    fRate = (MNSceneGetMusicFitTimer() % 72) / 72.0f;
    fRate = -sinf(fRate * 6.2831855f);
    dmy = MNSceneGetMusicFitTimer() % 180;
    pfw->cani.fswing = fRate;

    if (TsANIME_GetRate(&pfw->awork, &rt0, &rt1, &rt2)) {
        spr->zoom.centerX = SAVEWZoom_CXY[0].x;
        spr->zoom.centerY = SAVEWZoom_CXY[0].y;
        spr->zoom.zoomX = rt1;
        spr->zoom.zoomY = rt0;
        spr->zoom.isOn = TRUE;
        ton = (int)(rt0 * 256.0f);
    } else {
        spr->zoom.isOn = FALSE;
        ton = 256;
    }

    spr->zx = 1.0f;
    spr->zy = 0.5f;
    PkALPHA_Add(pk, 0x44);

    spr->rgba0 = GetDToneColor(0x404040, 0x80808080, ton);
    ppat = SAVE_MENU_SELPAT;
    pfw->cani.habgr = GetDToneColor(0x404040, 0x80ffffff, ton);
    pfw->cani.nabgr = GetDToneColor(0x404040, 0x80808080, ton);

    for (i = 0; i < 2; i++, ppat++) {
        bPut = 0;
        bState = 0;
        if (i == pfw->selno) {
            bPut = 1;
            if (pfw->cani.okTim != 0) {
                bState = 2;
                bPut = 2;
            } else {
                bState = (pfw->cani.onTim != 0) ? 3 : 1;
            }
        }

        switch (bPut) {
        case 0:
            spr->rgba0 = pfw->cani.nabgr;
            break;
        case 1:
            spr->rgba0 = pfw->cani.habgr;
            break;
        }

        TsPopCusPut(pk, spr, 3, &pfw->cani, bState, i, ppat, 0, 0);
    }

    spr->zoom.isOn = FALSE;
}
