#include "menu/menusub.h"

#include "main/cdctrl.h"
#include "main/etc.h"

#include "menu/menudata.h"
#include "menu/menufont.h"
#include "menu/menu_mdl.h"
#include "menu/mntm2hed.h"
#include "menu/p3mc.h"

#include "os/cmngifpk.h"
#include "os/syssub.h"
#include "os/system.h"
#include "os/tim2.h"

#include <prlib/prlib.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/* data 18b0e0 */ extern MNMAPPOS mnmapMap1[]; /* static */
/* data 18b160 */ extern MNMAPPOS mnmapMap[]; /* static */
/* data 18b3a0 */ extern MNMAPPOS mnmapMap2[]; /* static */
// /* data 18b620 */ static short RShopRute0[0];
// /* data 18b630 */ static short RShopRute1[0];
// /* data 18b638 */ static short RShopRute2[0];
// /* data 18b648 */ static short *RecordShopRute[10];
/* data 18b670 */ extern MNMAPPOS mnmapCityHall[]; /* static */
// /* data 18b730 */ short AnmCHallPara_OptRet[0];
// /* data 18b738 */ short AnmCHallPara_Opt[0];
// /* data 18b740 */ short AnmCHallPara_RepRet[0];
// /* data 18b748 */ short AnmCHallPara_Rep[0];
// /* data 18b750 */ short AnmCHallFphs_OptRet[0];
// /* data 18b758 */ short AnmCHallFphs_Opt[0];
// /* data 18b760 */ short AnmCHallFphs_RepRet[0];
// /* data 18b768 */ short AnmCHallFphs_Rep[0];
// /* data 18b770 */ short AnmCHallChar_Log[0];
// /* data 18b780 */ short AnmCHallChar_Opt[0];
// /* data 18b790 */ short AnmCHallChar_Rep[0];
// /* sdata 399748 */ static u_char *UserName_InitialStr;
// /* sdata 39974c */ static u_char *UserName_InitialStr2;
// /* data 18b7a0 */ static u_char UserName_AsciiSetB[41];
// /* data 18b7d0 */ static u_char UserName_AsciiSetS[41];
/* data 18b800 */ extern USERNAME_CSET UserName_CharSet[];
// /* data 18b810 */ static u_char *TeachersName_Tbl[0];
// /* sdata 39975c */ static u_char *UserName_RankingNoSave;
// /* data 18b830 */ static char *_MONTH_STR[0];
/* data 18b880 */ extern MAPBGM MapBgmTbl[]; /* static */
/* data 18b908 */ extern TSVOICE_TBL TsVoiceTbl[]; /* static */
// /* data 18bac8 */ static u_short FussenWAIT0[0];
// /* data 18bad8 */ static u_short FussenWAIT1[0];
// /* data 18bae8 */ static u_short LoadConfLst[0];
/* data 18baf8 */ extern TSVSNDSEQ VSNDSEQ_Tbl[]; /* static */
/* data 18bb10 */ extern TSTEX_TBL TexTable[]; /* static */
// /* data 18bcb0 */ static PATPOS PAT_ALERT_WIN_ABOVE;
// /* data 18bcc0 */ static PATPOS PAT_ALERT_WIN_CENTER;
// /* data 18bcd0 */ static PATPOS PAT_ALERT_WIN_BELOW;
// /* data 18bce0 */ static PATPOS PAT_ALERT_WIN_FFACE[0];
/* data 18bd10 */ extern PATPOS SAVE_MENU_SELPAT[]; /* static */
/* data 18bd28 */ extern PTPOS SAVEWZoom_CXY[]; /* static */
// /* data 18bd30 */ static PATPOS LG_SCROLL_MARK[0];
// /* data 18bd48 */ static PATPOS RP_SCROLL_MARK[0];
// /* data 18bd60 */ static PATPOS LLG_SCROLL_MARK[0];
// /* data 18bd78 */ static PATPOS CSSLASH_MARK;
// /* data 18bd88 */ static STRPOS PAGENO_StrCOD[0];
// /* data 18bd98 */ static PTPOS CellCusPos[0];
// /* data 18bdb8 */ static STRPOS LOGS_StrCOD[0];
// /* data 18bde0 */ static STRPOS LOGL_StrCOD[0];
// /* data 18be08 */ static STRPOS REPLAY_StrCOD[0];
// /* data 18be38 */ static STRPOS VSREPLAY_StrCOD[0];
// /* data 18be78 */ static PATPOS VS_MARK;
// /* data 18be88 */ static PATPOS VS_WINMARK1;
// /* data 18be98 */ static PATPOS VS_WINMARK2;
// /* data 18bea8 */ static PATPOS LG_NEWDATA_MARK;
// /* data 18beb8 */ static PATPOS RP_NEWDATA_MARK;
// /* data 18bec8 */ static PATPOS STGCNameBox[0];
// /* data 18bf08 */ static PATPOS STGCNameBoxOK;
// /* data 18bf18 */ static PATPOS VS1PNameBox[0];
// /* data 18bf58 */ static PATPOS VS1PNameBoxOK;
// /* data 18bf68 */ static PATPOS VS2PNameBox[0];
// /* data 18bfa8 */ static PATPOS VS2PNameBoxOK;
// /* data 18bfb8 */ static MNOPT_OBJ MNOptObj_Lang[0];
// /* data 18bfd8 */ static MNOPT_OBJ MNOptObj_Subt[0];
// /* data 18bff8 */ static MNOPT_OBJ MNOptObj_Vibr[0];
// /* data 18c018 */ static MNOPT_OBJ MNOptObj_Oneb[0];
// /* data 18c038 */ static PATPOS MNOptMiniFrm[0];
// /* data 18c068 */ static PATPOS MNOptLRBtn[0];
// /* data 18c0c8 */ static PATPOS PopMenuSel_Pat[0];
// /* data 18c108 */ static PATPOS VSComMenuSel_Pat[0];
// /* data 18c138 */ static PATPOS VSComMenuSelH_Pat[0];
// /* data 18c168 */ static PATPOS SIRanking_Pat[0];
// /* data 18c180 */ static PATPOS Ranking_PatScroll[0];
// /* data 18c198 */ static PATPOS RankSISTNo_PAT[0];
// /* data 18c208 */ static PATPOS VSL1Ranking_Pat[0];
// /* data 18c248 */ static PATPOS VSL2Ranking_Pat[0];
// /* data 18c288 */ static PATPOS VSL3Ranking_Pat[0];
// /* data 18c2c8 */ static PATPOS VSL4Ranking_Pat[0];
// /* data 18c308 */ static PATPOS *VSRanking_PatTbl[0];
// /* data 18c318 */ static PATPOS RankVSSTNo_PAT[0];
// /* data 18c388 */ static STRPOS SRanking_Str[0];
// /* data 18c3a0 */ static STRPOS VRanking_Str[0];
// /* data 18c3b8 */ static PTPOS PopMenu_Pos[0];
// /* data 18c3c0 */ static PTPOS Ranking_Pos[0];
// /* data 18c3c8 */ static POPRNK_PPOS PopRnk_pPos[0];
// /* data 18c420 */ static int PopRnkPos_No[0][9];
// /* data 18c4b0 */ static int PopBubblePat_No[0][9];
// /* data 18c540 */ static PTPOS POPWZoom_CXY[0];
/* data 18c588 */ extern PTPOS JUKEBOX_Pos[]; /* static */
/* data 18c5b0 */ extern PATPOS JUKEJKT_Pat[]; /* static */
// /* data 18c628 */ static float JUKEWAV_INITBL[0];
/* data 18c650 */ extern PATPOS JUKEJKT_PatS; /* static */
/* data 18c660 */ extern PATPOS JUKEREC_Pat[]; /* static */
/* data 18c6d8 */ extern PATPOS JUKEREC_PatS; /* static */
/* data 18c6e8 */ extern u_int HosiColor[][8]; /* static */
/* data 18c748 */ extern HOSI_TYPE hTypeTable[17];
/* sdata 3997bc */ extern TSTEX_INF *tblTex; /* static */
/* data 18c8e0 */ extern u_int RPPadBit[]; /* static */
/* data 18c8f0 */ extern MCDATA_TBL McVoiceTbl[23]; /* static */
/* data 18c9a8 */ extern MCDATA_TBL McFaceTbl[23]; /* static */
/* sdata 3997c0 */ extern int UserList_Sw; /* static */
/* sdata 3997c4 */ extern int OptionList_Sw; /* static */
/* sdata 3997c8 */ extern int PopMenu_Sw; /* static */
/* sdata 3997cc */ extern int SaveMenu_Sw; /* static */
/* sdata 3997d0 */ extern int JukeMenu_Sw; /* static */
/* data 18ca60 */ extern USERLISTTYPE_TABLE ULTypeT_CITY_STGCLR; /* static */
/* data 18ca70 */ extern USERLISTTYPE_TABLE ULTypeT_CITY_REPLAY; /* static */
// /* data 18ca80 */ static USERLISTTYPE_TABLE ULTypeT_SAVE_LOG;
// /* data 18ca90 */ static USERLISTTYPE_TABLE ULTypeT_SAVE_REPLAY;
/* data 18caa0 */ extern int POPBtn2Sel[]; /* static */
// /* data 18cab8 */ static int POPSel2Btn[0];
// /* data 18cad0 */ static int Pop_CmpMesNo[0];
// /* data 18cae8 */ static int POPSel2BtnDir[0];
// /* data 18cb00 */ static int SaveMenu_CmpMesNo[0];
// /* data 18cb08 */ static BGM_TABLE JukeBgmTbl[0];
// /* data 18cb58 */ static int JukeMenu_CmpMesNo[0];
// /* data 18cb80 */ static MNOPT_SELINF OptionSelTbl[0];
/* data 18cbc0 */ extern USERLIST_TYPE UserListTbl[]; /* static */
/* sdata 399820 */ extern int _TexFunc; /* static */
/* sdata 399824 */ extern HOSI_OBJ *HOSIObj; /* static */
/* bss 1c77ac0 */ extern TSREPPAD menuPadState[2][4]; /* static */
/* bss 1c77ae0 */ extern TSSND_CHAN TsSndChan[15]; /* static */
/* bss 1c77c10 */ extern BGMSTATE TsBGMState; /* static */
/* sbss 399b18 */ extern P3GAMESTATE *pP3GameState; /* static */
/* sbss 399b1c */ extern int _bMapCaptureReq; /* static */
/* sbss 399b20 */ extern int _MNwaitTime; /* static */
/* data 18cc38 */ extern MN_SCENE MNS_StageMap;
/* bss 1c77cb0 */ extern MN_SCENE MNS_StageMap2; /* static */
/* bss 1c78e08 */ extern MN_SCENE MNS_CityHall; /* static */
/* bss 1c79f60 */ extern MN_SCENE MNS_OptCounter; /* static */
/* bss 1c7b0b8 */ extern MN_SCENE MNS_RepCounter; /* static */
/* bss 1c7c210 */ extern MN_SCENE MNS_StgCounter[2]; /* static */
/* bss 1c7e4b8 */ extern MN_SCENE MNS_JimakuBak; /* static */
/* sbss 399b24 */ extern int CurMapOldFlg; /* static */
/* sbss 399b28 */ extern int CurMapNo; /* static */
/* sbss 399b2c */ extern int CurMapBakFlg; /* static */
/* sbss 399b30 */ extern int CurMapState; /* static */
/* sbss 399b34 */ extern USER_DATA *UserWork; /* static */
/* sbss 399b38 */ extern P3MC_STAGERANK *pCStageRank; /* static */
// /* bss 1c7f610 */ static P3MC_RANKSCORE CurRankScore;
/* sbss 399b3c */ extern P3MC_USRLST *UserLst; /* static */
/* sbss 399b40 */ extern MN_USERLST_WORK *UserDispWork; /* static */
/* bss 1c7f628 */ extern CURFILEINFO CurFileInfo; /* static */
/* bss 1c7f640 */ extern TsUSERPKT MnPkt; /* static */
/* bss 1c7f6e0 */ extern TsUSERPKT MnLPkt; /* static */
/* bss 1c7f780 */ extern sceGifPacket FPacket; /* static */
/* sdata 399828 */ extern MAP_TIME MapTime;
/* sbss 399b44 */ extern int UCheckLoadError; /* static */
/* sbss 399b48 */ extern int UCheckSaveError; /* static */
/* sbss 399b4c */ extern int subStatus; /* static */
/* sbss 399b50 */ extern int ret; /* static */
/* sbss 399b54 */ extern int errorNo; /* static */
/* sbss 399b58 */ extern int waitTime; /* static */
// /* sbss 399b5c */ static MCRWDATA_HDL *pGameData;
/* bss 1c7f790 */ extern MCMES_WORK MCMesWork; /* static */
/* bss 1c7f7c8 */ extern CMPMES_WORK CmpMesWork; /* static */
// /* bss 1c7f7e0 */ static RANKLIST RankLst[20];
// /* bss 1c7f970 */ static POPUP_MENU PopupMenu;
typedef struct {
    /* 0x00 */ int state;
    /* 0x04 */ int exitflg;
    /* 0x08 */ int selno;
    /* 0x0c */ ANIME_WK awork;
    /* 0x20 */ POPCTIM cani;
} SAVE_MENU;
/* bss 1c7fa88 */ extern SAVE_MENU SaveMenu; /* static */
/* bss 1c7fb48 */ extern JUKE_MENU JukeMenu; /* static */
// /* bss 1c80f60 */ static OPTION_MENU OptionMenu;
/* bss 1c80fb0 */ extern USERLIST_MENU UserListMenu; /* static */
/* bss 1c810c8 */ extern SCFADE ScFade; /* static */

static int   TsGetMenuPadIsRepeat(int no, int npad);
static void  TSSNDPLAY(int n);
static void  TSSNDSTOP(int chan);
static void  TSSNDMASK_CHAN(int chan, int mskflag);
static void  TSSND_SKIPSTOP(int n);
static void  TSSND_SKIPPLAY(int n);
static int   TSSND_CHANISSTOP(int chan);
static void  tsBGMONEPlay(int no);
static void  tsBGMONEStop(int no);
static void  tsBGMONEVol(int no, int vol);
static void  tsBGMONETop(int no, int vol);
static void  tsBGMONEflow(void);
static void  tsBGMONEPause(int flg);
/* static */ void  TsBGMInit(void);
/* static */ void  TsBGMPlay(int no, int time);
static void  TsBGMStop(int time);
static void  TsBGMMute(int time);
static int   TsBGMLoadCheck(void);
static void  TsBGMPause(int flg);
/* static */ void  TsBGMPoll(void);
static void* TsCmnPkOpen(sceGifPacket *pgifpk);
static void  TsCmnPkClose(sceGifPacket *pgifpk, void *pk, int pri);
static void  TsClearMenuPad(int no);
static void  TsGetMenuPad(int no, u_int *getpad);
static void  TsSndFlow(int flg);
static int   TSNumMov(int cn, int dn, int scale);
static int   TSLOOP(int no, int max);
static int   TSLIMIT(int no, int min, int max);
static int   TsMENU_GetMapNo(int *psize);
static void  TsMENU_GetMapTimeState(int flg);
/* static */ void  TsSetScene_Map(MN_SCENE *pScene, int mapNo, int tflg, int bFocus);
static void  TsSet_ParappaCapColor(void);
/* static */ void  TsClearSet(P3GAMESTATE *pstate);
static void  TsCheckEnding(P3GAMESTATE *pstate);
static void  TsSetRankingName(P3MC_STAGERANK *pRankTop, u_char *name);
static void  TsSetRanking2UData(USER_DATA *puser, P3MC_STAGERANK *wkRank);
/* static */ void  TsSetSaveData(MCRWDATA_HDL *pDataW, int mode, USER_DATA *puser);
/* static */ void  TsRestoreSaveData(MCRWDATA_HDL *pDataW, int mode);
/* static */ int   TsRanking_Set(void);
/* static */ int   TsCheckTimeMapChange(void);
static int   TsMemCardCheck_Flow(int flg, u_int tpad);
static int   TsMap_Flow(int flg, u_int tpad, u_int tpad2);
/* static */ void  TsMakeUserWork(int mode);
/* static */ void  TsSaveSuccessProc(void);
/* static */ int   MpSave_Flow(int flg, u_int tpad, u_int tpad2);
static int   MpCityHall_Flow(int flg, u_int tpad, u_int tpad2);
/* static */ void  MpCityHallParaStart(int pos);
static void  MpCityHallFPHSSoundMask(int flg);
/* static */ int   MpCityHallFPHSMove(int pos, int fpos);
static void  MpCityHallFPHOK(int flg);
/* static */ void  MpCityHallCharPosSet(int pos);
static int   MpPopMenu_Flow(int flg, u_int tpad);
/* static */ int   MpMapMenu_Flow(int flg, MAPPOS *mpw, u_int tpad);
static int   _MapGetMovableDir(MAPPOS *mpw);
/* static */ int   McErrorMess(int err);
static void  McInitFlow(void);
/* static */ int   McStartCheckFlow(int flg);
/* static */ int   McUserCheckFlow(int type, int mode, int *bError);
/* static */ int   McUserSaveFlow(USER_DATA *puser);
/* static */ int   McUserLoadFlow(int fileNo, int mode, int bBroken);
static void  TsMCAMes_Init(void);
static int   TsMCAMes_GetSelect(void);
static int   TsMCAMes_IsON(void);
/* static */ void  TsMCAMes_Flow(u_int tpad);
/* static */ void  TsMCAMes_Draw(SPR_PKT pk, SPR_PRM *spr);
static void  TsCMPMes_Draw(SPR_PKT pk, SPR_PRM *spr);
static void  TsANIME_Init(ANIME_WK *wk);
static int   TsANIME_Poll(ANIME_WK *wk);
static void  TsANIME_Start(ANIME_WK *wk, int state, int tim);
static int   TsANIME_GetRate(ANIME_WK *wk, float *rt0, float *rt1, float *rt2);
/* static */ void  _TsSortSetRanking(P3MC_RANKSCORE **ptRank, int n, P3MC_RANKSCORE *pRank, int bNameCmp);
/* static */ RANKLIST* TsGetRankingList(int flag, int vsLev, int stageNo, int *nrank);
/* static */ int   TsPopMenu_Flow(int flg, u_int tpad);
/* static */ void  TsPopMenu_Draw(SPR_PKT pk, SPR_PRM *spr);
/* static */ int   TsSaveMenu_Flow(int flg, u_int tpad);
/* static */ void  TsSaveMenu_Draw(SPR_PKT pk, SPR_PRM *spr);
static void  TSJukeCDObj_Init(JUKECDOBJ *pw, int pno);
/* static */ void  _TsJkJacketPut(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, float zx, float rot, u_int abgr, u_int abgrs);
/* static */ void  _TsJkRecordPut(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, float zr, float rrot, u_int abgr, u_int abgrs);
/* static */ void  TSJukeCDObj_Draw(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, int anmtime);
static int   TsJukeIsObjAnime(int isComp);
/* static */ int   TsJukeObjAnime(int isOut);
/* static */ int   TsJukeObjAnime2(int isOut);
/* static */ int   _TsJKMoveCus(int *cx, int *cy, int mx, int my, JUKECDOBJ *cobj);
/* static */ void  _TsJKSetPadArrow(int sel, JUKECDOBJ *cobj);
/* static */ int   TsJukeMenu_Flow(int flg, u_int tpad);
static void  TsJukeMenu_Draw(SPR_PKT pk, SPR_PRM *spr);
static void  TsCmnCell_CusorSET(CELLOBJ *obj);
static void  TsCmnCell_CusorON(CELLOBJ *obj);
static void  TsCmnCell_CusorOFF(CELLOBJ *obj);
static void  TsCmnCell_CusorSEL(CELLOBJ *obj);
static void  TsCmnCell_CusorMASK(CELLOBJ *obj);
/* static */ void  TsCmnCell_CusorDraw(SPR_PKT pk, SPR_PRM *spr, int n, CELLOBJ *obj, int ox, int oy, int CurColor);
/* static */ int   TsOption_Flow(int flg, u_int tpad);
/* static */ void  TsOption_Draw(SPR_PKT pk, SPR_PRM *spr);
static int   TsUserList_GetCurFileNo(int *isBroken);
static int   TsUserList_IsGetFileSave(void);
static int   TsUserList_SortUser(void);
static void  TsUserList_SetCurUserData(USER_DATA *psrc);
static void  TsUserList_SetCurDispUserData(USER_DATA *psrc);
/* static */ void  TsUserList_SetCurFileNoCusor(int fileNo, u_int *fDate);
static void  TsUserList_SetType(USERLISTTYPE_TABLE *ptbl, int mode, int curTag);
static int   TsUserList_TagChangeAble(USERLIST_MENU *pfw, int *pno);
/* static */ int   TsUserList_SetCurTag(USERLIST_MENU *pfw, int no);
/* static */ int   TsUserList_Flow(int flg, u_int tpad, u_int tpad2);
/* static */ void  TsUserList_Draw(SPR_PKT pk, SPR_PRM *spr);
static void  NameSpaceCut(u_char *dst, u_char *src);
/* static */ void  TsUser_PanelDraw(SPR_PKT pk, SPR_PRM *spr, USER_DATA *user, int px, int py, int pflg, int isLog);
/* static */ void  TsNAMEINBox_SetName(NAMEINW *pfw, u_char *name);
static void  TsNAMEINBox_GetName(NAMEINW *pfw, u_char *name);
/* static */ int   TsNAMEINBox_Flow(int flg, NAMEINW *pfw, u_int tpad);
/* static */ void  TsNAMEINBox_Draw(SPR_PKT pk, SPR_PRM *spr, int px, int py, int isLog, NAMEINW *pfw, int side);
static void  TsSCFADE_Flow(int flg, int prm);
/* static */ void  TsSCFADE_Draw(SPR_PKT pk, SPR_PRM *spr, int prio);
static void  TsPatTexFnc(int flg);
/* static */ void  _TsPatSetPrm(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy);
static void  TsPatPut(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy);
static void  TsPatGetSize(PATPOS *ppos, int *x, int *y, int *w, int *h);
/* static */ void  TsPatPutRZoom(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, float zrate, float rot);
/* static */ void  TsPatPutMZoom(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, float Zrx, float Zry, int mx, int my, float Crx, float Cry);
static void  TsPatPutSwing(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, int mx, int my, float Crx);
static void  TsPatPutUneri(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, int mx, int my, float Crx, float Drt);
static void  TsCELBackInit(void);
/* static */ void  _TsCELBackObjDraw(SPR_PKT pk, SPR_PRM *spr, int sw, int sh, u_int *colTbl);
/* static */ void  TsHosiPut(SPR_PKT pk, SPR_PRM *spr, TSTEX_INF *ptex, float px, float py, float zrate, float rot);

static int TsGetMenuPadIsRepeat(int no, int npad) {
    return (menuPadState[no][npad].state >= 2);
}

static void TSSNDPLAY(int n) {
    TSVOICE_TBL *ptap;
    TSSND_CHAN  *pchan;
    int          bMsk;
    TSVSNDSEQ   *pSeq;

    if (n >= 0) {
        if (n & 0x8000) {
            pSeq  = &VSNDSEQ_Tbl[n & ~0x8000];
            pchan = &TsSndChan[pSeq->chanNo];

            bMsk = pchan->bMsk;

            memset(pchan, 0, sizeof(*pchan));

            pchan->isSeq = TRUE;

            pchan->pSeq = pSeq->pSeqTop;
            pchan->bMsk = bMsk;
#ifdef VERSION_PAL
        } else if (n < 0x39) {
#else
        } else if (n < 0x38) {
#endif
            ptap  = &TsVoiceTbl[n];
            pchan = &TsSndChan[ptap->chanNo];

            bMsk = pchan->bMsk;

            memset(pchan, 0, sizeof(*pchan));

            pchan->pTap = ptap;
            pchan->bMsk = bMsk;
        }
    }
}

static void TSSNDSTOP(int chan) {
    memset(&TsSndChan[chan], 0, sizeof(TSSND_CHAN));
    MenuVoiceStop(chan);
}

static void TSSNDMASK_CHAN(int chan, int mskflag) {
    TSSND_CHAN *pchan = &TsSndChan[chan];

    pchan->bMsk = mskflag;
    if (pchan->pTap != NULL || pchan->pSeq != NULL) {
        MenuVoiceStop(chan);
    }
}

static void TSSND_SKIPSTOP(int n) {
    TSVOICE_TBL *ptap;
    TSSND_CHAN  *pchan;

#ifdef VERSION_PAL
    if (n >= 0x39) {
#else
    if (n >= 0x38) {
#endif
        return;
    }

    ptap  = &TsVoiceTbl[n];
    pchan = &TsSndChan[ptap->chanNo];

    if (pchan->pTap == ptap) {
        memset(pchan, 0, sizeof(*pchan));
        MenuVoiceStop(ptap->chanNo);
    }
}

static void TSSND_SKIPPLAY(int n) {
    TSVOICE_TBL *ptap;
    TSSND_CHAN  *pchan;

#ifdef VERSION_PAL
    if (n >= 0x39) {
#else
    if (n >= 0x38) {
#endif
        return;
    }

    ptap  = &TsVoiceTbl[n];
    pchan = &TsSndChan[ptap->chanNo];

    if (pchan->pTap == ptap && !pchan->isOn) {
        pchan->tim = ptap->ontim;
    }
}

static int TSSND_CHANISSTOP(int chan) {
    TSSND_CHAN *pchan = &TsSndChan[chan];
    int         ret   = FALSE;

    if (pchan->pTap == NULL) {
        ret = (pchan->pSeq == NULL);
    }

    return ret;
}

static void tsBGMONEPlay(int no) {
    BGMONE *wbgm  = &TsBGMState.wbgm[no];
    MAPBGM *mpbgm = &MapBgmTbl[no];

    wbgm->pbgm = mpbgm;
    wbgm->vol  = 0;
    
    if (mpbgm->lpTimeF != 0) {
        wbgm->tim = mpbgm->lpTimeF;
        MenuVoicePlayVol(mpbgm->chan, mpbgm->tapNo, 0);
    }
}

static void tsBGMONEStop(int no) {
    BGMONE *wbgm = &TsBGMState.wbgm[no];

    wbgm->pbgm = &MapBgmTbl[no];
    wbgm->vol  = 0;
    wbgm->tim  = 0;
    MenuVoiceStop(MapBgmTbl[no].chan);

    wbgm->pbgm = NULL;
}

static void tsBGMONEVol(int no, int vol) {
    BGMONE *wbgm = &TsBGMState.wbgm[no];

    if (wbgm->vol != vol) {
        wbgm->vol = vol;
        MenuVoiceSetVol(wbgm->pbgm->chan, wbgm->pbgm->tapNo, vol);
    }
}

static void tsBGMONETop(int no, int vol) {
    BGMONE *wbgm = &TsBGMState.wbgm[no];
    MAPBGM *mpbgm = &MapBgmTbl[no];

    wbgm->pbgm = mpbgm;
    wbgm->vol  = vol;
    wbgm->tim  = mpbgm->lpTimeF;
    MenuVoicePlayVol(mpbgm->chan, mpbgm->tapNo, vol);
}

static void tsBGMONEflow(void) {
    BGMONE *wbgm = TsBGMState.wbgm;
    int     i;

    for (i = 0; i < 11; i++, wbgm++) {
        if (wbgm->bPause || wbgm->pbgm == NULL) {
            continue;
        }

        if (--wbgm->tim > 0) {
            continue;
        }
        if (wbgm->pbgm->lpTime == 0) {
            continue;
        }

        wbgm->tim = wbgm->pbgm->lpTime;
        MenuVoicePlayVol(wbgm->pbgm->chan, wbgm->pbgm->tapNo, wbgm->vol);
    }
}


static void tsBGMONEPause(int flg) {
    BGMONE *wbgm = TsBGMState.wbgm;
    int     i;

    for (i = 0; i < PR_ARRAYSIZE(TsBGMState.wbgm); i++, wbgm++) {
        wbgm->bPause = (flg != 0);
    }
}

void TsBGMInit(void) {
    memset(&TsBGMState, 0, sizeof(TsBGMState));
}

/* static */ void TsBGMPlay(int no, int time) {
    BGMSTATE *pbgm = &TsBGMState;
    int       i;
    int       bPlay = 0;

    if (no >= 11) {
        return;
    }

    if (MenuVoiceBankSet(-1)) {
        pbgm->state  = 1;
        pbgm->wtLoad = 1;
        pbgm->wtNo   = no;
        pbgm->wtTim  = time;
        pbgm->chgReq = 0;
        pbgm->cstate = 0;
        pbgm->ctim   = 0;
        return;
    }

    if ((pbgm->state & 1) && pbgm->wtLoad == 0) {
        if (pbgm->sndno == no && pbgm->vol == 0x100) {
            pbgm->sndno = no;
            pbgm->vol   = 0x100;
            pbgm->state = 1;
            pbgm->ttim0 = 0;
            pbgm->ttim  = 0;
            tsBGMONEVol(pbgm->sndno, 0x100);
            return;
        }
        bPlay = 1;
    }

    pbgm->chgReq = 0;
    pbgm->cstate = 0;
    pbgm->ctim   = 0;
    pbgm->wtLoad = 0;

    if (time > 0) {
        pbgm->state = 7;
        pbgm->vol   = 0;
        pbgm->ttim0 = time;
        pbgm->sndno = no;
        pbgm->ttim  = 0;
    } else {
        pbgm->vol   = 0x100;
        pbgm->state = 1;
        pbgm->sndno = no;
        pbgm->ttim0 = 0;
        pbgm->ttim  = 0;
    }

    if (!bPlay) {
        MNSceneMusicFitTimerClear();
        for (i = 0; i < 11; i++) {
            tsBGMONEPlay(i);
        }
    }

    tsBGMONEVol(pbgm->sndno, pbgm->vol);
}

static void TsBGMStop(int time) {
    BGMSTATE *pbgm = &TsBGMState;
    int       i;

    if (pbgm->state != 0) {
        pbgm->ttim0 = 0;
        pbgm->ttim = 0;

        if (time > 0) {
            pbgm->state = 3;
            pbgm->vol = 0x100;

            if (pbgm->ctim == 0) {
                tsBGMONEVol(pbgm->sndno, 0x100);
            }

            do {} while (0); /* TODO: Get rid of this? */
        } else {
            pbgm->chgReq = 0;
            pbgm->cstate = 0;
            pbgm->ctim = 0;

            pbgm->state = 0;
            pbgm->vol = 0;

            for (i = 0; i < 11; i++) {
                tsBGMONEStop(i);
            }
        }
    }
}

static void TsBGMMute(int time) {
    BGMSTATE *pbgm = &TsBGMState;

    if (pbgm->state != 0) {
        pbgm->chgReq = 0;
        pbgm->cstate = 0;
        pbgm->ctim = 0;

        if (time > 0) {
            pbgm->state = 11;
            pbgm->vol = 0x100;
            pbgm->ttim0 = time;
            pbgm->ttim = 0;
        } else {
            pbgm->state = 9;
            pbgm->vol = 0;
            pbgm->ttim0 = 0;
            pbgm->ttim = 0;
        }

        tsBGMONEVol(pbgm->sndno, pbgm->vol);
    }
}

static int TsBGMLoadCheck(void) {
    return MenuVoiceBankSet(-1);
}

static void TsBGMPause(int flg) {
    BGMSTATE *pbgm = &TsBGMState;

    if (pbgm->state != 0) {
        pbgm->chgReq = 0;
        pbgm->cstate = 0;
        pbgm->ctim = 0;
        tsBGMONEPause(flg);
    }
}

void TsBGMChangePos(int no) {
    BGMSTATE *pbgm = &TsBGMState;

    if (no >= 11) {
        return;
    }

    pbgm->vol    = 256;
    pbgm->state  = 1;
    pbgm->ttim0  = 0;
    pbgm->ttim   = 0;
    pbgm->chgReq = no + 1;
}

/* static */ void TsBGMPoll(void) {
    BGMSTATE *pbgm = &TsBGMState;
    int       vol;

    MNSceneMusicFitTimerFrame();
    if (pbgm->state == 0) {
        return;
    }

    if (pbgm->wtLoad != 0) {
        MNSceneMusicFitTimerClear();
        if (MenuVoiceBankSet(-1) != 0) {
            return;
        }
        pbgm->wtLoad = 0;
        TsBGMPlay(pbgm->wtNo, pbgm->wtTim);
    }

    tsBGMONEflow();

    if (pbgm->chgReq != 0 && (pbgm->wbgm[0].tim % 36) == 0) {
        pbgm->oldno  = pbgm->sndno;
        pbgm->sndno  = pbgm->chgReq - 1;
        pbgm->ctim   = 1;
        pbgm->cstate = 0;
        pbgm->chgReq = 0;
    } else if (pbgm->ctim != 0) {
        if (pbgm->ctim == 75) {
            tsBGMONEVol(0, 0);
            MenuVoiceSetVol(3, 23, 0);
            tsBGMONEVol(pbgm->oldno, 0);
            tsBGMONEVol(pbgm->sndno, pbgm->vol);
            pbgm->ctim = 0;
        } else {
            if (pbgm->ctim < 2) {
                vol = pbgm->ctim;
                tsBGMONEVol(0, vol * pbgm->vol);
            } else if (pbgm->ctim == 72) {
                tsBGMONEVol(0, 0);
                MenuVoicePlay(3, 23);
            } else if (pbgm->ctim > 72) {
                vol = 75 - pbgm->ctim;
                MenuVoiceSetVol(3, 23, (vol * pbgm->vol) / 3);
            }

            if (pbgm->ctim < 2) {
                vol = 1 - pbgm->ctim;
                tsBGMONEVol(pbgm->oldno, vol * pbgm->vol);
            } else if (pbgm->ctim == 72) {
                if (pbgm->sndno == 10) {
                    tsBGMONETop(pbgm->sndno, pbgm->vol);
                } else {
                    tsBGMONETop(pbgm->sndno, 0);
                }
            } else if (pbgm->ctim > 72) {
                if (pbgm->sndno != 10) {
                    vol = pbgm->ctim - 72;
                    tsBGMONEVol(pbgm->sndno, (vol * pbgm->vol) / 72);
                }
            }
            pbgm->ctim++;
        }
    }

    if (pbgm->ttim0 != 0) {
        pbgm->ttim++;
        if (pbgm->ttim > pbgm->ttim0) {
            pbgm->ttim0 = 0;
            pbgm->ttim  = 0;
            if (pbgm->state & 4) {
                pbgm->vol    = 0x100;
                pbgm->state  = 1;
                pbgm->cstate = 0;
                pbgm->ctim   = 0;
                tsBGMONEVol(pbgm->sndno, 0x100);
            } else {
                pbgm->vol    = 0;
                pbgm->ttim0  = 0;
                pbgm->ttim   = 0;
                pbgm->cstate = 0;
                pbgm->ctim   = 0;
                if (!(pbgm->state & 9)) {
                    pbgm->state = 0;
                    TsBGMStop(0);
                } else {
                    pbgm->state = 9;
                    tsBGMONEVol(pbgm->sndno, 0);
                }
            }
        } else {
            vol = (pbgm->ttim << 8) / pbgm->ttim0;
            pbgm->vol = vol;
            if (!(pbgm->state & 4)) {
                pbgm->vol = 0x100 - vol;
            }
            if (pbgm->ctim == 0) {
                tsBGMONEVol(pbgm->sndno, pbgm->vol);
            }
        }
    }
}

static void* TsCmnPkOpen(sceGifPacket *pgifpk) {
    CmnGifOpenCmnPk(pgifpk);

    sceGifPkAddGsAD(pgifpk, SCE_GS_PABE, 0);
    sceGifPkAddGsAD(pgifpk, SCE_GS_FBA_1, 0);
    sceGifPkAddGsAD(pgifpk, SCE_GS_TEST_1, 0x30000);

    sceGifPkCloseGifTag(pgifpk);
    return pgifpk->pCurrent;
}

INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00395F10);
INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00395F20);

static void TsCmnPkClose(sceGifPacket *pgifpk, void *pk, int pri) {
    u_long giftag[2] = { 0x1000000000008000, 0xe };

    pgifpk->pCurrent = pk;
    sceGifPkOpenGifTag(pgifpk, *(u_long128*)giftag);
    CmnGifCloseCmnPk(pgifpk, pri);
}

int _P3DATA_SIZE(int mode) {
    u_int size;

    switch (mode) {
    case 1:
#ifdef VERSION_PAL
        size = 0x190;
#else
        size = 0x188;
#endif
        break;
    case 2:
        size = 0x4528;
        break;
    default:
        size = 0;
        break;
    }

    return size;
}

void TsGetTm2Tex(void *ptim2, TSTEX_INF *tex) {
    TIM2INFO info;
    int      ptw, pth;

    GetTim2Info(ptim2, &info, 1);

    tex->w = info.picturH->ImageWidth;
    tex->h = info.picturH->ImageHeight;
    tex->tex0 = info.picturH->GsTex0;

    ptw = (((int*)&tex->tex0)[0] >> 0x1a) & 0xf; /* TW */
    pth = (((u_int*)&tex->tex0)[0] >> 0x1e) | ((((u_int*)&tex->tex0)[1] & 0x3) << 0x2); /* TH */

    tex->rUsize = 1.0f / (1 << ptw);
    tex->rVsize = 1.0f / (1 << pth);

    ((u_int*)&tex->tex0)[1] |= 0x4; /* TCC = 1 */
}

