extern float JUKEWAV_INITBL[];

void TSJukeCDObj_Draw(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, int anmtime) {
    int     jx, jy;
    int     ton;
    int     i;
    u_int   abgr, abgrs;
    u_int   rabgr, rabgrs;
    PATPOS *ppat;
    float   ox, oy;
    float   rot, rrot;
    float   aox, aoy;
    float   fRate, fSin;
    float   zrate, jzr, rzr;
    float   fx, fy;
    float   rx, ry;
    float   wvrate;

    px += pw->patPos->x;
    py += pw->patPos->y;
    ppat = &JUKEJKT_Pat[pw->patNo];

    jzr = 1.0f;
    ox = spr->ofsx;
    rzr = 1.0f;
    zrate = 1.0f;
    wvrate = 1.0f;
    oy = spr->ofsy;

    PkALPHA_Add(pk, 0x44);

    rot = 0.0f;
    aox = 0.0f;
    rrot = 0.0f;
    aoy = 0.0f;

    switch (pw->state) {
    case TSJKCUS_MSK:
        return;
    case TSJKCUS_DEF:
        abgr = 0x80707070;
        abgrs = 0x30808080;
        pw->vrate = 0.0f;
        break;
    case TSJKCUS_CUR:
    case TSJKCUS_SELOK:
        abgr = 0x80c0c0c0;
        abgrs = 0x30808080;
        zrate = 1.25f;
        pw->vrate = 0.0f;
        break;
    case TSJKCUS_ON:
        pw->time++;
        fRate = pw->time * (1.0f / 15.0f);
        if (pw->time >= 15) {
            pw->time = 0;
            pw->state = TSJKCUS_CUR;
        }
        fSin = fRate * 3.1415927f;
        abgrs = 0x30808080;
        zrate = (sinf(fSin) * 0.2f + 1.0f) * (fRate * 0.25f + 1.0f);
        pw->vrate = sinf(fSin) * 0.1f;
        abgr = GetDToneColor(0x80707070, 0x80c0c0c0, (int)(fRate * 256.0f));
        break;
    case TSJKCUS_OFF:
        pw->time++;
        fRate = pw->time * (1.0f / 15.0f);
        if (pw->time >= 15) {
            pw->time = 0;
            pw->state = TSJKCUS_DEF;
        }
        fSin = fRate * 3.1415927f;
        abgrs = 0x30808080;
        zrate = (1.0 - sinf(fSin) * 0.25) * ((1.0f - fRate) * 0.25f + 1.0f);
        pw->vrate = sinf(fSin) * -0.08f;
        abgr = GetDToneColor(0x80c0c0c0, 0x80707070, (int)(fRate * 256.0f));
        break;
    default:
        abgr = 0x80c0c0c0;
        abgrs = 0x30808080;
        pw->vrate = 0.0f;
        break;
    }

    if (pw->bMsk) {
        abgr = 0x30808080;
        abgrs = 0x18808080;
        pw->vrate = 0.0f;
    }

    rabgr = abgr;
    rabgrs = abgrs;

    switch (pw->anime) {
    case TSJKANM_IN:
        if (pw->atime != 0) {
            pw->innm = 0x200;
            pw->atime--;
            return;
        }
        pw->innm = TSNumMov(pw->innm, 0, 5);
        fRate = 1.0f - (pw->innm / 512.0f);
        rot = (1.0f - fRate) * 6.2831855f;
        aox = (1.0f - fRate) * ((pw->dir != 0) ? 200.0f : -200.0f);
        if (pw->dir2 != 0) {
            aoy = (1.0f - fRate) * 20.0f;
        } else {
            aoy = (1.0f - fRate) * -20.0f;
        }
        ton = (int)(fRate * 256.0f);
        abgr = GetDToneColor(0xffffff, abgr, ton);
        abgrs = GetDToneColor(0x808080, abgrs, ton);
        jzr = (1.0f - fRate) * 1.5f + 1.0f;
        if (pw->innm == 0) {
            pw->anime = TSJKANM_OFF;
            pw->time = 0;
            pw->atime = 0;
        }
        break;

    case TSJKANM_OUT:
        if (pw->atime != 0) {
            pw->atime--;
            pw->innm = 0x100;
            break;
        }
        pw->innm = TSNumMov(pw->innm, 0, 5);
        fRate = 1.0f - (pw->innm / 256.0f);
        rot = fRate * -9.424778f;
        aox = fRate * ((pw->dir != 0) ? -200.0f : 200.0f);
        if (pw->dir2 == 0) {
            aoy = fRate * -20.0f;
        } else {
            aoy = fRate * 20.0f;
        }
        ton = (int)(fRate * 256.0f);
        abgr = GetDToneColor(abgr, 0xffffff, ton);
        abgrs = GetDToneColor(abgr, 0x808080, ton);
        jzr = fRate * 1.2f + 0.5f;
        if (pw->innm == 0) {
            pw->anime = TSJKANM_OFF;
            pw->state = TSJKCUS_MSK;
            pw->time = 0;
            pw->atime = 0;
        }
        break;

    case TSJKANM_PLAY2:
    case TSJKANM_RETURN2:
        if (pw->atime != 0) {
            pw->atime--;
            pw->innm = 0x80;
            pw->oy = pw->rot = pw->ox = 0.0f;
            if (pw->anime == TSJKANM_RETURN2) {
                return;
            }
            break;
        }
        pw->innm = TSNumMov(pw->innm, 0, 12);
        fRate = 1.0f - (pw->innm / 128.0f);
        TsPatGetSize(ppat, &jx, &jy, NULL, NULL);
        fx = (jx + px) - 318;
        fy = (jy + py) - 161;
        if (pw->anime == TSJKANM_RETURN2) {
            fRate = 1.0f - fRate;
        }
        ton = (int)(fRate * 256.0f);
        aox = fx * 0.35f * fRate;
        aoy = fy * 0.5f * fRate;
        abgr = GetDToneColor(abgr, 0x404040, ton);
        abgrs = GetDToneColor(abgrs, 0x808080, ton);
        jzr = 1.0f - fRate * 0.3f;
        if (pw->innm == 0) {
            if (pw->anime == TSJKANM_RETURN2) {
                pw->state = TSJKCUS_DEF;
            } else {
                pw->state = TSJKCUS_MSK;
            }
            pw->anime = TSJKANM_OFF;
            pw->time = 0;
            pw->atime = 0;
        }
        break;

    case TSJKANM_PLAY1:
    case TSJKANM_RETURN1:
        if (pw->atime != 0) {
            pw->atime--;
            pw->innm = 0x200;
            pw->rox = pw->rrot = pw->ox = pw->oy = pw->rot = 0.0f;
            if (pw->anime == TSJKANM_RETURN1) {
                pw->rox = zrate * 0.0f;
                pw->roy = zrate * -43.0f;
                abgr = 0x80c0c0c0;
                abgrs = 0x30808080;
                rabgr = 0x80c0c0c0;
                rabgrs = 0x30808080;
                wvrate = 0.0f;
            }
            break;
        }
        fx = pw->rox = pw->roy = pw->ox = pw->oy = 0.0f;
        fy = -43.0f;
        pw->innm = TSNumMov(pw->innm, 0, 7);
        fRate = 1.0f - (pw->innm / 512.0f);
        rzr = 1.0f;
        if (pw->anime == TSJKANM_RETURN1) {
            fRate = 1.0f - fRate;
        }
        wvrate = 1.0f - fRate;
        fSin = fRate * 0.5f * 3.1415927f;
        rabgr = abgr;
        rabgrs = abgrs;
        pw->rox = (1.0f - cosf(fSin)) * fx * zrate * jzr;
        pw->roy = sinf(fSin) * fy * zrate * jzr;
        if (pw->innm == 0) {
            pw->time = 0;
            pw->atime = 0;
            if (pw->anime == TSJKANM_RETURN1) {
                pw->state = TSJKCUS_CUR;
                pw->anime = TSJKANM_OFF;
            } else {
                pw->state = TSJKCUS_CUR;
                pw->anime = TSJKANM_ROTATE;
            }
        }
        break;

    case TSJKANM_ROTATE:
        wvrate = 0.0f;
        if (pw->atime == 0) {
            pw->innm = pw->innm2 = 0x400;
            pw->atime = 1;
            pw->rrot = 0.0f;
        } else {
            pw->atime++;
            pw->rrot += ((pw->atime < 110) ? pw->atime : 110) * 0.0022999999f;
            if (pw->rrot > 6.2831855f) {
                pw->rrot -= 6.2831855f;
            }
        }
        pw->rox = zrate * 0.0f;
        pw->roy = zrate * -43.0f;
        rrot = pw->rrot;
        rzr = 1.0f;
        if (pw->atime >= 71) {
            pw->innm2 = TSNumMov(pw->innm2, 0, 11);
            fRate = 1.0f - (pw->innm2 / 1024.0f);
            ton = (int)(fRate * 256.0f);
            abgr = GetDToneColor(abgr, 0xffffff, ton);
            abgrs = GetDToneColor(abgrs, 0x808080, ton);
            jzr = 1.0f - fRate * 0.5f;
        }
        if (pw->atime >= 101) {
            pw->innm = TSNumMov(pw->innm, 0, 14);
            fRate = 1.0f - (pw->innm / 1024.0f);
            ton = (int)(fRate * 256.0f);
            rabgr = GetDToneColor(rabgr, 0xffffff, ton);
            rabgrs = GetDToneColor(rabgrs, 0x808080, ton);
            rzr += fRate * 0.8f;
            TsPatGetSize(ppat, &jx, &jy, NULL, NULL);
            fx = (jx + px) - 318;
            fy = (jy + py) - 181;
            pw->rox = (zrate * 0.0f) - (fx * fRate);
            pw->roy = (zrate * -43.0f) - (fy * fRate);
        }
        if (pw->innm == 0 && pw->innm2 == 0) {
            pw->state = TSJKCUS_MSK;
            pw->anime = TSJKANM_OFF;
            pw->time = 0;
            pw->atime = 0;
        }
        break;

    case TSJKANM_ROTSTOP:
        wvrate = 0.0f;
        if (pw->atime == 0) {
            pw->atime = 80;
            pw->rrot = 0.0f;
            for (i = 0; i < 60; i++) {
                pw->rrot -= (float)i;
            }
            pw->innm = 0x400;
            TsPatGetSize(ppat, &jx, &jy, NULL, NULL);
            abgr = 0xffffff;
            abgrs = 0x808080;
            fx = (jx + px) - 318;
            fy = (jy + py) - 181;
            pw->roy = zrate * -43.0f - fy;
            pw->rox = zrate * 0.0f - fx;
            break;
        }
        pw->atime--;
        if (pw->atime - 20 > 0) {
            pw->rrot += (float)(pw->atime - 20);
            rrot = pw->rrot * 0.005f;
        } else {
            rot = 0.0f;
        }
        pw->innm = TSNumMov(pw->innm, 0, 6);
        fRate = pw->innm / 1024.0f;
        ton = (int)(fRate * 256.0f);
        rzr = fRate * 0.8f + 1.0f;
        jzr = 1.0f - fRate * 0.5f;
        abgr = GetDToneColor(abgr, 0xffffff, ton);
        abgrs = GetDToneColor(abgrs, 0x808080, ton);
        TsPatGetSize(ppat, &jx, &jy, NULL, NULL);
        fx = (jx + px) - 318;
        fy = (jy + py) - 181;
        rx = zrate * 0.0f;
        ry = zrate * -43.0f;
        pw->roy = ry - fy * fRate;
        pw->rox = rx - fx * fRate;
        ton = (int)(fRate * 256.0f * 0.5f);
        rabgr = GetDToneColor(rabgr, 0xffffff, ton);
        rabgrs = GetDToneColor(rabgrs, 0x808080, ton);
        if (pw->atime == 0) {
            pw->state = TSJKCUS_CUR;
            pw->atime = 1;
            pw->anime = TSJKANM_RETURN1;
            pw->rox = rx;
            pw->roy = ry;
            pw->time = 0;
        }
        break;

    default:
        pw->oy = pw->rot = pw->ox = 0.0f;
        break;
    }

    jzr *= zrate;
    rzr *= zrate;
    spr->ofsx += aox + pw->ox;
    spr->ofsy += aoy + pw->oy;
    fRate = (float)(anmtime % 240) * 2.0f / 240.0f;
    spr->ofsy += sinf((fRate + JUKEWAV_INITBL[pw->patNo]) * 3.1415927f) * 8.0f * wvrate;

    if (pw->anime == TSJKANM_ROTATE || pw->anime == TSJKANM_ROTSTOP) {
        _TsJkJacketPut(pk, spr, pw, px, py, jzr, rot, abgr, abgrs);
        _TsJkRecordPut(pk, spr, pw, px, py, rzr, rrot, rabgr, rabgrs);
    } else {
        _TsJkRecordPut(pk, spr, pw, px, py, rzr, rrot, rabgr, rabgrs);
        _TsJkJacketPut(pk, spr, pw, px, py, jzr, rot, abgr, abgrs);
    }

    spr->ofsx = ox;
    spr->ofsy = oy;
}