void TsGetTm2HedTex(int no, TSTEX_INF *tex) {
    int           ptw, pth;
    MENU_TM2_HED *ptm2h;

    ptm2h = TsGetTM2Hed(no);

    tex->w = ptm2h->w;
    tex->h = ptm2h->h;
    tex->tex0 = ptm2h->GsTex0;

    ptw = (((int*)&ptm2h->GsTex0)[0] >> 0x1a) & 0xf; /* TW */
    pth = (((u_int*)&ptm2h->GsTex0)[0] >> 0x1e) | ((((u_int*)&ptm2h->GsTex0)[1] & 0x3) << 0x2); /* TH */

    tex->rUsize = 1.0f / (1 << ptw);
    tex->rVsize = 1.0f / (1 << pth);

    ((u_int*)&tex->tex0)[1] |= 0x4; /* TCC = 1 */
}

static void TsClearMenuPad(int no) {
    if (no >= 2) {
        return;
    }

    memset(&menuPadState[no][0], 0, sizeof(TSREPPAD) * 4);
}

static void TsGetMenuPad(int no, u_int *getpad) {
    PADD     *pPad;
    u_int     one, shot;
    TSREPPAD *pRpPad;
    u_int    *pPadBit;
    u_int     padMsk;
    u_int     i;

    pPadBit = RPPadBit;
    if (no >= 2) {
        return;
    }

    *getpad = 0;

    pRpPad = &menuPadState[no][0];
    padMsk = 0;
    pPad = &pad[no];

    one = pPad->mone;
    shot = pPad->mshot;

    for (i = 0; i < 4; i++, pRpPad++, pPadBit++) {
        padMsk |= *pPadBit;
        if (one & *pPadBit) {
            pRpPad->time = 0;
            pRpPad->state = 1;
            *getpad |= *pPadBit;
        } else if (shot & *pPadBit) {
            pRpPad->time++;
            if (pRpPad->time >= 19) {
                if (pRpPad->time % 5) {
                    pRpPad->state = 2;
                } else {
                    pRpPad->state = 3;
                    *getpad |= *pPadBit;
                }
            }
        } else {
            pRpPad->state = 0;
            pRpPad->time = 0;
        }
    }

    *getpad |= (one & ~padMsk);
}

static void TsSndFlow(int flg) {
    int          i;
    TSVOICE_TBL *ptap;
    TSSND_CHAN  *pchan;
    u_short     *pSeq, *pCur;

    if (flg == 1) {
        memset(&TsSndChan, 0, sizeof(TsSndChan));
        return;
    }

    pchan = TsSndChan;

    for (i = 0; i < 15; i++, pchan++) {
        ptap = pchan->pTap;
        if (ptap != NULL) {
            pchan->tim++;

            if (!pchan->isOn) {
                if (ptap->ontim < pchan->tim) {
                    if (!pchan->bMsk) {
                        MenuVoicePlay(i, ptap->vsetNo);
                    }

                    pchan->isOn = 1;
                    if (ptap->offtim == 0) {
                        memset(pchan, 0, sizeof(*pchan));
                    }
                }

                if (!pchan->isOn) {
                    continue;
                }
            }

            if (ptap->offtim != 0 && pchan->tim >= ptap->offtim) {
                MenuVoiceStop(i);
                memset(pchan, 0, sizeof(*pchan));
            }
        } else {
            pSeq = pchan->pSeq;
            if (pSeq != NULL) {
                pchan->tim++;
                pCur = &pSeq[pchan->sqIdx * 2];
    
                if (!pchan->isOn) {
                    pchan->isOn = 1;
                } else {
                    if (pchan->tim < pCur[1]) {
                        continue;
                    }
                    pchan->tim -= pCur[1];
                    pchan->sqIdx++;
                    pCur = &pSeq[pchan->sqIdx * 2];
                }
    
                switch (pCur[0]) {
                case 0xffff:
                    break;
                case 0xfffe:
                    pchan->sqIdx = 0;
                    pCur = pSeq;
                    break;
                case 0xfffd:
                    pchan->sqIdx = 0;
                    MenuVoiceStop(i);
                    memset(pchan, 0, sizeof(*pchan));
                    continue;
                }
                
                if (!pchan->bMsk && !(pCur[0] & 0xf000)) {
                    MenuVoicePlay(i, pCur[0]);
                }
            }
        }
    }
}

static int TSNumMov(int cn, int dn, int scale) {
    int d;
    int dv;
    int da;

    d = cn - dn;
    if (d != 0) {
        da = abs(d);

        switch (scale) {
        case 0:
            dv = (da - (da >> 3)) + 1;
            scale = 1;
            break;
        case 1:
            dv = (da - (da >> 2)) + 1;
            break;
        case 2:
            dv = (da >> 1) + 1;
            scale = 1;
            break;
        default:
            dv = (da >> 2) + 1;
            scale -= 2;
            break;
        }

        if (scale >= 2) {
            dv = ((dv * 16) - dv) / (scale * 10);
        }

        if (dv == 0) {
            dv = 1;
        }

        if (d < 0) {
            cn += dv;
            if (cn > dn) {
                cn = dn;
            }
        } else {
            cn -= dv;
            if (cn < dn) {
                cn = dn;
            }
        }
    }

    return cn;
}

float TSNumRBack(float rt, float bkrt) {
    float hrt = bkrt + 1.0f;

    rt *= (bkrt * 2.0f) + 1.0f;
    if (rt > hrt) {
        rt = (hrt * 2.0f) - rt;
    }

    return rt;
}

static int TSLOOP(int no, int max) {
    return (no + max) % max;
}

static int TSLIMIT(int no, int min, int max) {
    if (no < min) {
        return min;
    }
    if (no >= max) {
        return max - 1;
    }
    return no;
}

static int TsMENU_GetMapNo(int *psize) {
    int mn;
    int size;
    int flg;

    mn = 9;

    if (pP3GameState->pLog->nRound <= 0) {
        flg = pP3GameState->pLog->clrFlg[0];
        for (mn = 1; flg != 0; mn++) {
            flg >>= 1;
        }

        if (mn > 9) {
            mn = 9;
        }
    }

    if (mn == 0) {
        size = 0;
    } else if (mn < 2) {
        size = 1;
    } else if (mn < 4) {
        size = 2;
    } else if (mn < 5) {
        size = 3;
    } else {
        size = 4;
    }

    if (psize != NULL) {
        *psize = size;
    }

    return mn;
}

/* Inline required to match. */
static inline int PrBcdInt(u_int n) {
    return (((n / 16) * 10) + (n % 16));
}

void sceScfGetLocalTimefromRTC(sceCdCLOCK *rtc);

static void TsMENU_GetMapTimeState(int flg) {
    /* sdata 3997d4 */ extern int nTim; /* static */
    int         err;
    short       hour;
    short       state;
    MAP_TIME   *mptim;
    sceCdCLOCK  clock;

    mptim = &MapTime;

    if (flg == 1) {
        CurMapOldFlg = -1;
        CurMapBakFlg = -1;
        memset(mptim, 0, sizeof(*mptim));
        nTim = flg;
    } else if (flg == 3) {
        nTim = 30;
        return;
    }

    nTim--;
    if (nTim > 0) {
        return;
    }

    nTim = 30;

    err = sceCdReadClock(&clock);
    mptim->date_pad = rand() % 200;
    if (err != 0 && clock.stat == 0) {
#ifdef VERSION_PAL
        sceScfGetLocalTimefromRTC(&clock);
#endif
        mptim->date_second = clock.second;
        mptim->date_minute = clock.minute;
        mptim->date_hour = clock.hour;
        mptim->date_day = clock.day;
        mptim->date_month = clock.month;
        mptim->date_year = clock.year + 0x2000;
        flg = FALSE;
    } else {
        mptim->date_second = 0x0;
        mptim->date_minute = 0x0;
        mptim->date_hour = 12; /* BUG: Value isn't valid BCD, */
        mptim->date_day = 0x1; /*      though it still works. */
        mptim->date_month = 0x1;
        mptim->date_year = 0x2000;
        flg = TRUE;
    }

    state = CurMapBakFlg;

    if (!flg) {
        hour = PrBcdInt(mptim->date_hour);

        /* Dumb nested ifs but required to match. */
        if (hour < 4) {
            state = 2;
        } else {
            state = 0;
            if (hour >= 7) {
                state = 1;
                if (hour >= 16) {
                    if (hour <= 18) {
                        state = 0;
                    } else {
                        state = 2;
                    }
                }
            }
        }
    } else {
        if (state <= -1) {
            state = 1;
        }
    }

    CurMapBakFlg = state;
}

extern MN_SCENETBL Scene_StageMapA;
extern MN_SCENETBL Scene_StageMapY;
extern MNANM_TBL StageMapAnime[];
extern MNANM_TBL StageMapAnimeBB[];
extern MNANM_TBL *StageMapCWptr[];
extern MNANM_TBL StageMapAnimeSEA[];
extern MNANM_TBL StageMapAnimeBK[];
extern char stg_clr_msk[8];

/* static */ void TsSetScene_Map(MN_SCENE *pScene, int mapNo, int tflg, int bFocus) {
    P3LOG_VAL *pLog;
    int        mn;
    int        nRound;
    int        i;
    int        cnt;
    int        ncool;
    int        cool[4];
    u_int      cval;
    int        j;

    mn = mapNo;
    pLog = pP3GameState->pLog;
    nRound = pLog->nRound;
    if (mapNo == 0) {
        mn = TsMENU_GetMapNo(NULL);
        if (mn == 9) {
            if (nRound >= 4) {
                mn = 10;
            }
        }
    }
    if (mapNo == 9 && nRound >= 4) {
        mapNo = 10;
        mn = 10;
    }

    MNScene_End(pScene);

    switch (tflg) {
    case 1:
        MNScene_Init(pScene, &Scene_StageMap, bFocus);
        MNScene_StartAnime(pScene, -1, &StageMapAnimeBK[0]);
        break;
    case 2:
        MNScene_Init(pScene, &Scene_StageMapY, bFocus);
        MNScene_StartAnime(pScene, -1, &StageMapAnimeBK[1]);
        break;
    case 0:
    default:
        MNScene_Init(pScene, &Scene_StageMapA, bFocus);
        MNScene_StartAnime(pScene, -1, &StageMapAnimeBK[2]);
        break;
    }

    MNScene_DispSw(pScene, 1);
    MNScene_StartAnime(pScene, -1, &StageMapAnime[mapNo]);
    MNScene_StartAnime(pScene, -1, &StageMapAnime[mn + 11]);
    MNScene_StartAnime(pScene, -1, &StageMapAnimeSEA[0]);

    for (i = 0; i < 8; i++) {
        cnt = pLog->clrCount[i];
        if (mapNo == 0) {
            if (stg_clr_msk[i] != 0) {
                cnt = 0;
            }
        }

        if (cnt > 0) {
            MNScene_StartAnime(pScene, -1, &StageMapAnimeBB[i * 2]);
            if (cnt > nRound + 1) {
                cnt = nRound + 1;
            }
            if (cnt >= 5) {
                cnt = 4;
            }
            MenuStageCl1Trans(i, (cnt > 0) ? (cnt - 1) : cnt);

            for (j = 0; j < 4; j++) {
                cool[j] = 0;
            }
            cval = pLog->logCOOL[i];
            for (ncool = 0, j = 0; j < 4 && cval != 0; j++, ncool++) {
                cool[j] = cval & 0xf;
                cval >>= 4;
            }
            if (ncool >= 5) {
                ncool = 4;
            }
            if (ncool <= 0) {
                MNScene_ModelDispSw(pScene, i + 12, 0);
            } else {
                MNScene_StartAnime(pScene, -1, &StageMapCWptr[i][ncool - 1]);
                for (j = 0; j < ncool; j++) {
                    cval = cool[(ncool - 1) - j];
                    if ((int)cval > 0) {
                        MenuCoolCl1Trans(i, j, cval - 1);
                    }
                }
            }
        } else {
            MNScene_StartAnime(pScene, -1, &StageMapAnimeBB[i * 2 + 1]);
            MNScene_ModelDispSw(pScene, i + 12, 0);
        }
    }

    if (mapNo == 0) {
        MNScene_StartAnime(pScene, -1, &StageMapAnimePA[0]);
        MNScene_StartAnime(pScene, -1, &StageMapAnimePA[6]);
    }
}

static void TsSet_ParappaCapColor(void) {
    P3LOG_VAL      *pLog = pP3GameState->pLog;
    TAP_ROUND_ENUM  n;

    switch (pLog->nRound) {
    case 0:
        n = TRND_R1;
        break;
    case 1:
        n = TRND_R2;
        break;
    case 2:
        n = TRND_R3;
        break;
    default:
        n = TRND_R4;
        break;
    }

    MenuRoundTim2Trans(n);
}

extern short *D_00199B00[10];

/* static */ void TsClearSet(P3GAMESTATE *pstate) {
    int        bRecShop = 0;
    int        recJacket = 0;
    P3LOG_VAL *pLog = pstate->pLog;
    int        nStage = pstate->nStage;
    int        stgIdx = nStage - 1;
    int        nRound = pLog->nRound;
    int        i;
    int        flg;
    int        vsLev;
    short     *pRute;

    if ((u_int)stgIdx >= 8) {
        return;
    }

    if (pstate->nMode == 1) {
        return;
    }

    if (pstate->nMode == 2) {
        vsLev = pstate->vsLev;
        if (nRound >= 4) {
            if (pLog->clrVSCOM1[stgIdx] < 4 && (vsLev + 1) >= 4) {
                flg = 0;
                for (i = 0; i < 8; i++) {
                    if (pLog->clrVSCOM1[i] >= 4) {
                        flg++;
                    }
                }
                if (flg == 7) {
                    bRecShop = 1;
                    recJacket = 9;
                }
            }
        }
        if (pLog->clrVSCOM1[stgIdx] < (vsLev + 1)) {
            pLog->clrVSCOM1[stgIdx] = vsLev + 1;
        }
    } else if (pstate->nMode == 0) {
        pstate->pAutoMove = NULL;
        if (nRound == 0) {
            if (pLog->clrCount[stgIdx] <= 0) {
                int nextStg = nStage + 1;
                pstate->autoMovePos[1] = -1;
                if (nextStg >= 9) {
                    nextStg = 1;
                }
                pstate->autoMovePos[0] = nextStg;
                pstate->pAutoMove = pstate->autoMovePos;
            }
        }
        if (nRound < 4) {
            pLog->clrFlg[nRound] |= (1 << stgIdx);
        }
        pLog->clrCount[stgIdx]++;
        if (pLog->clrCount[stgIdx] > (nRound + 1)) {
            pLog->clrCount[stgIdx] = nRound + 1;
        }
        if (pP3GameState->bCoolClr != 0) {
            if (nRound >= 4) {
                if (pLog->clrCOOL[stgIdx] < 4) {
                    bRecShop = 1;
                    recJacket = stgIdx;
                }
            }
            pLog->clrCOOL[stgIdx] = nRound + 1;
            pLog->logCOOL[stgIdx] = ((pLog->logCOOL[stgIdx] << 4) & 0xfff0) | (((nRound + 1) < 5) ? (nRound + 1) : 4);
        }
        flg = 1;
        for (i = 0; i < 8; i++) {
            if (pLog->clrCount[i] < (nRound + 1)) {
                flg = 0;
            }
        }
        if (flg != 0) {
            pLog->nRound++;
            if (pLog->nRound > 1000000) {
                pLog->nRound = 1000000;
            }
            printf("*** Round Up = (%d)\n", pLog->nRound);
            if (pLog->nRound == 4) {
                bRecShop = 1;
                recJacket = 0;
            }
        }
    }

    if (bRecShop != 0) {
        pRute = D_00199B00[stgIdx + 1];
        for (i = 0; i < 9; i++) {
            pstate->autoMovePos[i] = pRute[i];
            if (pRute[i] < 0) {
                break;
            }
        }
        pstate->autoMovePos[i] = -2;
        pstate->pAutoMove = pstate->autoMovePos;
        pstate->curRecJacket = recJacket;
    }
}

static void TsCheckEnding(P3GAMESTATE *pstate) {
    int        nRound;
    int        nStage;
    int        i;
    int        flg;
    P3LOG_VAL *pLog; /* note: not in STABS. */

    nRound = pstate->pLog->nRound;
    nStage = pstate->nStage - 1;

    if (pstate->nMode != 0) {
        pstate->endingGame = 0;
        return;
    }

    flg = 0;
    pLog = pstate->pLog;
    for (i = 0; i < PR_ARRAYSIZE(pLog->clrCount); i++) {
        if (pLog->clrCount[i] >= (nRound + 1)) {
            flg++;
        }
    }

    pstate->endingGame = 0;

    if ((flg % 2) != 0) {
        if (pLog->clrCount[nStage] < (nRound + 1)) {
            pstate->endingGame = (flg / 2) + 2;
            if (pstate->endingGame > 4) {
                pstate->endingGame = 4;
            }
        }
    }

    if (flg == 7) {
        if (pLog->clrCount[nStage] < (nRound + 1)) {
            pstate->endingGame = 1;
        }
    }
}

void TsMENU_InitSystem(void) {
    int i;

    P3MC_InitReady();
    P3MC_CheckChangeSet();

    memset(&MNS_JimakuBak, 0, sizeof(MNS_JimakuBak));
    memset(&MNS_RepCounter, 0, sizeof(MNS_RepCounter));
    memset(&MNS_OptCounter, 0, sizeof(MNS_OptCounter));
    memset(&MNS_CityHall, 0, sizeof(MNS_CityHall));
    memset(&MNS_StageMap, 0, sizeof(MNS_StageMap));
    memset(&MNS_StageMap2, 0, sizeof(MNS_StageMap2));

    for (i = 0; i < 2; i++) {
        memset(&MNS_StgCounter[i], 0, sizeof(MNS_StgCounter[i]));
    }

    tblTex = NULL;
    UserLst = NULL;
    UserDispWork = NULL;

    UserWork = (USER_DATA*)memalign(16, sizeof(USER_DATA));
    memset(UserWork, 0, sizeof(*UserWork));

    pCStageRank = (P3MC_STAGERANK*)memalign(16, sizeof(P3MC_STAGERANK[8]));
    memset(pCStageRank, 0, sizeof(P3MC_STAGERANK[8]));

    memset(&CurFileInfo, 0, sizeof(CurFileInfo));
    CurFileInfo.logFileNo = -1;
    CurFileInfo.repFileNo = -1;
}

void TsMENU_EndSystem(void) {
    if (pCStageRank != NULL) {
        free(pCStageRank);
    }

    if (UserWork != NULL) {
        free(UserWork);
    }
}

void TsMenu_RankingClear(void) {
    int             i;
    P3MC_STAGERANK *pRank = pCStageRank;

    for (i = 0; i < 8; i++, pRank++) {
        memset(pRank, 0, sizeof(*pRank));
    }
}

u_int UsrMemEndAlloc(int size);

void TsMenu_Init(int iniflg, P3GAMESTATE *pstate) {
    int   i;
    void *ptim2;

    pP3GameState = pstate;

    PrSetStage(0);

#ifdef VERSION_PAL
    P3MC_SetCheckSaveSize(1, 0x8c, 0x190);
#else
    P3MC_SetCheckSaveSize(1, 0x8c, 0x188);
#endif
    P3MC_SetCheckSaveSize(2, 0xa0, 0x4528);

    if (!iniflg) {
#ifdef VERSION_PAL
        MENUSubt_PadFontSw(1);
#else
        MENUSubt_PadFontSw(0);
#endif
        TsMemCardCheck_Flow(1, 0);
    } else {
        MENUSubt_PadFontSw(1);

        MNScene_Init(&MNS_CityHall, &Scene_CityHall, 1);
        MNScene_Init(&MNS_StageMap, &Scene_StageMap, 1);

        MNScene_Init(&MNS_OptCounter, &Scene_OptCounter, 0);
        MNScene_Init(&MNS_RepCounter, &Scene_RepCounter, 0);
        MNScene_Init(&MNS_JimakuBak, &Scene_JimakuBak, 0);

        MNScene_Init(&MNS_StgCounter[0], &Scene_StgCounterLoad, 0);
        MNScene_Init(&MNS_StgCounter[1], &Scene_StgCounterSave, 0);

        MNScene_DispSw(&MNS_OptCounter, 0);
        MNScene_DispSw(&MNS_RepCounter, 0);
        MNScene_DispSw(&MNS_JimakuBak, 0);

        for (i = 0; i < 2; i++) {
            MNScene_DispSw(&MNS_StgCounter[i], 0);
        }

#ifdef VERSION_PAL
        tblTex = (TSTEX_INF*)UsrMemEndAlloc(sizeof(TSTEX_INF) * 108);

        for (i = 0; i < 108; i++) {
#else
        tblTex = (TSTEX_INF*)malloc(sizeof(TSTEX_INF) * 104);

        for (i = 0; i < 104; i++) {
#endif
            switch (TexTable[i].flg) {
            case 0:
                TsGetTm2HedTex(TexTable[i].fno, &tblTex[i]);
                break;
            case 1:
                ptim2 = GetIntAdrsCurrent(TexTable[i].fno);
                Tim2Trans(ptim2);
                TsGetTm2Tex(ptim2, &tblTex[i]);
                break;
            case 2:
                ptim2 = GetIntAdrsCurrent(TexTable[i].fno);
                TsGetTm2Tex(ptim2, &tblTex[i]);
                break;
            }
        }
    }

#ifdef VERSION_PAL
    UserLst = (P3MC_USRLST*)UsrMemEndAlloc(sizeof(P3MC_USRLST));
#else
    UserLst = (P3MC_USRLST*)memalign(16, sizeof(P3MC_USRLST));
#endif
    memset(UserLst, 0, sizeof(*UserLst));
    P3MC_CheckChangeSet();

#ifdef VERSION_PAL
    UserDispWork = (MN_USERLST_WORK*)UsrMemEndAlloc(sizeof(MN_USERLST_WORK));
#else
    UserDispWork = (MN_USERLST_WORK*)memalign(16, sizeof(MN_USERLST_WORK));
#endif
    memset(UserDispWork, 0, sizeof(*UserDispWork));

    TsInitUPacket(&MnPkt, NULL, 0x20000);
    TsInitUPacket(&MnLPkt, NULL, 0x2000);

    TsMCAMes_Init();
    TsMCAMes_SetMes(-1);
    TsCMPMes_SetMes(-1);

    TsClearMenuPad(0);
    TsClearMenuPad(1);

    TsSCFADE_Flow(1, 0);
    TsBGMInit();

    _bMapCaptureReq = FALSE;
    UserList_Sw     = FALSE;
    OptionList_Sw   = FALSE;
    PopMenu_Sw      = FALSE;
    SaveMenu_Sw     = FALSE;
    JukeMenu_Sw     = FALSE;

    TsSndFlow(1);

    TsMENU_GetMapTimeState(1);
}

void UsrMemEndFree(void);

void TsMenu_End(void) {
    int i;

    TsBGMStop(0);

    TsCELBackEnd();
    TsEndUPacket(&MnLPkt);
    TsEndUPacket(&MnPkt);

    MNScene_End(&MNS_JimakuBak);
    MNScene_End(&MNS_RepCounter);
    MNScene_End(&MNS_OptCounter);

    for (i = 0; i < 2; i++) {
        MNScene_End(&MNS_StgCounter[i]);
    }

    MNScene_End(&MNS_StageMap2);
    MNScene_End(&MNS_StageMap);
    MNScene_End(&MNS_CityHall);

#ifdef VERSION_PAL
    if (tblTex != NULL) {
        UsrMemEndFree();
    }
    tblTex = NULL;

    if (UserDispWork != NULL) {
        UsrMemEndFree();
    }

    if (UserLst != NULL) {
        UsrMemEndFree();
    }
    UserDispWork = NULL;
    UserLst = NULL;
#else
    if (tblTex != NULL) {
        free(tblTex);
    }

    if (UserDispWork != NULL) {
        free(UserDispWork);
    }

    if (UserLst != NULL) {
        free(UserLst);
    }
#endif
}

void TsMenu_InitFlow(P3GAMESTATE *pstate) {
    pP3GameState = pstate;

    pstate->pAutoMove = NULL;
    pstate->curRecJacket = 0;

    TsSet_ParappaCapColor();

    switch (pstate->endFlg) {
    case 0:
        TsMap_Flow(1, 3, 0);
        break;
    case 1:
        TsClearSet(pP3GameState);
        TsMap_Flow(1, 2, 0);
        break;
    case 2:
        TsMap_Flow(1, 1, 0);
        break;
    }
}

int TsMenuMemcChk_Flow(void) {
    u_int tpad, tpad2;
    int   ret;

    TsGetMenuPad(0, &tpad);
    TsGetMenuPad(1, &tpad2);

    TsMCAMes_Flow(tpad);
    ret = TsMemCardCheck_Flow(0, tpad);

    TsSCFADE_Flow(0, 0);
    TsBGMPoll();

    return ret;
}

int TsMenu_Flow(void) {
    u_int tpad, tpad2;
    int   ret;

    TsGetMenuPad(0, &tpad);
    TsGetMenuPad(1, &tpad2);

    ret = TsMap_Flow(0, tpad, tpad2);

    TsMCAMes_Flow(tpad);
    TsSCFADE_Flow(0, 0);
    TsBGMPoll();
    TsSndFlow(0);

    return ret;
}

void TsMenu_Draw(void) {
    SPR_PRM    SprPrm;
    u_long128 *pkt;
    SPR_PRM   *spr;
    SPR_PKT    pk;
    int        flg;
    int        i;

    pk  = &pkt;
    spr = &SprPrm;

    MNScene_Draw(&MNS_StageMap);
    MNScene_Draw(&MNS_StageMap2);
    MNScene_Draw(&MNS_CityHall);

    flg = TsCELBackDraw(&MnPkt, spr, MNS_OptCounter.isDisp, 0);
    MNScene_Draw(&MNS_OptCounter);
    flg |= TsCELBackDraw(&MnPkt, spr, MNS_RepCounter.isDisp, 1);
    MNScene_Draw(&MNS_RepCounter);

    for (i = 0; i < PR_ARRAYSIZE(MNS_StgCounter); i++) {
        flg |= TsCELBackDraw(&MnPkt, spr, MNS_StgCounter[i].isDisp, 2);
        MNScene_Draw(&MNS_StgCounter[i]);
    }

    if (!flg) {
        TsCELBackEnd();
    }

    TsPatTexFnc(0);

    MnPkt.ptop = PR_UNCACHED(MnPkt.pkt[MnPkt.idx].PaketTop);
    pkt = (u_long128*)MnPkt.ptop;

    PkSprPkt_SetDefault(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));

    if (_bMapCaptureReq) {
        TsMenu_CaptureVram(pk, spr);
        _bMapCaptureReq = FALSE;
    }
    if (UserList_Sw) {
        TsUserList_Draw(pk, spr);
    }
    if (OptionList_Sw) {
        TsOption_Draw(pk, spr);
    }

    TsSCFADE_Draw(pk, spr, 2);

    if (PopMenu_Sw) {
        TsPopMenu_Draw(pk, spr);
    }
    if (SaveMenu_Sw) {
        TsSaveMenu_Draw(pk, spr);
    }
    if (JukeMenu_Sw) {
        TsJukeMenu_Draw(pk, spr);
    }

    MnPkt.ptop = (u_int)pkt;
    sceGsSyncPath(0, 0);

    TsDrawUPacket(&MnPkt);
    sceGsSyncPath(0, 0);

    MnPkt.ptop = PR_UNCACHED(MnPkt.pkt[MnPkt.idx].PaketTop);
    pkt = (u_long128*)MnPkt.ptop;

    PkSprPkt_SetDefault(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));
#ifdef VERSION_PAL
    TsCMPMes_SetPos(0x140, 0xd6);
#else
    TsCMPMes_SetPos(0x140, 0xba);
#endif
    TsCMPMes_Draw(pk, spr);
    TsSCFADE_Draw(pk, spr, 1);
#ifdef VERSION_PAL
    TsMCAMes_SetPos(0x140, 0x74);
#else
    TsMCAMes_SetPos(0x140, 0x65);
#endif

    TsMCAMes_Draw(pk, spr);
    MnPkt.ptop = (u_int)pkt;
    sceGsSyncPath(0, 0);

    TsDrawUPacket(&MnPkt);
    sceGsSyncPath(0, 0);

    pkt = TsCmnPkOpen(&FPacket);

    PkSprPkt_SetDefault(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));
    TsSCFADE_Draw(pk, spr, 0);

    TsCmnPkClose(&FPacket, pkt, 0xf);
}

static void TsSetRankingName(P3MC_STAGERANK *pRankTop, u_char *name) {
    P3MC_STAGERANK *pRank;
    int             i, j, k;

    pRank = pRankTop;
    for (i = 0; i < 8; i++, pRank++) {
        for (j = 0; j < pRank->nSplay; j++) {
            if (pRank->splay[j].name[0] == 0) {
                memcpy(pRank->splay[j].name, name, sizeof(pRank->splay[j].name));
            }
        }

        for (k = 0; k < 4; k++) {
            for (j = 0; j < pRank->nVplay[k]; j++) {
                if (pRank->vplay[k][j].name[0] == 0) {
                    memcpy(pRank->vplay[k][j].name, name, sizeof(pRank->vplay[k][j].name));
                }
            }
        }
    }
}

static void TsSetRanking2UData(USER_DATA *puser, P3MC_STAGERANK *wkRank) {
    int i;

    for (i = 0; i < PR_ARRAYSIZE(puser->stageRank); i++) {
        puser->stageRank[i] = wkRank[i];
    }
}

typedef struct {
    /* 0x00 */ GAME_STATUS base;
#ifdef VERSION_PAL
    /* 0xd0 */ long pad_pal;
#endif
} GAME_STATUS_PAL;

typedef struct {
    /* 0x000 */ int nRound;
    /* 0x004 */ char name[12];
    /* 0x010 */ char name1[12];
    /* 0x01c */ char name2[12];
    /* 0x028 */ u_int clrFlg[4];
    /* 0x038 */ int clrCount[8];
    /* 0x058 */ int clrVSCOM1[8];
    /* 0x078 */ int clrCOOL[8];
    /* 0x098 */ u_int logCOOL[8];
#ifdef VERSION_PAL
    /* 0x0b8 */ u_int game_status[54];
#else
    /* 0x0b8 */ GAME_STATUS game_status;
#endif
} P3LOG_VAL_PAL;

extern P3MC_RANKSCORE CurRankScore;

/* static */ void TsSetSaveData(MCRWDATA_HDL *pDataW, int mode, USER_DATA *puser) {
    int             stgIdx;
    int             vsLev;
    P3MC_STAGERANK *pRank;
    P3MC_RANKSCORE *pScore;
    P3LOG_VAL      *pLog;

    if (pDataW->pMemTop == NULL) {
        return;
    }

    switch (mode) {
    case 1:
        *(GAME_STATUS_PAL *)&pP3GameState->pLog->game_status = *(GAME_STATUS_PAL *)pP3GameState->pGameStatus;
        *(P3LOG_VAL_PAL *)pDataW->pData = *(P3LOG_VAL_PAL *)pP3GameState->pLog;

        pLog = (P3LOG_VAL *)pDataW->pData;
        memcpy(pLog->name, puser->name, 12);
        pLog->name[11] = '\0';

        TsSetRanking2UData(&pDataW->pHead->user, pCStageRank);
        TsSetRankingName(pDataW->pHead->user.stageRank, (u_char *)pLog->name);
        return;
    case 2:
        *(MC_REP_STR *)pDataW->pData = *pP3GameState->pReplayArea;

        pRank = pDataW->pHead->user.stageRank;
        stgIdx = puser->stageNo - 1;
        memset(pRank, 0, sizeof(pDataW->pHead->user.stageRank));

        if ((u_int)stgIdx < 8) {
            if (puser->isVs != 0) {
                if (puser->isVs == 2) {
                    vsLev = puser->vsLev;
                    pScore = pRank[stgIdx].vplay[vsLev];
                    pRank[stgIdx].nVplay[vsLev] = 1;
                    pScore[0] = CurRankScore;
                }
            } else {
                pRank[stgIdx].nSplay = 1;
                pRank[stgIdx].splay[0] = CurRankScore;
            }
        }

        TsSetRankingName(pRank, (u_char *)puser->name1);
        return;
    default:
        return;
    }
}

/* static */ void TsRestoreSaveData(MCRWDATA_HDL *pDataW, int mode) {
    int i;

    if (pDataW->pMemTop == NULL) {
        return;
    }

    switch (mode) {
    case 1:
        *UserWork = pDataW->pHead->user;
        *(P3LOG_VAL_PAL *)pP3GameState->pLog = *(P3LOG_VAL_PAL *)pDataW->pData;
        *(GAME_STATUS_PAL *)pP3GameState->pGameStatus = *(GAME_STATUS_PAL *)&pP3GameState->pLog->game_status;

        CurFileInfo.logFileNo = pDataW->pHead->user.fileNo;
        memcpy(CurFileInfo.logDate, &pDataW->pHead->user.date_day, 8);

        for (i = 0; i < 8; i++) {
            pCStageRank[i] = UserWork->stageRank[i];
        }
        break;
    case 2:
        *pP3GameState->pReplayArea = *(MC_REP_STR *)pDataW->pData;

        switch (pDataW->pHead->user.isVs) {
        case 0:
            pP3GameState->nMode = 0;
            pP3GameState->vsLev = 0;
            break;
        case 1:
            pP3GameState->nMode = pDataW->pHead->user.isVs;
            pP3GameState->vsLev = 0;
            break;
        case 2:
            pP3GameState->nMode = pDataW->pHead->user.isVs;
            pP3GameState->vsLev = pDataW->pHead->user.vsLev;
            if (pP3GameState->vsLev >= 4) {
                pP3GameState->vsLev = 3;
            }
            break;
        }
        pP3GameState->nStage = pDataW->pHead->user.stageNo;
        CurFileInfo.repFileNo = pDataW->pHead->user.fileNo;
        memcpy(CurFileInfo.repDate, &pDataW->pHead->user.date_day, 8);
        break;
    }
}

int DateChgInt(u_int n) {
    /* Convert BCD to decimal */
    return 
    (
        ((n & 0xf0) >> 4) * 10 +
         (n & 0xf)
    );
}

void GetRankScoreID(MAP_TIME *mptim, u_int *dat) {
    int year, sec, hour, day, month, min;

    year  = DateChgInt(mptim->date_year);
    sec   = DateChgInt(mptim->date_second);
    hour  = DateChgInt(mptim->date_hour);
    day   = DateChgInt(mptim->date_day);
    month = DateChgInt(mptim->date_month);
    min   = DateChgInt(mptim->date_minute);

    dat[0] = (year % 50) * (12 * 31 * 24 * 60 * 60) + (month % 12) * (31 * 24 * 60 * 60) + (day % 31) * (24 * 60 * 60) + (hour % 24) * (60 * 60) + (min % 60) * 60 + (sec % 60);
    dat[1] = ((rand() % 0x10000) << 8) + mptim->date_pad;
}

extern P3MC_RANKSCORE CurRankScore;

/* static */ int TsRanking_Set(void) {
    P3GAMESTATE    *pstate = pP3GameState;
    int             stageNo = pstate->nStage;
    int             ret, i;
    int             maxRank;
    u_int           score;
    int            *pnRank;
    P3MC_RANKSCORE *pRank;

    memset(&CurRankScore, 0, sizeof(CurRankScore));

    if (stageNo <= 0 || stageNo > 8) {
        return -1;
    }
    if (pstate->nMode == 1) {
        return -1;
    }

    if (pstate->nMode == 2) {
        pRank   = pCStageRank[stageNo - 1].vplay[pstate->vsLev];
        pnRank  = &pCStageRank[stageNo - 1].nVplay[pstate->vsLev];
        score   = pstate->score;
        maxRank = 10;
        if (pstate->winPlayer > 0) {
            return -1;
        }
    } else {
        score   = pstate->score + pstate->bonusG;
        maxRank = 20;
        pRank   = pCStageRank[stageNo - 1].splay;
        pnRank  = &pCStageRank[stageNo - 1].nSplay;
    }

    GetRankScoreID(&MapTime, CurRankScore.scDate);
    CurRankScore.score = score;
    CurRankScore.name[0] = 0;

    i = 0;
    ret = *pnRank;
    if (ret < maxRank) {
        for (; i < ret; i++) {
            if (pRank[i].score < score) {
                break;
            }
        }
        (*pnRank)++;
    } else {
        for (; i < maxRank; i++) {
            if (pRank[i].score < score) {
                break;
            }
        }
    }

    if (i < maxRank) {
        ret = i;
        for (i = maxRank - 1; ret < i; i--) {
            pRank[i] = pRank[i - 1];
        }
        pRank[ret] = CurRankScore;
    } else {
        ret = -1;
    }

    return ret;
}

void TsMENU_SetMapScreen(int mapNo) {
    CurMapNo = mapNo;
    CurMapOldFlg = CurMapBakFlg;

    TsSetScene_Map(&MNS_StageMap, mapNo, CurMapBakFlg, 1);

    CurMapState = 0;
}

/* static */ int TsCheckTimeMapChange(void) {
    if (CurMapState == 0) {
        if (CurMapBakFlg != CurMapOldFlg) {
            if (MNScene_isSeniAnime(&MNS_StageMap)) {
                return 0;
            }
            _bMapCaptureReq = 1;
            CurMapState = 1;
            return 1;
        }
    }

    switch (CurMapState) {
    case 0:
        TsMENU_GetMapTimeState(0);
        break;
    case 1:
        TsSetScene_Map(&MNS_StageMap2, CurMapNo, CurMapBakFlg, 0);
        MNScene_CopyState(&MNS_StageMap2, &MNS_StageMap);
        TsMENU_SetMapScreen(CurMapNo);
        MNScene_CopyState(&MNS_StageMap, &MNS_StageMap2);
        MNScene_DispSw(&MNS_StageMap, 0);
        MNScene_DispSw(&MNS_StageMap2, 1);
        CurMapState = 2;
    case 2:
        if (TsSCFADE_Set(5, 30, 2)) {
            break;
        }
        MNScene_DispSw(&MNS_StageMap2, 0);
        MNScene_End(&MNS_StageMap2);
        MNScene_DispSw(&MNS_StageMap, 1);
        CurMapState = 0;
        break;
    }

    return CurMapState;
}

int TsAnimeWait_withKeySkip(u_int tpad, MN_SCENE *scene, int ltim, u_int bnk) {
    if (bnk == -1) {
        return MNScene_isAnime(scene, ltim);
    } else {
        return MNScene_isAnimeBank(scene, ltim, bnk);
    }
}

static int TsMemCardCheck_Flow(int flg, u_int tpad) {
    extern int state;
    extern int mesNo;
    int ret;

    if (flg == 1) {
        McInitFlow();
        state = 0;
        mesNo = -1;
        McStartCheckFlow(1);
        TsMCAMes_SetMes(-1);
        return 0;
    }

    switch (state) {
    case 0:
        ret = McStartCheckFlow(0);
        if (ret == 0) {
            state = 0x1000;
            return 0;
        }

        if (ret >= 0) {
            if (mesNo != ret) {
                mesNo = ret;

                switch (ret) {
                case 1:
                    TsMCAMes_SetMes(0x1030001);
                    break;
                case 2:
                    TsMCAMes_SetMes(0x1030000);
                    break;
                }
            }

            if (TsMCAMes_GetSelect()) {
                state = 0x1000;
            }
        }

        break;
    case 0x1000:
        if (TsSCFADE_Set(2, 0xf, 0)) {
            return 0;
        }
        TsMCAMes_SetMes(-1);
        McStartCheckFlow(2);
        P3MC_CheckChangeSet();
        _MNwaitTime = 30;
        state = 0x1010;
    /* fallthrough */
    case 0x1010:
        if (--_MNwaitTime <= 0) {
            return 1;
        }
        break;
    }

    return 0;
}

static int TsMap_Flow(int flg, u_int tpad, u_int tpad2) {
    /* TODO: Fix names once made static. */
    /* sbss 399ad0 */ extern int state_tmp_253;
    /* bss 1c77a50 */ extern MAPPOS MapCity_tmp_254;
    int ret;
    int mn;

    if (flg == 1) {
        switch (tpad) {
        case 0:
            pP3GameState->nStage = 0;
            MenuVoiceBankSet(0);
            state_tmp_253 = 0;
            break;
        case 1:
            MenuVoiceBankSet(0);
            state_tmp_253 = 0x6500;
            break;
        case 2:
            MenuVoiceBankSet(0);
            TsBGMPlay(1, 0x14);
            state_tmp_253 = 0x2000;
            break;
        case 3:
            MenuVoiceBankSet(0);
            state_tmp_253 = 0;
            break;
        }

        TsMENU_GetMapTimeState(1);
        return 0;
    }

    switch (state_tmp_253) {
    case 0:
        CurMapOldFlg = -1;
        mn = TsMENU_GetMapNo(NULL);
        TsMENU_SetMapScreen(mn);
        MapCity_tmp_254.pscene = &MNS_StageMap;
        MapCity_tmp_254.panime = StageMapAnimePA;
        MapCity_tmp_254.lmtPos = mn + 1;

        if (mn < 2) {
            MapCity_tmp_254.mnmap = mnmapMap1;
        } else if (pP3GameState->pLog->nRound >= 4) {
            MapCity_tmp_254.mnmap = mnmapMap2;
        } else {
            MapCity_tmp_254.mnmap = mnmapMap;
        }

        MpMapMenu_Flow(1, &MapCity_tmp_254, 0);

        switch (pP3GameState->nStage) {
        case 1:
            mn = 1;
            break;
        case 2:
            mn = 2;
            break;
        case 3:
            mn = 3;
            break;
        case 4:
            mn = 4;
            break;
        case 5:
            mn = 5;
            break;
        case 6:
            mn = 6;
            break;
        case 7:
            mn = 7;
            break;
        case 8:
            mn = 8;
            break;
        case 9:
            mn = 9;
            break;
        case 0:
        default:
            mn = 0;
            break;
        }

        MpMapMenu_Flow(3, &MapCity_tmp_254, mn);
        TsCMPMes_SetMes(-1);
        TsSet_ParappaCapColor();
        if (pP3GameState->pAutoMove == NULL) {
            TsBGMPlay(MapCity_tmp_254.curPos + 1, 0xa);
        }
        state_tmp_253 = 0x1000;
        /* fallthrough */
    case 0x1000:
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap, 1);
        state_tmp_253 = 0x1010;
        break;
    case 0x1010:
        if (!pP3GameState->isWipeEnd || TsBGMLoadCheck()) {
            return 0;
        }
        if (TsAnimeWait_withKeySkip(tpad, &MNS_StageMap, 0, -1)) {
            return 0;
        }
        /* fallthrough */
    case 0x1018:
        mn = 0;

        if (pP3GameState->pAutoMove != NULL) {
            switch (*pP3GameState->pAutoMove) {
            case 1:
                mn = 1;
                break;
            case 2:
                mn = 2;
                break;
            case 3:
                mn = 3;
                break;
            case 4:
                mn = 4;
                break;
            case 5:
                mn = 5;
                break;
            case 6:
                mn = 6;
                break;
            case 7:
                mn = 7;
                break;
            case 8:
                mn = 8;
                break;
            case 9:
                mn = 9;
                break;
            case 0:
                mn = 0;
                break;
            case -2:
                mn = -2;
                pP3GameState->pAutoMove = NULL;
                break;
            case -1:
            default:
                pP3GameState->pAutoMove = NULL;
                mn = 0;
                break;            
            }

            if (pP3GameState->pAutoMove != NULL) {
                MpMapMenu_Flow(4, &MapCity_tmp_254, mn);

                if (MapCity_tmp_254.sndtrg == 1) {
                    TsBGMChangePos(MapCity_tmp_254.curPos + 1);
                }

                pP3GameState->pAutoMove++;
                if (*pP3GameState->pAutoMove == -1) {
                    pP3GameState->pAutoMove = NULL;
                }
            }
        } 

        if (mn == -2) {
            state_tmp_253 = 0x5010;
            break;
        }

        state_tmp_253 = 0x1020;
        /* fallthrough */
    case 0x1020:
        if (!MapCity_tmp_254.bMove) {
            if (TsCheckTimeMapChange()) {
                break;
            }
        }

        ret = MpMapMenu_Flow(0, &MapCity_tmp_254, tpad);
        if (MapCity_tmp_254.anmStop != 0) {
            if (pP3GameState->pAutoMove != NULL) {
                state_tmp_253 = 0x1018;
                break;
            }
        }

        switch (MapCity_tmp_254.sndtrg) {
        case 1:
            TsBGMChangePos(MapCity_tmp_254.curPos + 1);
            break;
        case 3:
            TSSNDPLAY(VSND_CANCEL);
            break;
        case 2:
            break;
        }

        if (ret == 0) {
            break;
        } else if (ret == 1) {
            state_tmp_253 = 0x1100;
        } else if (ret == -1) {
            state_tmp_253 = 0x1200;
            break;
        } else {
            break;
        }

        /* fallthrough */
    case 0x1100:
        if (MapCity_tmp_254.curPos == 0) {
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
            TSSNDPLAY(VSND_SELMODE);
            state_tmp_253 = 0x6000;
            break;
        } else if (MapCity_tmp_254.curPos == 9) {
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
            TSSNDPLAY(VSND_SELMODE);
            state_tmp_253 = 0x5000;
            break;
        }

        TSSNDPLAY(VSND_SELMODE);
        pP3GameState->nStage = MapCity_tmp_254.curPos;
        state_tmp_253 = 0x3000;
        break;
    case 0x1200:
        TsBGMStop(0x26);
        _MNwaitTime = 40;
        TsCMPMes_SetMes(-1);
        pP3GameState->nStage = MapCity_tmp_254.curPos;
        state_tmp_253 = 0x1210;
        /* fallthrough */
    case 0x1210:
        if (--_MNwaitTime <= 0) {
            return P3MRET_TOTITLE;
        }
        break;
    case 0x2000:
        if (pP3GameState->nStage < 1 || pP3GameState->nStage > 8) {
            state_tmp_253 = 0;
            return 0;
        }
        TsRanking_Set();
        MpSave_Flow(1, 0, 0);
        state_tmp_253 = 0x2010;
        /* fallthrough */
    case 0x2010:
        if (!pP3GameState->isWipeEnd) {
            break;
        }
        _MNwaitTime = 30;
        state_tmp_253 = 0x2020;
        /* fallthrough */
    case 0x2020:
        if (--_MNwaitTime <= 0) {
            state_tmp_253 = 0x2100;
        } else {
            break;
        }        
        /* fallthrough */
    case 0x2100:
        if (!MpSave_Flow(0, tpad, tpad2)) {
            return 0;
        }
        state_tmp_253 = 0x2200;
        /* fallthrough */
    case 0x2200:
        if (TsSCFADE_Set(2, 0x1e, 0)) {
            return 0;
        }
        TsSet_ParappaCapColor();
        state_tmp_253 = 0x2400;
        /* fallthrough */
    case 0x2400:
        TsSCFADE_Set(1, 0x1e, 0);
        state_tmp_253 = 0;
        break;
    case 0x3000:
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
        MpPopMenu_Flow(1, 0);
        state_tmp_253 = 0x3010;
        /* fallthrough */
    case 0x3010:
        ret = MpPopMenu_Flow(0, tpad);

        if (ret == 0) {
            break;
        } else if (ret == -1) {
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[0]);
            state_tmp_253 = 0x1000;
            break;
        }

        switch (ret & 0xff) {
        case 1:
            pP3GameState->nMode = 0;
            pP3GameState->vsLev = 0;
            break;
        case 2:
            pP3GameState->nMode = 1;
            pP3GameState->vsLev = 0;
            break;
        case 3:
            pP3GameState->nMode = 2;
            pP3GameState->vsLev = ret >> 0x8;
            break;
        }

        state_tmp_253 = 0x4000;
        break;
    case 0x4000:
        TsCheckEnding(pP3GameState);
        TsBGMStop(0x20);
        _MNwaitTime = 32;
        TsCMPMes_SetMes(-1);
        state_tmp_253 = 0x4010;
        /* fallthrough */
    case 0x4010:
        if (--_MNwaitTime <= 0) {
            return P3MRET_PLAYGAME;
        }
        break;
    case 0x5000:
        pP3GameState->curRecJacket = 0;
        state_tmp_253 = 0x5100;
        break;
    case 0x5010:
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[0]);
        state_tmp_253 = 0x5018;
        _MNwaitTime = 40;
        /* fallthrough */
    case 0x5018:
        if (--_MNwaitTime <= 0) {
            TSSNDPLAY(VSND_SELMODE);
            state_tmp_253 = 0x5100;
        }
        break;
    case 0x5100:
        pP3GameState->nStage = MapCity_tmp_254.curPos;
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
        TsJukeMenu_Flow(1, pP3GameState->curRecJacket);
        state_tmp_253 = 0x5200;
        /* fallthrough */
    case 0x5200:
        if (!TsJukeMenu_Flow(0, tpad)) {
            return 0;
        }
        TsJukeMenu_Flow(2, 0);
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[0]);
        state_tmp_253 = 0x1000;
        break;
    case 0x6000:
        MenuVoiceBankSet(0);
        state_tmp_253 = 0x6001;
        /* fallthrough */
    case 0x6001:
        if (TsSCFADE_Set(2, 0x14, 0)) {
            return 0;
        }
        state_tmp_253 = 0x6010;
        /* fallthrough */
    case 0x6010:
        TsBGMPlay(1, 0x14);
        TsSCFADE_Set(1, 0x14, 0);
        MpCityHall_Flow(1, 0, 0);
        state_tmp_253 = 0x7000;
        break;
    case 0x6500:
        TsBGMPlay(1, 0x14);
        MpCityHall_Flow(1, 1, 0);
        state_tmp_253 = 0x6510;
        /* fallthrough */
    case 0x6510:
        if (!pP3GameState->isWipeEnd) {
            break;
        }
        state_tmp_253 = 0x7000;
        /* fallthrough */
    case 0x7000:
        ret = MpCityHall_Flow(0, tpad, tpad2);
        if (ret == 0) {
            break;
        } else if (ret == 2) {
            state_tmp_253 = 0xff00;
        } else if (ret == 3) {
            state_tmp_253 = 0x7020;
        } else {
            state_tmp_253 = 0x7010;
        }
        break;
    case 0x7010:
        MenuVoiceBankSet(0);
        state_tmp_253 = 0x7011;
        /* fallthrough */
    case 0x7011:
        if (TsSCFADE_Set(2, 0x1e, 0)) {
            return 0;
        }
        state_tmp_253 = 0x7020;
        /* fallthrough */
    case 0x7020:
        if (TsBGMLoadCheck()) {
            return 0;
        }
        TsMap_Flow(1, 0, 0);
        TsSCFADE_Set(1, 0xf, 0);
        break;
    case 0xff00:
        return P3MRET_REPLAY;
    }

    return 0;
}

extern u_char *TeachersName_Tbl[];

/* static */ void TsMakeUserWork(int mode) {
    int stageNo;
    int i;
    int roundNo;
    int score;

    stageNo = pP3GameState->nStage;
    if (stageNo < 0) {
        stageNo = 0;
    }
    if (stageNo >= 9) {
        stageNo = 8;
    }
    memset(UserWork, 0, sizeof(*UserWork));

    memcpy(UserWork->name, pP3GameState->pLog->name, sizeof(UserWork->name));
    UserWork->name[11] = '\0';

    if (mode == 2) {
        memcpy(UserWork->name1, pP3GameState->pLog->name1, sizeof(UserWork->name1));
        UserWork->name1[11] = '\0';

        if (pP3GameState->nMode == 2) {
            int stg = stageNo;
            if (stg >= 9) {
                stg = 8;
            }
            if (stg > 0) {
                stg--;
            }
            memcpy(UserWork->name2, TeachersName_Tbl[stg], sizeof(UserWork->name2));
            UserWork->name2[11] = '\0';
        } else {
            memcpy(UserWork->name2, pP3GameState->pLog->name2, sizeof(UserWork->name2));
            UserWork->name2[11] = '\0';
        }
    }

    UserWork->mode = mode;
    UserWork->flg = 1;

    roundNo = pP3GameState->pLog->nRound;
    for (i = 0; i < 8; i++) {
        if (roundNo < pP3GameState->pLog->clrCount[i]) {
            break;
        }
    }

    if (i >= 8) {
        roundNo--;
    }

    UserWork->stageNo = stageNo;
    score = pP3GameState->score;
    if (score < 0) {
        score = 0;
    }
    UserWork->score = score;
    if (roundNo < 0) {
        roundNo = 0;
    }
    if (roundNo >= 99) {
        roundNo = 98;
    }
    UserWork->roundNo = roundNo;
    UserWork->score2 = pP3GameState->score2P;

    if (pP3GameState->nMode == 1 || pP3GameState->nMode == 2) {
        UserWork->winner = pP3GameState->winPlayer;
    } else {
        UserWork->winner = 0;
    }

    UserWork->fileNo = 0;
    if (UserWork->mode == 1) {
        int mn = TsMENU_GetMapNo(NULL) - 1;
        if (mn <= 0) {
            mn = 0;
        }
        UserWork->stageNo = mn;
    }

    switch (pP3GameState->nMode) {
    case 0:
        UserWork->isVs = 0;
        UserWork->vsLev = 0;
        break;
    case 1:
        UserWork->isVs = 1;
        UserWork->vsLev = 0;
        break;
    case 2:
        UserWork->isVs = 2;
        UserWork->vsLev = pP3GameState->vsLev;
        if (UserWork->vsLev >= 4) {
            UserWork->vsLev = 3;
        }
        break;
    }
}

/* static */ void TsSaveSuccessProc(void) {
    TsUserList_SetCurUserData(UserWork);

    memcpy(pP3GameState->pLog->name, UserWork->name, sizeof(UserWork->name));
    pP3GameState->pLog->name[11] = '\0';

    if (UserWork->mode == 2) {
        memcpy(pP3GameState->pLog->name1, UserWork->name1, sizeof(UserWork->name1));
        pP3GameState->pLog->name1[11] = '\0';

        if (UserWork->isVs == 1) {
            memcpy(pP3GameState->pLog->name2, UserWork->name2, sizeof(UserWork->name2));
            pP3GameState->pLog->name2[11] = '\0';
        }
    }

    if (UserWork->mode == 1) {
        TsSetRankingName(pCStageRank, pP3GameState->pLog->name);
    } else {
        TsSetRankingName(pCStageRank, pP3GameState->pLog->name1);
    }

    TsSetRanking2UData(UserWork, pCStageRank);

    if (UserWork->mode == 1) {
        CurFileInfo.logFileNo = UserWork->fileNo;
        memcpy(CurFileInfo.logDate, &UserWork->date_day, sizeof(CurFileInfo.logDate));
    } else {
        CurFileInfo.repFileNo = UserWork->fileNo;
        memcpy(CurFileInfo.repDate, &UserWork->date_day, sizeof(CurFileInfo.repDate));
    }
}

extern USERLISTTYPE_TABLE ULTypeT_SAVE_LOG;
extern USERLISTTYPE_TABLE ULTypeT_SAVE_REPLAY;

/* static */ int MpSave_Flow(int flg, u_int tpad, u_int tpad2) {
    extern int state_tmp_mpsave;
    extern int saveMode_tmp_mpsave;
    extern int waitTime_tmp_mpsave;
    int ret;
    int chkMode;

    if (flg == 1) {
        if (tpad == 0) {
            saveMode_tmp_mpsave = 1;
        } else {
            saveMode_tmp_mpsave = tpad;
        }
        CurMapOldFlg = -1;
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap, 1);
        TsMENU_SetMapScreen(0);
        TsCMPMes_SetMes(-1);
        state_tmp_mpsave = 0;
        return 0;
    }

    switch (state_tmp_mpsave) {
    case 0:
        state_tmp_mpsave = 0x1000;
        /* fallthrough */
    case 0x1000:
        TsSaveMenu_Flow(1, saveMode_tmp_mpsave - 1);
        TsUserList_Flow(1, 0, 0);
        state_tmp_mpsave = 0x1010;
        /* fallthrough */
    case 0x1010:
        ret = TsSaveMenu_Flow(0, tpad);
        if (ret != 0) {
            saveMode_tmp_mpsave = ret;
            if (ret < 3) {
                if (ret > 0) {
                    state_tmp_mpsave = 0x2000;
                    break;
                }
            }
            state_tmp_mpsave = 0xf000;
        }
        break;
    case 0x2000:
        TsCMPMes_SetMes(-1);
        McInitFlow();
        state_tmp_mpsave = 0x2010;
        /* fallthrough */
    case 0x2010:
        if (saveMode_tmp_mpsave != 1) {
            chkMode = 2;
        } else {
            chkMode = 1;
        }
        ret = McUserCheckFlow(2, chkMode, NULL);
        if (ret < 0) {
            break;
        }
        if (ret > 0 && ret < 3) {
            TsMENU_GetMapTimeState(1);
            MpSave_Flow(1, saveMode_tmp_mpsave, 0);
            if (UserList_Sw != 0) {
                state_tmp_mpsave = 0x5000;
            } else {
                state_tmp_mpsave = 0;
            }
            break;
        }
        state_tmp_mpsave = 0x2020;
        break;
    case 0x2020:
        if (saveMode_tmp_mpsave != 1) {
            chkMode = 2;
        } else {
            chkMode = 1;
        }
        TsMakeUserWork(chkMode);
        state_tmp_mpsave = 0x2030;
        /* fallthrough */
    case 0x2030:
        if (TsSCFADE_Set(2, 0x14, 0)) {
            return 0;
        }
        state_tmp_mpsave = 0x2040;
        /* fallthrough */
    case 0x2040:
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap, 0);
        if (saveMode_tmp_mpsave == 1) {
            TsUserList_SetType(&ULTypeT_SAVE_LOG, pP3GameState->nMode, 0);
        } else {
            TsUserList_SetType(&ULTypeT_SAVE_REPLAY, pP3GameState->nMode, 0);
        }
        UserList_Sw = 1;
        state_tmp_mpsave = 0x2050;
        /* fallthrough */
    case 0x2050:
        if (TsSCFADE_Set(1, 0x14, 0) >= 9) {
            break;
        }
        state_tmp_mpsave = 0x2060;
        /* fallthrough */
    case 0x2060:
        ret = TsUserList_Flow(0, tpad, tpad2);
        if (ret == 0) {
            return 0;
        }
        McInitFlow();
        switch (ret) {
        case 1:
            state_tmp_mpsave = 0x3100;
            break;
        case -1:
            state_tmp_mpsave = 0x5000;
            break;
        case -3:
            state_tmp_mpsave = 0x2200;
            break;
        default:
            return 0;
        }
        break;
    case 0x2200:
        state_tmp_mpsave = 0x2210;
        /* fallthrough */
    case 0x2210:
        state_tmp_mpsave = 0x2000;
        break;
    case 0x3100:
        UserWork->fileNo = TsUserList_GetCurFileNo(NULL);
        state_tmp_mpsave = 0x3110;
        /* fallthrough */
    case 0x3110:
        ret = McUserSaveFlow(UserWork);
        if (ret < 0) {
            TsUserList_SetCurDispUserData(UserWork);
            return 0;
        }
        if (ret != 0) {
            if (ret == 1) {
                state_tmp_mpsave = 0x2020;
            }
            if (ret == 2) {
                state_tmp_mpsave = 0x5000;
            }
            if (ret == 4) {
                state_tmp_mpsave = 0x2200;
                break;
            }
            return 0;
        }
        /* fallthrough */
    case 0x4000:
    case 0x4010:
        if (saveMode_tmp_mpsave == 1) {
            saveMode_tmp_mpsave = 2;
        } else {
            saveMode_tmp_mpsave = 1;
        }
        TsSaveSuccessProc();
        waitTime_tmp_mpsave = 0;
        state_tmp_mpsave = 0x4020;
        /* fallthrough */
    case 0x4020:
        TsCMPMes_SetMes(-1);
        if (++waitTime_tmp_mpsave >= 35) {
            waitTime_tmp_mpsave = 0;
            state_tmp_mpsave = 0x5000;
        } else {
            break;
        }
        /* fallthrough */
    case 0x5000:
        if (TsSCFADE_Set(2, 0x14, 0)) {
            return 0;
        }
        TsUserList_Flow(2, 0, 0);
        UserList_Sw = 0;
        TsMENU_GetMapTimeState(1);
        MpSave_Flow(1, saveMode_tmp_mpsave, 0);
        state_tmp_mpsave = 0x5020;
        /* fallthrough */
    case 0x5020:
        if (TsSCFADE_Set(1, 0x14, 0) < 9) {
            state_tmp_mpsave = 0;
            break;
        }
        return 0;
    case 0xf000:
        if (pP3GameState->pAutoMove == NULL) {
            TsBGMMute(20);
        }
        state_tmp_mpsave = 0xf005;
        /* fallthrough */
    case 0xf005:
        if (TsSCFADE_Set(2, 0x1e, 0) >= 2) {
            break;
        }
        state_tmp_mpsave = 0xf010;
        /* fallthrough */
    case 0xf010:
        if (pP3GameState->pLog->name[0] != 0) {
            TsSetRankingName(pCStageRank, pP3GameState->pLog->name);
            TsSetRanking2UData(UserWork, pCStageRank);
        } else if (pP3GameState->pLog->name1[0] != 0) {
            TsSetRankingName(pCStageRank, pP3GameState->pLog->name1);
            TsSetRanking2UData(UserWork, pCStageRank);
        }
        TsMENU_GetMapTimeState(1);
        return 1;
    }

    return 0;
}

static int MpCityHall_Flow(int flg, u_int tpad, u_int tpad2) {
    /* TODO: Fix names once made static. */
    /* sbss 399ae0 */ extern int state_tmp_270;
    /* sbss 399ae4 */ extern int waitTime_tmp_271;
    /* bss 1c77a88 */ extern MAPPOS MapCHall_tmp_272;
    /* sbss 399ae8 */ extern int scstate_tmp_273;
    /* sbss 399aec */ extern int scstPos_tmp_274;
    /* sbss 399af0 */ extern int curTag_tmp_275;
    /* sbss 399af4 */ extern int fphs_pos_tmp_276;
    /* sbss 399af8 */ extern u_int AnmBit_tmp_277;
    int chkType, chkMode;
    int bBroken;
    int ret;
    int anmno;
    int cmpmesNo;
    int isError;
    int ntag;

    if (flg == 1) {
        switch (tpad) {
        case 0:
            MNScene_DispSw(&MNS_StageMap, 0);
            MNScene_DispSw(&MNS_CityHall, 1);
            AnmBit_tmp_277 = MNScene_StartAnime(&MNS_CityHall, -1, CityHallAnime);
            TsCMPMes_SetMes(-1);
            MapCHall_tmp_272.pscene = &MNS_CityHall;
            MapCHall_tmp_272.panime = CityHallAnime;
            MapCHall_tmp_272.mnmap = mnmapCityHall;
            MpMapMenu_Flow(1, &MapCHall_tmp_272, 0);
            MapCHall_tmp_272.curPos = 0;
            waitTime_tmp_271 = 0;
            state_tmp_270 = 0;
            scstate_tmp_273 = 0;
            scstPos_tmp_274 = 0;
            MpCityHallCharPosSet(0);
            fphs_pos_tmp_276 = 0;
            MpCityHallFPHSSoundMask(0);
            TSSNDPLAY(0x8001);
            return 0;
        case 1:
            MNScene_DispSw(&MNS_StageMap, 0);
            MNScene_DispSw(&MNS_CityHall, 1);
            MapCHall_tmp_272.pscene = &MNS_CityHall;
            MapCHall_tmp_272.panime = CityHallAnime;
            MapCHall_tmp_272.mnmap = mnmapCityHall;
            MpMapMenu_Flow(1, &MapCHall_tmp_272, 0);
            MpMapMenu_Flow(3, &MapCHall_tmp_272, 2);
            TsCMPMes_SetMes(-1);
            scstate_tmp_273 = 0;
            state_tmp_270 = 0x100;
            scstPos_tmp_274 = 0;
            MpCityHallCharPosSet(2);
            fphs_pos_tmp_276 = 2;
            return 0;
        }
    }

    switch (scstate_tmp_273) {
    case 0:
        if (scstPos_tmp_274 != 0) {
            scstate_tmp_273 = 0x2100;
        }
        break;
    case 0x100:
        if (TsSCFADE_Set(2, 0xa, 1)) {
            break;
        }
        TsUserList_Flow(2, 0, 0);
        UserList_Sw = 0;
        MNScene_DispSw(&MNS_OptCounter, 0);
        OptionList_Sw = 0;
        scstate_tmp_273 = 0x110;
        /* fallthrough */
    case 0x110:
        MNScene_DispSw(&MNS_CityHall, 1);
        MpMapMenu_Flow(3, &MapCHall_tmp_272, MapCHall_tmp_272.curPos);
        MpCityHallCharPosSet(MapCHall_tmp_272.curPos);
        fphs_pos_tmp_276 = MapCHall_tmp_272.curPos;
        MpCityHallFPHSSoundMask(0);
        scstate_tmp_273 = 0x120;
        /* fallthrough */
    case 0x120:
        if (!TsSCFADE_Set(1, 0x14, 1)) {
            scstate_tmp_273 = 0;
        }
        break;
    case 0x2000:
        if (scstPos_tmp_274 == 0) {
            scstate_tmp_273 = 0x100;
        }
        break;
    case 0x2100:
        anmno = -1;
        cmpmesNo = -1;

        switch (MapCHall_tmp_272.curPos) {
        case 0:
            anmno = 3;
            cmpmesNo = MENU_LOGLOAD_CAM;
            MpCityHallFPHOK(0);
            break;
        case 2:
            anmno = 9;
            cmpmesNo = MENU_REPLOAD_CAM;
            MpCityHallFPHOK(1);
            break;
        case 1:
            anmno = 6;
            cmpmesNo = MENU_OPT_CAM;
            MpCityHallFPHOK(2);
            break;
        }

        AnmBit_tmp_277 = 0x80000000;
        if (anmno >= 0) {
            AnmBit_tmp_277 |= MNScene_StartAnime(&MNS_CityHall, -1, &CityHallAnime[anmno]);
        }

        MpCityHallFPHSSoundMask(1);
        TsCMPMes_SetMes(cmpmesNo);
        scstate_tmp_273 = 0x2200;
        /* fallthrough */
    case 0x2200:
        if (scstPos_tmp_274 == 0) {
            scstate_tmp_273 = 0x100;
        }
        if (!TsAnimeWait_withKeySkip(tpad, &MNS_CityHall, 0xa, AnmBit_tmp_277)) {
            scstate_tmp_273 = 0x2000;
        }
        break;
    }

    fphs_pos_tmp_276 = MpCityHallFPHSMove(MapCHall_tmp_272.curPos, fphs_pos_tmp_276);

    switch (state_tmp_270) {
    case 0:
        if (++waitTime_tmp_271 == 70) {
            TsCMPMes_SetMes(MENU_HALL_INSIDE);
            TSSNDPLAY(VSND_MENU1);
        }
        if (TsAnimeWait_withKeySkip(tpad, &MNS_CityHall, 0, -1)) {
            return 0;
        }
        TSSND_SKIPPLAY(VSND_MENU1);
        TsCMPMes_SetMes(MENU_HALL_INSIDE);
        MpMapMenu_Flow(3, &MapCHall_tmp_272, 0);
        state_tmp_270 = 0x100;
        /* fallthrough */
    case 0x100:
        ret = MpMapMenu_Flow(0, &MapCHall_tmp_272, tpad);
        if (MapCHall_tmp_272.anmStop != 0) {
            TSSND_SKIPSTOP(2);
        }
        if (MapCHall_tmp_272.anmtrg != 0) {
            MpCityHallParaStart(MapCHall_tmp_272.anmtrg);
        }

        switch (MapCHall_tmp_272.sndtrg) {
        case 1:
            TSSNDPLAY(VSND_MVCUS_LR);
            break;
        case 2:
            TSSNDPLAY(VSND_SELPOPUP);
            break;
        case 3:
            TSSNDPLAY(VSND_CANCEL);
            break;
        }

        if (ret != 0) {
            if (ret == 1) {
                switch (MapCHall_tmp_272.curPos) {
                case 0:
                    TSSNDSTOP(3);
                    TSSNDPLAY(VSND_MENU2);
                    break;
                case 2:
                    TSSNDSTOP(3);
                    TSSNDPLAY(VSND_MENU3);
                    break;
                case 1:
                    TSSNDSTOP(3);
                    TSSNDPLAY(VSND_MENU4);
                    break;
                }

                state_tmp_270 = 0x1000;
            }

            if (ret == -1) {
                state_tmp_270 = 0xf000;
            }

            return 0;
        }

        break;
    case 0x1000:
        if (MapCHall_tmp_272.curPos == 1) {
            state_tmp_270 = 0x1500;
            break;
        }
        McInitFlow();
        state_tmp_270 = 0x1010;
        /* fallthrough */
    case 0x1010:
        if (MapCHall_tmp_272.curPos == 0) {
            curTag_tmp_275 = 0;
            chkMode = 1;
            chkType = 3;
        } else {
            curTag_tmp_275 = 0;
            chkType = 1;
            chkMode = 2;
        }

        ret = McUserCheckFlow(chkType, chkMode, 0);
        if (ret == -2) {
            break;
        }
        if (ret > 0 && ret < 3) {
            state_tmp_270 = 0x5000;
            break;
        }
        state_tmp_270 = 0x1500;
        /* fallthrough */
    case 0x1500:
        scstPos_tmp_274 = 1;
    
        switch (MapCHall_tmp_272.curPos) {
        case 0:
        case 2:
            state_tmp_270 = 0x2000;
            break;
        case 1:
            state_tmp_270 = 0x3000;
            break;
        }
    
        break;
    case 0x2000:
        if (MapCHall_tmp_272.curPos == 0) {
            chkMode = 1;
            chkType = 3;
        } else {
            chkMode = 2;
            chkType = 1;
        }

        isError = 0;
        ret = McUserCheckFlow(chkType, chkMode, &isError);
        if (ret < 0) {
            if (isError == 1) {
                scstPos_tmp_274 = 0;
            }
            if (isError == 2) {
                scstPos_tmp_274 = 1;
                break;
            }
            return 0;
        }
        if (ret > 0 && ret < 3) {
            state_tmp_270 = 0x5000;
            break;
        }
        TsMCAMes_SetMes(-1);
        state_tmp_270 = 0x2020;
        break;
    case 0x2020:
        if (scstate_tmp_273 & 0xfff) {
            return 0;
        }
        state_tmp_270 = 0x2021;
        /* fallthrough */
    case 0x2021:
        if (UserList_Sw) {
            if (TsSCFADE_Set(2, 0x14, 0)) {
                return 0;
            }
        }
        TsMakeUserWork(1);
        state_tmp_270 = 0x2022;
        /* fallthrough */
    case 0x2022:
        if (MapCHall_tmp_272.curPos == 0) {
            if (UCheckLoadError != 0) {
                ntag = 1;
                curTag_tmp_275 = 1;
            } else {
                ntag = curTag_tmp_275;
            }
            TsUserList_SetType(&ULTypeT_CITY_STGCLR, pP3GameState->nMode, ntag);
        } else {
            TsUserList_SetType(&ULTypeT_CITY_REPLAY, pP3GameState->nMode, 0);
        }

        MNScene_End(&MNS_StageMap2);
        MNScene_Init(&MNS_StageMap2, &Scene_CityHall, 0);
        MNScene_CopyState(&MNS_StageMap2, &MNS_CityHall);
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap2, 2);
        state_tmp_270 = 0x2024;
        /* fallthrough */
    case 0x2024:
        if (!UserList_Sw) {
            chkType = 5;
            TsSCFADE_Set(chkType, 0x14, 2);
        } else {
            chkType = 1;
            TsSCFADE_Set(chkType, 0x14, 0);
        }
        UserList_Sw = 1;
        state_tmp_270 = 0x2026;
        /* fallthrough */
    case 0x2026:
        if (TsSCFADE_Set(0, 0, 0) >= 9) {
            break;
        }
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap2, 0);
        MNScene_End(&MNS_StageMap2);
        state_tmp_270 = 0x2028;
        /* fallthrough */
    case 0x2028:
        ret = TsUserList_Flow(0, tpad, tpad2);

        if (ret != 0) {
            switch (ret) {
            case -1:
                state_tmp_270 = 0x5000;
                break;
            case 1:
                if (TsUserList_IsGetFileSave()) {
                    state_tmp_270 = 0x2830;
                } else {
                    state_tmp_270 = 0x2030;
                }
                break;
            case -3:
                if (TsUserList_IsGetFileSave()) {
                    curTag_tmp_275 = 1;
                } else {
                    curTag_tmp_275 = 0;
                }

                McInitFlow();
                state_tmp_270 = 0x2000;
                break;
            }

            McInitFlow();
            return 0;
        }

        break;
    case 0x2030:
        if (MapCHall_tmp_272.curPos == 0) {
            chkMode = 1;
        } else {
            chkMode = 2;
        }

        ret = McUserLoadFlow(TsUserList_GetCurFileNo(&bBroken), chkMode, bBroken);
        if (ret < 0) {
            return 0;
        }

        waitTime_tmp_271 = 0;
        if (ret != 0) {
            if (ret == 1) {
                state_tmp_270 = 0x2028;
            }
            if (ret == 2) {
                state_tmp_270 = 0x2050;
            }
            if (ret != 4) {
                return 0;
            }
            McInitFlow();
            state_tmp_270 = 0x2000;
            break;
        }

        TsCMPMes_SetMes(-1);
        state_tmp_270 = 0x2040;
        /* fallthrough */
    case 0x2040:
        if (++waitTime_tmp_271 >= 35) {
            waitTime_tmp_271 = 0;
            TsSet_ParappaCapColor();
#ifdef VERSION_PAL
            {
                void func_00103028(void);
                void MenuLangTim2Trans(int lang);
                func_00103028();
                MenuLangTim2Trans(game_status.language_type);
            }
#endif
            if (MapCHall_tmp_272.curPos == 2) {
                state_tmp_270 = 0xf010;
            } else {
                state_tmp_270 = 0xef00;
            }
        }
        break;
    case 0x2830:
        UserWork->fileNo = TsUserList_GetCurFileNo(NULL);
        state_tmp_270 = 0x2840;
        /* fallthrough */
    case 0x2840:
        ret = McUserSaveFlow(UserWork);
        if (ret < 0) {
            TsUserList_SetCurDispUserData(UserWork);
            return 0;
        }
        if (ret != 0) {
            if (ret == 1) {
                state_tmp_270 = 0x2020;
            }
            if (ret == 2) {
                state_tmp_270 = 0x2900;
            }
            if (ret == 4) {
                McInitFlow();
                state_tmp_270 = 0x2000;
                break;
            }
            return 0;
        }
        /* fallthrough */
    case 0x2850:
        TsSaveSuccessProc();
        waitTime_tmp_271 = 0;
        state_tmp_270 = 0x2860;
        /* fallthrough */
    case 0x2860:
        TsCMPMes_SetMes(-1);
        if (++waitTime_tmp_271 >= 35) {
            waitTime_tmp_271 = 0;
            state_tmp_270 = 0x2900;
        } else {
            break;
        }
        /* fallthrough */
    case 0x2050:
    case 0x2900:
        state_tmp_270 = 0x5000;
        break;
    case 0x3000:
        if (scstate_tmp_273 & 0xfff) {
            return 0;
        }
        OptionList_Sw = 1;
        TsOption_Flow(1, tpad);
        MNScene_End(&MNS_StageMap2);
        MNScene_Init(&MNS_StageMap2, &Scene_CityHall, 0);
        MNScene_CopyState(&MNS_StageMap2, &MNS_CityHall);
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap2, 2);
        MNScene_DispSw(&MNS_OptCounter, 1);
        state_tmp_270 = 0x3005;
        /* fallthrough */
    case 0x3005:
        if (TsSCFADE_Set(5, 0x14, 2)) {
            return 0;
        }
        MNScene_DispSw(&MNS_CityHall, 0);
        MNScene_DispSw(&MNS_StageMap2, 0);
        MNScene_End(&MNS_StageMap2);
        state_tmp_270 = 0x3010;
        /* fallthrough */
    case 0x3010:
        ret = TsOption_Flow(0, tpad);
        if (ret != 0) {
            if (ret == -1) {
                state_tmp_270 = 0x3f00;
            }
            if (ret == 1) {
                state_tmp_270 = 0x3f00;
            }
            return 0;
        }
        break;
    case 0x3f00:
        state_tmp_270 = 0x5000;
        /* fallthrough */
    case 0x5000:
        scstPos_tmp_274 = 0;
        state_tmp_270 = 0x5008;
        break;
    case 0x5008:
        if (scstate_tmp_273 & 0xfff) {
            return 0;
        }
        state_tmp_270 = 0x100;
        break;
    case 0xef00:
        TSSNDSTOP(3);
        MenuVoiceBankSet(0);
        state_tmp_270 = 0xef10;
        /* fallthrough */
    case 0xef10:
        if (TsSCFADE_Set(2, 0x14, 1)) {
            return 0;
        }
        TsUserList_Flow(2, 0, 0);
        UserList_Sw = 0;
        MNScene_DispSw(&MNS_OptCounter, 0);
        OptionList_Sw = 0;
        TSSNDSTOP(3);
        return 3;
    case 0xf000:
        TSSNDSTOP(3);
        return 1;
    case 0xf010:
        TSSNDSTOP(3);
        scstPos_tmp_274 = 0;
        state_tmp_270 = 0xf014;
        break;
    case 0xf014:
        TsCMPMes_SetMes(-1);
        if (scstate_tmp_273 & 0xfff) {
            return 0;
        }
        TsBGMStop(12);
        _MNwaitTime = 10;
        state_tmp_270 = 0xf018;
        /* fallthrough */
    case 0xf018:
        if (--_MNwaitTime <= 0) {
            TSSNDSTOP(3);
            return 2;
        }
        break;
    }

    return 0;
}

/* data 199be8 */ extern short D_00199BE8[]; /* static */
/* data 199bf0 */ extern short D_00199BF0[]; /* static */
/* data 199bf8 */ extern short D_00199BF8[]; /* static */
/* data 199c00 */ extern short D_00199C00[]; /* static */

void MpCityHallParaStart(int pos) {
    short *anmTbl = NULL;
    int    n;

    switch (pos) {
    case 1:
        anmTbl = D_00199BF0;
        break;
    case 2:
        anmTbl = D_00199C00;
        break;
    case 3:
        anmTbl = D_00199BE8;
        break;
    case 4:
        anmTbl = D_00199BF8;
        break;
    }

    if (anmTbl != NULL) {
        while ((n = *anmTbl) != -1) {
            if (n & 0x1000) {
                MNScene_ContinueAnime(&MNS_CityHall, -1, &CityHallAnime[n & ~0x1000]);
            } else {
                MNScene_StartAnime(&MNS_CityHall, -1, &CityHallAnime[n]);
            }
            anmTbl++;
        }
    }
}

static void MpCityHallFPHSSoundMask(int flg) {
    TSSNDMASK_CHAN(3, flg);
}

/* data 199c08 */ extern short D_00199C08[]; /* static */
/* data 199c10 */ extern short D_00199C10[]; /* static */
/* data 199c18 */ extern short D_00199C18[]; /* static */
/* data 199c20 */ extern short D_00199C20[]; /* static */

/* static */ int MpCityHallFPHSMove(int pos, int fpos) {
    short *anmTbl = NULL;
    int    n;

    if (pos == fpos) {
        if (!TsAnimeWait_withKeySkip(0, &MNS_CityHall, 0, 6)) {
            if (TSSND_CHANISSTOP(3)) {
                TSSNDPLAY(0x8002);
            }
        }
        return fpos;
    }

    if (TsAnimeWait_withKeySkip(0, &MNS_CityHall, 2, 6)) {
        return fpos;
    }

    if (pos > 0 && fpos > 0) {
        pos = 0;
    }

    switch (fpos) {
    case 0:
        switch (pos) {
        case 1:
            anmTbl = D_00199C10;
            break;
        case 2:
            anmTbl = D_00199C20;
            break;
        }
        break;
    case 1:
        if (pos == 0) {
            anmTbl = D_00199C08;
        }
        break;
    case 2:
        if (pos == 0) {
            anmTbl = D_00199C18;
        }
        break;
    }

    if (anmTbl == NULL) {
        return fpos;
    }

    TSSNDPLAY(53);

    while ((n = *anmTbl) != -1) {
        if (n & 0x1000) {
            MNScene_ContinueAnime(&MNS_CityHall, -1, &CityHallAnime[n & ~0x1000]);
        } else {
            MNScene_StartAnime(&MNS_CityHall, -1, &CityHallAnime[n]);
        }
        anmTbl++;
    }

    return pos;
}

static void MpCityHallFPHOK(int flg) {
    int n;

    TSSNDMASK_CHAN(3, flg);

    switch (flg) {
    case 0:
        n = 0x10;
        break;
    case 1:
        n = 0x11;
        break;
    case 2:
    default:
        n = 0x12;
        break;
    }

    if (!TsAnimeWait_withKeySkip(0, &MNS_CityHall, 1, 6)) {
        MNScene_ContinueAnime(&MNS_CityHall, 1, NULL);
        MNScene_StartAnime(&MNS_CityHall, -1, &CityHallAnime[n]);
    } else {
        MNScene_ContinueAnime(&MNS_CityHall, -1, &CityHallAnime[n]);
    }
}

/* data 199c28 */ extern short D_00199C28[];
/* data 199c38 */ extern short D_00199C38[];
/* data 199c48 */ extern short D_00199C48[];

static void MpCityHallCharPosSet(int pos) {
    short *anmTbl = NULL;
    int    n;
    u_int  bnk;

    switch (pos) {
    case 0:
        anmTbl = D_00199C28;
        break;
    case 1:
        anmTbl = D_00199C38;
        break;
    case 2:
        anmTbl = D_00199C48;
        break;
    }

    bnk = 0x80000000;
    while ((n = *anmTbl) != -1) {
        bnk |= MNScene_StartAnime(&MNS_CityHall, -1, &CityHallAnime[n & ~0x1000]);
        anmTbl++;
    }

    MNScene_SetAnimeBankEnd(&MNS_CityHall, bnk);
    TSSNDPLAY(0x8002);
}

static int MpPopMenu_Flow(int flg, u_int tpad) {
    /* sbss 399afc */ extern int state_tmp_296;
    int ret;
    int mpsize;

    if (flg == 1) {
        state_tmp_296 = 0;
        return 0;
    }

    switch (state_tmp_296) {
    case 0:
        state_tmp_296 = 0x1000;
        break;
    case 0x1000:
        TsMENU_GetMapNo(&mpsize);
        TsPopMenu_Flow(1, mpsize);
        state_tmp_296 = 0x1010;
    /* fallthrough */
    case 0x1010:
        ret = TsPopMenu_Flow(0, tpad);
        if (ret == 0) {
            break;
        }

        TsPopMenu_Flow(2, 0);

        if (ret == -1) {
            state_tmp_296 = 0xf000;
            break;
        }

        if (ret < 0) {
            state_tmp_296 = 0xf000;
            break;
        }

        return ret;
    case 0xf000:
        return -1;
    }

    return 0;
}

/* static */ int MpMapMenu_Flow(int flg, MAPPOS *mpw, u_int tpad) {
    int       state;
    int       posNo;
    int       dir;
    int       anmNo;
    MNMAPPOS *mpos;

    if (flg == 1) {
        mpw->state = 0;
        mpw->anmStop = 0;
        mpw->bMove = 0;
        mpw->curPos = 0;
        mpw->mvFlag = 0;
        MENUSubt_PadFontArrowSet(0);
        return 0;
    }

    if (flg == 3) {
        mpw->curPos = tpad;
        if (mpw->mnmap != NULL) {
            mpos = &mpw->mnmap[tpad];
            mpw->mvFlag = _MapGetMovableDir(mpw);
            MENUSubt_PadFontArrowSet(mpw->mvFlag);
            TsCMPMes_SetMes(mpos->cmpmes);
            if (mpw->pscene != NULL) {
                if (mpw->panime != NULL) {
                    anmNo = mpos->posanm0;
                    mpw->anmBit = 0x80000000;
                    if (anmNo != -1) {
                        mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                    }
                    anmNo = mpos->posanm1;
                    if (anmNo != -1) {
                        mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                    }
                    MNScene_SetAnimeEnd(mpw->pscene);
                }
            }
        }
        mpw->anmtrg = 0;
        return 0;
    }

    if (flg == 4) {
        anmNo = mpw->curPos;
        if (mpw->mnmap == NULL) {
            return 0;
        }
        mpos = &mpw->mnmap[anmNo];
        for (dir = 0; dir < 4; dir++) {
            if (tpad == mpos->mapdir[dir].mapNo) {
                break;
            }
        }
        if (dir >= 4) {
            return 0;
        }
        posNo = mpos->mapdir[dir].mapNo & ~0x8000;
        if (mpw->lmtPos != 0) {
            if (posNo >= mpw->lmtPos) {
                return 0;
            }
            if (mpos->mapdir[dir].mapNo & 0x8000) {
                if (anmNo == (mpw->lmtPos - 1) || posNo == (mpw->lmtPos - 1)) {
                    return 0;
                }
            }
        }
        mpw->curPos = posNo;
        mpw->anmLtim = mpos->mapdir[dir].anmLtim;
        if (mpw->pscene != NULL && mpw->panime != NULL) {
            anmNo = mpos->movanm1;
            mpw->bMove = 1;
            mpw->anmBit = 0x80000000;
            if (anmNo != -1) {
                mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
            }
            anmNo = mpos->mapdir[dir].anmNo;
            if (anmNo != -1) {
                mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
            }
        }
        mpw->anmtrg = mpos->mapdir[dir].exflg;
        mpw->sndtrg = 1;
        mpw->state = 0x1100;
        return 0;
    }

    if (mpw->mnmap == NULL) {
        return 0;
    }

    state = mpw->state;
    posNo = mpw->curPos;
    mpw->anmtrg = 0;
    mpw->sndtrg = 0;
    mpw->anmStop = 0;

    switch (state) {
    case 0:
        mpw->bMove = 0;
        state = 0x1000;
        /* fallthrough */
    case 0x1000:
        mpos = &mpw->mnmap[posNo];
        if (mpw->bMove != 0) {
            if (!TsAnimeWait_withKeySkip(tpad, mpw->pscene, 0, mpw->anmBit)) {
                mpw->bMove = 0;
                if (mpw->pscene != NULL && mpw->panime != NULL) {
                    anmNo = mpos->posanm0;
                    mpw->anmBit = 0x80000000;
                    if (anmNo != -1) {
                        mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                    }
                    anmNo = mpos->posanm1;
                    if (anmNo != -1) {
                        mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                    }
                    MNScene_SetAnimeBankEnd(mpw->pscene, mpw->anmBit);
                }
            }
        }

        dir = -1;
        mpw->mvFlag = _MapGetMovableDir(mpw);
        MENUSubt_PadFontArrowSet(mpw->mvFlag);
        TsCMPMes_SetMes(mpos->cmpmes);

        if (tpad & 0x40) {
            mpw->sndtrg = 2;
            state = 0x1200;
        } else if (tpad & 0x10) {
            mpw->sndtrg = 3;
            state = 0x1f00;
        }

        if (tpad & 0x8000) {
            dir = 0;
        } else if (tpad & 0x2000) {
            dir = 1;
        } else if (tpad & 0x1000) {
            dir = 2;
        } else if (tpad & 0x4000) {
            dir = 3;
        }

        if (dir >= 0) {
            if ((mpw->mvFlag >> dir) & 1) {
                posNo = mpos->mapdir[dir].mapNo;
                if (posNo != -1) {
                    posNo &= ~0x8000;
                    mpw->curPos = posNo;
                    mpw->anmLtim = mpos->mapdir[dir].anmLtim;
                    if (mpw->pscene != NULL && mpw->panime != NULL) {
                        anmNo = mpos->movanm1;
                        mpw->bMove = 1;
                        mpw->anmBit = 0x80000000;
                        if (anmNo != -1) {
                            mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                        }
                        anmNo = mpos->mapdir[dir].anmNo;
                        if (anmNo != -1) {
                            mpw->anmBit |= MNScene_StartAnime(mpw->pscene, -1, &mpw->panime[anmNo]);
                        }
                    }
                    mpw->anmtrg = mpos->mapdir[dir].exflg;
                    mpw->sndtrg = 1;
                    state = 0x1100;
                }
            }
        }
        break;
    case 0x1100:
        if (!TsAnimeWait_withKeySkip(tpad, mpw->pscene, mpw->anmLtim, mpw->anmBit)) {
            mpw->anmStop = 1;
            state = 0x1000;
        }
        break;
    case 0x1200:
        state = 0x2000;
        break;
    case 0x2000:
    case 0xf000:
        mpw->state = 0;
        MENUSubt_PadFontArrowSet(0);
        return 1;
    case 0x1f00:
    case 0xf010:
        mpw->state = 0;
        MENUSubt_PadFontArrowSet(0);
        return -1;
    }

    mpw->state = state;
    return 0;
}

static int _MapGetMovableDir(MAPPOS *mpw) {
    int       posNo, ret, DirMask;
    MNMAPPOS *mpos;
    int       flg, i;

    DirMask = 1;
    mpos = &mpw->mnmap[mpw->curPos];

    ret = 0;

    for (i = 0; i < PR_ARRAYSIZE(mpos->mapdir); i++, DirMask <<= 1) {
        flg = TRUE;
        posNo = mpos->mapdir[i].mapNo;

        if (posNo == -1) {
            flg = FALSE;
        } else {
            posNo &= ~0x8000;
            if (mpw->lmtPos != 0) {
                if (posNo >= mpw->lmtPos) {
                    flg = FALSE;
                } else {
                    if (mpos->mapdir[i].mapNo & 0x8000) {
                        if (mpw->curPos == (mpw->lmtPos - 1) || posNo == (mpw->lmtPos - 1)) {
                            flg = FALSE;
                        }
                    }
                }
            }
        }

        if (flg) {
            ret |= DirMask;
        }
    }

    return ret;
}

int McErrorMess(int err) {
    int mes;

    if (!TsMCAMes_IsON()) {
        switch (err) {
        case 1:
            mes = 0x2000014;
            break;
        case 2:
            mes = 0x2000008;
            break;
        case 3:
            mes = 0x2000002;
            break;
        case 4:
            mes = 0x2000005;
            break;
        case 5:
            mes = 0x2000014;
            break;
        case 6:
            mes = 0x2000009;
            break;
        case 7:
            mes = 0x200000d;
            break;
        case 15:
            mes = 0x200000e;
            break;
        case 10:
            mes = 0x2000012;
            break;
        case 12:
            mes = 0x2000004;
            break;
        case 40:
            mes = 0x2000006;
            break;
        case 50:
            mes = 0x200000b;
            break;
        case 60:
            mes = 0x200000c;
            break;
        case 70:
            mes = 0xc000018;
            break;
        case 80:
            mes = 0xc000017;
            break;
        case 100:
            mes = 0xc00000a;
            break;
        case 200:
            mes = 0xc000015;
            break;
        default:
            return 0;
        }

        TsMCAMes_SetMes(mes);
    }

    if (TsMCAMes_GetSelect() > 0) {
        TsMCAMes_SetMes(-1);
        return 0;
    }

    return -1;
}

static void McInitFlow(void) {
    subStatus = 0;
    ret       = 0;
    errorNo   = 0;
    waitTime  = 0;
}

static int McStartCheckFlow(/* a0 4 */ int flg) {
    /* v1 3 */ int ret;

    if (flg == 1) {
        McInitFlow();
        P3MC_OpeningCheckStart();
        return 0;
    }

    if (flg == 2) {
        P3MC_OpeningCheckEnd();
        return 0;
    }

    switch (subStatus) {
    case 0:
        subStatus = 1;
        break;
    case 1:
        ret = P3MC_OpeningCheck();
        if (ret >= 0) {
            if (ret & 1) {
                ret = 0;
            } else {
                ret = 1;
            }
        }

        switch (ret) {
        case -2:
            subStatus = 0x100;
            break;
        case 0:
            subStatus = 0x100;
            break;
        case 1:
            subStatus = 0x10;
            break;
        case -3:
            subStatus = 0x20;
            break;
        }
    
        break;
    case 0x10:
        ret = P3MC_OpeningCheck();
        if (ret >= 0) {
            ret = (ret & 0x1) ^ 0x1;
        }

        switch (ret) {
        case -3:
            subStatus = 0x20;
            break;
        case -2:
        case 0:
            subStatus = 0x100;
            break;
        case -1:
        case 1:
        default:
            break;
        }

        break;
    case 0x20:
        ret = P3MC_OpeningCheck();
        if (ret >= 0) {
            ret = (ret & 0x1) ^ 0x1;
        }

        switch (ret) {
        case -2:
        case 0:
            subStatus = 0x100;
            break;
        case 1:
            subStatus = 0x10;
            break;
        case -3:
        case -1:
        default:
            break;
        }

        break;
    case 0x100:
        return 0;
    }

    switch (subStatus) {
    case 0x10:
        return 1;
    case 0x20:
        return 2;
    }

    return -1;
}

/* static */ int McUserCheckFlow(int type, int mode, int *bError) {
    extern int D_003834A0;
    int r;

    switch (subStatus) {
    case 0:
        UCheckSaveError = 0;
        D_003834A0 = -2;
        UCheckLoadError = 0;
        if (type == 0) {
            r = P3MC_GetUserStart(mode, UserLst, 0);
        } else {
            r = P3MC_GetUserStart(mode, UserLst, 1);
        }
        if (r != 0) {
            subStatus = 0x100;
        } else {
            subStatus = 0xf000;
        }
        break;
    case 0x100:
        D_003834A0 = -2;
        ret = P3MC_GetUserCheck();
        errorNo = 0;
        if (ret < 0) {
            if (ret == -1) {
                D_003834A0 = -1;
                subStatus = 0x102;
            }
            break;
        }
        errorNo = ret;
        subStatus = 0x150;
        break;
    case 0x102:
        D_003834A0 = -1;
        waitTime = 90;
        TsMCAMes_SetMes(3);
        if (errorNo == 0) {
            subStatus = 0x103;
        } else {
            subStatus = 0x104;
        }
        break;
    case 0x103:
        D_003834A0 = -1;
        waitTime--;
        errorNo = P3MC_GetUserCheck();
        if (errorNo < 0) {
            break;
        }
        subStatus = 0x104;
        break;
    case 0x104:
        D_003834A0 = -1;
        if (--waitTime > 0) {
            break;
        }
        if (type == 0 && errorNo == 0) {
            if (P3MC_CheckBrokenUser(UserLst, mode) != 0) {
                errorNo = 80;
            }
        }
        subStatus = 0x150;
        break;
    case 0x150:
        UCheckSaveError = 0;
        UCheckLoadError = 0;
        if ((type & 2) || type == 0) {
            if (errorNo == 2) {
                errorNo = 0;
                UCheckLoadError = 4;
            }
            if (type == 0 && (u_int)(errorNo - 3) < 3) {
                memset(UserLst, 0, sizeof(P3MC_USRLST));
                errorNo = 0;
            }
        }
        if ((type & 2) && errorNo == 4) {
            errorNo = 0;
            UCheckLoadError = 4;
            if (P3MC_CheckIsNewSave(mode) == 0) {
                if (mode == 2) {
                    UCheckSaveError = 15;
                } else {
                    UCheckSaveError = 7;
                }
            }
        }
        if (errorNo == 0) {
            if (type == 2) {
                errorNo = UCheckSaveError;
            }
            if (type == 1) {
                errorNo = UCheckLoadError;
            }
            if (type == 3) {
                UCheckSaveError = 0;
            }
            if (errorNo == 0) {
                subStatus = 0xf000;
                break;
            }
        }
        if (errorNo == 1) {
            if (type == 0) {
                errorNo = 70;
            } else {
                errorNo = 12;
            }
        }
        if (errorNo == 3) {
            subStatus = 0xe000;
        } else {
            subStatus = 0x160;
        }
        if (errorNo == 4 && mode == 2) {
            errorNo = 40;
        }
        break;
    case 0x160:
        TsMCAMes_SetMes(-1);
        if (D_003834A0 == -1) {
            if (bError != NULL) {
                *bError = 1;
            }
        }
        subStatus = 0x165;
        /* fallthrough */
    case 0x165:
        ret = P3MC_CheckChange();
        if (ret == 3 || ret == 5) {
            subStatus = 0xe000;
            break;
        }
        if (McErrorMess(errorNo) < 0) {
            break;
        }
        subStatus = 0x166;
        /* fallthrough */
    case 0x166:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        subStatus = 0xf0f0;
        break;
    case 0xe000:
        ret = P3MC_CheckChange();
        if (ret == 0 || ret == 5) {
            if (D_003834A0 == -1) {
                if (bError != NULL) {
                    *bError = 2;
                }
            }
            subStatus = 0;
            break;
        }
        if (errorNo == 3 && type == 2) {
            if (mode == 2) {
                errorNo = 60;
            } else {
                errorNo = 50;
            }
        }
        if (McErrorMess(errorNo) >= 0) {
            subStatus = 0xee10;
        }
        break;
    case 0xee10:
        subStatus = 0xf0f0;
        break;
    case 0xf0f0:
        if (type == 0) {
            if (errorNo == 70 || errorNo == 80) {
                if (errorNo == 70) {
                    memset(UserLst, 0, sizeof(P3MC_USRLST));
                }
                subStatus = 0xf000;
                break;
            }
        }
        subStatus = 0xf002;
        break;
    case 0xf000:
        P3MC_GetUserEnd();
        TsMCAMes_SetMes(-1);
        return 0;
    case 0xf001:
        P3MC_GetUserEnd();
        TsMCAMes_SetMes(-1);
        return 1;
    case 0xf002:
        P3MC_GetUserEnd();
        TsMCAMes_SetMes(-1);
        return 2;
    }

    return D_003834A0;
}

extern MCRWDATA_HDL *pGameData;

/* static */ int McUserSaveFlow(USER_DATA *puser) {
    switch (subStatus) {
    case 0:
        ret = P3MC_CheckChange();
        if (ret < 0) {
            break;
        }
        pGameData = P3MC_MakeDataWork(_P3DATA_SIZE(puser->mode), puser);
        TsSetSaveData(pGameData, puser->mode, puser);
        if (ret != 0) {
            subStatus = 0x2010;
            break;
        }
        subStatus = 0x100;
        /* fallthrough */
    case 0x100:
        P3MC_SaveUser(pGameData, 5);
        subStatus = 0x2000;
        break;
    case 0x1000:
        TsMCAMes_SetMes(0x10000010);
        subStatus = 0x1010;
        /* fallthrough */
    case 0x1010:
        ret = P3MC_CheckChange();
        if (ret == 3 || ret == 5) {
            TsMCAMes_SetMes(-1);
            subStatus = 0xe000;
            break;
        }
        ret = TsMCAMes_GetSelect();
        if (ret != 0) {
            if (ret == 1) {
                subStatus = 0x1020;
            } else {
                subStatus = 0xf002;
            }
        }
        break;
    case 0x1020:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        TsMCAMes_SetMes(-1);
        subStatus = 0x1030;
        /* fallthrough */
    case 0x1030:
        P3MC_SaveUser(pGameData, 3);
        subStatus = 0x2000;
        break;
    case 0x2000:
        ret = P3MC_SaveCheck();
        if (ret == -2) {
            TsMCAMes_SetMes(19);
        }
        if (ret == -3) {
            TsMCAMes_SetMes(17);
        }
        if (ret < 0) {
            break;
        }
        subStatus = 0x2010;
        /* fallthrough */
    case 0x2010:
        if (ret != 0) {
            errorNo = 0;
            switch (ret) {
            case 9:
                subStatus = 0x1000;
                break;
            case 3:
                if (puser->mode == 2) {
                    errorNo = 60;
                } else {
                    errorNo = 50;
                }
                break;
            case 1:
            case 8:
                errorNo = 1;
                break;
            case 7:
                if (puser->mode == 2) {
                    errorNo = 15;
                } else {
                    errorNo = 7;
                }
                break;
            default:
                errorNo = ret;
                break;
            }
            if (errorNo != 0) {
                subStatus = 0x2200;
            }
            break;
        }
        subStatus = 0x2100;
        /* fallthrough */
    case 0x2100:
        *puser = pGameData->pHead->user;
        subStatus = 0x21f0;
        TsMCAMes_SetMes(-1);
        /* fallthrough */
    case 0x21f0:
        if (McErrorMess(200) < 0) {
            break;
        }
        subStatus = 0xf000;
        break;
    case 0x2200:
        TsMCAMes_SetMes(-1);
        subStatus = 0x2201;
        /* fallthrough */
    case 0x2201:
        ret = P3MC_CheckChange();
        if (ret == 3 || ret == 5) {
            subStatus = 0xe000;
            break;
        }
        if (McErrorMess(errorNo) < 0) {
            break;
        }
        subStatus = 0x2202;
        /* fallthrough */
    case 0x2202:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        subStatus = 0xf002;
        break;
    case 0xe000:
        ret = P3MC_CheckChange();
        if (ret == 0 || ret == 5) {
            subStatus = 0xf004;
            break;
        }
        if (puser->mode == 2) {
            errorNo = 60;
        } else {
            errorNo = 50;
        }
        if (McErrorMess(errorNo) >= 0) {
            subStatus = 0xee10;
        }
        break;
    case 0xee10:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        subStatus = 0xf002;
        break;
    case 0xf000:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 0;
    case 0xf001:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 1;
    case 0xf002:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 2;
    case 0xf004:
        P3MC_DeleteDataWork(pGameData);
        return 4;
    }

    return -1;
}

/* static */ int McUserLoadFlow(int fileNo, int mode, int bBroken) {
    int r;

    switch (subStatus) {
    case 0:
        r = P3MC_CheckChange();
        ret = r;
        if (r < 0) {
            break;
        }
        pGameData = P3MC_MakeDataWork(_P3DATA_SIZE(mode), NULL);
        if (ret != 0) {
            subStatus = 0x2010;
            break;
        }
        if (bBroken != 0) {
            ret = 6;
            subStatus = 0x2010;
            break;
        }
        subStatus = 0x100;
        /* fallthrough */
    case 0x100:
        waitTime = 90;
        TsMCAMes_SetMes(7);
        P3MC_LoadUser(mode, fileNo, pGameData, 0);
        subStatus = 0x2000;
        break;
    case 0x2000:
        waitTime--;
        r = P3MC_LoadCheck();
        ret = r;
        if (r < 0) {
            break;
        }
        subStatus = 0x2002;
        break;
    case 0x2002:
        if (--waitTime > 0) {
            break;
        }
        subStatus = 0x2010;
        /* fallthrough */
    case 0x2010:
        if (ret != 0) {
            errorNo = 0;
            switch (ret) {
            case 1:
            case 2:
            case 4:
                errorNo = 2;
                break;
            default:
                errorNo = ret;
                break;
            }
            if (errorNo != 0) {
                subStatus = 0x2200;
            }
            break;
        }
        subStatus = 0x2100;
        /* fallthrough */
    case 0x2100:
        TsRestoreSaveData(pGameData, mode);
        subStatus = 0x2f00;
        TsMCAMes_SetMes(-1);
        /* fallthrough */
    case 0x2f00:
        if (McErrorMess(100) < 0) {
            break;
        }
        subStatus = 0xf000;
        break;
    case 0x2200:
        TsMCAMes_SetMes(-1);
        subStatus = 0x2201;
        /* fallthrough */
    case 0x2201:
        ret = P3MC_CheckChange();
        if (ret == 3 || ret == 5) {
            subStatus = 0xe000;
            break;
        }
        if (McErrorMess(errorNo) < 0) {
            break;
        }
        subStatus = 0x2202;
        /* fallthrough */
    case 0x2202:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        if (errorNo == 6) {
            if (bBroken != 0) {
                subStatus = 0xf001;
            } else {
                subStatus = 0xf002;
            }
        } else {
            subStatus = 0xf002;
        }
        break;
    case 0xe000:
        ret = P3MC_CheckChange();
        if (ret == 0 || ret == 5) {
            subStatus = 0xf004;
            break;
        }
        if (McErrorMess(errorNo) >= 0) {
            subStatus = 0xee10;
        }
        break;
    case 0xee10:
        if (P3MC_CheckChange() < 0) {
            break;
        }
        TsMCAMes_SetMes(-1);
        if (errorNo == 6) {
            if (bBroken != 0) {
                subStatus = 0xf001;
            } else {
                subStatus = 0xf002;
            }
        } else {
            subStatus = 0xf002;
        }
        break;
    case 0xf000:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 0;
    case 0xf001:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 1;
    case 0xf002:
        P3MC_DeleteDataWork(pGameData);
        TsMCAMes_SetMes(-1);
        return 2;
    case 0xf004:
        P3MC_DeleteDataWork(pGameData);
        return 4;
    }

    return -1;
}

static void TsMCAMes_Init(void) {
    memset(&MCMesWork, 0, sizeof(MCMesWork));
    MCMesWork.mesflg = -1;
}

static int TsMCAMes_GetSelect(void) {
    MCMES_WORK *pmesw = &MCMesWork;

    if (pmesw->mesflg < 0) {
        return 1;
    } else if (pmesw->mesflg & 0xff000000) {
        return pmesw->selflg;
    }

    return 1;
}

static int TsMCAMes_IsON(void) {
    MCMES_WORK *pmesw = &MCMesWork;
    return (u_int)~pmesw->mesflg >> 0x1f;
}

void TsMCAMes_SetPos(int x, int y) {
    MCMES_WORK *pmesw = &MCMesWork;

    pmesw->px = x;
    pmesw->py = y;
}

void TsMCAMes_SetMes(int no) {
    MCMES_WORK *pmesw;
    int         i;

    pmesw = &MCMesWork;

    if (no < 0) {
        pmesw->mesflg = -1;
        pmesw->selflg = 0;
        pmesw->seltim = 0;
        pmesw->line = 0;
        pmesw->btflg = 0;
        pmesw->backSw = 0;
        pmesw->faceNo = 0;
        return;
    }

    if (pmesw->mesflg != no) {
        if (TSSND_CHANISSTOP(2)) {
            for (i = 0; i < PR_ARRAYSIZEU(McVoiceTbl); i++) {
                if (McVoiceTbl[i].mesNo == no) {
                    TSSNDPLAY(McVoiceTbl[i].dataNo);
                    break;
                }
            }
        }
    }

    pmesw->mesflg = no;
    pmesw->faceNo = 0;

    for (i = 0; i < PR_ARRAYSIZEU(McFaceTbl); i++) {
        if (McFaceTbl[i].mesNo == no) {
            pmesw->faceNo = McFaceTbl[i].dataNo;
            break;
        }
    }

    if (no & 0x10000) {
        pmesw->backSw = 0;
        pmesw->btflg = 0;
    } else {
        pmesw->backSw = 1;
        pmesw->btflg = 1;
    }

    if (no & 0x20000) {
        pmesw->color = 1;
    } else {
        pmesw->color = 0;
    }

    pmesw->selflg = 0;
    pmesw->seltim = 0;
    pmesw->line = _PkMCMsgGetLine(pmesw->mesflg & 0xffff);
}

static void TsMCAMes_Flow(u_int tpad) {
    MCMES_WORK *pmesw;
    int         cLine;
    float       fRate;
    int         isOK, isCAN;

    pmesw = &MCMesWork;
    if (pmesw->backSw && pmesw->mesflg >= 0) {
        cLine = pmesw->line;
        if (cLine == 1) {
            cLine = 2;
        }

        cLine *= 4096;
        if (pmesw->D0line == 0) {
            pmesw->Dline = pmesw->D0line = cLine;
        } else {
            pmesw->Dline = pmesw->D0line = TSNumMov(pmesw->D0line, cLine, 4);
        }

        fRate = (MNSceneGetMusicFitTimer() % 72) / 72.0f;
        fRate *= 6.2831855f;
        fRate = cosf(fRate);
        fRate = ((fRate * fRate) * 1228.8f);
        pmesw->Dline += (int)fRate;
    } else {
        pmesw->D0line = 0;
        pmesw->Dline = 0;
    }

    if (pmesw->btflg != 0) {
        if (pmesw->btton < 0x40) {
            pmesw->btton += 7;
            if (pmesw->btton > 0x40) {
                pmesw->btton = 0x40;
            }
        }
    } else {
        if (pmesw->btton > 0) {
            pmesw->btton -= 7;
            if (pmesw->btton < 0) {
                pmesw->btton = 0;
            }
        }
    }

    if (pmesw->mesflg >= 0 && (pmesw->mesflg & 0xff000000)) {
        if (pmesw->mesflg & 0xc000000) {
            if (!TSSND_CHANISSTOP(1)) {
                pmesw->seltim++;
                return;
            }
        }

        if (pmesw->mesflg & 0x2000000) {
            isCAN = TRUE;
        } else {
            isCAN = FALSE;
        }
    
        if (pmesw->mesflg & 0x1000000) {
            isOK = TRUE;
        } else {
            isOK = FALSE;
        }
        
        if (pmesw->mesflg & 0x14000000) {
            isOK = TRUE;
            isCAN = TRUE;
        }
    
        if (isOK && (tpad & 0x40)) {
            pmesw->selflg = 1;
            TSSNDPLAY(6);
        }
    
        if (isCAN && (tpad & 0x10)) {
            pmesw->selflg = 2;
            if (pmesw->mesflg & 0x4000000) {
                TSSNDPLAY(6);
            } else {
                TSSNDPLAY(9);
            }
        }
    
        if (pmesw->mesflg & 0xc000000) {
            pmesw->seltim++;
            if (pmesw->seltim >= 0x79) {
                pmesw->selflg = 1;
            }
        }
    }
}

extern PATPOS PAT_ALERT_WIN_ABOVE;
extern PATPOS PAT_ALERT_WIN_CENTER;
extern PATPOS PAT_ALERT_WIN_BELOW;
extern PATPOS PAT_ALERT_WIN_FFACE[];

/* static */ void TsMCAMes_Draw(SPR_PKT pk, SPR_PRM *spr) {
    MCMES_WORK *pmesw = &MCMesWork;
    int         px, py;
    int         x, y;
    float       fLine, fH;
    float       oy0;
    u_int       col;

    if (pmesw->btton != 0) {
        spr->rgba0 = pmesw->btton << 24;
        spr->zy = 1.0f;
        spr->zx = 1.0f;
        SetSprScreenXYWH(spr);
        PkCRect_Add(pk, spr, 0);
    }

    if (pmesw->mesflg < 0) {
        return;
    }

    px = pmesw->px;
    py = pmesw->py;

    if (pmesw->Dline <= 4096) {
        fH = 0.0f;
        fLine = 0.0f;
    } else {
        fLine = (float)(pmesw->Dline - 4096) / 4096.0f;
        fH = (fLine * 12.0f) - 2.0f;
    }

    if (pmesw->backSw != 0) {
        x = px - 292;
        y = py + 5;
        oy0 = spr->ofsy;
        spr->zx = 1.0f;
        spr->zy = 0.5f;
        spr->rgba0 = 0x80808080;
        spr->ofsy = oy0 - ((fH * 0.5f) + 36.0f);

        TsPatPut(pk, spr, &PAT_ALERT_WIN_ABOVE, x, y);
        spr->ofsy += 35.5f;

        if (fH > 0.0f) {
            spr->zy = fLine * 0.5f;
            TsPatPut(pk, spr, &PAT_ALERT_WIN_CENTER, x, y);
            spr->ofsy += fH - 1.0f;
        }

        spr->zy = 0.5f;
        TsPatPut(pk, spr, &PAT_ALERT_WIN_BELOW, x, py + 1);

        if (pmesw->faceNo > 0) {
            TsPatPut(pk, spr, &PAT_ALERT_WIN_FFACE[pmesw->faceNo - 1], px + 204, y);
        }

        spr->ofsy = oy0;
    }

    if (pmesw->mesflg >= 0) {
        y = py - ((pmesw->line * 13) >> 1);
        col = 0x80220061;
        switch (pmesw->color) {
        case 0:
            break;
        case 1:
            col = 0x807f7f7f;
            break;
        }
        _PkMCMsgPut(pk, spr, pmesw->mesflg & 0xffff, px, y, col);
    }
}

void TsCMPMes_SetPos(int x, int y) {
    CMPMES_WORK *pmesw = &CmpMesWork;
    pmesw->px = x;
    pmesw->py = y;
}

void TsCMPMes_SetMes(int no) {
    CMPMES_WORK *pmesw = &CmpMesWork;

    if (no < 0) {
        pmesw->mesflg = -1;
        pmesw->backSw = 0;

        pmesw->wh = 0;
        pmesw->ww = 0;
    } else {
        pmesw->mesflg = no;
        pmesw->backSw = 1;

        pmesw->ww = 600;
        pmesw->wh = 26;
    }
}

static void TsCMPMes_Draw(SPR_PKT pk, SPR_PRM *spr) {
    CMPMES_WORK *pmesw = &CmpMesWork;

    if (pmesw->backSw != 0) {
        MNScene_DispSw(&MNS_JimakuBak, 1);
        MNScene_Draw(&MNS_JimakuBak);
    } else {
        MNScene_DispSw(&MNS_JimakuBak, 0);
    }

    if (pmesw->mesflg < 0x1000u) {
        _PkSubMsgPut(pk, spr, pmesw->mesflg & 0xffff, pmesw->px, pmesw->py, 0x807f7f7f);
    }
}

void TsANIME_Init(ANIME_WK *wk) {
    memset(wk, 0, sizeof(*wk));
}

static int TsANIME_Poll(ANIME_WK *wk) {
    if (wk->atim != 0) {
        wk->atim--;
    }

    if (wk->aflg == 0) {
        return wk->atim;
    }

    if (!(wk->aflg & 0x1)) {
        wk->atrn = TSNumMov(wk->atrn, 0, 3);
        wk->atrn2 = TSNumMov(wk->atrn2, 0, 4);
        wk->atrn3 = TSNumMov(wk->atrn3, 0, 6);
    } else {
        wk->atrn = TSNumMov(wk->atrn, 0, 4);
        wk->atrn2 = TSNumMov(wk->atrn2, 0, 3);
        wk->atrn3 = TSNumMov(wk->atrn3, 0, 6);
    }

    if (wk->atrn == 0 && wk->atrn2 == 0 && wk->atrn3 == 0) {
        wk->aflg = 0;
    }

    return wk->atim;
}

static void TsANIME_Start(ANIME_WK *wk, int state, int tim) {
    wk->atrn2 = 0x200;
    wk->atrn  = 0x200;
    wk->atrn3 = 0x200;

    wk->atim = tim;
    wk->aflg = state;
}

static int TsANIME_GetRate(ANIME_WK *wk, float *rt0, float *rt1, float *rt2) {
    float p0, p1, p2;

    if (wk->aflg) {
        p0 = wk->atrn  * 0.001953125f;
        p1 = wk->atrn2 * 0.001953125f;
        p2 = wk->atrn3 * 0.001953125f;

        if (!(wk->aflg & 1)) {
            p0 = 1.0f - p0;
            p1 = 1.0f - p1;
        } else {
            p2 = 1.0f - p2;
        }

        if (rt0 != NULL) {
            *rt0 = p0;
        }
        if (rt1 != NULL) {
            *rt1 = p1;
        }
        if (rt2 != NULL) {
            *rt2 = p2;
        }

        return 1;
    } else {
        if (rt0 != NULL) {
            *rt0 = 1.0f;
        }
        if (rt1 != NULL) {
            *rt1 = 1.0f;
        }
        if (rt2 != NULL) {
            *rt2 = 1.0f;
        }

        return 0;
    }
}

/* static */ void _TsSortSetRanking(P3MC_RANKSCORE **ptRank, int n, P3MC_RANKSCORE *pRank, int bNameCmp) {
    P3MC_RANKSCORE *pcur;
    int             i, j, k, l, flg;

    for (i = 0; i < n; i++, pRank++) {
        for (j = 0; j < 20; j++) {
            pcur = ptRank[j];
            if (pcur == NULL || pcur->score < pRank->score) {
                for (l = 19; j < l; l--) {
                    ptRank[l] = ptRank[l - 1];
                }
                ptRank[j] = pRank;
                break;
            }

            if (pcur->score == pRank->score && pcur->scDate[0] == pRank->scDate[0] && pcur->scDate[1] == pRank->scDate[1]) {
                flg = 1;
                if (bNameCmp) {
                    for (k = 0; k < 8; k++) {
                        if (ptRank[j]->name[k] != pRank->name[k]) {
                            flg = 0;
                            break;
                        }
                        if (ptRank[j]->name[k] == 0) {
                            break;
                        }
                    }
                }
                if (flg) {
                    break;
                }
            }
        }
    }
}

extern u_char *UserName_RankingNoSave;
extern RANKLIST RankLst[20];

/* static */ RANKLIST* TsGetRankingList(int flag, int vsLev, int stageNo, int *nrank) {
    P3MC_RANKSCORE *ptRank[20];
    int             i;
    int             n;
    int             cnt;
    USER_DATA      *puser;
    int             nRank;
    P3MC_RANKSCORE *pRank;

    for (i = 19; i >= 0; i--) {
        ptRank[i] = NULL;
    }

    if (flag == 0) {
        nRank = pCStageRank[stageNo].nSplay;
        pRank = pCStageRank[stageNo].splay;
    } else {
        nRank = pCStageRank[stageNo].nVplay[vsLev];
        pRank = pCStageRank[stageNo].vplay[vsLev];
    }
    _TsSortSetRanking(ptRank, nRank, pRank, 1);

    n = P3MC_SortUser(UserLst, 1, 0);
    for (i = 0; i < n; i++) {
        puser = UserLst->pUserTbl[i];
        if (puser->flg == 1) {
            int nRank2;
            P3MC_RANKSCORE *pRank2;
            if (flag == 0) {
                nRank2 = puser->stageRank[stageNo].nSplay;
                pRank2 = puser->stageRank[stageNo].splay;
            } else {
                nRank2 = puser->stageRank[stageNo].nVplay[vsLev];
                pRank2 = puser->stageRank[stageNo].vplay[vsLev];
            }
            _TsSortSetRanking(ptRank, nRank2, pRank2, 1);
        }
    }

    n = P3MC_SortUser(UserLst, 2, 0);
    for (i = 0; i < n; i++) {
        puser = UserLst->pUserTbl[i];
        if (puser->flg == 1) {
            int nRank3;
            P3MC_RANKSCORE *pRank3;
            if (flag == 0) {
                if (puser->isVs != 0) {
                    continue;
                }
                nRank3 = puser->stageRank[stageNo].nSplay;
                pRank3 = puser->stageRank[stageNo].splay;
            } else {
                if (puser->isVs != 2) {
                    continue;
                }
                nRank3 = puser->stageRank[stageNo].nVplay[vsLev];
                pRank3 = puser->stageRank[stageNo].vplay[vsLev];
            }
            _TsSortSetRanking(ptRank, nRank3, pRank3, 0);
        }
    }

    cnt = 0;
    for (i = 0; i < 20 && ptRank[i] != NULL; i++, cnt++) {
        RankLst[i].score = ptRank[i]->score;
        memcpy(RankLst[i].name, ptRank[i]->name, 8);
        RankLst[i].name[8] = '\0';
        if (RankLst[i].name[0] == '\0') {
            strcpy(RankLst[i].name, UserName_RankingNoSave);
        }
    }

    *nrank = cnt;
    return RankLst;
}

void TsPopCusAOff(POPCTIM *pfw) {
    POPCOFF *poff;
    int      i;

    poff = pfw->offinf;

    for (i = 0; i < PR_ARRAYSIZE(pfw->offinf); i++, poff++) {
        if (i == pfw->onTNo) {
            poff->cltm = 0xf;
        }

        if (poff->bCur != 0) {
            poff->bCur = 0;
            poff->time = 0xf;
        } else {
            poff->time = 0;
            poff->bCur = 0;
            poff->cltm = 0;
        }
    }
}

void TsPopCusDim(POPCTIM *pfw, int n, int flg) {
    if (n >= PR_ARRAYSIZEU(pfw->bDim)) {
        return;
    }

    pfw->bDim[n] = flg;
}

void TsPopCusInit(POPCTIM *pfw,  int curIdx) {
    memset(pfw, 0, sizeof(*pfw));
    pfw->onTNo = curIdx;
}

void TsPopCusFlow(POPCTIM *pfw) {
    int i;

    if (pfw->okTim != 0) {
        pfw->okTim--;
    }
    if (pfw->onTim != 0) {
        pfw->onTim--;
    }
    if (pfw->srTim != 0) {
        pfw->srTim--;
    }

    for (i = 0; i < PR_ARRAYSIZE(pfw->offinf); i++) {
        if (pfw->offinf[i].time) {
            pfw->offinf[i].time--;
        }
        if (pfw->offinf[i].cltm) {
            pfw->offinf[i].cltm--;
        }
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsPopCusPut);
#else
void TsPopCusPut(SPR_PKT pk, SPR_PRM *spr, int dflg, POPCTIM *pfw, int bPut, int idx, PATPOS *ppat, int px, int py) {
    int mode;
    float rate;
    float v;
    float fv;
    float a, b, c, d;
    int ton;

    rate = 0.0f;
    mode = 4;
    if (pfw->bDim[idx] == 0) {
        mode = bPut;
    }

    switch (mode) {
    case 0:
        if (!(dflg & 1)) {
            return;
        }
        if (pfw->offinf[idx].time != 0) {
            mode = 1;
            rate = (float)pfw->offinf[idx].time * 0.06666667f;
        }
        break;
    case 1:
        if (!(dflg & 1)) {
            return;
        }
        rate = 1.0f;
        if (pfw->srTNo == idx && pfw->srTim != 0) {
            rate = 1.0f - (float)pfw->srTim * 0.125f;
        }
        break;
    case 2:
        if (!(dflg & 2)) {
            return;
        }
        pfw->srTNo = idx;
        pfw->srTim = 8;
        break;
    case 3:
        if (!(dflg & 1)) {
            return;
        }
        pfw->srTNo = idx;
        pfw->srTim = 8;
        break;
    case 4:
        if (dflg != 0) {
            return;
        }
        spr->rgba0 = 0x40808080;
        break;
    case 5:
        if (!(dflg & 4)) {
            return;
        }
        spr->rgba0 = 0x80707070;
        break;
    case 6:
        if (!(dflg & 4)) {
            return;
        }
        break;
    default:
        break;
    }

    if (mode < 2 || mode == 5) {
        if (pfw->offinf[idx].cltm != 0) {
            fv = (float)pfw->offinf[idx].cltm * 0.06666667f;
            spr->rgba0 = GetDToneColor(pfw->nabgr, pfw->habgr, (int)(fv * 240.0f * fv));
        }
    }

    switch (mode - 1) {
    case 0:
        TsPatPutSwing(pk, spr, ppat, px, py, 2, 8, pfw->fswing * rate);
        if (pfw->offinf[idx].time == 0) {
            pfw->offinf[idx].bCur = 1;
        }
        return;
    case 1:
        v = (float)pfw->okTim / 25.0f;
        a = sinf(v * 9.424778f);
        b = sinf(v * 6.2831855f);
        v = a * (v * 0.9f * v + 0.1f);
        spr->rgba0 = GetDToneColor(0xFFFFFF, 0x80FFFFFF, (int)(b * 256.0f * b));
        TsPatTexFnc(2);
        TsPatPutMZoom(pk, spr, ppat, px, py,
                      (float)(1.0 - (double)v * 0.2),
                      (float)((double)v * 0.6 + 1.0),
                      8, 4,
                      (float)((double)v * -0.2),
                      (float)((double)v * 0.6));
        TsPatTexFnc(0);
        pfw->srTim = 8;
        pfw->srTNo = idx;
        pfw->offinf[idx].bCur = 1;
        return;
    case 2:
        v = sinf((float)pfw->onTim * 2.1991148f * 0.1f);
        a = v * 0.05f + 1.0f;
        b = v * 0.14f + 1.0f;
        pfw->srTNo = idx;
        pfw->srTim = 8;
        c = v * -0.05f;
        d = v * -0.3f;
        TsPatPutMZoom(pk, spr, ppat, px, py, a, b, 8, 4, c, d);
        pfw->offinf[idx].bCur = 1;
        return;
    case 4:
    case 5:
        ton = MNSceneGetMusicFitTimer() % 45;
        v = sinf((float)ton * 3.1415927f / 45.0f);
        a = v * 0.1f + 0.95f;
        pfw->srTNo = idx;
        pfw->srTim = 8;
        TsPatPutMZoom(pk, spr, ppat, px, py, a, a, 4, 4, 0.0f, 0.0f);
        pfw->offinf[idx].bCur = 1;
        return;
    default:
        TsPatPut(pk, spr, ppat, px, py);
        pfw->offinf[idx].bCur = 0;
        return;
    }
}
#endif

int TsPUPCheckMove(int nbtn, int bank, POPCTIM *pfw) {
    return (pfw->bDim[POPBtn2Sel[(bank != 0) ? (nbtn + 3) : (nbtn + 0)]] == FALSE);
}

INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00396068);

INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00396070);

INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00396078);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsPopMenu_Flow);
#else
extern POPUP_MENU PopupMenu; // PAL bss
extern int POPSel2BtnDir[]; // PAL data 19b0f8 (jul 18cae8)
extern int Pop_CmpMesNo[]; // PAL data (jul 18cad0)
extern int PopRnkPos_No[][9]; // PAL data 19a9e0 (jul 18c420)
extern int PopBubblePat_No[][9]; // PAL data 19aa70 (jul 18c4b0)

int TsPopMenu_Flow(int flg, u_int tpad) {
    POPUP_MENU *pfw = &PopupMenu;
    ANIME_WK *awk = &pfw->awork;
    POPCTIM *cani = &pfw->cani;
    int s0;
    int s2;
    int i;
    int idx;
    int btn, nbtn;
    int sel, nsel;
    int cur, ncur;
    int pg;
    P3LOG_VAL *plog;

    if (flg == 1) {
        TsANIME_Init(awk);
        pfw->isRnkWAnime = 0;
        TsPopCusInit(cani, 0);
        pfw->state = 0;
        plog = pP3GameState->pLog;
        pfw->selno = 0;
        pfw->btnNo = 0;
        pfw->bSelRank = 0;
        pfw->selLev = 0;
        pfw->rVsLev = 0;
        pfw->urTim = 0;
        idx = tpad - 1;
        if (plog->nRound == 0) {
            if (!(((int)plog->clrFlg[0] >> (pP3GameState->nStage - 1)) & 1)) {
                idx = tpad - 1;
                for (i = 0; i < 5; i++) {
                    if (i != 0) {
                        cani->bDim[i] = 1;
                    }
                }
            }
        }
        pfw->levMax = (pP3GameState->pLog->clrVSCOM1[pP3GameState->nStage - 1] != 0) ? 4 : 1;
        if (tpad != 0) {
            tpad = idx;
        }
        pfw->nPPosSet = PopRnkPos_No[tpad][pP3GameState->nStage];
        PopMenu_Sw = 1;
        pfw->isSelLev = 0;
        pfw->isRankOn = 0;
        pfw->nPBubPat = PopBubblePat_No[tpad][pP3GameState->nStage];
        return 0;
    }

    if (flg == 2) {
        pfw->isSelLev = 0;
        PopMenu_Sw = 0;
        pfw->isRankOn = 0;
        return 0;
    }

    s0 = TsANIME_Poll(awk);
    TsPopCusFlow(cani);
    s2 = pfw->state;

    switch (s2) {
    case 0:
        s2 = 0x800;
        TsANIME_Start(awk, 2, 15);
        s0 = TsANIME_Poll(awk);
        pfw->isRnkWAnime = 0;
        /* fallthrough */
    case 0x800:
        if (s0 != 0) {
            break;
        }
        /* fallthrough */
    case 0x1000:
        pfw->isSelLev = 0;
        s2 = 0x1010;
        pfw->selLev = 0;
        pfw->exitflg = 0;
        break;
    case 0x1010:
        plog = pP3GameState->pLog;
        if (plog->nRound != 0 || (((int)plog->clrFlg[0] >> (pP3GameState->nStage - 1)) & 1)) {
            MENUSubt_PadFontArrowSet(POPSel2BtnDir[pfw->selno]);
        } else {
            MENUSubt_PadFontArrowSet(0);
        }
        TsCMPMes_SetMes(Pop_CmpMesNo[pfw->selno]);
        if (TsCheckTimeMapChange()) {
            break;
        }

        btn = pfw->btnNo;
        nbtn = btn;
        sel = POPBtn2Sel[(pfw->bSelRank != 0) ? (btn + 3) : btn];
        if (tpad & 0x8000) {
            nbtn = btn - 1;
        }
        if (tpad & 0x2000) {
            nbtn = nbtn + 1;
        }
        if (btn != nbtn) {
            nbtn = TSLOOP(nbtn, 3);
            if (TsPUPCheckMove(nbtn, pfw->bSelRank, cani)) {
                pfw->btnNo = nbtn;
                pfw->bSelRank = 0;
                TSSNDPLAY(2);
            }
        }
        btn = pfw->btnNo;
        if (btn != 1) {
            cur = pfw->bSelRank;
            ncur = cur;
            if (tpad & 0x1000) {
                ncur = cur - 1;
            }
            if (tpad & 0x4000) {
                ncur = ncur + 1;
            }
            if (cur != ncur) {
                ncur = TSLOOP(ncur, 2);
                if (TsPUPCheckMove(pfw->btnNo, ncur, cani)) {
                    pfw->bSelRank = ncur;
                    TSSNDPLAY(5);
                }
            }
        } else {
            pfw->bSelRank = 0;
        }
        nsel = POPBtn2Sel[(pfw->bSelRank != 0) ? (btn + 3) : btn];
        pfw->selno = nsel;
        if (sel != nsel) {
            TsPopCusAOff(cani);
            cani->onTim = 10;
            cani->onTNo = pfw->selno;
        } else {
            nsel = sel;
        }
        TsCMPMes_SetMes(Pop_CmpMesNo[nsel]);
        if (tpad & 0x10) {
            s2 = 0xF020;
            pfw->exitflg = 1;
            TSSNDPLAY(9);
        }
        if (tpad & 0x40) {
            pfw->exitflg = 0;
            switch (pfw->selno) {
            case 0:
            case 1:
                TSSNDPLAY(8);
                break;
            case 2:
                if (pfw->levMax < 2) {
                    TSSNDPLAY(8);
                } else {
                    TSSNDPLAY(6);
                }
                break;
            case 3:
            case 4:
                TSSNDPLAY(6);
                break;
            default:
                break;
            }
            s2 = 0x2000;
        }
        break;
    case 0x2000:
        switch (pfw->selno) {
        case 0:
        case 1:
            s2 = 0xF000;
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[2]);
            break;
        case 2:
            if (pfw->levMax < 2) {
                s2 = 0xF000;
                MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[2]);
                pfw->selLev = 0;
            } else {
                s2 = 0x3000;
            }
            break;
        case 3:
        case 4:
            s2 = 0x5000;
            break;
        default:
            break;
        }
        break;
    case 0x3000:
        cani->okTim = 25;
        cani->onTNo = pfw->selno;
        s2 = 0x3005;
        /* fallthrough */
    case 0x3005:
        if (cani->okTim != 0) {
            break;
        }
        pfw->selLev = 0;
        pfw->isSelLev = 1;
        TsCMPMes_SetMes(0x18);
        s2 = 0x3010;
        /* fallthrough */
    case 0x3010:
        if (TsCheckTimeMapChange()) {
            break;
        }
        cur = pfw->selLev;
        ncur = cur;
        if (tpad & 0x8000) {
            ncur = cur - 1;
        }
        if (tpad & 0x2000) {
            ncur = ncur + 1;
        }
        ncur = TSLOOP(ncur, pfw->levMax);
        if (cur != ncur) {
            pfw->selLev = ncur;
            TSSNDPLAY(2);
        }
        if (tpad & 0x10) {
            s2 = 0x1000;
            TSSNDPLAY(9);
        }
        if (tpad & 0x40) {
            TSSNDPLAY(8);
            s2 = 0xF000;
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[2]);
            pfw->isSelLev = 0;
        }
        break;
    case 0x5000:
        cani->okTim = 25;
        cani->onTNo = pfw->selno;
        if (pfw->selno == 3) {
            pfw->nTagMax = 1;
            pfw->nRankMax = 20;
        } else {
            pfw->nRankMax = 10;
            pfw->nTagMax = pfw->levMax;
        }
        pfw->rVsLev = 0;
        s2 = 0x5010;
        McInitFlow();
        break;
    case 0x5010:
        ret = McUserCheckFlow(0, 3, NULL);
        if (ret >= 0) {
            if ((u_int)(ret - 1) < 2) {
                pfw->isRankOn = 0;
                s2 = 0x1010;
            } else {
                s2 = 0x6000;
            }
        }
        break;
    case 0x6000:
        TsANIME_Start(awk, 4, 15);
        pfw->isRnkWAnime = 1;
        s0 = TsANIME_Poll(awk);
        /* fallthrough */
    case 0x6002:
        pfw->selrnkpg = 0;
        pfw->isRankOn = 1;
        pfw->rsline = 0;
        pfw->rStageNo = pP3GameState->nStage - 1;
        pfw->rankFlg = (pfw->selno == 3) ? 0 : 1;
        pfw->pRanking = TsGetRankingList(pfw->rankFlg, pfw->rVsLev, pfw->rStageNo, &pfw->nRanking);
        if (pfw->nRankMax < pfw->nRanking) {
            pfw->nRanking = pfw->nRankMax;
        }
        pfw->nPageMax = (pfw->nRanking + 9) / 10;
        if (pfw->nPageMax == 0) {
            pfw->nPageMax = 1;
        }
        s2 = 0x6005;
        /* fallthrough */
    case 0x6005:
        if (s0 != 0) {
            break;
        }
        if (pfw->nPageMax < 2) {
            if (pfw->rankFlg != 0) {
                TsCMPMes_SetMes(30);
            } else {
                TsCMPMes_SetMes(29);
            }
        } else {
            TsCMPMes_SetMes(28);
        }
        s2 = 0x6010;
        break;
    case 0x6010:
        if (pfw->rsline != 0) {
            pfw->rsline = TSNumMov(pfw->rsline, 0, 5);
        } else {
            if (TsCheckTimeMapChange()) {
                break;
            }
            pg = pfw->selrnkpg;
            cur = pg;
            ncur = pg;
            if (tpad & 0x1000) {
                ncur = pg - 1;
            }
            if (tpad & 0x4000) {
                ncur = ncur + 1;
            }
            ncur = TSLIMIT(ncur, 0, pfw->nPageMax);
            if (cur != ncur) {
                pfw->rsline = (ncur < cur) ? 160 : -160;
                pfw->selrnkpg = ncur;
                if (ncur < pg) {
                    TSSNDPLAY(3);
                } else {
                    TSSNDPLAY(4);
                }
            }
            cur = pfw->rVsLev;
            ncur = cur;
            if (tpad & 0x8000) {
                ncur = cur - 1;
            }
            if (tpad & 0x2000) {
                ncur = ncur + 1;
            }
            ncur = TSLOOP(ncur, pfw->nTagMax);
            if (cur != ncur) {
                pfw->rVsLev = ncur;
                TSSNDPLAY(2);
                s2 = 0x6002;
                break;
            }
        }
        if (tpad & 0x10) {
            pfw->exitflg = 0;
            s2 = 0x6020;
            TSSNDPLAY(9);
        }
        break;
    case 0x6020:
        TsANIME_Start(awk, 5, 15);
        s2 = 0x6030;
        pfw->isRnkWAnime = 1;
        s0 = TsANIME_Poll(awk);
        /* fallthrough */
    case 0x6030:
        if (s0 != 0) {
            break;
        }
        pfw->isRankOn = 0;
        s2 = 0x1010;
        break;
    case 0xF000:
        cani->okTim = 25;
        cani->onTNo = pfw->selno;
        s2 = 0xF010;
        /* fallthrough */
    case 0xF010:
        if (cani->okTim != 0) {
            break;
        }
        /* fallthrough */
    case 0xF020:
        TsANIME_Start(awk, 3, 15);
        s2 = 0xF080;
        s0 = TsANIME_Poll(awk);
        pfw->isRnkWAnime = 0;
        /* fallthrough */
    case 0xF080:
        if (s0 != 0) {
            break;
        }
        /* fallthrough */
    case 0xF100:
        PopMenu_Sw = 0;
        pfw->isRankOn = 0;
        if (pfw->exitflg != 0) {
            s2 = 0xFF20;
            break;
        }
        if (pfw->selno == 1) {
            ret = 2;
        } else if (pfw->selno == 0) {
            ret = 1;
        } else if (pfw->selno == 2) {
            ret = (pfw->selLev << 8) | 3;
        }
        return ret;
    case 0xFF20:
        return -1;
    default:
        break;
    }

    pfw->state = s2;
    return 0;
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsPopMenu_Draw);
#else
typedef struct { // 0x8
    short x;
    short y;
    u_int abgr;
} STRPOS;

typedef struct { // 0x8
    PTPOS *p0;
    PTPOS *p1;
} POPRNK_PPOS;

extern PATPOS PopMenuSel_Pat[]; // PAL data 19a698 (jul 18c0c8)
extern PATPOS SIRanking_Pat[]; // PAL data 19a728 (jul 18c168)
extern PATPOS Ranking_PatScroll[]; // PAL data 19a740 (jul 18c180)
extern PATPOS RankSISTNo_PAT[]; // PAL data 19a758 (jul 18c198)
extern PATPOS *VSRanking_PatTbl[]; // PAL data 19a8c8 (jul 18c308)
extern PATPOS RankVSSTNo_PAT[]; // PAL data 19a8d8 (jul 18c318)
extern STRPOS SRanking_Str[]; // PAL data 19a948 (jul 18c388), 3 entries
extern STRPOS VRanking_Str[]; // PAL data 19a960 (jul 18c3a0)
extern POPRNK_PPOS PopRnk_pPos[]; // PAL data 19a988 (jul 18c3c8)
extern PTPOS POPWZoom_CXY[]; // PAL data 19ab00 (jul 18c540)

void TsPopMenu_Draw(SPR_PKT pk, SPR_PRM *spr) {
    char buf[32];
    float rt0, rt1, rt2;
    int ton;
    int tm180;
    RANKLIST *prank;
    int nrank;
    int sc_y;
    int sc_lim;
    u_int ccus, cnorm;
    short ox, oy;
    short bx, by;
    PTPOS *pp;
    PATPOS *pcur;
    PATPOS *plist;
    PATPOS *pnum;
    PATPOS *pnum1;
    STRPOS *pstr;
    int i, n;
    int rx, ry;
    u_int col;
    float zr, f0, f1;
    float rate;

    PopupMenu.cani.fswing = -cosf(((float)(MNSceneGetMusicFitTimer() % 72) / 72.0f) * 6.2831855f);
    tm180 = MNSceneGetMusicFitTimer() % 180;
    spr->zx = 1.0f;
    spr->zy = 0.5f;
    PkALPHA_Add(pk, 0x44);

    if (PopupMenu.isRnkWAnime == 0 && TsANIME_GetRate(&PopupMenu.awork, &rt0, &rt1, &rt2) != 0) {
        pp = &POPWZoom_CXY[PopupMenu.nPBubPat];
        ton = (int)(rt0 * 256.0f);
        spr->zoom.centerX = (float)pp->x;
        spr->zoom.zoomY = rt1;
        spr->zoom.isOn = 1;
        spr->zoom.zoomX = rt0;
        spr->zoom.centerY = (float)pp->y;
    } else {
        spr->zoom.isOn = 0;
        ton = 0x100;
    }

    pp = PopRnk_pPos[PopupMenu.nPPosSet].p0;
    oy = pp->y;
    ox = pp->x;
    spr->rgba0 = GetDToneColor(0x404040, 0x80808080, ton);
    if (PopupMenu.urTim != 0) {
        PopupMenu.urTim--;
    }
    ccus = GetDToneColor(0x404040, 0x80FFFFFF, ton);
    cnorm = GetDToneColor(0x404040, 0x80808080, ton);
    TsPopMenCus_Draw(pk, spr, &PopupMenu, ox, oy, ccus, cnorm, 0);
    TsPopMenCus_Draw(pk, spr, &PopupMenu, ox, oy, ccus, cnorm, 1);
    TsPopMenCus_Draw(pk, spr, &PopupMenu, ox, oy, ccus, cnorm, 6);
    spr->zoom.isOn = 0;

    if (PopupMenu.isRankOn != 0) {
        pp = PopRnk_pPos[PopupMenu.nPPosSet].p1;
        by = pp->y;
        bx = pp->x;
        if (PopupMenu.isRnkWAnime != 0) {
            if (TsANIME_GetRate(&PopupMenu.awork, &rt0, &rt1, &rt2) == 0) {
                spr->zoom.isOn = 0;
                ton = 0x100;
                rate = 0.0f;
            } else {
                if (PopupMenu.rankFlg == 0) {
                    pcur = &PopMenuSel_Pat[3];
                } else {
                    pcur = &PopMenuSel_Pat[4];
                }
                n = pcur->x + ox;
                spr->zoom.zoomY = rt1 * 0.9f + 0.1f;
                spr->zoom.isOn = 1;
                spr->zoom.centerX = (float)(n + 0x20) + ((float)(n - bx) / 1.8f);
                rate = rt2 * 0.6f;
                spr->zoom.zoomX = rt0 * 0.65f + 0.35f;
                spr->zoom.centerY = (float)(pcur->y + oy + 6);
                ton = (int)(rt0 * 156.0f) + 0x64;
            }
        } else {
            ton = 0x100;
            rate = 0.0f;
        }
        spr->rgba0 = GetDToneColor(0x20FFFFFF, 0x80808080, ton);

        if (PopupMenu.rankFlg != 0) {
            pnum = RankVSSTNo_PAT;
            n = 5;
            plist = VSRanking_PatTbl[PopupMenu.rVsLev];
        } else {
            plist = SIRanking_Pat;
            pnum = RankSISTNo_PAT;
            n = 2;
        }

        for (i = 0; i < n; i++) {
            if (rate != 0.0f) {
                f0 = sinf(rate * 6.2831855f);
                f1 = f0 * 0.3f;
                zr = f0 * 0.2f + 1.0f;
                TsPatPutMZoom(pk, spr, &plist[i], bx, by, zr, zr, 8, 8, f1, f1);
                PopupMenu.urTim = 20;
            } else if (i == 0) {
                if (PopupMenu.urTim != 0) {
                    f1 = 1.0f - (float)PopupMenu.urTim * 0.05f;
                } else {
                    f1 = 1.0f;
                }
                TsPatPutUneri(pk, spr, &plist[i], bx, by, 8, 10, (float)tm180 / 180.0f, f1);
            } else {
                TsPatPut(pk, spr, &plist[i], bx, by);
            }
        }
        pnum1 = &pnum[1];

        if (rate < 0.15f) {
            ton = (int)((0.15f - rate) * 256.0f * 6.6666665f);
        } else {
            ton = 0;
        }
        spr->rgba0 = GetDToneColor(0x808080, 0x80808080, ton);
        TsPatPut(pk, spr, pnum, bx, by);
        TsPatPut(pk, spr, &pnum1[PopupMenu.rStageNo], bx, by);
        n = PopupMenu.selrnkpg;
        if (n > 0) {
            TsPatPut(pk, spr, &Ranking_PatScroll[0], bx, by);
        }
        if (n < PopupMenu.nPageMax - 1) {
            TsPatPut(pk, spr, &Ranking_PatScroll[1], bx, by);
        }

        prank = PopupMenu.pRanking;
        nrank = PopupMenu.nRanking;
        if (PopupMenu.rankFlg == 0) {
            pstr = SRanking_Str;
        } else {
            pstr = VRanking_Str;
        }
        i = 0;
        n = by + pstr[0].y;
        sc_y = n - 5;
        SetSprScreenXYWH(spr);
        PkSCISSOR_Add(pk, spr->px, (short)sc_y, spr->sw, 0x50);
        ry = by - (PopupMenu.selrnkpg * 0x50 + (PopupMenu.rsline >> 1));
        if (nrank > 0) {
            sc_lim = n - 0xD;
            do {
                rx = ry + pstr[0].y;
                if (sc_lim < rx && rx < sc_y + 0x58) {
                    col = GetDToneColor(pstr[0].abgr & 0xFFFFFF, pstr[0].abgr, ton);
                    sprintf(buf, "%d", i + 1);
                    MENUFontPutR(pk, spr, bx + pstr[0].x, rx, col, 0x100, (u_char *)buf, 1.0f);
                    sprintf(buf, "%d", prank[i].score);
                    MENUFontPutR(pk, spr, bx + pstr[1].x, ry + pstr[1].y, col, 0x102, (u_char *)buf, 1.0f);
                    sprintf(buf, "%s", prank[i].name);
                    MENUFontPutR(pk, spr, bx + pstr[2].x, ry + pstr[2].y, col, 0x100, (u_char *)buf, 1.1f);
                }
                i++;
                ry += 8;
            } while (i < nrank);
        }
        PkDefSCISSOR_Add(pk);
        spr->zoom.isOn = 0;
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsPopMenCus_Draw);
#else
extern PATPOS PopMenuSel_Pat[];
extern PATPOS VSComMenuSel_Pat[];
extern PATPOS VSComMenuSelH_Pat[];
extern int POPSel2Btn[];

void TsPopMenCus_Draw(SPR_PKT pk, SPR_PRM *spr, POPUP_MENU *pfw, int px, int py, u_int hicol, u_int nmcol, int dflg) {
    int i, j;
    int bPut;
    int bState;
    int st;
    float oy;
    PATPOS *ppat;

    oy = spr->ofsy;
    bState = 0;
    i = 0;
    do {
        pfw->cani.habgr = hicol;
        pfw->cani.nabgr = nmcol;
        bPut = 0;
        spr->ofsy = oy + sinf((float)(MNSceneGetMusicFitTimer() % 480) * 6.2831855f * 0.0020833334f + (float)POPSel2Btn[i] * 2.5132742f) * 3.3f;

        if (i == pfw->selno && pfw->cani.okTim != 0) {
            bState = 2;
            bPut = 2;
        } else {
            bState = ((POPSel2Btn[i] ^ pfw->btnNo) != 0) ? 0 : 1;
            if (i == pfw->selno) {
                bPut = 1;
                if (pfw->cani.onTim != 0) {
                    bState = 3;
                }
            }
        }

        if (pfw->isSelLev != 0) {
            bState = ((i ^ 2) != 0) ? 0 : 5;
        }

        if (bPut == 0) {
            spr->rgba0 = pfw->cani.nabgr;
        } else if (bPut == 1) {
            spr->rgba0 = pfw->cani.habgr;
        }

        TsPopCusPut(pk, spr, dflg, &pfw->cani, bState, i, &PopMenuSel_Pat[i], px, py);

        if (i == 2) {
            pfw->cani.habgr = nmcol;
            pfw->cani.nabgr = nmcol;
            for (j = 0; j < pfw->levMax; j++) {
                spr->rgba0 = nmcol;
                ppat = &VSComMenuSel_Pat[j];
                st = bState;
                if (pfw->isSelLev != 0) {
                    st = 6;
                    if (j == pfw->selLev) {
                        spr->rgba0 = GetDToneColor(0x80FFFFFF, 0x80606060,
                                (int)(sinf((float)(MNSceneGetMusicFitTimer() % 30) * 3.1415927f / 30.0f) * 256.0f));
                        ppat = &VSComMenuSelH_Pat[j];
                    } else {
                        spr->rgba0 = 0x80707070;
                    }
                }
                TsPopCusPut(pk, spr, dflg, &pfw->cani, st, 2, ppat, px, py);
            }
            i = 3;
        } else {
            i++;
        }
    } while (i < 5);

    spr->ofsy = oy;
}
#endif

/* data 18cb00 */ extern int SaveMenu_CmpMesNo[]; /* static */

/* static */ int TsSaveMenu_Flow(int flg, u_int tpad) {
    SAVE_MENU *pfw;
    int        state;
    int        bAni;
    int        sel;

    pfw = &SaveMenu;

    if (flg == 1) {
        if (tpad < 2) {
            pfw->selno = tpad;
        } else {
            pfw->selno = 0;
        }
        pfw->state = 0;
        TsANIME_Init(&pfw->awork);
        TsPopCusInit(&pfw->cani, 0);
        SaveMenu_Sw = 1;
        return 0;
    }

    if (flg == 2) {
        SaveMenu_Sw = 0;
        return 0;
    }

    state = pfw->state;
    bAni = TsANIME_Poll(&pfw->awork);
    TsPopCusFlow(&pfw->cani);

    switch (state) {
    case 0:
        state = 0x100;
        TsANIME_Start(&pfw->awork, 2, 15);
        bAni = TsANIME_Poll(&pfw->awork);
        /* fallthrough */
    case 0x100:
        if (bAni != 0) {
            break;
        }
        /* fallthrough */
    case 0x1000:
        pfw->exitflg = 0;
        state = 0x1010;
        break;
    case 0x1010:
        TsCMPMes_SetMes(SaveMenu_CmpMesNo[pfw->selno]);
        if (TsCheckTimeMapChange()) {
            break;
        }

        sel = pfw->selno;
        if (tpad & 0x8000) {
            sel--;
        }
        if (tpad & 0x2000) {
            sel++;
        }

        if (pfw->selno != sel) {
            sel = TSLOOP(sel, 2);
            pfw->selno = sel;
            TsPopCusAOff(&pfw->cani);
            pfw->cani.onTNo = sel;
            pfw->cani.onTim = 10;
            TSSNDPLAY(2);
            TsCMPMes_SetMes(SaveMenu_CmpMesNo[pfw->selno]);
        }

        if (tpad & 0x10) {
            pfw->exitflg = 1;
            TSSNDPLAY(9);
            state = 0xf020;
        } else if (tpad & 0x40) {
            pfw->exitflg = 0;
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
            state = 0xf000;
            TSSNDPLAY(6);
        }
        break;
    case 0xf000:
        pfw->cani.okTim = 25;
        state = 0xf010;
        pfw->cani.onTNo = pfw->selno;
        /* fallthrough */
    case 0xf010:
        if (pfw->cani.okTim != 0) {
            break;
        }
        /* fallthrough */
    case 0xf020:
        state = 0xf080;
        TsANIME_Start(&pfw->awork, 3, 20);
        bAni = TsANIME_Poll(&pfw->awork);
        /* fallthrough */
    case 0xf080:
        if (bAni != 0) {
            break;
        }
        /* fallthrough */
    case 0xf100:
        SaveMenu_Sw = 0;
        if (pfw->exitflg != 0) {
            state = 0xff20;
            break;
        }
        switch (pfw->selno) {
        case 0:
            ret = 1;
            break;
        case 1:
            ret = 2;
            break;
        default:
            ret = 3;
            break;
        }
        return ret;
    case 0xff20:
        return -1;
    }

    pfw->state = state;
    return 0;
}

void TsSaveMenu_Draw(SPR_PKT pk, SPR_PRM *spr) {
    register PATPOS *ppat asm("$16");
    SAVE_MENU *pfw;
    float      fRate;
    float      rt0, rt1, rt2;
    int        ton;
    int        i;
    int        dmy;
    int        bPut, bState;

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

    {
        u_int c = GetDToneColor(0x404040, 0x80808080, ton);
        ppat = SAVE_MENU_SELPAT;
        spr->rgba0 = c;
    }
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

static void TSJukeCDObj_Init(JUKECDOBJ *pw, int pno) {
    memset(pw, 0, sizeof(*pw));
    pw->patPos = &JUKEBOX_Pos[pno];
    pw->patNo = pno;
}

void _TsJkJacketPut(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, float zx, float rot, u_int abgr, u_int abgrs) {
    if (pw->vrate == 0.0f) {
        spr->rgba0 = abgrs;
        TsPatPutRZoom(pk, spr, &JUKEJKT_PatS, px + 10, py + 5, zx, rot);
        spr->rgba0 = abgr;
        TsPatPutRZoom(pk, spr, &JUKEJKT_Pat[pw->patNo], px, py, zx, rot);
    } else {
        spr->rgba0 = abgrs;
        TsPatPutMZoom(pk, spr, &JUKEJKT_PatS, px + 10, py + 5, zx + pw->vrate, zx + pw->vrate, 8, 8, pw->vrate, pw->vrate);
        spr->rgba0 = abgr;
        TsPatPutMZoom(pk, spr, &JUKEJKT_Pat[pw->patNo], px, py, zx + pw->vrate, zx + pw->vrate, 8, 8, pw->vrate, pw->vrate);
    }
}

void _TsJkRecordPut(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, float zr, float rrot, u_int abgr, u_int abgrs) {
    float ox, oy;

    if (pw->rox == 0.0f && pw->roy == 0.0f) {
        return;
    }

    ox = spr->ofsx;
    oy = spr->ofsy;
    spr->ofsx = ox + pw->rox;
    spr->rgba0 = abgrs;
    spr->ofsy = oy + pw->roy;
    TsPatPutRZoom(pk, spr, &JUKEREC_PatS, px + 10, py + 5, zr, rrot);
    spr->rgba0 = abgr;
    TsPatPutRZoom(pk, spr, &JUKEREC_Pat[pw->patNo], px, py, zr, rrot);
    spr->ofsx = ox;
    spr->ofsy = oy;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TSJukeCDObj_Draw);
#else
extern float JUKEWAV_INITBL[]; // PAL data (jul 18c628)

/* static */ void TSJukeCDObj_Draw(SPR_PKT pk, SPR_PRM *spr, JUKECDOBJ *pw, int px, int py, int anmtime) {
    PTPOS *pp;
    PATPOS *jpat;
    int ox, oy;
    int psx, psy;
    float sox, soy;
    float rate;
    float jz, rz;
    float f26, f29, f31;
    float offx, offy;
    float rot, rrot;
    float f20, f22, f28, f27;
    u_int c, cs, rc, rcs;
    int ton, ton2;
    int a, na, i;
    float rox0, roy0;

    offy = 0.0f;
    pp = pw->patPos;
    oy = py + pp->y;
    ox = px + pp->x;
    f26 = 1.0f;
    jpat = &JUKEJKT_Pat[pw->patNo];
    f29 = 1.0f;
    sox = spr->ofsx;
    rate = 1.0f;
    f31 = 1.0f;
    soy = spr->ofsy;
    PkALPHA_Add(pk, 0x44);
    rot = 0.0f;
    offx = 0.0f;
    rrot = 0.0f;

    switch (pw->state) {
    case 1:
        c = 0x80707070;
        cs = 0x30808080;
        pw->vrate = 0.0f;
        break;
    case 2:
    case 5:
        rate = 1.25f;
        c = 0x80C0C0C0;
        pw->vrate = 0.0f;
        cs = 0x30808080;
        break;
    case 3:
        pw->time++;
        f22 = (float)pw->time * 0.06666667f;
        if (pw->time >= 15) {
            pw->time = 0;
            pw->state = 2;
        }
        cs = 0x30808080;
        f20 = f22 * 3.1415927f;
        rate = (sinf(f20) * 0.2f + 1.0f) * (f22 * 0.25f + 1.0f);
        pw->vrate = sinf(f20) * 0.1f;
        c = GetDToneColor(0x80707070, 0x80C0C0C0, (int)(f22 * 256.0f));
        break;
    case 4:
        pw->time++;
        f22 = (float)pw->time * 0.06666667f;
        if (pw->time >= 15) {
            pw->time = 0;
            pw->state = 1;
        }
        cs = 0x30808080;
        f20 = f22 * 3.1415927f;
        rate = (float)((1.0 - (double)sinf(f20) * 0.25) * (double)((1.0f - f22) * 0.25f + 1.0f));
        pw->vrate = sinf(f20) * -0.08f;
        c = GetDToneColor(0x80C0C0C0, 0x80707070, (int)(f22 * 256.0f));
        break;
    default:
        c = 0x80C0C0C0;
        pw->vrate = rot;
        cs = 0x30808080;
        break;
    }

    if (pw->bMsk != 0) {
        pw->vrate = 0.0f;
        c = 0x30808080;
        cs = 0x18808080;
    }

    rc = c;
    rcs = cs;
    rz = 0.0f;
    jz = 0.0f;

    switch (pw->anime) {
    case 1:
        if (pw->atime != 0) {
            pw->innm = 0x200;
            pw->atime--;
            return;
        }
        pw->innm = TSNumMov(pw->innm, 0, 5);
        f22 = 1.0f - (float)pw->innm * 0.001953125f;
        rot = (1.0f - f22) * 6.2831855f;
        offx = (1.0f - f22) * ((pw->dir != 0) ? 200.0f : -200.0f);
        offy = (1.0f - f22) * ((pw->dir2 != 0) ? 20.0f : -20.0f);
        ton = (int)(f22 * 256.0f);
        c = GetDToneColor(0xFFFFFF, c, ton);
        cs = GetDToneColor(0x808080, cs, ton);
        f20 = (1.0f - f22) * 1.5f + 1.0f;
        if (pw->innm == 0) {
            jz = f20 * rate;
            pw->anime = 0;
            rz = 1.0f * rate;
            pw->time = 0;
            pw->atime = 0;
        } else {
            jz = f20 * rate;
            rz = f29 * rate;
        }
        break;
    case 2:
        if (pw->atime != 0) {
            jz = 1.0f * rate;
            pw->atime--;
            pw->innm = 0x100;
            rz = jz;
        } else {
            pw->innm = TSNumMov(pw->innm, 0, 5);
            f22 = 1.0f - (float)pw->innm * 0.00390625f;
            rot = f22 * -9.424778f;
            offx = f22 * ((pw->dir != 0) ? -200.0f : 200.0f);
            offy = f22 * ((pw->dir2 != 0) ? 20.0f : -20.0f);
            ton = (int)(f22 * 256.0f);
            c = GetDToneColor(c, 0xFFFFFF, ton);
            cs = GetDToneColor(c, 0x808080, ton);
            f20 = f22 * 1.2f + 0.5f;
            if (pw->innm == 0) {
                jz = f20 * rate;
                pw->anime = 0;
                rz = 1.0f * rate;
                pw->state = 0;
                pw->time = 0;
                pw->atime = 0;
            } else {
                jz = f20 * rate;
                rz = f29 * rate;
            }
        }
        break;
    case 5:
    case 6:
        if (pw->atime != 0) {
            pw->atime--;
            pw->innm = 0x80;
            pw->ox = 0.0f;
            pw->rot = 0.0f;
            pw->oy = 0.0f;
            if (pw->anime == 6) {
                return;
            }
            jz = 1.0f * rate;
            rz = jz;
        } else {
            pw->innm = TSNumMov(pw->innm, 0, 12);
            f22 = 1.0f - (float)pw->innm * 0.0078125f;
            TsPatGetSize(jpat, &psx, &psy, NULL, NULL);
            if (pw->anime == 6) {
                f22 = 1.0f - f22;
            }
            ton = (int)(f22 * 256.0f);
            offy = (float)((psy + oy) - 0xA1) * 0.5f * f22;
            offx = (float)((psx + ox) - 0x13E) * 0.35f * f22;
            c = GetDToneColor(c, 0x404040, ton);
            cs = GetDToneColor(cs, 0x808080, ton);
            f20 = 1.0f - f22 * 0.3f;
            if (pw->innm == 0) {
                if (pw->anime != 6) {
                    pw->state = 0;
                } else {
                    pw->state = 1;
                }
                jz = f20 * rate;
                pw->anime = 0;
                rz = 1.0f * rate;
                pw->time = 0;
                pw->atime = 0;
            } else {
                jz = f20 * rate;
                rz = f29 * rate;
            }
        }
        break;
    case 3:
    case 4:
        if (pw->atime != 0) {
            pw->atime--;
            pw->innm = 0x200;
            pw->rot = 0.0f;
            pw->oy = 0.0f;
            pw->ox = 0.0f;
            pw->rrot = 0.0f;
            pw->rox = 0.0f;
            if (pw->anime == 4) {
                rz = rate;
                pw->rox = rate * 0.0f;
                f31 = 0.0f;
                pw->roy = rate * -43.0f;
                jz = 1.0f * rz;
                c = 0x80C0C0C0;
                cs = 0x30808080;
                rc = 0x80C0C0C0;
                rcs = 0x30808080;
            } else {
                jz = 1.0f * rate;
                rz = jz;
            }
        } else {
            pw->oy = 0.0f;
            pw->ox = 0.0f;
            pw->roy = 0.0f;
            pw->rox = 0.0f;
            pw->innm = TSNumMov(pw->innm, 0, 7);
            f22 = 1.0f - (float)pw->innm * 0.001953125f;
            if (pw->anime == 4) {
                f22 = 1.0f - f22;
            }
            f31 = 1.0f - f22;
            rc = c;
            rcs = cs;
            f20 = f22 * 0.5f * 3.1415927f;
            pw->rox = (1.0f - cosf(f20)) * 0.0f * rate * 1.0f;
            pw->roy = sinf(f20) * -43.0f * rate * 1.0f;
            if (pw->innm == 0) {
                pw->time = 0;
                pw->atime = 0;
                if (pw->anime == 4) {
                    rz = rate;
                    pw->state = 2;
                    jz = 1.0f * rz;
                    pw->anime = 0;
                } else {
                    rz = rate;
                    pw->state = 2;
                    pw->anime = 7;
                    jz = 1.0f * rz;
                }
            } else {
                rz = rate;
                jz = 1.0f * rz;
            }
        }
        break;
    case 7:
        f31 = 0.0f;
        a = pw->atime;
        na = a + 1;
        if (a == 0) {
            pw->innm2 = 0x400;
            pw->atime = 1;
            pw->rrot = 0.0f;
            pw->innm = 0x400;
        } else {
            pw->atime = na;
            f20 = pw->rrot + (float)((na < 0x6F) ? na : 0x6E) * 0.0023f;
            pw->rrot = f20;
            if (6.2831855f < f20) {
                pw->rrot = f20 - 6.2831855f;
            }
        }
        rox0 = rate * 0.0f;
        roy0 = rate * -43.0f;
        f29 = 1.0f;
        rrot = pw->rrot;
        pw->rox = rox0;
        pw->roy = roy0;
        if (pw->atime >= 0x47) {
            pw->innm2 = TSNumMov(pw->innm2, 0, 11);
            f22 = 1.0f - (float)pw->innm2 * 0.0009765625f;
            ton = (int)(f22 * 256.0f);
            c = GetDToneColor(c, 0xFFFFFF, ton);
            cs = GetDToneColor(cs, 0x808080, ton);
            f26 = 1.0f - f22 * 0.5f;
        }
        if (pw->atime >= 0x65) {
            pw->innm = TSNumMov(pw->innm, 0, 14);
            f22 = 1.0f - (float)pw->innm * 0.0009765625f;
            ton = (int)(f22 * 256.0f);
            rc = GetDToneColor(rc, 0xFFFFFF, ton);
            rcs = GetDToneColor(rcs, 0x808080, ton);
            f29 = f22 * 0.8f + 1.0f;
            TsPatGetSize(jpat, &psx, &psy, NULL, NULL);
            pw->rox = rox0 - (float)((psx + ox) - 0x13E) * f22;
            pw->roy = roy0 - (float)((psy + oy) - 0xB5) * f22;
        }
        jz = f26 * rate;
        if (pw->innm == 0 && pw->innm2 == 0) {
            pw->state = 0;
            rz = f29 * rate;
            pw->anime = 0;
            pw->time = 0;
            pw->atime = 0;
        } else {
            rz = f29 * rate;
        }
        break;
    case 8:
        f31 = 0.0f;
        a = pw->atime;
        if (a == 0) {
            jz = 1.0f * rate;
            pw->atime = 0x50;
            pw->rrot = 0.0f;
            rz = jz;
            for (i = 0; i < 0x3C; i++) {
                pw->rrot -= (float)i;
            }
            pw->innm = 0x400;
            TsPatGetSize(jpat, &psx, &psy, NULL, NULL);
            c = 0xFFFFFF;
            cs = 0x808080;
            pw->roy = (rate * -43.0f) - (float)((psy + oy) - 0xB5);
            pw->rox = (rate * 0.0f) - (float)((psx + ox) - 0x13E);
        } else {
            na = a - 0x15;
            pw->atime = a - 1;
            if (na > 0) {
                f20 = pw->rrot + (float)na;
                rrot = f20 * 0.005f;
                pw->rrot = f20;
            } else {
                rot = 0.0f;
            }
            pw->innm = TSNumMov(pw->innm, 0, 6);
            f22 = (float)pw->innm * 0.0009765625f;
            f20 = f22 * 256.0f;
            ton = (int)f20;
            f29 = f22 * 0.8f + 1.0f;
            f26 = 1.0f - f22 * 0.5f;
            c = GetDToneColor(c, 0xFFFFFF, ton);
            cs = GetDToneColor(cs, 0x808080, ton);
            TsPatGetSize(jpat, &psx, &psy, NULL, NULL);
            f28 = rate * 0.0f;
            f27 = rate * -43.0f;
            ton2 = (int)(f20 * 0.5f);
            pw->roy = f27 - (float)((psy + oy) - 0xB5) * f22;
            pw->rox = f28 - (float)((psx + ox) - 0x13E) * f22;
            rc = GetDToneColor(rc, 0xFFFFFF, ton2);
            rcs = GetDToneColor(rcs, 0x808080, ton2);
            if (pw->atime == 0) {
                jz = f26 * rate;
                pw->state = 2;
                rz = f29 * rate;
                pw->atime = 1;
                pw->anime = 4;
                pw->rox = f28;
                pw->roy = f27;
                pw->time = 0;
            } else {
                jz = f26 * rate;
                rz = f29 * rate;
            }
        }
        break;
    default:
        jz = 1.0f * rate;
        rz = jz;
        pw->ox = 0.0f;
        pw->rot = 0.0f;
        pw->oy = 0.0f;
        break;
    }

    spr->ofsx += offx + pw->ox;
    spr->ofsy += offy + pw->oy;
    spr->ofsy += sinf((2.0f * (float)(anmtime % 240) / 240.0f + JUKEWAV_INITBL[pw->patNo]) * 3.1415927f) * 8.0f * f31;

    if ((u_int)(pw->anime - 7) < 2) {
        _TsJkJacketPut(pk, spr, pw, ox, oy, jz, rot, c, cs);
        _TsJkRecordPut(pk, spr, pw, ox, oy, rz, rrot, rc, rcs);
    } else {
        _TsJkRecordPut(pk, spr, pw, ox, oy, rz, rrot, rc, rcs);
        _TsJkJacketPut(pk, spr, pw, ox, oy, jz, rot, c, cs);
    }

    spr->ofsy = soy;
    spr->ofsx = sox;
}
#endif

static int TsJukeIsObjAnime(int isComp) {
    int        i;
    JUKE_MENU *pfw;

    pfw = &JukeMenu;

    for (i = 0; i < PR_ARRAYSIZE(pfw->cusObj); i++) {
        if (pfw->cusObj[i].bMsk) {
            continue;
        }

        if (!isComp) {
            if (pfw->cusObj[i].anime == TSJKANM_ROTATE) {
                if (pfw->cusObj[i].atime > 120) {
                    continue;
                }
            }

            if (pfw->cusObj[i].anime == TSJKANM_ROTSTOP) {
                if (pfw->cusObj[i].atime > 20) {
                    continue;
                }
            }
        }

        if (pfw->cusObj[i].anime != TSJKANM_OFF) {
            return TRUE;
        }
    }

    return FALSE;
}

/* static */ int TsJukeObjAnime(int isOut) {
    int        i;
    JUKE_MENU *pfw;

    pfw = &JukeMenu;

    for (i = 0; i < PR_ARRAYSIZE(pfw->cusObj); i++) {
        if (i < 5) {
            pfw->cusObj[i].dir2 = i & 1;
            pfw->cusObj[i].dir  = 1;
            if (!isOut) {
                pfw->cusObj[i].state = 1;
                pfw->cusObj[i].anime = TSJKANM_IN;
                pfw->cusObj[i].atime = (5 - i) * 6 + 1;
            } else {
                pfw->cusObj[i].anime = TSJKANM_OUT;
                pfw->cusObj[i].atime = (5 - i) * 6 + 1;
            }
        } else {
            pfw->cusObj[i].dir2 = (i + 1) & 1;
            pfw->cusObj[i].dir  = 0;
            if (!isOut) {
                pfw->cusObj[i].state = 1;
                pfw->cusObj[i].anime = TSJKANM_IN;
                pfw->cusObj[i].atime = (i - 5) * 6 + 19;
            } else {
                pfw->cusObj[i].anime = TSJKANM_OUT;
                pfw->cusObj[i].atime = (i - 5) * 6 + 19;
            }
        }
    }

    return 0;
}

/* static */ int TsJukeObjAnime2(int isOut) {
    int        i;
    JUKE_MENU *pfw;

    pfw = &JukeMenu;

    for (i = 0; i < PR_ARRAYSIZE(pfw->cusObj); i++) {
        if (i == pfw->selno) {
            switch (isOut) {
            case 0:
                pfw->cusObj[i].atime = 1;
                pfw->cusObj[i].state = 2;
                pfw->cusObj[i].anime = TSJKANM_PLAY1;
                break;
            case 2:
                pfw->cusObj[i].atime = 0;
                pfw->cusObj[i].state = isOut;
                pfw->cusObj[i].anime = TSJKANM_ROTSTOP;
                break;
            }
        } else {
            pfw->cusObj[i].atime = (rand() % 3) * 3 + 1;
            switch (isOut) {
            case 0:
                pfw->cusObj[i].anime = TSJKANM_PLAY2;
                break;
            case 1:
                pfw->cusObj[i].state = isOut;
                pfw->cusObj[i].anime = TSJKANM_RETURN2;
                break;
            }
        }
    }

    return 0;
}

/* static */ int _TsJKMoveCus(int *cx, int *cy, int mx, int my, JUKECDOBJ *cobj) {
    int ocx, ocy;
    int x, y, pos;
    int i, ret;

    ocy = *cy;
    x = ocx = *cx;

    y = TSLOOP(ocy + my, 2);
    pos = y * 5 + ocx;

    if (mx != 0) {
        pos = TSLOOP(pos + mx, 10);
        x = pos % 5;
        y = pos / 5;
    }

    if (!cobj[pos].bMsk) {
        *cx = x;
        *cy = y;
        return 1;
    }

    if (mx != 0) {
        if (mx > 0) {
            mx = 1;
        } else {
            mx = -1;
        }

        for (i = 0; i < 10; i++) {
            pos = TSLOOP(pos + mx, 10);
            if (!cobj[pos].bMsk) {
                x = pos % 5;
                y = pos / 5;
                *cx = x;
                *cy = y;
                ret = 0;
                if (ocx != x || ocy != y) {
                    ret = 1;
                }
                return ret;
            }
        }
        return 0;
    }

    for (i = x; i < 5; i++) {
        pos = y * 5 + i;
        if (!cobj[pos].bMsk) {
            y = pos / 5;
            *cx = pos % 5;
            *cy = y;
            return 1;
        }
    }

    for (i = x; i >= 0; i--) {
        pos = y * 5 + i;
        if (!cobj[pos].bMsk) {
            x = pos % 5;
            y = pos / 5;
            *cx = x;
            *cy = y;
            return (ocx != x || ocy != y);
        }
    }

    return 0;
}

/* static */ void _TsJKSetPadArrow(int sel, JUKECDOBJ *cobj) {
    int cx, cy;
    int x, y;
    int flg = 0;

    x = sel % 5;
    y = sel / 5;

    cx = x;
    cy = y;
    if (_TsJKMoveCus(&cx, &cy, -1, 0, cobj)) {
        flg |= 1;
    }

    cx = x;
    cy = y;
    if (_TsJKMoveCus(&cx, &cy, 1, 0, cobj)) {
        flg |= 2;
    }

    cx = x;
    cy = y;
    if (_TsJKMoveCus(&cx, &cy, 0, -1, cobj)) {
        flg |= 4;
    }

    cx = x;
    cy = y;
    if (_TsJKMoveCus(&cx, &cy, 0, 1, cobj)) {
        flg |= 8;
    }

    MENUSubt_PadFontArrowSet(flg);
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsJukeMenu_Flow);
#else
typedef struct { // 0x8
    int id;
    int len;
} JUKEBGM_TBL;

extern JUKEBGM_TBL JukeBgmTbl[]; // PAL data 19b0c8
extern int JukeMenu_CmpMesNo[]; // PAL data 19b118
extern int JukeMapAnmState; // PAL sdata 383800
extern int JukeBgmReq; // PAL sdata 383804
extern MNANM_TBL StageMapBGMCamera[];

/* static */ int TsJukeMenu_Flow(int flg, u_int tpad) {
    JUKE_MENU *pfw = &JukeMenu;
    int state;
    int i, flag;
    int pos[2];
    int sel, nsel;
    int mapNo;
    u_int bnk;

    if (flg == 1) {
        memset(pfw, 0, sizeof(*pfw));
        pfw->selno = tpad;
        if (tpad >= 10) {
            pfw->selno = 0;
        }
        pfw->state = 0;
        JukeMenu_Sw = flg;
        pfw->anmTime = 0;
        JukeMapAnmState = 0;
        JukeBgmReq = 0;
        for (i = 0; i < 10; i++) {
            TSJukeCDObj_Init(&pfw->cusObj[i], i);
        }
        pfw->cusObj[8].bMsk = 0;

        for (i = 0; i < 8; i++) {
            if (pP3GameState->pLog->clrCOOL[i] < 4) {
                pfw->cusObj[i].bMsk = 1;
            }
        }

        flag = 0;
        for (i = 0; i < 8; i++) {
            if (pP3GameState->pLog->clrVSCOM1[i] < 4) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            pfw->cusObj[9].bMsk = 1;
        }

        if (pfw->cusObj[pfw->selno].bMsk != 0) {
            if (pfw->cusObj[0].bMsk == 0) {
                pfw->selno = 0;
            } else {
                for (i = 1; i < 10; i++) {
                    if (pfw->cusObj[i].bMsk == 0) {
                        pfw->selno = i;
                        break;
                    }
                }
            }
        }
        return 0;
    }

    if (flg == 2) {
        JukeMenu_Sw = 0;
        JukeMapAnmState = 0;
        JukeBgmReq = 0;
        return 0;
    }

    switch (JukeMapAnmState) {
    case 0:
        if (JukeBgmReq != 0) {
            JukeMapAnmState = 0x2100;
        }
        break;
    case 0x2000:
        if (JukeBgmReq == 0) {
            JukeMapAnmState = 0x100;
        }
        break;
    case 0x100:
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[1]);
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[34]);
        JukeMapAnmState = 0x110;
        /* fallthrough */
    case 0x110:
        if (TsAnimeWait_withKeySkip(tpad, &MNS_StageMap, 0, 0xFFFFFFFF) == 0) {
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[3]);
            JukeMapAnmState = 0;
        }
        break;
    case 0x2100:
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[4]);
        JukeMapAnmState = 0x2180;
        break;
    case 0x2180:
        if (TsAnimeWait_withKeySkip(tpad, &MNS_StageMap, 0, 0xFFFFFFFF) != 0) {
            break;
        }
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[1]);
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[33]);
        JukeMapAnmState = 0x2200;
        /* fallthrough */
    case 0x2200:
        if (TsAnimeWait_withKeySkip(tpad, &MNS_StageMap, 0, 0xFFFFFFFF) == 0) {
            MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimePA[5]);
            JukeMapAnmState = 0x2000;
        }
        break;
    default:
        break;
    }

    state = pfw->state;
    pfw->anmTime++;

    switch (state) {
    case 0:
        TsJukeObjAnime(0);
        state = 0x100;
        pfw->cusObj[pfw->selno].time = 0;
        pfw->cusObj[pfw->selno].state = 2;
        /* fallthrough */
    case 0x100:
        if (TsJukeIsObjAnime(1)) {
            pfw->state = state;
            return 0;
        }
        /* fallthrough */
    case 0x1000:
        pfw->exitflg = 0;
        state = 0x1100;
        break;
    case 0x1100:
        _TsJKSetPadArrow(pfw->selno, pfw->cusObj);
        TsCMPMes_SetMes(JukeMenu_CmpMesNo[pfw->selno]);
        if (TsCheckTimeMapChange()) {
            pfw->state = state;
            return 0;
        }
        sel = pfw->selno;
        pos[0] = sel % 5;
        pos[1] = sel / 5;
        if (tpad & 0x1000) {
            _TsJKMoveCus(&pos[0], &pos[1], 0, -1, pfw->cusObj);
        } else if (tpad & 0x4000) {
            _TsJKMoveCus(&pos[0], &pos[1], 0, 1, pfw->cusObj);
        } else if (tpad & 0x8000) {
            _TsJKMoveCus(&pos[0], &pos[1], -1, 0, pfw->cusObj);
        } else if (tpad & 0x2000) {
            _TsJKMoveCus(&pos[0], &pos[1], 1, 0, pfw->cusObj);
        }
        nsel = pos[1] * 5 + pos[0];
        if (sel != nsel) {
            nsel = TSLIMIT(nsel, 0, 10);
            pfw->selno = nsel;
            pfw->cusObj[sel].state = 4;
            pfw->cusObj[sel].time = 0;
            pfw->cusObj[nsel].state = 3;
            pfw->cusObj[nsel].time = 0;
            TSSNDPLAY(2);
        }
        if (tpad & 0x10) {
            pfw->exitflg = 1;
            TSSNDPLAY(9);
            state = 0xF020;
        } else if (tpad & 0x40) {
            pfw->exitflg = 0;
            state = 0x3000;
            pfw->cusObj[nsel].state = 5;
            pfw->cusObj[nsel].time = 0;
            TSSNDPLAY(7);
        }
        break;
    case 0x3000:
        JukeBgmReq = 1;
        TsCMPMes_SetMes(-1);
        state = 0x3010;
        TsJukeObjAnime2(0);
        MenuDataDiskSndReq(JukeBgmTbl[pfw->selno].id);
        /* fallthrough */
    case 0x3010:
        if (TsJukeIsObjAnime(0)) {
            pfw->state = state;
            return 0;
        }
        TsBGMMute(40);
        state = 0x3020;
        /* fallthrough */
    case 0x3020:
        if (!TsJukeIsObjAnime(1) && (tpad & 0x810)) {
            pfw->exitflg = 1;
            TSSNDPLAY(9);
            state = 0x6000;
        }
        if (MenuDataDiskSndReady()) {
            pfw->state = state;
            return 0;
        }
        /* fallthrough */
    case 0x3F00:
        TsBGMPause(1);
        MenuDataDiskSndPlay();
        state = 0x4000;
        MenuDataDiskVolume(128);
        memset(&pfw->MNS_StageMapW, 0, sizeof(pfw->MNS_StageMapW));
        MNScene_CopyState(&pfw->MNS_StageMapW, &MNS_StageMap);
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapAnimeSEA[1]);
        MNScene_StartAnime(&MNS_StageMap, -1, &StageMapBGMCamera[pfw->selno]);
        pfw->timeV = 0;
        break;
    case 0x4000:
        if (TsJukeIsObjAnime(1)) {
            pfw->state = state;
            return 0;
        }
        i = pfw->timeV;
        pfw->timeV = i + 1;
        if (pfw->timeV < JukeBgmTbl[pfw->selno].len) {
            if (i + 61 < JukeBgmTbl[pfw->selno].len && (tpad & 0x810)) {
                pfw->exitflg = 1;
                TSSNDPLAY(9);
                state = 0x5000;
                pfw->state = state;
                return 0;
            }
        } else {
            state = 0x6000;
        }
        break;
    case 0x5000:
        state = 0x5008;
        _bMapCaptureReq = 1;
        break;
    case 0x5008:
        mapNo = TsMENU_GetMapNo(NULL);
        TsSetScene_Map(&MNS_StageMap2, CurMapNo, CurMapOldFlg, 0);
        state = 0x5010;
        MNScene_CopyState(&MNS_StageMap, &pfw->MNS_StageMapW);
        TsMENU_SetMapScreen(mapNo);
        MNScene_CopyState(&MNS_StageMap, &pfw->MNS_StageMapW);
        MNScene_DispSw(&MNS_StageMap2, 1);
        bnk = MNScene_StartAnime(&MNS_StageMap2, -1, &StageMapAnimePA[33]);
        MNScene_SetAnimeBankEnd(&MNS_StageMap2, bnk | 0x80000000);
        MNScene_DispSw(&MNS_StageMap, 0);
        pfw->bgmFadeVol = 128;
        TsJukeObjAnime2(2);
        /* fallthrough */
    case 0x5010:
        if (pfw->bgmFadeVol > 0) {
            pfw->bgmFadeVol -= 3;
            if (pfw->bgmFadeVol < 0) {
                pfw->bgmFadeVol = 0;
            }
            MenuDataDiskVolume(pfw->bgmFadeVol);
        }
        if (TsSCFADE_Set(5, 60, 2)) {
            pfw->state = state;
            return 0;
        }
        state = 0x5020;
        MNScene_DispSw(&MNS_StageMap2, 0);
        MNScene_End(&MNS_StageMap2);
        MNScene_StartAnime(&MNS_StageMap, -1, StageMapAnimeSEA);
        MNScene_DispSw(&MNS_StageMap, 1);
        MenuDataDiskSndEnd();
        TsJukeObjAnime2(1);
        JukeBgmReq = 0;
        TsBGMPause(0);
        TsBGMPlay(pP3GameState->nStage + 1, 20);
        if (!TsJukeIsObjAnime(1)) {
            state = 0x1000;
        }
        break;
    case 0x5020:
    case 0x6020:
        if (!TsJukeIsObjAnime(1)) {
            state = 0x1000;
        }
        break;
    case 0x6000:
        TsJukeObjAnime2(2);
        MenuDataDiskSndEnd();
        state = 0x6010;
        mapNo = TsMENU_GetMapNo(NULL);
        MNScene_CopyStateMdl(&pfw->MNS_StageMapW, &MNS_StageMap);
        TsMENU_SetMapScreen(mapNo);
        MNScene_CopyState(&MNS_StageMap, &pfw->MNS_StageMapW);
        MNScene_StartAnime(&MNS_StageMap, -1, StageMapAnimeSEA);
        JukeBgmReq = 0;
        TsBGMPause(0);
        TsBGMPlay(pP3GameState->nStage + 1, 60);
        pfw->state = state;
        return 0;
    case 0x6010:
        if (!TsJukeIsObjAnime(1) || TsJukeIsObjAnime(0)) {
            TsJukeObjAnime2(1);
            state = 0x6020;
            if (!TsJukeIsObjAnime(1)) {
                state = 0x1000;
            }
        }
        break;
    case 0xF000:
    case 0xF010:
    case 0xF020:
        TsJukeObjAnime(1);
        state = 0xF080;
        /* fallthrough */
    case 0xF080:
        if (TsJukeIsObjAnime(1)) {
            pfw->state = state;
            return 0;
        }
        /* fallthrough */
    case 0xF100:
        if (pfw->exitflg == 0) {
            return -1;
        }
        JukeMenu_Sw = 0;
        state = 0xFF20;
        break;
    case 0xFF20:
        return -1;
    default:
        break;
    }

    pfw->state = state;
    return 0;
}
#endif

static void TsJukeMenu_Draw(SPR_PKT pk, SPR_PRM *spr) {
    JUKE_MENU *pfw;
    int        i;

    pfw = &JukeMenu;

    spr->zoom.isOn = FALSE;
    spr->zx = 1.0f;
    spr->zy = 0.5f;

    PkALPHA_Add(pk, 0x44);

    for (i = 0; i < PR_ARRAYSIZE(pfw->cusObj); i++) {
        if (i != pfw->selno) {
            TSJukeCDObj_Draw(pk, spr, &pfw->cusObj[i], 0, 0, pfw->anmTime);
        }
    }

    if (pfw->selno < PR_ARRAYSIZEU(pfw->cusObj)) {
        TSJukeCDObj_Draw(pk, spr, &pfw->cusObj[pfw->selno], 0, 0, pfw->anmTime);
    }

    spr->zoom.isOn = FALSE;
}

static void TsCmnCell_CusorSET(CELLOBJ *obj) {
    obj->state = 0;
    obj->flg = 1;
    obj->ton = 0x80;
}

static void TsCmnCell_CusorON(CELLOBJ *obj) {
    obj->state = 1;
    obj->flg = 0;
    obj->tim = 0x10;
}

static void TsCmnCell_CusorOFF(CELLOBJ *obj) {
    obj->state = 2;
    obj->flg = 1;
    obj->tim = 0x10;
}

static void TsCmnCell_CusorSEL(CELLOBJ *obj) {
    obj->state = 3;
    obj->flg = 1;
    obj->tim = 0x18;
}

static void TsCmnCell_CusorMASK(CELLOBJ *obj) {
    obj->state = 0;
    obj->flg = 0xffff;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsCmnCell_CusorDraw);
#else
extern PTPOS CellCusPos[];

/* static */ void TsCmnCell_CusorDraw(SPR_PKT pk, SPR_PRM *spr, int n, CELLOBJ *obj, int ox, int oy, int CurColor) {
    TSTEX_INF *ptex;
    int        flg;
    int        ton;
    int        n1;
    int        isCell;

    n1 = n + 1;
    if ((u_int)n1 >= 9) {
        return;
    }
    isCell = (u_int)n < 7;

    if (isCell) {
        if (obj != NULL) {
            int state = obj->state;

            switch (state) {
            case 1:
                if (--obj->tim == 0) {
                    obj->flg = state;
                    obj->state = 0;
                } else {
                    obj->ton = (obj->tim << 3) + 0x100;
                }
                obj->flg = 1;
                break;
            case 2:
                if (--obj->tim == 0) {
                    obj->state = 0;
                    obj->flg = 0;
                } else if (obj->tim * 16 < 0x5b) {
                    obj->flg = 0;
                    obj->ton = ((0x5a - obj->tim * 16) << 8) / 0x5a;
                } else {
                    obj->flg = 1;
                    obj->ton = (((obj->tim * 16) - 0x5a) << 8) / 0xa6;
                }
                break;
            case 3:
                if (--obj->tim == 0) {
                    obj->state = 0;
                    obj->flg = 1;
                } else {
                    float f = sinf((obj->tim & 7) * 0.125f * 3.1415927f);
                    obj->flg = 1;
                    obj->ton = (int)(f * 256.0f) + 0x100;
                }
                break;
            }

            if (obj->state == 0) {
                obj->ton = 0x100;
            }
        }
    }

    if (isCell) {
        flg = obj->flg;
        ton = obj->ton;
    } else {
        flg = 0;
        ton = 0x100;
    }

    switch (flg) {
    case 1:
        PkALPHA_Add(pk, 0x48);
        spr->rgba0 = GetDToneColor(0, CurColor, ton);
        break;
    case -1:
        return;
    case 0:
    default:
        PkALPHA_Add(pk, 0x42);
        spr->rgba0 = GetDToneColor(0x808080, 0x10808080, ton);
        break;
    }

    ptex = &tblTex[1];
    spr->zx = 1.0f;
    spr->zy = 0.47f;
    PkTEX0_Add(pk, ptex->tex0);
    spr->ux = 0;
    spr->uy = 0;
    spr->uw = ptex->w;
    spr->uh = ptex->h;
    spr->px = CellCusPos[n1].x + ox;
    spr->sw = 0x234;
    spr->py = CellCusPos[n1].y + oy;
    spr->sh = ptex->h * 2;
    PkNSprite_Add(pk, spr, 1);
    PkALPHA_Add(pk, 0x44);
}
#endif
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsOption_Flow);
#else
typedef struct { // 0x8
    int val;
    int val2;
} MNOPT_VAL;

typedef struct { // 0x10
    int val;
    PATPOS pat;
} MNOPT_ENTRY;

typedef struct { // 0x1c
    int type;
    int max;
    int min;
    MNOPT_ENTRY *list;
    int msgSel;
    int sndSel;
    int msgAdj;
} MNOPT_SELINF;

typedef struct { // 0x88
    int state;
    int cancel;
    int cur;
    MNOPT_VAL item[5];
    CELLOBJ cell[5];
    short anm[5][4];
    int arrow;
} OPTION_MENU;

extern OPTION_MENU OptionMenu; // PAL bss 1c6cf80
extern MNOPT_SELINF OptionSelTbl[]; // PAL data 19b140
extern PATPOS D_0019A558[]; // PAL data 19a558
extern PATPOS D_0019A598[][4]; // PAL data 19a598
extern int OptTmp_Lang; // PAL sdata 383850
extern int OptTmp_Subt; // PAL sdata 383854
extern int OptTmp_Vibr; // PAL sdata 383858
extern int OptTmp_Oneb; // PAL sdata 38385c
extern int OptTmp_AdjX; // PAL sdata 383860
extern int OptTmp_AdjY; // PAL sdata 383864

#define OPT_GS_ADJX(gs) (*(int *)((char *)(gs) + 0xd0))
#define OPT_GS_ADJY(gs) (*(int *)((char *)(gs) + 0xd4))

void func_00102f78(int x, int y);
void func_00103028(void);
void cmnfTim2Trans(void);
void MenuLangTim2Trans(int lang);

static int TSLOOP2(int no, int min, int max) {
    if (no < min) {
        return max - 1;
    }
    if (no < max) {
        return no;
    }
    return min;
}

static int *OptTmp_Tbl(int i) {
    switch (i) {
    case 0: return &OptTmp_Lang;
    case 1: return &OptTmp_Subt;
    case 2: return &OptTmp_Vibr;
    default: return &OptTmp_Oneb;
    }
}

static void TsOption_Init(OPTION_MENU *pfw) {
    GAME_STATUS *gs;
    int i, k;
    int *pv;
    MNOPT_SELINF *tbl;

    gs = pP3GameState->pGameStatus;
    OptTmp_AdjX = OPT_GS_ADJX(gs);
    OptTmp_Lang = gs->language_type;
    OptTmp_Subt = gs->subtitle;
    OptTmp_Vibr = gs->vibration;
    OptTmp_Oneb = gs->play_table_modeG;
    OptTmp_AdjY = OPT_GS_ADJY(gs);

    memset(pfw->cell, 0, sizeof(pfw->cell));
    memset(pfw->anm, 0, sizeof(pfw->anm));
    TsCmnCell_CusorSET(&pfw->cell[pfw->cur]);

    for (i = 0; i < 5; i++) {
        tbl = &OptionSelTbl[i];
        switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
            pv = OptTmp_Tbl(i);
            k = 0;
            if (tbl->max > 0) {
                for (k = 0; k < tbl->max; k++) {
                    if (tbl->list[k].val == *pv) {
                        break;
                    }
                }
                if (k < tbl->max) {
                    pfw->item[i].val = k;
                } else {
                    pfw->item[i].val = 0;
                }
            } else {
                pfw->item[i].val = 0;
            }
            break;
        case 4:
            pfw->item[i].val = OptTmp_AdjX;
            pfw->item[i].val2 = OptTmp_AdjY;
            break;
        }
    }

    func_00103028();
}

static void TsOption_Apply(OPTION_MENU *pfw) {
    GAME_STATUS *gs;
    int i;

    for (i = 0; i < 5; i++) {
        switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
            *OptTmp_Tbl(i) = OptionSelTbl[i].list[pfw->item[i].val].val;
            break;
        case 4:
            OptTmp_AdjX = pfw->item[i].val;
            OptTmp_AdjY = pfw->item[i].val2;
            break;
        }
    }

    gs = pP3GameState->pGameStatus;
    OPT_GS_ADJX(gs) = OptTmp_AdjX;
    gs->language_type = OptTmp_Lang;
    gs->subtitle = OptTmp_Subt;
    OPT_GS_ADJY(gs) = OptTmp_AdjY;
    gs->vibration = OptTmp_Vibr;
    gs->play_table_modeG = OptTmp_Oneb;
    func_00103028();
    cmnfTim2Trans();
    MenuLangTim2Trans(pP3GameState->pGameStatus->language_type);
}

int TsOption_Flow(int flg, u_int tpad) {
    OPTION_MENU *pfw = &OptionMenu;
    int state;
    int cur, nxt;
    int v0, v1, o0, o1;
    MNOPT_SELINF *tbl;

    if (flg == 1) {
        pfw->state = 0;
        pfw->cur = 0;
        TsOption_Init(pfw);
        pfw->arrow = -1;
        return 0;
    }
    if (flg == 2) {
        return 0;
    }

    state = pfw->state;
    switch (state) {
    case 0:
        state = 0x1000;
        /* fallthrough */
    case 0x1000:
        if (tpad & 0x40) {
            pfw->cancel = 0;
            TSSNDPLAY(6);
            state = 0x2000;
            pfw->arrow = pfw->cur;
        } else if (tpad & 0x10) {
            state = 0xF000;
            pfw->cancel = 1;
            TSSNDPLAY(9);
        } else {
            cur = pfw->cur;
            nxt = cur;
            if (tpad & 0x1000) {
                nxt = cur - 1;
            }
            if (tpad & 0x4000) {
                nxt = nxt + 1;
            }
            if (cur != nxt) {
                nxt = TSLOOP(nxt, 5);
                TsCmnCell_CusorOFF(&pfw->cell[cur]);
                TsCmnCell_CusorON(&pfw->cell[nxt]);
                pfw->cur = nxt;
                TSSNDPLAY(OptionSelTbl[nxt].sndSel);
                TSSNDPLAY(5);
                cur = pfw->cur;
            }
            TsCMPMes_SetMes(OptionSelTbl[cur].msgSel);
        }
        pfw->state = state;
        return 0;
    case 0x2000:
        cur = pfw->cur;
        v0 = o0 = pfw->item[cur].val;
        v1 = o1 = pfw->item[cur].val2;
        if (tpad & 0x8000) {
            v0--;
        }
        if (tpad & 0x2000) {
            v0++;
        }
        if (cur == 4) {
            if (tpad & 0x1000) {
                v1--;
            }
            if (tpad & 0x4000) {
                v1++;
            }
        }
        if (o0 != v0 || o1 != v1) {
            tbl = &OptionSelTbl[cur];
            if (o0 != v0) {
                pfw->anm[cur][(v0 < o0) ? 0 : 1] = 6;
            }
            if (o1 != v1) {
                pfw->anm[cur][(v1 < o1) ? 2 : 3] = 6;
            }
            if (tbl->type == 0) {
                v0 = TSLOOP2(v0, tbl->min, tbl->max);
                v1 = TSLOOP2(v1, tbl->min, tbl->max);
            } else if (tbl->type == 1) {
                v0 = TSLIMIT(v0, tbl->min, tbl->max);
                v1 = TSLIMIT(v1, tbl->min, tbl->max);
            }
            pfw->item[pfw->cur].val = v0;
            pfw->item[pfw->cur].val2 = v1;
            if (pfw->cur == 4) {
                func_00102f78(v0, v1);
            }
            TSSNDPLAY(2);
        }
        if (tpad & 0x40) {
            state = 0x1000;
            TSSNDPLAY(6);
            TsOption_Apply(pfw);
            TsCMPMes_SetMes(-1);
            pfw->arrow = -1;
            memset(pfw->anm, 0, sizeof(pfw->anm));
        }
        if (tpad & 0x10) {
            TSSNDPLAY(9);
            state = 0x1000;
            TsOption_Init(pfw);
            pfw->arrow = -1;
            memset(pfw->anm, 0, sizeof(pfw->anm));
        }
        TsCMPMes_SetMes(OptionSelTbl[pfw->cur].msgAdj);
        pfw->state = state;
        return 0;
    case 0xF000:
        if (pfw->cancel == 0) {
            TsOption_Apply(pfw);
            TsCMPMes_SetMes(-1);
        }
        pfw->state = (pfw->cancel == 0) ? 0xFF10 : 0xFF20;
        return 0;
    case 0xF100:
        pfw->state = (pfw->cancel == 0) ? 0xFF10 : 0xFF20;
        return 0;
    case 0xFF10:
        return 1;
    case 0xFF20:
        return -1;
    default:
        pfw->state = state;
        return 0;
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsOption_Draw);
#else
void TsOption_Draw(SPR_PKT pk, SPR_PRM *spr) {
    OPTION_MENU *pfw = &OptionMenu;
    char buf[32];
    float rots[4] = { -1.5707964f, 1.5707964f, 0.0f, 3.1415927f };
    int i, j;
    float zr;
    PATPOS *ppat;

    for (i = 0; i < 5; i++) {
        TsCmnCell_CusorDraw(pk, spr, i, &pfw->cell[i], 0, 0, 0x1A808080);
    }

    for (i = 0; i < 5; i++) {
        spr->rgba0 = 0x800062FF;
        TsPatPut(pk, spr, &D_0019A558[i], 0, 0);
        if (pfw->arrow == i) {
            for (j = 0; j < 4; j++) {
                ppat = &D_0019A598[i][j];
                if (ppat->texNo >= 0) {
                    if (pfw->anm[i][j] > 0) {
                        pfw->anm[i][j]--;
                        spr->rgba0 = 0x80FFFFFF;
                        zr = (float)((double)(float)pfw->anm[i][j] * 0.5 * 0.1666666716337204 + 1.0);
                    } else {
                        zr = 1.0f;
                        spr->rgba0 = 0x80808080;
                    }
                    TsPatPutRZoom(pk, spr, ppat, 0, 0, zr, rots[j]);
                }
            }
        }
    }

    PkALPHA_Add(pk, 0x44);
    spr->zx = 1.0f;
    spr->zy = 0.5f;
    spr->rgba0 = 0x80808080;
    for (i = 0; i < 5; i++) {
        if (i != 4) {
            TsPatPut(pk, spr, &OptionSelTbl[i].list[pfw->item[i].val].pat, 0, 0);
        }
    }

    sprintf(buf, "X:%d Y:%d", pfw->item[4].val, pfw->item[4].val2);
    MENUFontPutS(pk, spr, OptionSelTbl[4].list->pat.x, OptionSelTbl[4].list->pat.y, 0x80808080, 1, (u_char *)buf);
}
#endif

static int TsUserList_GetCurFileNo(int *isBroken) {
    USERLIST_MENU *pfw   = &UserListMenu;
    USER_DATA     *puser = pfw->pusrlst->pUserTbl[pfw->curuser + pfw->curPageTop];

    if (isBroken != NULL) {
        *isBroken = (puser->flg == 2);
    }

    return puser->fileNo;
}

static int TsUserList_IsGetFileSave(void) {
    USERLIST_MENU *pfw = &UserListMenu;
    return pfw->isSave;
}

static int TsUserList_SortUser(void) {
    USERLIST_MENU *pfw;
    int            i, maxn;

    pfw = &UserListMenu;
    pfw->pusrdspWk = UserDispWork;
    memset(pfw->pusrdspWk, 0, sizeof(*pfw->pusrdspWk));

    if (pfw->pusrlst == NULL) {
        return 0;
    }

    maxn = P3MC_SortUser(pfw->pusrlst, pfw->dataMode, pfw->isSave);
    for (i = 0; i < maxn; i++) {
        *(pfw->pusrdspWk->pUserDisp + i) = *pfw->pusrlst->pUserTbl[i];
    }

    return maxn;
}

static void TsUserList_SetCurUserData(USER_DATA *psrc) {
    USERLIST_MENU *pfw = &UserListMenu;
    USER_DATA     *puser;

    if (pfw->pusrlst == NULL) {
        return;
    }

    puser = pfw->pusrlst->pUserTbl[pfw->curuser + pfw->curPageTop];
    if (puser->flg == 0) {
        P3MC_AddUser(pfw->pusrlst, pfw->dataMode, psrc);
    } else {
        *puser = *psrc;
    }
}

static void TsUserList_SetCurDispUserData(USER_DATA *psrc) {
    USERLIST_MENU *pfw = &UserListMenu;

    *(pfw->pusrdspWk->pUserDisp + (pfw->curuser + pfw->curPageTop)) = *psrc;
}

/* static */ void TsUserList_SetCurFileNoCusor(int fileNo, u_int *fDate) {
    USERLIST_MENU *pfw = &UserListMenu;
    USER_DATA     *puser;
    int            i;

    if (fileNo < 0) {
        return;
    }

    if (pfw->pusrlst == NULL) {
        return;
    }

    for (i = 0; i < pfw->userMax; i++) {
        puser = pfw->pusrlst->pUserTbl[i];
        if (puser->fileNo == fileNo && *(u_int*)&puser->date_day == fDate[0] && *(u_int*)&puser->date_pad == fDate[1]) {
            break;
        }
    }

    if (i < pfw->userMax) {
        pfw->curuser = 2;
        pfw->curPageTop = i - 2;
        if (i + 3 >= pfw->userMax) {
            pfw->curPageTop = pfw->userMax - 5;
            pfw->curuser = i - pfw->curPageTop;
        }
        if (pfw->curPageTop < 0) {
            pfw->curuser += pfw->curPageTop;
            pfw->curPageTop = 0;
        }
    }
}

static void TsUserList_SetType(USERLISTTYPE_TABLE *ptbl, int mode, int curTag) {
    USERLIST_MENU *pfw = &UserListMenu;

    TsUserList_Flow(1, 0, 0);

    pfw->ptypttbl = ptbl;
    pfw->gameMode = mode;

    TsUserList_Flow(3, curTag, 0);
}

static int TsUserList_TagChangeAble(USERLIST_MENU *pfw, int *pno) {
    int flg, no;

    no = *pno;
    if (no < 0) {
        no = 0;
    }
    if (no >= pfw->ptypttbl->nType) {
        no = (pfw->ptypttbl->nType - 1);
    }

    if (UserListTbl[pfw->ptypttbl->typeNo[no]].isSave) {
        flg = UCheckSaveError;
    } else {
        flg = UCheckLoadError;
    }

    if (flg != 0) {
        *pno = no;
    }

    return flg;
}

/* static */ int TsUserList_SetCurTag(USERLIST_MENU *pfw, int no) {
    USERLIST_TYPE *ptype;
    int            fileNo;

    TsUserList_TagChangeAble(pfw, &no);
    TsUserList_Flow(2, 0, 0);

    ptype = &UserListTbl[pfw->ptypttbl->typeNo[no]];
    pfw->scene     = ptype->pScene;
    pfw->isSave    = ptype->isSave;
    pfw->cmpMesTbl = ptype->cmpMesTbl;
    pfw->dataMode  = ptype->dataMode;
    pfw->dispColor = ptype->dispColor;
    pfw->nTag      = no;

    fileNo = -1;
    if (pfw->dataMode == 1) {
        memcpy(pfw->curFileDate, CurFileInfo.logDate, sizeof(pfw->curFileDate));
        if (!pfw->isSave) {
            fileNo = CurFileInfo.logFileNo;
        } else {
            fileNo = CurFileInfo.logFileNo;
        }
    } else {
        memcpy(pfw->curFileDate, CurFileInfo.repDate, sizeof(pfw->curFileDate));
        if (!pfw->isSave) {
            fileNo = CurFileInfo.repFileNo;
        }
    }
    pfw->curFileNo = fileNo;

    return 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsUserList_Flow);
#else
extern MNANM_TBL CounterAnime[]; // PAL data (jul 18afa8)

/* static */ int TsUserList_Flow(int flg, u_int tpad, u_int tpad2) {
    USERLIST_MENU *pfw = &UserListMenu;
    int s2;
    int i;
    int r;
    int v0;
    int cnt;
    int cpg;
    int s0;
    int s1;
    int s4;
    int s7;
    int tag, ntag, pno;
    int isRep;
    float sl;
    int nameR0, nameR1;

    if (flg == 1) {
        pfw->curFileNo = -1;
        pfw->state = 0;
        pfw->wuser = UserWork;
        pfw->ptypttbl = NULL;
        pfw->scene = NULL;
        pfw->nTag = 0;
        pfw->dataMode = 0;
        pfw->dispColor = 0;
        pfw->isSave = 0;
        pfw->cmpMesTbl = NULL;
        pfw->gameMode = 0;
        pfw->curuser = 0;
        pfw->curPageTop = 0;
        pfw->isNameIn = 0;
        pfw->sline = 0.0f;
        pfw->pusrlst = NULL;
        pfw->exitflg = 0;
        pfw->userMax = TsUserList_SortUser();
        pfw->mcerrNo = 0;
        pfw->mcRetTag = 0;
        TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
        TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
        memset(pfw->cellcs, 0, 0x30);
        for (i = 4; i >= 0; i--) {
            TsCmnCell_CusorMASK(&pfw->cellcs[4 - i]);
        }
        return 0;
    }

    if (flg == 3) {
        if (pfw->ptypttbl == NULL || TsUserList_SetCurTag(pfw, tpad) == 0) {
            pfw->isNameIn = 0;
            pfw->pusrlst = UserLst;
            pfw->sline = 0.0f;
            pfw->exitflg = 0;
            pfw->userMax = TsUserList_SortUser();
            TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
            TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
            pfw->curuser = 0;
            pfw->curPageTop = 0;
            cnt = (pfw->userMax >= 6) ? 5 : pfw->userMax;
            if (pfw->curFileNo >= 0) {
                TsUserList_SetCurFileNoCusor(pfw->curFileNo, pfw->curFileDate);
            }
            if (pfw->scene != NULL) {
                MNScene_StartAnime(pfw->scene, -1, &CounterAnime[cnt]);
                MNScene_DispSw(pfw->scene, 1);
            }
            memset(pfw->cellcs, 0, 0x30);
            for (i = cnt; i < 5; i++) {
                TsCmnCell_CusorMASK(&pfw->cellcs[i]);
            }
            if (pfw->userMax > 0) {
                TsCmnCell_CusorSET(&pfw->cellcs[pfw->curuser]);
            }
            if (pfw->cmpMesTbl != NULL) {
                TsCMPMes_SetMes(pfw->cmpMesTbl[0]);
            }
        }
        return 0;
    }

    s2 = pfw->state;

    if (flg == 2) {
        pfw->isNameIn = 0;
        pfw->pusrlst = NULL;
        pfw->exitflg = 0;
        if (pfw->ptypttbl != NULL) {
            if (pfw->ptypttbl->nType > 0) {
                i = 0;
                do {
                    MNScene_DispSw(UserListTbl[pfw->ptypttbl->typeNo[i]].pScene, 0);
                    i++;
                } while (i < pfw->ptypttbl->nType);
            }
        }
        pfw->scene = NULL;
        return 0;
    }

    if (s2 <= 0xDFFF) {
        r = P3MC_CheckChange();
        if (r == 3 || r == 5) {
            TsCMPMes_SetMes(-1);
            s2 = 0xE000;
            TsMCAMes_SetMes(-1);
            pfw->isNameIn = 0;
            TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
            TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
        }
    }

    switch (s2) {
    case 0x1000:
        if (pfw->mcerrNo == 0 || McErrorMess(pfw->mcerrNo) >= 0) {
            s2 = 0x3000;
            pfw->nTag = pfw->mcRetTag;
            TsUserList_Flow(3, pfw->mcRetTag, 0);
        }
        break;
    case 0x3F00:
        pfw->wtim--;
        if (pfw->wtim <= 0) {
            s2 = 0x3F08;
        }
        break;
    case 0x4000:
        s2 = 0x4010;
        if (pfw->pusrlst->pUserTbl[pfw->curuser + pfw->curPageTop]->fileNo == 0xFFFF) {
            s2 = 0x4005;
            /* fallthrough */
    case 0x4005:
            if (McErrorMess(((pfw->dataMode ^ 2) != 0) ? 7 : 15) >= 0) {
                s2 = 0x3000;
            }
        }
        break;
    case 0x4F00:
        s2 = 0xF000;
        break;
    case 0x3000:
    case 0:
        pfw->isNameIn = 0;
        s2 = 0x3100;
        pfw->exitflg = 0;
        /* fallthrough */
    case 0x3100:
        if (pfw->cmpMesTbl != NULL) {
            TsCMPMes_SetMes(pfw->cmpMesTbl[0]);
        }
        isRep = 0;
        if (pfw->sline != 0.0f) {
            if (pfw->sline > 0.0f) {
                sl = pfw->sline - 3.625f;
                pfw->sline = sl;
                if (sl < 0.0f) {
                    pfw->sline = 0.0f;
                }
            } else {
                sl = pfw->sline + 3.625f;
                pfw->sline = sl;
                if (sl > 0.0f) {
                    pfw->sline = 0.0f;
                }
            }
            isRep = 1;
            if (pfw->sline != 0.0f) {
                break;
            }
        }

        s4 = tpad & 0x8000;
        tag = pfw->nTag;
        s1 = tag;
        if (s4 != 0) {
            s1 = tag - 1;
        }
        if (tpad & 0x2000) {
            s1 = s1 + 1;
        }
        ntag = TSLIMIT(s1, 0, pfw->ptypttbl->nType);
        if (tag != ntag) {
            if (s4 != 0) {
                TSSNDPLAY(0);
            } else {
                TSSNDPLAY(1);
            }
            pno = ntag;
            v0 = TsUserList_TagChangeAble(pfw, &pno);
            if (v0 != 0) {
                s2 = 0x1000;
                pfw->mcerrNo = v0;
                pfw->mcRetTag = pfw->nTag;
            }
            pfw->nTag = ntag;
            TsUserList_Flow(3, ntag, 0);
            break;
        }

        s1 = pfw->curuser;
        cpg = s1;
        s4 = s1;
        s7 = pfw->curPageTop;
        if (pfw->userMax > 0) {
            if (tpad & 0x1000) {
                s1 = s1 - 1;
            }
            if (tpad & 0x4000) {
                s1 = s1 + 1;
            }
            if (isRep != 0 && s1 == cpg) {
                if (TsGetMenuPadIsRepeat(0, 0) != 0) {
                    s1 = s1 - 1;
                }
                if (TsGetMenuPadIsRepeat(0, 1) != 0) {
                    s1 = s1 + 1;
                }
            }
        }
        if (s4 != s1) {
            s0 = 1;
            if (s1 < 0) {
                s0 = 2;
                pfw->curPageTop += s1;
                s1 = 0;
            }
            if (s1 >= 5) {
                s0 = 3;
                pfw->curPageTop = pfw->curPageTop - 4 + s1;
                s1 = 4;
            }
            if (pfw->curPageTop < 0) {
                pfw->curPageTop = 0;
                s0 = 0;
            }
            if (pfw->curPageTop + s1 >= pfw->userMax) {
                s0 = 0;
                if (pfw->curPageTop == 0) {
                    s1 = pfw->userMax - 1;
                } else {
                    pfw->curPageTop = pfw->userMax - (s1 + 1);
                }
            }
            pfw->curuser = s1;
            if (s0 != 0) {
                if (s1 < s4) {
                    TSSNDPLAY(3);
                } else {
                    TSSNDPLAY(4);
                }
            }
            if (s7 != pfw->curPageTop) {
                if (pfw->curPageTop < s7) {
                    pfw->sline = -29.0f;
                } else {
                    pfw->sline = 29.0f;
                }
            }
            if (s4 != s1) {
                TsCmnCell_CusorOFF(&pfw->cellcs[s4]);
                TsCmnCell_CusorON(&pfw->cellcs[s1]);
            }
            if (s0 == 2) {
                TsCmnCell_CusorOFF(&pfw->cellcs[s1 + 1]);
                TsCmnCell_CusorON(&pfw->cellcs[s1]);
            } else if (s0 == 3) {
                TsCmnCell_CusorOFF(&pfw->cellcs[s1 - 1]);
                TsCmnCell_CusorON(&pfw->cellcs[s1]);
            }
        }
        if (tpad & 0x40) {
            s2 = 0x3F00;
            TsCmnCell_CusorSEL(&pfw->cellcs[pfw->curuser]);
            pfw->wtim = 28;
            TSSNDPLAY(6);
        }
        if (tpad & 0x10) {
            s2 = 0x3F10;
            TSSNDPLAY(9);
        }
        break;
    case 0x3F08:
        s2 = (pfw->isSave != 0) ? 0x4000 : 0x5000;
        break;
    case 0x3F10:
        s2 = 0xFF20;
        pfw->exitflg = 1;
        break;
    case 0x5000:
        s2 = 0x5010;
        TsMCAMes_SetMes(0x10000016);
        /* fallthrough */
    case 0x5010:
        r = TsMCAMes_GetSelect();
        if (r != 0) {
            s2 = 0xFF10;
            if (r != 1) {
                s2 = 0x3000;
                TsMCAMes_SetMes(-1);
            }
        }
        break;
    case 0x4010:
        if (pfw->pusrlst->pUserTbl[pfw->curuser + pfw->curPageTop]->flg == 0) {
            s2 = 0x4020;
        } else {
            s2 = 0x4015;
            TsMCAMes_SetMes(0x1000000F);
    case 0x4015:
            r = TsMCAMes_GetSelect();
            if (r != 0) {
                s2 = ((r ^ 1) == 0) ? 0x4020 : 0x3000;
                TsMCAMes_SetMes(-1);
            }
        }
        break;
    case 0x4020:
        pfw->isNameIn = 1;
        pfw->wuser->fileNo = TsUserList_GetCurFileNo(NULL);
        if (pfw->dataMode == 1) {
            TsNAMEINBox_Flow(1, &pfw->nameinw[0], (u_int)pfw->wuser);
        } else {
            TsNAMEINBox_Flow(1, &pfw->nameinw[0], (u_int)pfw->wuser->name1);
        }
        if (pfw->dataMode == 1) {
            pfw->nameinw[0].dispType = 0;
        } else if (pfw->gameMode == 0) {
            pfw->nameinw[0].dispType = 0;
        } else {
            pfw->nameinw[0].dispType = 1;
            if (pfw->gameMode == 1) {
                TsNAMEINBox_Flow(1, &pfw->nameinw[1], (u_int)pfw->wuser->name2);
                pfw->nameinw[1].dispType = 2;
            }
        }
        s2 = 0x4030;
        if (pfw->cmpMesTbl != NULL) {
            TsCMPMes_SetMes(pfw->cmpMesTbl[1]);
        }
        /* fallthrough */
    case 0x4030:
        pfw->nameinw[0].isCan = 1;
        nameR1 = 1;
        nameR0 = TsNAMEINBox_Flow(0, &pfw->nameinw[0], tpad);
        if (pfw->gameMode == 1 && pfw->dataMode != 1) {
            pfw->nameinw[1].isCan = 1;
            nameR1 = TsNAMEINBox_Flow(0, &pfw->nameinw[1], tpad2);
        }
        if (nameR0 == -2 || nameR1 == -2) {
            TsNAMEINBox_Flow(3, &pfw->nameinw[0], 0);
            TsNAMEINBox_Flow(3, &pfw->nameinw[1], 0);
        }
        if (nameR0 == -1 || nameR1 == -1) {
            s2 = 0x4F10;
        }
        if (nameR0 == 1) {
            if ((nameR1 ^ 1) == 0) {
                s2 = 0x4F00;
            }
        }
        break;
    case 0x4F10:
        TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
        s2 = 0x3000;
        TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
        pfw->exitflg = 1;
        break;
    case 0xE000:
        r = P3MC_CheckChange();
        if (r == 0 || r == 5) {
            s2 = 0xFF40;
        } else {
            v0 = 3;
            if (pfw->isSave != 0) {
                v0 = ((pfw->dataMode ^ 2) == 0) ? 60 : 50;
            }
            if (McErrorMess(v0) >= 0) {
                s2 = 0xEE10;
            }
        }
        break;
    case 0xEE10:
        s2 = 0xFF20;
        break;
    case 0xF000:
        TsUserList_SetCurDispUserData(pfw->wuser);
        pfw->state = 0xF100;
        /* fallthrough */
    case 0xF100:
        TsMCAMes_SetMes(-1);
        /* fallthrough */
    case 0xFF10:
        if (P3MC_CheckChange() >= 0) {
            pfw->exitflg = 0;
            pfw->state = 0x3000;
            pfw->isNameIn = 0;
            TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
            TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
            return 1;
        }
        break;
    case 0xFF20:
        if (P3MC_CheckChange() >= 0) {
            pfw->isNameIn = 0;
            pfw->exitflg = 1;
            TsMCAMes_SetMes(-1);
            TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
            TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
            return -1;
        }
        break;
    case 0xFF40:
        pfw->isNameIn = 0;
        pfw->exitflg = 1;
        TsNAMEINBox_Flow(2, &pfw->nameinw[0], 0);
        TsNAMEINBox_Flow(2, &pfw->nameinw[1], 0);
        return -3;
    default:
        break;
    }

    pfw->state = s2;
    return 0;
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsUserList_Draw);
#else
extern PATPOS LG_SCROLL_MARK[]; // PAL data (jul 18bd30)
extern PATPOS RP_SCROLL_MARK[]; // PAL data (jul 18bd48)
extern PATPOS LLG_SCROLL_MARK[]; // PAL data (jul 18bd60)
extern PATPOS CSSLASH_MARK; // PAL data (jul 18bd78)
extern STRPOS PAGENO_StrCOD[]; // PAL data (jul 18bd88)
extern PTPOS CellCusPos[]; // PAL data (jul 18bd98)

/* static */ void TsUserList_Draw(SPR_PKT pk, SPR_PRM *spr) {
    USERLIST_MENU *pfw = &UserListMenu;
    char buf[16];
    float oy;
    int dispColor;
    PATPOS *pscr;
    int scrolling;
    int py;
    int k;
    int cnt;
    int i;
    int idx;
    int pflg;
    int tm;
    USER_DATA *pud;
    short x, y;

    oy = spr->ofsy;
    spr->zx = 1.0f;
    spr->zy = 0.5f;
    PkALPHA_Add(pk, 0x44);
    spr->rgba0 = 0x80808080;

    dispColor = pfw->dispColor;
    if (dispColor != 1) {
        if (dispColor < 2) {
            pscr = RP_SCROLL_MARK;
            if (dispColor == 0) {
                pscr = LG_SCROLL_MARK;
            }
        } else {
            pscr = RP_SCROLL_MARK;
        }
    } else {
        pscr = LLG_SCROLL_MARK;
    }
    if (pfw->curPageTop > 0) {
        TsPatPut(pk, spr, &pscr[0], 0, 0);
    }
    if (pfw->curPageTop + 5 < pfw->userMax) {
        TsPatPut(pk, spr, &pscr[1], 0, 0);
    }
    TsPatPut(pk, spr, &CSSLASH_MARK, 0, 0);

    scrolling = 1;
    sprintf(buf, "%d", pfw->curuser + pfw->curPageTop + 1);
    MENUFontPutS(pk, spr, PAGENO_StrCOD[0].x, PAGENO_StrCOD[0].y, PAGENO_StrCOD[0].abgr, 0x201, (u_char *)buf);
    sprintf(buf, "%d", pfw->userMax);
    MENUFontPutS(pk, spr, PAGENO_StrCOD[1].x, PAGENO_StrCOD[1].y, PAGENO_StrCOD[1].abgr, 0x201, (u_char *)buf);
    if (pfw->sline == 0.0f) {
        scrolling = 0;
    }

    if (scrolling != 0) {
        TsMenu_CaptureVram(pk, spr);
        py = 0x2F;
        PkSCISSOR_Add(pk, 0x26, 0x30, 0x230, 0x8F);
        spr->zy = 1.0f;
        spr->zx = 1.0f;
        spr->ofsy += pfw->sline;
        if (pfw->sline > 1.0f) {
            py = 0x12;
        }
        PkALPHA_Add(pk, ((u_long)0x80 << 32) | 0x64);
        spr->rgba0 = 0x80808080;
        spr->ux = 0x26;
        spr->uy = 0x4C;
        k = 5;
        spr->uw = 0x230;
        spr->uh = 0x1D;
        spr->py = py;
        do {
            spr->px = 0x26;
            spr->sw = 0x230;
            spr->sh = 0x1D;
            k--;
            py += 0x1D;
            PkNSprite_AddAdj(pk, spr, 1);
            if (k >= 0) {
                spr->py = py;
            }
        } while (k >= 0);
        spr->ofsy = oy;
        spr->zx = 1.0f;
        spr->zy = 0.5f;
        PkALPHA_Add(pk, 0x44);
    }

    spr->rgba0 = 0x80808080;
    cnt = 5;
    if (scrolling != 0) {
        cnt = 6;
        spr->ofsy += pfw->sline;
    }
    for (i = 0; i < cnt; i++) {
        idx = i;
        if (pfw->sline > 0.0f) {
            idx = i - 1;
        }
        if (idx < 0) {
            TsCmnCell_CusorDraw(pk, spr, idx, NULL, 0, 0, 0x20808080);
        } else {
            TsCmnCell_CusorDraw(pk, spr, idx, &pfw->cellcs[idx], 0, 0, 0x20808080);
        }
    }

    spr->ofsy = oy;
    cnt = 5;
    if (scrolling != 0) {
        cnt = 6;
        spr->ofsy = oy + pfw->sline;
    }
    for (i = 0; i < cnt; i++) {
        idx = i;
        if (pfw->sline > 0.0f) {
            idx = i - 1;
        }
        tm = idx + pfw->curPageTop;
        if (tm >= 0 && tm < pfw->userMax) {
            pud = &pfw->pusrdspWk->pUserDisp[tm];
            if (pfw->curuser == i) {
                pflg = ((pfw->nameinw[0].nameMsk ^ 1) != 0) ? 0 : 2;
                if ((pfw->nameinw[1].nameMsk ^ 1) == 0) {
                    pflg = pflg | 4;
                }
                if (pfw->nameinw[0].nameMsk != 0 || pfw->nameinw[1].nameMsk != 0) {
                    pud = pfw->wuser;
                }
            } else {
                pflg = 0;
            }
            TsUser_PanelDraw(pk, spr, pud, CellCusPos[idx + 1].x, CellCusPos[idx + 1].y, pflg, dispColor);
        }
    }

    spr->ofsy = oy;
    PkDefSCISSOR_Add(pk);
    x = CellCusPos[pfw->curuser + 1].x;
    y = CellCusPos[pfw->curuser + 1].y;
    if (pfw->nameinw[0].isOn != 0) {
        TsNAMEINBox_Draw(pk, spr, x, y, dispColor, &pfw->nameinw[0], 0);
    }
    if (pfw->nameinw[1].isOn != 0) {
        TsNAMEINBox_Draw(pk, spr, x, y, dispColor, &pfw->nameinw[1], 1);
    }
}
#endif

static void NameSpaceCut(u_char *dst, u_char *src) {
    int     i, l;
    u_char *ps;
    u_char  c;

    ps = src;

    while (*ps == ' ') {
        ps++;
    }

    l = 0;

    while ((c = *ps++) != '\0') {
        dst[l] = c;
        l++;
    }

    dst[l] = '\0';

    for (i = l - 1; i > 0; i--) {
        if (dst[i] != ' ') {
            break;
        }

        dst[i] = '\0';
    }
}

INCLUDE_RODATA("asm/nonmatchings/menu/menusub", D_00396120);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsUser_PanelDraw);
#else
extern char *StageWordStr[]; // PAL data 199568: per-language "STAGE"
extern char *RoundWordStr[]; // PAL data 199550: per-language "ROUND"
extern char *BrokenStr[]; // PAL data 199580
extern char *_MONTH_STR[]; // PAL data 199cd8
extern STRPOS LOGS_StrCOD[]; // PAL data 19a278 (6 entries)
extern STRPOS LOGL_StrCOD[]; // PAL data 19a2a8 (6 entries)
extern STRPOS REPLAY_StrCOD[]; // PAL data 19a2d8 (7 entries)
extern STRPOS VSREPLAY_StrCOD[]; // PAL data 19a310 (9 entries)
extern PATPOS VS_MARK; // PAL data 19a358
extern PATPOS VS_WINMARK1; // PAL data 19a368
extern PATPOS VS_WINMARK2; // PAL data 19a378
extern PATPOS LG_NEWDATA_MARK; // PAL data 19a388
extern PATPOS RP_NEWDATA_MARK; // PAL data 19a398

/* static */ void TsUser_PanelDraw(SPR_PKT pk, SPR_PRM *spr, USER_DATA *user, int px, int py, int pflg, int isLog) {
    char buf[32];
    char buf2[16];
    STRPOS *pstr;
    PATPOS *pmark;
    int r;
    u_int m;
    int num;

    spr->zx = 1.0f;
    spr->zy = 0.5f;
    PkALPHA_Add(pk, 0x44);

    if (user != NULL && user->flg != 0) {
        if (user->mode == 2) {
            if (user->isVs != 0) {
                spr->rgba0 = 0x80808080;
                TsPatPut(pk, spr, &VS_MARK, px, py);
                if (user->winner == 1) {
                    TsPatPut(pk, spr, &VS_WINMARK2, px, py);
                } else if (user->winner == 0) {
                    TsPatPut(pk, spr, &VS_WINMARK1, px, py);
                }
                pstr = VSREPLAY_StrCOD;
            } else {
                pstr = REPLAY_StrCOD;
            }
            sprintf(buf, "%s", StageWordStr[game_status.language_type]);
            num = user->stageNo;
            sprintf(buf2, "%d", num);
        } else {
            if (isLog != 0) {
                pstr = LOGL_StrCOD;
            } else {
                pstr = LOGS_StrCOD;
            }
            if (user->roundNo != 0) {
                r = user->roundNo + 1;
                sprintf(buf, "%s", RoundWordStr[game_status.language_type]);
                num = (u_short)((r >= 100) ? 99 : r);
                sprintf(buf2, "%d", num);
            } else {
                sprintf(buf, "%s", StageWordStr[game_status.language_type]);
                sprintf(buf2, "%d", user->stageNo);
            }
        }
        if (user->flg == 2) {
            sprintf(buf, "%s", StageWordStr[game_status.language_type]);
            sprintf(buf2, "?");
        }
        MENUFontPutS(pk, spr, pstr[0].x + px, pstr[0].y + py, pstr[0].abgr, 0x201, (u_char *)buf);
        MENUFontPutS(pk, spr, pstr[1].x + px, pstr[1].y + py, pstr[1].abgr, 0x201, (u_char *)buf2);

        if (user->flg != 2 && user->date_year != 0) {
            m = user->date_month;
            sprintf(buf, "%02x.%s.%04x", user->date_day, _MONTH_STR[(m >= 0x13U) ? 0x12 : m], user->date_year);
            MENUFontPutS(pk, spr, pstr[2].x + px, pstr[2].y + py, pstr[2].abgr, 0x201, (u_char *)buf);
            sprintf(buf, "%02x:%02x", user->date_hour, user->date_minute);
            MENUFontPutS(pk, spr, pstr[3].x + px, pstr[3].y + py, pstr[3].abgr, 0x201, (u_char *)buf);
        } else {
            MENUFontPutS(pk, spr, pstr[2].x + px, pstr[2].y + py, pstr[2].abgr, 0x201, (u_char *)"--.---.----");
            MENUFontPutS(pk, spr, pstr[3].x + px, pstr[3].y + py, pstr[3].abgr, 0x201, (u_char *)"--:--");
        }
        sprintf(buf, "%02d", user->fileNo + 1);
        MENUFontPutS(pk, spr, pstr[4].x + px, pstr[4].y + py, pstr[4].abgr, 0x201, (u_char *)buf);

        if (!(pflg & 2)) {
            if (user->mode == 1) {
                NameSpaceCut((u_char *)buf, (u_char *)user);
            } else {
                NameSpaceCut((u_char *)buf, (u_char *)user->name1);
            }
            if (user->flg == 2) {
                NameSpaceCut((u_char *)buf, (u_char *)BrokenStr[game_status.language_type]);
            }
            MENUFontPutL(pk, spr, pstr[5].x + px, pstr[5].y + py, pstr[5].abgr, 0x201, (u_char *)buf);
        }

        if (user->mode == 2) {
            sprintf(buf, "%06d", user->score);
            MENUFontPutS(pk, spr, pstr[6].x + px, pstr[6].y + py, pstr[6].abgr, 0x201, (u_char *)buf);
            if (user->isVs != 0) {
                if (!(pflg & 4)) {
                    NameSpaceCut((u_char *)buf, (u_char *)user->name2);
                    MENUFontPutL(pk, spr, pstr[7].x + px, pstr[7].y + py, pstr[7].abgr, 0x201, (u_char *)buf);
                }
                sprintf(buf, "%06d", user->score2);
                MENUFontPutS(pk, spr, pstr[8].x + px, pstr[8].y + py, pstr[8].abgr, 0x201, (u_char *)buf);
            }
        }
    } else {
        spr->rgba0 = 0x80808080;
        if (isLog >= 0 && isLog < 2) {
            pmark = &LG_NEWDATA_MARK;
        } else {
            pmark = &RP_NEWDATA_MARK;
        }
        TsPatPut(pk, spr, pmark, px, py);
    }
}
#endif

extern u_char *UserName_InitialStr;

static void TsNAMEINBox_SetName(NAMEINW *pfw, u_char *name) {
    int            i, j, k;
    u_short       *pcode;
    u_char        *ptbl;
    USERNAME_CSET *pcset;

    pcode = pfw->curnchr;
    if (name == NULL || *name == '\0') {
        name = UserName_InitialStr;
    }

    for (i = 0; i < PR_ARRAYSIZE(pfw->curnchr); i++, pcode++) {
        if (*name == '\0') {
            *pcode = 0x26;
        } else {
            pcset = UserName_CharSet;
            for (j = 0; j < 2; j++, pcset++) {
                ptbl = pcset->ptbl;
                for (k = 0; k < pcset->len; k++, ptbl++) {
                    if (*name == *ptbl) {
                        break;
                    }
                }
                if (k < pcset->len) {
                    break;
                }
            }
            if (j >= 2) {
                *pcode = 0x26;
            } else {
                *pcode = k | (j << 12);
            }
            name++;
        }
    }
}

static void TsNAMEINBox_GetName(NAMEINW *pfw, u_char *name) {
    u_short *pcode = pfw->curnchr;
    int      i;

    for (i = 0; i < PR_ARRAYSIZE(pfw->curnchr); i++, pcode++) {
        *name++ = UserName_CharSet[(*pcode / 4096)].ptbl[*pcode & 0xfff];
    }

    *name = '\0';
}

extern u_char *UserName_InitialStr2;

/* static */ int TsNAMEINBox_Flow(int flg, NAMEINW *pfw, u_int tpad) {
    int state;
    int anm;
    int pos;
    int isMaru;
    int chr;
    int i;

    if (flg == 1) {
        TsANIME_Init(&pfw->awork);
        pfw->isOn = 1;
        pfw->isCan = 1;
        pfw->nameMsk = 0;
        pfw->onTime = 0;
        pfw->desname = (char *)tpad;
        pfw->state = 0;
        pfw->dispType = 0;
        if (pfw->desname[0] != '\0') {
            pfw->curnpos = 8;
        } else {
            pfw->curnpos = 0;
        }
        pfw->curchrmode = 0;
        return 0;
    }

    if (flg == 2) {
        pfw->isOn = 0;
        pfw->state = 0;
        pfw->nameMsk = 0;
        pfw->onTime = 0;
        TsANIME_Init(&pfw->awork);
        return 0;
    }

    if (flg == 3) {
        if (pfw->isOn != 0 && pfw->state <= 0xff2f) {
            TsANIME_Init(&pfw->awork);
            pfw->nameMsk = 0;
            pfw->state = 0xff30;
        }
        return 0;
    }

    anm = TsANIME_Poll(&pfw->awork);
    state = pfw->state;

    switch (state) {
    case 0:
        TsNAMEINBox_SetName(pfw, (u_char *)pfw->desname);
        state = 0x80;
        pfw->isOn = 1;
        pfw->nameMsk = 1;
        TsANIME_Start(&pfw->awork, 2, 15);
        anm = TsANIME_Poll(&pfw->awork);
        /* fallthrough */
    case 0x80:
        if (anm != 0) {
            break;
        }
        /* fallthrough */
    case 0x1000:
        state = 0x4100;
        /* fallthrough */
    case 0x4100:
        pos = pfw->curnpos;
        if (tpad & 0x8000) {
            pos--;
        }
        if (tpad & 0x2000) {
            pos++;
        }
        if (pos < 8 && (tpad & 0x40)) {
            pos++;
            tpad &= ~0x40;
        }
        isMaru = tpad & 0x40;

        if (pfw->curnpos != pos) {
            pos = TSLOOP(pos, 9);
            pfw->curnpos = pos;
            pfw->curchrmode = pfw->curnchr[pos] >> 12;
            TSSNDPLAY(VSND_MVCUS_LR);
        }

        if (pos < 8) {
            pos = pfw->curchrmode;
            if (tpad & 0x100) {
                pos++;
            }
            if (pfw->curchrmode != pos) {
                pfw->curchrmode = TSLOOP(pos, 2);
                pfw->curnchr[pfw->curnpos] = (pfw->curnchr[pfw->curnpos] & 0xfff) | ((u_short)pfw->curchrmode << 12);
                TSSNDPLAY(VSND_MVCUS_UD);
            }

            pos = pfw->curnchr[pfw->curnpos] & 0xfff;
            chr = pos;
            if (tpad & 0x1000) {
                pos++;
            }
            if (tpad & 0x4000) {
                pos--;
            }
            if (chr != pos) {
                pfw->curnchr[pfw->curnpos] = TSLOOP(pos, UserName_CharSet[pfw->curchrmode].len - 1) | ((u_short)pfw->curchrmode << 12);
                TSSNDPLAY(VSND_MVCUS_UD);
            }
        }

        if (isMaru && pfw->curnpos == 8) {
            pfw->onTime = 30;
            TsNAMEINBox_GetName(pfw, (u_char *)pfw->desname);
            state = 0xff00;
            TSSNDPLAY(VSND_SELPOPUP);
        }

        if (pfw->isCan != 0 && (tpad & 0x10)) {
            state = 0xff20;
            TSSNDPLAY(VSND_CANCEL);
        }

        if (tpad & 0x80) {
            for (i = 7; i >= 0; i--) {
                TsNAMEINBox_SetName(pfw, UserName_InitialStr2);
                pfw->curnpos = 0;
            }
            TSSNDPLAY(VSND_CANCEL);
        }
        break;
    case 0xff00:
        if (pfw->onTime != 0) {
            pfw->onTime--;
            break;
        }
        /* fallthrough */
    case 0xff10:
        pfw->nameMsk = 2;
        TsANIME_Start(&pfw->awork, 3, 15);
        state = 0xff18;
        anm = TsANIME_Poll(&pfw->awork);
        /* fallthrough */
    case 0xff18:
        if (anm != 0) {
            break;
        }
        pfw->isOn = 0;
        return 1;
    case 0xff20:
        TsNAMEINBox_Flow(3, pfw, 0);
        return -2;
    case 0xff30:
        pfw->onTime = 0;
        pfw->nameMsk = 0;
        TsANIME_Start(&pfw->awork, 3, 15);
        state = 0xff38;
        anm = TsANIME_Poll(&pfw->awork);
        /* fallthrough */
    case 0xff38:
        if (anm != 0) {
            break;
        }
        pfw->isOn = 0;
        return -1;
    }

    pfw->state = state;
    return 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/menu/menusub", TsNAMEINBox_Draw);
#else
extern PATPOS STGCNameBox[]; // PAL data 19a3a8 (5 entries)
extern PATPOS STGCNameBoxOK; // PAL data 19a3e8
extern PATPOS VS1PNameBox[]; // PAL data 19a3f8 (5 entries)
extern PATPOS VS1PNameBoxOK; // PAL data 19a438
extern PATPOS VS2PNameBox[]; // PAL data 19a448 (5 entries)
extern PATPOS VS2PNameBoxOK; // PAL data 19a488

/* static */ void TsNAMEINBox_Draw(SPR_PKT pk, SPR_PRM *spr, int px, int py, int isLog, NAMEINW *pfw, int side) {
    char buf[2];
    float oxs, oys;
    float rt0, rt1;
    float f0, f2;
    float ang;
    PATPOS *pbox;
    PATPOS *pok;
    int ton;
    u_int col, col2, col3, c;
    int x0, y0, x, y;
    int i;
    u_short *pc;
    u_short ch;

    oxs = spr->ofsx;
    oys = spr->ofsy;
    f0 = (float)(MNSceneGetMusicFitTimer() % 360) / 360.0f;
    if (side != 0) {
        ang = 2.0f * f0 * 3.1415927f - 1.5707964f;
    } else {
        ang = 2.0f * f0 * 3.1415927f;
    }
    rt0 = sinf(ang) * 4.0f;
    spr->zx = 1.0f;
    spr->zy = 0.5f;
    spr->ofsy += rt0;

    if (pfw->dispType != 1) {
        if (pfw->dispType < 2) {
            pbox = STGCNameBox;
            pok = &STGCNameBoxOK;
        } else if (pfw->dispType != 2) {
            pbox = STGCNameBox;
            pok = &STGCNameBoxOK;
        } else {
            pok = &VS2PNameBoxOK;
            pbox = VS2PNameBox;
        }
    } else {
        pok = &VS1PNameBoxOK;
        pbox = VS1PNameBox;
    }

    if (TsANIME_GetRate(&pfw->awork, &rt0, &rt1, NULL) != 0) {
        spr->zoom.centerX = (float)(px + pbox[0].x + 0x72);
        spr->zoom.centerY = (float)(py + pbox[0].y + 0xA);
        f0 = TSNumRBack(rt1, 0.3f);
        if (!(pfw->awork.aflg & 1)) {
            f0 = (1.0f - f0) * 1.2f + 1.0f;
        }
        spr->zoom.zoomX = f0;
        f2 = TSNumRBack(rt0, 0.15f);
        spr->zoom.isOn = 1;
        spr->zoom.zoomY = f2 * f2;
        ton = (int)(rt0 * rt0 * 256.0f);
    } else {
        spr->zoom.isOn = 0;
        ton = 0x100;
    }

    col = GetDToneColor(0x808080, 0x80808080, ton);
    spr->rgba0 = col;
    PkALPHA_Add(pk, 0x44);
    pc = pfw->curnchr;
    i = 0;
    do {
        TsPatPut(pk, spr, &pbox[i], px, py);
        i++;
    } while (i < 5);

    f0 = sinf(((float)(MNSceneGetMusicFitTimer() % 12) / (float)12) * 3.1415927f) * 256.0f;
    if (pfw->onTime != 0) {
        c = GetDToneColor(0x800A6EC8, 0x80FFFFFF, (int)f0);
    } else {
        c = GetDToneColor(0x803CA0FF, 0x80B4FFFF, (int)f0);
    }
    x0 = px + pbox[0].x + 0x2A;
    y0 = py + pbox[0].y + 0xA;
    if (pfw->onTime != 0) {
        spr->px = x0;
        spr->py = y0;
        spr->sw = 0xB8;
        spr->sh = 0x19;
    } else {
        spr->py = y0;
        spr->px = pfw->curnpos * 0x14 + x0;
        spr->sh = 0x19;
        spr->sw = ((pfw->curnpos ^ 8) == 0) ? 0x18 : 0x15;
    }
    spr->rgba0 = GetDToneColor(c & 0xFFFFFF, c, ton);
    PkCRect_Add(pk, spr, 2);
    spr->rgba0 = col;
    TsPatPut(pk, spr, pok, px, py);

    x = px + pbox[0].x + 0x35;
    y = py + pbox[0].y + 0xA;
    switch (isLog) {
    case 0:
        col2 = 0x80660000;
        break;
    case 1:
        col2 = 0x801E4000;
        break;
    default:
        col2 = 0x80000061;
        break;
    }
    col3 = GetDToneColor(0x808080, col2, ton);
    i = 7;
    do {
        ch = *pc;
        buf[0] = UserName_CharSet[ch >> 12].ptbl[ch & 0xFFF];
        buf[1] = 0;
        MENUFontPutL(pk, spr, x, y, col3, 1, (u_char *)buf);
        x += 0x14;
        pc++;
        i--;
    } while (i >= 0);

    spr->ofsy = oys;
    spr->ofsx = oxs;
}
#endif

int TsSCFADE_Set(int flg, int num, int prio) {
    SCFADE *pfw = &ScFade;
    int     isSet = 0;
    int     ret;

    switch (flg) {
    case 1:
    case 5:
        if (pfw->state == flg) {
            isSet = 1;
        } else {
            TsSCFADE_Flow(1, 0);
            pfw->ton = 256;
        }
        break;
    case 2:
    case 6:
        if (pfw->state == flg) {
            isSet = 1;
        } else {
            TsSCFADE_Flow(1, 0);
            pfw->ton = 0;
        }
        break;
    default:
        isSet = 1;
        break;
    }

    if (isSet) {
        ret = pfw->ttim0 - 1;
        ret -= pfw->ttim;
        if (ret <= 0) {
            return 0;
        }
        return ret;
    }
    if (num != 0) {
        pfw->prio  = prio;
        pfw->state = flg;
        pfw->ttim0 = num;
        pfw->ttim  = 0;
    }
    return num;

}

static void TsSCFADE_Flow(int flg, int prm) {
    SCFADE *pfw = &ScFade;

    if (flg == 1 || flg == 3) {
        memset(pfw, 0, sizeof(*pfw));
        if (flg == 3) {
            pfw->ton = prm;
        }
        return;
    }

    if (pfw->ttim0 != 0) {
        pfw->ttim++;
    }

    switch (pfw->state) {
    case 1:
    case 5:
        pfw->ton = 256 - ((pfw->ttim * 256) / pfw->ttim0);
        if (pfw->ton <= 0) {
            TsSCFADE_Flow(1, 0);
        }
        break;
    case 2:
    case 6:
        pfw->ton = (pfw->ttim * 256) / pfw->ttim0;
        if (pfw->ton >= 256) {
            TsSCFADE_Flow(3, 256);
        }
        break;
    }
}

/* static */ void TsSCFADE_Draw(SPR_PKT pk, SPR_PRM *spr, int prio) {
    SCFADE *pfw = &ScFade;

    if (pfw->ton == 0) {
        return;
    }

    if (prio != pfw->prio) {
        return;
    }

    switch (pfw->state) {
    case 5:
    case 6:
        spr->rgba0 = SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0);
        spr->zy = 1.0f;
        spr->zx = 1.0f;
        PkALPHA_Add(pk, SCE_GS_SET_ALPHA(0, 1, 2, 1, (pfw->ton * 128) >> 8));
        PkSprPkt_SetTexVram(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));
        SetSprScreenXYWH(spr);
        PkNSprite_AddAdj(pk, spr, 1);
        PkALPHA_Add(pk, 0x44);
        break;
    case 1:
    case 2:
    default:
        spr->zy = 1.0f;
        spr->zx = 1.0f;
        spr->rgba0 = GetDToneColor(0, 0x80000000, pfw->ton);
        SetSprScreenXYWH(spr);
        PkALPHA_Add(pk, 0x44);
        PkCRect_Add(pk, spr, 0);
        break;
    }
}

void _PkMCMsgPut(SPR_PKT pk, SPR_PRM *spr, int id, int x, int y, u_int abgr) {
    int flg;

    if (pP3GameState != NULL) {
        flg = pP3GameState->pGameStatus->language_type;
    } else {
        flg = game_status.language_type;
    }

    MENUSubtPut(pk, spr, x, y, abgr, 1, MenuMsgGetMessageMc(id, flg), flg);
}

int _PkMCMsgGetLine(int id) {
    int flg;

    if (pP3GameState != NULL) {
        flg = pP3GameState->pGameStatus->language_type;
    } else {
        flg = game_status.language_type;
    }

    return MENUSubtGetLine(MenuMsgGetMessageMc(id, flg), flg);
}

void _PkSubMsgPut(SPR_PKT pk, SPR_PRM *spr, int id, int x, int y, u_int abgr) {
    int flg;

    if (pP3GameState != NULL) {
        flg = pP3GameState->pGameStatus->language_type;
    } else {
        flg = game_status.language_type;
    }

    MENUSubtPut(pk, spr, x, y, abgr, 1, MenuMsgGetMessageSub(id, flg), flg);
}

void TsMenu_CleanVram(int nFrm) {
    SPR_PRM        SprPrm;
    u_long128     *pkt;
    SPR_PKT        pk;
    sceGsDrawEnv1 *pdenv;

    pk = &pkt;

    pdenv = DrawGetDrawEnvP(nFrm);
    if (pdenv != NULL) {
        MnLPkt.ptop = PR_UNCACHED(MnLPkt.pkt[MnLPkt.idx].PaketTop);
        pkt = (u_long128*)MnLPkt.ptop;

        PkSprPkt_SetDefault(pk, &SprPrm, pdenv);
        PkSprPkt_SetDrawEnv(pk, &SprPrm, pdenv);
        PkZBUFMask_Add(pk, FALSE);

        SprPrm.zdepth = 0;
        SprPrm.rgba0 = 0x80000000;
    
        SetSprScreenXYWH(&SprPrm);
        PkCRect_Add(pk, &SprPrm, 0);

        PkSprPkt_SetDrawEnv(pk, &SprPrm, DrawGetDrawEnvP(DNUM_DRAW));
        PkZBUFMask_Add(pk, TRUE);

        MnLPkt.ptop = (u_int)pkt;
        sceGsSyncPath(0, 0);

        TsDrawUPacket(&MnLPkt);
        sceGsSyncPath(0, 0);
    }
}

void TsMenu_CaptureVram(SPR_PKT pk, SPR_PRM *spr) {
    PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));
    PkSprPkt_SetTexVram(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));

    PkTEX1_Add(pk, SCE_GS_SET_TEX1(0, 0, 0, 0, 0, 0, 0));
    PkALPHA_Add(pk, SCE_GS_SET_ALPHA(2, 2, 0, 0, 0));

    spr->zy = 1.0f;
    spr->zx = 1.0f;
    spr->zdepth = 0;
    spr->rgba0 = SCE_GS_SET_RGBAQ(128, 128, 128, 128, 0);

    SetSprScreenXYWH(spr);
    spr->ux = spr->px;
    spr->uy = spr->py;
    spr->uw = spr->sw;
    spr->uh = spr->sh;

    PkNSprite_AddAdj(pk, spr, 1);
    PkTEX1_Add(pk, 0x2020);

    PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));
    PkSprPkt_SetTexVram(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));

    PkALPHA_Add(pk, SCE_GS_SET_ALPHA(0, 1, 0, 1, 0));
}

void TsSetCTransSpr(SPR_PKT pk, SPR_PRM *spr, int mx, int my, float zx, float zy) {
    PKMESH *mesh;
    PKMSPT *pmspt;
    int     x, y;
    int     cx, cy;
    float   wx, wy;
    float   rx, ry;
    float   fx, fy;

    mesh = PkMesh_Create(mx, my);
    PkMesh_SetXYWH(mesh, spr->px, spr->py, spr->sw * spr->zx, spr->sh * spr->zy);
    PkMesh_SetUVWH(mesh, spr->ux, spr->uy, spr->uw, spr->uh);

    cx = (mesh->mw + 1) >> 1;
    cy = (mesh->mh + 1) >> 1;
    wx = (mesh->sw * 0.5f) * zx;
    wy = (mesh->sh * 0.5f) * zy;

    for (y = 0; y < (mesh->mh + 1); y++) {
        for (x = 0; x < (mesh->mw + 1); x++) {
            rx = (float)(cx - x) / (float)cx;
            ry = (float)(cy - y) / (float)cy;
            pmspt = &mesh->pmspt[(y * (mesh->mw + 1)) + x];

            fx = cosf(rx * 1.5707964f);
            fy = cosf(ry * 1.5707964f);

            pmspt->ofsx += (wx * (1.0f - fy)) * rx;
            pmspt->ofsy += (wy * (1.0f - fx)) * ry;
        }
    }

    PkFTMesh_Add(pk, spr, mesh);
    PkMesh_Delete(mesh);
}

void TsSetSLTransSpr(SPR_PKT pk, SPR_PRM *spr, int mx, int my, float zx) {
    PKMESH *mesh;
    int     i;
    float   oy0;
    float   zy, h, w;

    oy0 = spr->ofsy;
    zy  = 1.08f - (sinf(((zx + 1.0f) * 3.1415927f) * 0.5f) * 0.08f);
    h   = spr->sh * spr->zy;
    zy  = zy * spr->zy;

    spr->ofsy += h - (spr->sh * zy);
    w = h * cosf((zx * 0.31415927f) + 1.5707964f);

    mesh = PkMesh_Create(mx, my);
    PkMesh_SetXYWH(mesh, spr->px, spr->py, spr->sw * spr->zx, spr->sh * zy);
    PkMesh_SetUVWH(mesh, spr->ux, spr->uy, spr->uw, spr->uh);

    for (i = 0; i < (mesh->mh + 1); i++) {
        PkMesh_SetHLinOfs(mesh, i, w * cosf(((float)i / (float)mesh->mh) * 1.5707964f), 0.0f);
    }

    PkFTMesh_Add(pk, spr, mesh);
    PkMesh_Delete(mesh);

    spr->ofsy = oy0;
}

void TsSetPNTransSpr(SPR_PKT pk, SPR_PRM *spr, int mx, int my, float wr, float dr) {
    PKMESH *mesh;
    int     i;
    float   r0, r1, r2;
    float   d0, d1;

    r0 = wr * 6.2831855f;
    mesh = PkMesh_Create(mx, my);

    r1 = r0 - 3.1415927f;
    r2 = r0 + 3.1415927f;

    PkMesh_SetXYWH(mesh, spr->px, spr->py, spr->sw * spr->zx, spr->sh * spr->zy);
    PkMesh_SetUVWH(mesh, spr->ux, spr->uy, spr->uw, spr->uh);

    for (i = 0; i < (mesh->mh + 1); i++) {
        d0 = sinf(r0 + (((float)i / (float)mesh->mh) * 3.1415927f)) * 1.5f;
        d0 += 0.75f;
        d1 = (-sinf(r1 - (((float)i / (float)mesh->mh) * 4.712389f)) * 1.5f) + 0.75f;
        PkMesh_SetHLinOfsLRX(mesh, i, d0 * dr, d1 * dr);
    }

    for (i = 0; i < (mesh->mw + 1); i++) {
        d0 = cosf(r0 + (((float)i / (float)mesh->mw) * 3.1415927f)) * 0.7f;
        d1 = -cosf(r2 - (((float)i / (float)mesh->mw) * 4.712389f)) * 0.7f;
        PkMesh_SetVLinOfsUDY(mesh, i, d0 * dr, d1 * dr);
    }

    PkFTMesh_Add(pk, spr, mesh);
    PkMesh_Delete(mesh);
}

static void TsPatTexFnc(int flg) {
    _TexFunc = flg;
}

static void _TsPatSetPrm(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy) {
    TSTEX_INF *ptex;
    int        px, py, w, h;

    ptex = &tblTex[ppos->texNo];
    switch (_TexFunc) {
    case 0:
        PkTEX0_Add(pk, ptex->tex0);
        break;
    case 1:
        PkTEX0_Add(pk, ptex->tex0 | (1UL << 36));
        break;
    case 2:
        PkTEX0_Add(pk, ptex->tex0 | (3UL << 35));
        break;
    }

    spr->ux = 0;
    spr->uy = 0;
    spr->uw = ptex->w;
    spr->uh = ptex->h;

    px = ox + ppos->x;
    py = oy + ppos->y;
    w  = ppos->w;
    if (w == 0) {
        w = ptex->w;
    } else if (w == -1) {
        w = ptex->w;
        spr->ux = w - 1;
        spr->uy = 0;
        spr->uw = -(w - 2);
        spr->uh = ptex->h;
    }

    h = ppos->h;
    if (h == 0) {
        h = ptex->h;
    } else if (h == -1) {
        h = ptex->h;
        spr->ux = 0;
        spr->uy = h - 1;
        spr->uw = ptex->w;
        spr->uh = -(h - 2);
    }

    spr->px = px;
    spr->py = py;
    spr->sw = w;
    spr->sh = h;
}

static void TsPatPut(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy) {
    _TsPatSetPrm(pk, spr, ppos, ox, oy);
    PkNSprite_Add(pk, spr, 3);
}

static void TsPatGetSize(PATPOS *ppos, int *x, int *y, int *w, int *h) {
    TSTEX_INF *ptex = &tblTex[ppos->texNo];

    if (w != NULL) {
        *w = ptex->w;
    }
    if (h != NULL) {
        *h = ptex->h;
    }
    if (x != NULL) {
        *x = ppos->x + (ptex->w / 2);
    }
    if (y != NULL) {
        *y = ppos->y + (ptex->h / 2);
    }
}

static void TsPatPutRZoom(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, float zrate, float rot) {
    float zx0, zy0;
    float ox0, oy0;

    zx0 = spr->zx;
    zy0 = spr->zy;
    ox0 = spr->ofsx;
    oy0 = spr->ofsy;

    spr->zx = zx0 * zrate;
    spr->zy = zy0 * zrate;

    _TsPatSetPrm(pk, spr, ppos, ox, oy);

    spr->ofsx -= (spr->sw * (spr->zx - zx0)) * 0.5f;
    spr->ofsy -= (spr->sh * (spr->zy - zy0)) * 0.5f;

    while (rot > 3.1415927f) {
        rot -= 6.2831855f;
    }
    while (rot < -3.1415927f) {
        rot += 6.2831855f;
    }

    spr->rot = rot;
    spr->cx  = spr->sw * 0.5f;
    spr->cy  = spr->sh * 0.5f;

    PkRSprite_Add(pk, spr, 3);

    spr->zx   = zx0;
    spr->zy   = zy0;
    spr->ofsx = ox0;
    spr->ofsy = oy0;
}

static void TsPatPutMZoom(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, float Zrx, float Zry, int mx, int my, float Crx, float Cry) {
    float zx0, zy0;
    float ox0, oy0;

    zx0 = spr->zx;
    zy0 = spr->zy;
    ox0 = spr->ofsx;
    oy0 = spr->ofsy;

    spr->zx = zx0 * Zrx;
    spr->zy = zy0 * Zry;

    _TsPatSetPrm(pk, spr, ppos, ox, oy);

    spr->ofsx -= (spr->sw * (spr->zx - zx0)) * 0.5f;
    spr->ofsy -= (spr->sh * (spr->zy - zy0)) * 0.5f;

    TsSetCTransSpr(pk, spr, mx, my, Crx, Cry);

    spr->zx   = zx0;
    spr->zy   = zy0;
    spr->ofsx = ox0;
    spr->ofsy = oy0;
}

static void TsPatPutSwing(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, int mx, int my, float Crx) {
    _TsPatSetPrm(pk, spr, ppos, ox, oy);
    TsSetSLTransSpr(pk, spr, mx,my, Crx);
}

static void TsPatPutUneri(SPR_PKT pk, SPR_PRM *spr, PATPOS *ppos, int ox, int oy, int mx, int my, float Crx, float Drt) {
    _TsPatSetPrm(pk, spr, ppos, ox, oy);
    TsSetPNTransSpr(pk, spr, mx, my, Crx, Drt);
}

static void TsCELBackInit(void) {
    int        i, l;
    HOSI_TYPE *type;
    int        num;

    type = hTypeTable;
    num  = 0;
    for (l = 0; l < PR_ARRAYSIZEU(hTypeTable); l++, type++) {
        num += type->num;
    }

    if (HOSIObj != NULL) {
        free(HOSIObj);
        HOSIObj = NULL;
    }

    if (num != 0) {
        HOSIObj = malloc(num * sizeof(HOSI_TYPE));
    } else {
        HOSIObj = NULL;
    }

    if (HOSIObj != NULL) {
        HOSI_OBJ *obj;
        memset(HOSIObj, 0, num * sizeof(HOSI_TYPE));

        type = hTypeTable;
        obj  = HOSIObj;

        for (l = 0; l < PR_ARRAYSIZEU(hTypeTable); l++, type++) {
            for (i = 0; i < type->num; i++, obj++) {
                obj->wtim = (rand() % (type->dispTime >> 3)) * 2;
            }
        }
    }
}

void TsCELBackEnd(void) {
    if (HOSIObj != NULL) {
        free(HOSIObj);
        HOSIObj = NULL;
    }
}

int TsCELBackDraw(TsUSERPKT *UPacket, SPR_PRM *spr, int dispSw, int colNo) {
    u_int      bkabgr;
    u_int     *colTbl;
    u_long128 *pkt;
    SPR_PKT    pk;
    int        bDrawVram;

    pk = &pkt;

    if (dispSw != 0 && (colNo >= 0 && colNo <= 2)) {
        if (HOSIObj == NULL) {
            TsCELBackInit();
            if (HOSIObj == NULL) {
                return 0;
            }
        }
    } else {
        return 0;
    }

    bDrawVram = (dispSw ^ 0x1);
    bDrawVram &= 1;

    (u_int)pkt = UPacket->ptop = PR_UNCACHED(UPacket->pkt[UPacket->idx].PaketTop);
    PkSprPkt_SetDefault(pk, spr, DrawGetDrawEnvP((bDrawVram) ? DNUM_VRAM2 : DNUM_DRAW));

    if (bDrawVram) {
        PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_VRAM2));
    }

    colTbl = HosiColor[colNo];
    bkabgr = colTbl[0];
    PkALPHA_Add(pk, 0x44);

    spr->zx = spr->zy = 1.0f;
    SetSprScreenXYWH(spr);
    spr->rgba0 = bkabgr;

    PkCRect_Add(pk, spr, 0);
    _TsCELBackObjDraw(pk, spr, spr->sw, spr->sh, colTbl);

    if (bDrawVram) {
        PkSprPkt_SetDrawEnv(pk, spr, DrawGetDrawEnvP(DNUM_DRAW));
    }

    UPacket->ptop = (u_int)pkt;
    sceGsSyncPath(0, 0);
    TsDrawUPacket(UPacket);
    sceGsSyncPath(0, 0);

    return 1;
}

static void _TsCELBackObjDraw(SPR_PKT pk, SPR_PRM *spr, int sw, int sh, u_int *colTbl) {
    HOSI_TYPE *type;
    HOSI_OBJ  *obj;
    TSTEX_INF *ptex;
    u_int      abgr;
    int        rw, rh, ox, oy;
    int        i, l;
    int        tim, t0, t1, ton;
    float      zrate, rot;

    type = hTypeTable;
    obj  = HOSIObj;

    for (l = 0; l < PR_ARRAYSIZEU(hTypeTable); l++, type++) {
        ptex = &tblTex[type->patNo + 0x4c];
        PkTEX0_Add(pk, ptex->tex0);

        abgr = colTbl[type->colIdx];
        zrate = type->rate * 0.01f;
        spr->zy = zrate;
        spr->zx = zrate * 2.0f;

        for (i = 0; i < type->num; i++, obj++) {
            if (obj->wtim > 0) {
                obj->wtim--;
                if (obj->wtim == 0) {
                    obj->tim = type->dispTime;
                    obj->dir = rand() & 1;

                    switch (rand() % 4) {
                    case 0:
                        rw = sw;
                        rh = sh >> 2;
                        ox = 0;
                        oy = 0;
                        break;
                    case 1:
                        rw = sw >> 3;
                        rh = sh;
                        ox = 0;
                        oy = 0;
                        break;
                    case 2:
                        rw = sw >> 3;
                        rh = sh;
                        ox = sw - rw;
                        oy = 0;
                        break;
                    case 3:
                    default:
                        rw = sw;
                        rh = sh >> 2;
                        ox = 0;
                        oy = sh - rh;
                        break;
                    }

                    ox += ((rand() % (rw / (type->patW >> 1))) * (type->patW >> 1)) - 20;
                    oy += ((rand() % (rh / (type->patW >> 2))) * (type->patW >> 2)) - 10;
                    obj->px = ox;
                    obj->vx = ((sw >> 1) - ox) * 0.001f;
                    obj->py = oy;
                    obj->vy = ((sh >> 1) - oy) * 0.001f;
                }
            } else {
                obj->tim--;
                tim = obj->tim;
                if (tim <= 0) {
                    obj->wtim = (rand() % (type->dispTime >> 3)) * 3;
                } else {
                    t0  = (type->dispTime * 3) >> 2;
                    t1  = type->dispTime >> 2;
                    ton = 256;
                    if (tim > t0) {
                        ton = 256 - (((tim - t0) << 8) / t1);
                    } else if (tim < t1) {
                        ton = (tim << 8) / t1;
                    }

                    spr->rgba0 = GetDToneColor(abgr, abgr | 0x80000000, ton);

                    zrate = (float)(obj->tim % t0) / (float)t0;
                    zrate = (cosf(zrate * 6.2831855f) * 0.1) + 0.9;
                    t0 = type->dispTime >> 1;
                    rot = (float)(obj->tim % t0) / (float)t0;
                    if (obj->dir != 0) {
                        rot = -rot;
                    }
                    rot *= 6.2831855f;

                    obj->px -= obj->vx;
                    obj->py -= obj->vy;

                    TsHosiPut(pk, spr, ptex, obj->px, obj->py, zrate, rot);
                }
            }
        }
    }
}

static void TsHosiPut(SPR_PKT pk, SPR_PRM *spr, TSTEX_INF *ptex, float px, float py, float zrate, float rot) {
    float zx0, zy0;
    float ox0, oy0;

    spr->ux = 0;
    spr->uy = 0;
    spr->uw = ptex->w;
    spr->uh = ptex->h;

    zx0 = spr->zx;
    zy0 = spr->zy;
    ox0 = spr->ofsx;
    oy0 = spr->ofsy;

    spr->px = 0;
    spr->py = 0;
    spr->zx = zx0 * zrate;
    spr->zy = zy0 * zrate;

    spr->sw = ptex->w;
    spr->sh = ptex->h;

    spr->ofsx = (ox0 - ((spr->sw * (spr->zx - zx0)) * 0.5f)) + px;
    spr->ofsy = (oy0 - ((spr->sh * (spr->zy - zy0)) * 0.5f)) + py;

    while (rot > 3.1415927f) {
        rot -= 6.2831855f;
    }
    while (rot < -3.1415927f) {
        rot += 6.2831855f;
    }

    spr->rot = rot;
    spr->cx  = spr->sw * 0.5f;
    spr->cy  = spr->sh * 0.5f;

    PkRSprite_Add(pk, spr, 3);

    spr->zx   = zx0;
    spr->zy   = zy0;
    spr->ofsx = ox0;
    spr->ofsy = oy0;
}
