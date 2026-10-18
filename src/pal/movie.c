#include "common.h"

#include <eetypes.h>
#include <eekernel.h>
#include <eeregs.h>
#include <libdma.h>
#include <libgraph.h>
#include <libgifpk.h>
#include <libipu.h>
#include <libmpeg.h>
#include <libcdvd.h>
#include <sif.h>
#include <sifdev.h>
#include <sifrpc.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "os/syssub.h"
#include "os/system.h"
#include "os/usrmem.h"
#include "main/cdctrl.h"

/* External SDK declarations */
int sceGsSetDefDrawEnv(sceGsDrawEnv1 *draw, short psm, short w, short h, short ztest, short zpsm) asm("func_001583F0");
void sceGsSetDefDBuffDc(sceGsDBuffDc *db, short psm, short w, short h, short ztest, short zpsm, short clear) asm("func_00159860");
void sceGifPkRef(sceGifPacket *pPacket, u_long128 *pRef, u_int size, u_int opt1, u_int opt2, u_int flag) asm("func_0015B480");
u_int *sceGifPkReserve(sceGifPacket *pPacket, u_int count) asm("func_0015B698");
int AddIntcHandler(int, int (*)(int), int) asm("func_0015CC40");
int RemoveIntcHandler(int, int) asm("func_0015CC60");
int AddDmacHandler(int, int (*)(int), int) asm("func_0015CC70");
int RemoveDmacHandler(int, int) asm("func_0015CC90");
int sceSifDmaStat(unsigned int id) asm("func_0015D300");
unsigned int sceSifSetDma(sceSifDmaData *sdd, int len) asm("func_0015D320");
int DisableIntc(int) asm("func_0015DA30");
int EnableIntc(int) asm("func_0015DA98");
int DisableDmac(int) asm("func_0015DB00");
int EnableDmac(int) asm("func_0015DB68");
int sceRead(int fd, void *buf, int nbyte) asm("func_00161568");
void *sceSifAllocIopHeap(unsigned int) asm("func_00163D50");
int sceSifFreeIopHeap(void *) asm("func_00163EB8");
int DIntr(void) asm("func_00164FD8");
int EIntr(void) asm("func_00165020");
int sceCdStInit(u_int bufmax, u_int bankmax, u_int iop_bufaddr) asm("func_00168230");
int sceCdStStart(u_int lbn, sceCdRMode *mode) asm("func_00168260");
int sceCdStStop(void) asm("func_001682F8");
int sceCdStRead(u_int size, u_int *buf, u_int mode, u_int *err) asm("func_00168330");
int sceMpegDemuxPssRing(sceMpeg *mp, u_char *pss, int pss_size, u_char *buf_top, int buf_size) asm("func_0016A150");
sceMpegCallback sceMpegAddStrCallback(sceMpeg *mp, sceMpegStrType strType, int ch, sceMpegCallback callback, void *anyData) asm("func_0016A468");
int sceMpegInit(void) asm("func_0016ACD0");
int sceMpegCreate(sceMpeg *mp, u_char *work_area, int work_area_size) asm("func_0016AD70");
int sceMpegDelete(sceMpeg *mp) asm("func_0016AF78");
int sceMpegGetPicture(sceMpeg *mp, sceIpuRGB32 *rgb32, int mbcount) asm("func_0016AFA8");
void sceMpegSetDecodeMode(sceMpeg *mp, int ni, int np, int nb) asm("func_0016B088");
int sceMpegIsEnd(sceMpeg *mp) asm("func_0016B0C0");
int sceMpegIsRefBuffEmpty(sceMpeg *mp) asm("func_0016B0D0");
int sceMpegReset(sceMpeg *mp) asm("func_0016B0E0");
sceMpegCallback sceMpegAddCallback(sceMpeg *mp, sceMpegCbType type, sceMpegCallback callback, void *anyData) asm("func_0016B188");
int sceSdRemote(int arg, ...) asm("func_001720B0");

#define MAX_WIDTH       640
#define MAX_HEIGHT      512
#define DISP_WIDTH      640
#define DISP_HEIGHT     256

#define Bound(val, x)   ((((val) + (x) - 1) / (x)) * (x))
#define UncachedAddr(x) ((void *)(((u_int)(x) & 0x0fffffff) | 0x20000000))
#define DmaAddr(x)      ((void *)((u_int)(x) & 0x0fffffff))

#define VOBUF_STATUS_        0
#define VOBUF_STATUS_TOPDONE 1
#define VOBUF_STATUS_FULL    2

#define VOBUF_TAG_SIZE  ((MAX_WIDTH / 16 * MAX_HEIGHT / 16 * 6 + 112) * 16)

typedef struct {
    u_char v[MAX_WIDTH * MAX_HEIGHT * 4];
} VoData;

typedef struct {
    int status;
    int dummy[15];
    u_int v[2][VOBUF_TAG_SIZE / sizeof(u_int)];
} VoTag;

typedef struct {
    VoData *data;
    VoTag *tag;
    volatile int write;
    volatile int count;
    int size;
} VoBuf;

extern VoBuf voBuf;

void voBufCreate(VoBuf *f, VoData *data, VoTag *tag, int size);
void voBufDelete(VoBuf *f);
void voBufReset(VoBuf *f);
int voBufIsFull(VoBuf *f);
void voBufIncCount(VoBuf *f);
VoData *voBufGetData(VoBuf *f);
int voBufIsEmpty(VoBuf *f);
VoTag *voBufGetTag(VoBuf *f);
void voBufDecCount(VoBuf *f);

/* =========================================================================
 * audiodec.c (0x0013F8C0..0x001401F0)
 * ========================================================================= */

#define AU_STATE_INIT   0
#define AU_STATE_PRESET 1
#define AU_STATE_PLAY   2
#define AU_STATE_PAUSE  3

#define AU_HDR_SIZE     (sizeof(SpuStreamHeader) + sizeof(SpuStreamBody))
#define UNIT_SIZE       1024
#define ZERO_BUFF_SIZE  0x800
#define AUTODMA_CH      0

#define MIN(x, y)       (((x) > (y)) ? (y) : (x))
#define MAX(x, y)       (((x) < (y)) ? (y) : (x))

typedef struct {
    char id[4];
    int size;
    int type;
    int rate;
    int ch;
    int interSize;
    int loopStart;
    int loopEnd;
} SpuStreamHeader;

typedef struct {
    char id[4];
    int size;
} SpuStreamBody;

typedef struct {
    int state;
    SpuStreamHeader sshd;
    SpuStreamBody ssbd;
    int hdrCount;
    u_char *data;
    int put;
    int count;
    int size;
    int totalBytes;
    int iopBuff;
    int iopBuffSize;
    int iopLastPos;
    int iopPausePos;
    int totalBytesSent;
    int iopZero;
} AudioDec;

static u_char _0_buf[ZERO_BUFF_SIZE] PR_ALIGNED(64);

static void iopGetArea(int *pd0, int *d0, int *pd1, int *d1, AudioDec *ad, int pos);
static int sendToIOP2area(int pd0, int d0, int pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1);
static int sendToIOP(int dst, u_char *src, int size);
static void audioDecSetMasterVolume(int val);
static void audioDecSetInputVolume(int val);

int audioDecCreate(AudioDec *ad, u_char *buff, int buffSize, int iopBuffSize) {
    ad->state = AU_STATE_INIT;
    ad->hdrCount = 0;
    ad->data = buff;
    ad->put = 0;
    ad->count = 0;
    ad->size = buffSize;
    ad->totalBytes = 0;
    ad->totalBytesSent = 0;

    ad->iopBuffSize = iopBuffSize;
    ad->iopLastPos = 0;
    ad->iopPausePos = 0;

    ad->iopBuff = (int)sceSifAllocIopHeap(iopBuffSize);
    if (ad->iopBuff < 0) {
        printf("Cannot allocate IOP memory\n");
        return 0;
    }
    printf("IOP memory 0x%08x(size:%d) is allocated\n", ad->iopBuff, iopBuffSize);

    ad->iopZero = (int)sceSifAllocIopHeap(ZERO_BUFF_SIZE);
    if (ad->iopZero < 0) {
        printf("Cannot allocate IOP memory\n");
        return 0;
    }
    printf("IOP memory 0x%08x(size:%d) is allocated\n", ad->iopZero, ZERO_BUFF_SIZE);

    memset(_0_buf, 0, ZERO_BUFF_SIZE);
    sendToIOP(ad->iopZero, _0_buf, ZERO_BUFF_SIZE);

    audioDecSetMasterVolume(0x3fff);

    return 1;
}

int audioDecDelete(AudioDec *ad) {
    sceSifFreeIopHeap((void *)ad->iopBuff);
    sceSifFreeIopHeap((void *)ad->iopZero);

    audioDecSetMasterVolume(0);
    return 1;
}

void audioDecPause(AudioDec *ad) {
    int data;
    ad->state = AU_STATE_PAUSE;

    audioDecSetInputVolume(0);

    data = sceSdRemote(1, 0x80e0, AUTODMA_CH, 2, NULL, 0) & 0x00ffffff;
    ad->iopPausePos = data - ad->iopBuff;

    sceSdRemote(1, 0x80d0, AUTODMA_CH, 0, ad->iopZero, 0x4000, ZERO_BUFF_SIZE);
}

void audioDecResume(AudioDec *ad) {
    audioDecSetInputVolume(0x7fff);

    sceSdRemote(1, 0x80e0, AUTODMA_CH, 0x13,
        ad->iopBuff,
        (ad->iopBuffSize / UNIT_SIZE) * UNIT_SIZE,
        ad->iopBuff + ad->iopPausePos);

    ad->state = AU_STATE_PLAY;
}

void audioDecStart(AudioDec *ad) {
    audioDecResume(ad);
}

void audioDecReset(AudioDec *ad) {
    audioDecPause(ad);

    ad->state = AU_STATE_INIT;
    ad->hdrCount = 0;
    ad->put = 0;
    ad->count = 0;
    ad->totalBytes = 0;
    ad->totalBytesSent = 0;
    ad->iopLastPos = 0;
    ad->iopPausePos = 0;
}

void audioDecBeginPut(AudioDec *ad, u_char **ptr0, int *len0, u_char **ptr1, int *len1) {
    int len;

    if (ad->state == AU_STATE_INIT) {
        *ptr0 = (u_char *)&ad->sshd + ad->hdrCount;
        *len0 = AU_HDR_SIZE - ad->hdrCount;
        *ptr1 = (u_char *)ad->data;
        *len1 = ad->size;
        return;
    }

    len = ad->size - ad->count;

    if (ad->size - ad->put >= len) {
        *ptr0 = ad->data + ad->put;
        *len0 = len;
        *ptr1 = NULL;
        *len1 = 0;
    } else {
        *ptr0 = ad->data + ad->put;
        *len0 = ad->size - ad->put;
        *ptr1 = ad->data;
        *len1 = len - (ad->size - ad->put);
    }
}

void audioDecEndPut(AudioDec *ad, int size) {
    if (ad->state == AU_STATE_INIT) {
        int hdr_add = MIN(size, AU_HDR_SIZE - ad->hdrCount);
        ad->hdrCount += hdr_add;

        if (ad->hdrCount >= AU_HDR_SIZE) {
            ad->state = AU_STATE_PRESET;

            printf("-------- audio information --------------------\n");
            printf("[%c%c%c%c]\n"
                   "header size:                            %d\n"
                   "type(0:PCM big, 1:PCM little, 2:ADPCM): %d\n"
                   "sampling rate:                          %dHz\n"
                   "channels:                               %d\n"
                   "interleave size:                        %d\n"
                   "interleave start block address:         %d\n"
                   "interleave end block address:           %d\n",
                ad->sshd.id[0],
                ad->sshd.id[1],
                ad->sshd.id[2],
                ad->sshd.id[3],
                ad->sshd.size,
                ad->sshd.type,
                ad->sshd.rate,
                ad->sshd.ch,
                ad->sshd.interSize,
                ad->sshd.loopStart,
                ad->sshd.loopEnd
            );

            printf("[%c%c%c%c]\n"
                   "data size:                              %d\n",
                ad->ssbd.id[0],
                ad->ssbd.id[1],
                ad->ssbd.id[2],
                ad->ssbd.id[3],
                ad->ssbd.size
            );
        }
        size -= hdr_add;
    }
    ad->put = (ad->put + size) % ad->size;
    ad->count += size;
    ad->totalBytes += size;
}

int audioDecIsPreset(AudioDec *ad) {
    return ad->totalBytesSent >= ad->iopBuffSize;
}

int audioDecSendToIOP(AudioDec *ad) {
    int pd0, pd1, d0, d1;
    u_char *ps0, *ps1;
    int s0, s1;
    int count_sent = 0;
    int countAdj;
    int pos = 0;

    switch (ad->state) {
    case AU_STATE_INIT:
        return 0;
        break;

    case AU_STATE_PRESET:
        pd0 = ad->iopBuff + (ad->totalBytesSent) % ad->iopBuffSize;
        d0 = ad->iopBuffSize - ad->totalBytesSent;
        pd1 = 0;
        d1 = 0;
        break;

    case AU_STATE_PLAY:
        pos = ((sceSdRemote(1, 0x8100, AUTODMA_CH) & 0x00ffffff) - ad->iopBuff);
        iopGetArea(&pd0, &d0, &pd1, &d1, ad, pos);
        break;

    case AU_STATE_PAUSE:
        return 0;
        break;
    }

    ps0 = ad->data + (ad->put - ad->count + ad->size) % ad->size;
    ps1 = ad->data;

    countAdj = (ad->count / UNIT_SIZE) * UNIT_SIZE;

    s0 = MIN(ad->size - (ps0 - ad->data), countAdj);
    s1 = countAdj - s0;

    if (d0 + d1 >= UNIT_SIZE && s0 + s1 >= UNIT_SIZE) {
        count_sent = sendToIOP2area(pd0, d0, pd1, d1, ps0, s0, ps1, s1);
    }

    ad->count -= count_sent;

    ad->totalBytesSent += count_sent;
    ad->iopLastPos = (ad->iopLastPos + count_sent) % ad->iopBuffSize;

    return count_sent;
}

static void iopGetArea(int *pd0, int *d0, int *pd1, int *d1, AudioDec *ad, int pos) {
    int len = (pos + ad->iopBuffSize - ad->iopLastPos - UNIT_SIZE) % ad->iopBuffSize;

    len = (len / UNIT_SIZE) * UNIT_SIZE;

    if (ad->iopBuffSize - ad->iopLastPos >= len) {
        *pd0 = ad->iopBuff + ad->iopLastPos;
        *d0 = len;
        *pd1 = 0;
        *d1 = 0;
    } else {
        *pd0 = ad->iopBuff + ad->iopLastPos;
        *d0 = ad->iopBuffSize - ad->iopLastPos;
        *pd1 = ad->iopBuff;
        *d1 = len - (ad->iopBuffSize - ad->iopLastPos);
    }
}

static int sendToIOP2area(int pd0, int d0, int pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1) {
    if (d0 + d1 < s0 + s1) {
        int diff = (s0 + s1) - (d0 + d1);
        if (diff >= s1) {
            s0 -= (diff - s1);
            s1 = 0;
        } else {
            s1 -= diff;
        }
    }

    if (s0 >= d0) {
        sendToIOP(pd0,           ps0,           d0);
        sendToIOP(pd1,           ps0 + d0,      s0 - d0);
        sendToIOP(pd1 + s0 - d0, ps1,           s1);
    } else {
        if (s1 >= d0 - s0) {
            sendToIOP(pd0,      ps0,           s0);
            sendToIOP(pd0 + s0, ps1,           d0 - s0);
            sendToIOP(pd1,      ps1 + d0 - s0, s1 - (d0 - s0));
        } else {
            sendToIOP(pd0,      ps0,           s0);
            sendToIOP(pd0 + s0, ps1,           s1);
        }
    }
    return s0 + s1;
}

static int sendToIOP(int dst, u_char *src, int size) {
    sceSifDmaData transData;
    int did;

    if (size <= 0) {
        return 0;
    }

    transData.data = (u_int)src;
    transData.addr = (u_int)dst;
    transData.size = size;
    transData.mode = 0;
    FlushCache(0);

    did = sceSifSetDma(&transData, 1);

    while (sceSifDmaStat(did) >= 0)
        ;

    return size;
}

static void audioDecSetMasterVolume(int val) {
    int i;
    for (i = 0; i < 2; i++) {
        sceSdRemote(1, 0x8010, i | 0x0980, val);
        sceSdRemote(1, 0x8010, i | 0x0a80, val);
    }
}

static void audioDecSetInputVolume(int val) {
    sceSdRemote(1, 0x8010, 0x0f80, val);
    sceSdRemote(1, 0x8010, 0x1080, val);
}

/* =========================================================================
 * readbuf.h / strfile.h / vibuf.h / videodec.h / disp.h definitions
 * ========================================================================= */

#define READ_UNIT_SIZE (64 * 1024)
#define N_READ_UNIT    5
#define READBUFF_SIZE  (READ_UNIT_SIZE * N_READ_UNIT)

typedef struct {
    u_char data[READBUFF_SIZE];
    int put;
    int count;
    int size;
} ReadBuf;

void readBufCreate(ReadBuf *buff);
void readBufDelete(ReadBuf *buff);
int readBufBeginPut(ReadBuf *buff, u_char **ptr);
int readBufEndPut(ReadBuf *buff, int size);
int readBufBeginGet(ReadBuf *buff, u_char **ptr);
int readBufEndGet(ReadBuf *buff, int size);

typedef struct {
    int isOnCD;
    int size;
    sceCdlFILE fp;
    u_char *iopBuf;
    int fd;
} StrFile;

int strFileOpen(StrFile *file, FILE_STR *fstr_pp);
int strFileClose(StrFile *file);
int strFileRead(StrFile *file, void *buff, int size);

#define DMA_ID_REFE 0
#define DMA_ID_NEXT 2
#define DMA_ID_REF  3

#define VIBUF_ELM_SIZE 2048
#define VIBUF_SIZE     256
#define VIBUF_TS_SIZE  (VIBUF_SIZE * 2)

#define TS_NONE (-1)

typedef struct {
    long pts;
    long dts;
    int pos;
    int len;
} TimeStamp;

typedef struct {
    u_long128 *data;
    u_long128 *tag;
    int n;
    int dmaStart;
    int dmaN;
    int readBytes;
    int buffSize;
    sceIpuDmaEnv env;
    int sema;
    int isActive;
    long totalBytes;
    TimeStamp *ts;
    int n_ts;
    int count_ts;
    int wt_ts;
} ViBuf;

int viBufCreate(ViBuf *f, u_long128 *data, u_long128 *tag, int size, TimeStamp *ts, int n_ts);
int viBufReset(ViBuf *f);
void viBufBeginPut(ViBuf *f, u_char **ptr0, int *len0, u_char **ptr1, int *len1);
void viBufEndPut(ViBuf *f, int size);
int viBufAddDMA(ViBuf *f);
int viBufStopDMA(ViBuf *f);
int viBufRestartDMA(ViBuf *f);
int viBufDelete(ViBuf *f);
int viBufIsActive(ViBuf *f);
int viBufCount(ViBuf *f);
void viBufFlush(ViBuf *f);
int viBufModifyPts(ViBuf *f, TimeStamp *new_ts);
int viBufPutTs(ViBuf *f, TimeStamp *ts);
int viBufGetTs(ViBuf *f, TimeStamp *ts);

#define VD_STATE_NORMAL 0
#define VD_STATE_ABORT  1
#define VD_STATE_FLUSH  2
#define VD_STATE_END    3

typedef struct {
    sceMpeg mpeg;
    ViBuf vibuf;
    u_int state;
    int sema;
    int hid_endimage;
    int hid_vblank;
} VideoDec;

int videoDecCreate(VideoDec *vd, u_char *mpegWork, int mpegWorkSize, u_long128 *data, u_long128 *tag, int tagSize, TimeStamp *pts, int n_pts);
void videoDecSetDecodeMode(VideoDec *vd, int ni, int np, int nb);
int videoDecSetStream(VideoDec *vd, int strType, int ch, sceMpegCallback h, void *data);
void videoDecBeginPut(VideoDec *vd, u_char **ptr0, int *len0, u_char **ptr1, int *len1);
void videoDecEndPut(VideoDec *vd, int size);
void videoDecReset(VideoDec *vd);
int videoDecDelete(VideoDec *vd);
void videoDecAbort(VideoDec *vd);
u_int videoDecGetState(VideoDec *vd);
u_int videoDecSetState(VideoDec *vd, u_int state);
int videoDecPutTs(VideoDec *vd, long pts_val, long dts_val, int pos, int len);
int videoDecInputCount(VideoDec *vd);
int videoDecInputSpaceCount(VideoDec *vd);
int videoDecFlush(VideoDec *vd);
int videoDecIsFlushed(VideoDec *vd);
void videoDecMain(VideoDec *vd);
int decBs0(VideoDec *vd);
int mpegError(sceMpeg *mp, sceMpegCbDataError *cberror, void *anyData);
int mpegNodata(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegStopDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegRestartDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegTS(sceMpeg *mp, sceMpegCbDataTimeStamp *cbts, void *anyData);

/* =========================================================================
 * disp.c + main.c (0x001401F0..0x001415C8)
 * ========================================================================= */

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

extern volatile int isCountVblank;
extern volatile int vblankCount;
extern volatile int isFrameEnd;
extern volatile int oddeven;
extern volatile int handler_error;
extern int isWithAudio;
extern int alloc_count;
extern int videoDecTh;
extern int defaultTh;
extern int frd;

extern sceGsDBuff db;

extern VoData *voBufData;
extern VoTag *voBufTag;
extern u_long128 *viBufTag;
extern u_char *mpegWork;
extern ReadBuf *readBuf;
extern u_char *audioBuff;
extern u_long128 *viBufData;
extern TimeStamp *timeStamp;

extern VideoDec videoDec;
extern AudioDec audioDec;
extern StrFile infile;
extern PADD pad[2];

static u_long128 test_tag[1000] PR_ALIGNED(64);
static char defaultStack[0x800] PR_ALIGNED(64);
static char videoDecStack[0x4000] PR_ALIGNED(64);
static struct ThreadParam th_param;

void clearGsMem(int r, int g, int b, int disp_width, int disp_height);
void setImageTag(u_int *tags, void *image, int index, int image_w, int image_h);
void startDisplay(int waitEven);
void endDisplay(void);
int vblankHandler(int val);
int handler_endimage(int val);
void ErrMessage(char *msg);
void switchThread(void);
void proceedAudio(void);
void *DmaEndAlloc(int size, int align);
void *movieMemAlign(int size, int align);
void movieMemFree(void);
static void initAll(FILE_STR *fstr_pp);
static void termAll(void);
static void defMain(void *dummy);
static int readMpeg(VideoDec *vd, ReadBuf *rb, StrFile *file);
static int isAudioOK(void);
int videoCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data);
int pcmCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data);
static int copy2area(u_char *pd0, int d0, u_char *pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1);

void clearGsMem(int r, int g, int b, int disp_width, int disp_height) {
    const u_long giftag_clear[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1), 0x000000000000000eL };
    sceGifPacket packet;
    u_long128 *packetBase = (u_long128 *)test_tag;

    sceGsSetDefDrawEnv(&db.draw0, SCE_GS_PSMCT32, disp_width, disp_height / 2, 0, 0);
    *(u_long *)&db.draw0.xyoffset1 = SCE_GS_SET_XYOFFSET_1(0, 0);
    FlushCache(0);
    sceGsSyncPath(0, 0);

    sceGifPkInit(&packet, packetBase);
    sceGifPkReset(&packet);

    sceGifPkEnd(&packet, 0, 0, 0);

    {
        sceGifPkOpenGifTag(&packet, *(u_long128 *)&giftag_clear);
        sceGifPkAddGsAD(&packet, SCE_GS_FRAME_1,
            SCE_GS_SET_FRAME_1(0, MAX_WIDTH / 64, SCE_GS_PSMCT32, 0));
        sceGifPkAddGsAD(&packet, SCE_GS_PRIM, SCE_GS_SET_PRIM(6, 0, 0, 0, 0, 0, 0, 0, 0));
        sceGifPkAddGsAD(&packet, SCE_GS_RGBAQ, SCE_GS_SET_RGBAQ(r, g, b, 0, 0));
        sceGifPkAddGsAD(&packet, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(0 << 4, 0 << 4, 0));
        sceGifPkAddGsAD(&packet, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(MAX_WIDTH << 4, MAX_HEIGHT * 5 << 4, 0));
        sceGifPkCloseGifTag(&packet);
    }

    sceGifPkTerminate(&packet);

    FlushCache(0);
    sceGsSyncPath(0, 0);
    sceDmaSend(sceDmaGetChan(SCE_DMA_GIF), (u_long128 *)((u_int)packet.pBase));

    sceGsSyncPath(0, 0);
}

void setImageTag(u_int *tags, void *image, int index, int image_w, int image_h) {
    u_int dbp, dbw, dpsm;
    u_int dir, dsa, rrw, rrh;
    u_int xdir;
    int mbx = image_w >> 4;
    int mby = image_h >> 4;
    int i, j;
    Rect tex;
    Rect poly;

    sceGifPacket packet;
    const u_long giftag[2] = { SCE_GIF_SET_TAG(0, 0, 0, 0, 0, 1), 0x000000000000000eL };
    const u_long giftag_eop[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1), 0x000000000000000eL };

    sceGifPkInit(&packet, (u_long128 *)UncachedAddr(tags));
    sceGifPkReset(&packet);

    if (index == 0) {
        sceGifPkCnt(&packet, 0, 0, 0);
        {
            dbp = (MAX_WIDTH * MAX_HEIGHT * 2) / 64;
            dbw = MAX_WIDTH / 64;
            dpsm = SCE_GS_PSMCT32;

            dir = 0;
            dsa = 0;
            rrw = 16;
            rrh = 16;
            xdir = 0;

            sceGifPkOpenGifTag(&packet, *(u_long128 *)giftag);
            sceGifPkAddGsAD(&packet, SCE_GS_BITBLTBUF,
                SCE_GS_SET_BITBLTBUF(0, 0, 0, dbp, dbw, dpsm));
            sceGifPkAddGsAD(&packet, SCE_GS_TRXREG, SCE_GS_SET_TRXREG(rrw, rrh));
            sceGifPkCloseGifTag(&packet);
        }

        for (i = 0; i < mbx; i++) {
            for (j = 0; j < mby; j++) {
                sceGifPkCnt(&packet, 0, 0, 0);
                {
                    sceGifPkOpenGifTag(&packet, *(u_long128 *)giftag);
                    sceGifPkAddGsAD(&packet, SCE_GS_TRXPOS,
                        SCE_GS_SET_TRXPOS(0, 0, 16 * i + 0, 16 * j + 0, 0));
                    sceGifPkAddGsAD(&packet, SCE_GS_TRXDIR, SCE_GS_SET_TRXDIR(xdir));
                    sceGifPkCloseGifTag(&packet);
                }
                {
                    u_long * const tag = (u_long *)sceGifPkReserve(&packet, 4);
                    tag[0] = SCE_GIF_SET_TAG(16 * 16 * 4 / 16, 0, 0, 0, 2, 0);
                    tag[1] = 0;
                }
                sceGifPkRef(&packet, DmaAddr(image), 16 * 16 * 4 / 16, 0, 0, 0);
                image = (u_char *)image + 16 * 16 * 4;
            }
        }
    }

    tex.x = 8;
    tex.y = 8;
    tex.w = (image_w << 4) - 16;
    tex.h = (image_h << 4) - 16;

    poly.x = (2048 - 320) << 4;
    poly.y = (2048 - (DISP_HEIGHT / 2) / 2) << 4;
    poly.w = DISP_WIDTH << 4;
    poly.h = (DISP_HEIGHT / 2) << 4;

    sceGifPkEnd(&packet, 0, 0, 0);

    sceGifPkOpenGifTag(&packet, *(u_long128 *)giftag_eop);
    sceGifPkAddGsAD(&packet, SCE_GS_TEXFLUSH, 0);
    sceGifPkAddGsAD(&packet, SCE_GS_TEX1_1,
        SCE_GS_SET_TEX1_1(0, 0, 1, 1, 0, 0, 0));
    sceGifPkAddGsAD(&packet, SCE_GS_TEX0_1,
        SCE_GS_SET_TEX0_1((MAX_WIDTH * MAX_HEIGHT * 2) / 64,
            MAX_WIDTH / 64, SCE_GS_PSMCT32, 10, 10, 0, 1, 0, 0, 0, 0, 0));
    sceGifPkAddGsAD(&packet, SCE_GS_PRIM,
        SCE_GS_SET_PRIM(6, 0, 1, 0, 0, 0, 1, 0, 0));
    sceGifPkAddGsAD(&packet, SCE_GS_UV,
        SCE_GS_SET_UV(tex.x, tex.y + index * 4));
    sceGifPkAddGsAD(&packet, SCE_GS_XYZ2,
        SCE_GS_SET_XYZ2(poly.x, poly.y, 0));
    sceGifPkAddGsAD(&packet, SCE_GS_UV,
        SCE_GS_SET_UV(tex.x + tex.w, tex.y + tex.h - (1 - index) * 8));
    sceGifPkAddGsAD(&packet, SCE_GS_XYZ2,
        SCE_GS_SET_XYZ2(poly.x + poly.w, poly.y + poly.h, 0));
    sceGifPkCloseGifTag(&packet);

    sceGifPkTerminate(&packet);
}

int vblankHandler(int val) {
    sceDmaChan *dmaGif_loadimage = sceDmaGetChan(SCE_DMA_GIF);
    oddeven = ((*GS_CSR) >> 13) & 1;

    if (isCountVblank) {
        VoTag *tag;

        vblankCount++;

        handler_error = sceGsSyncPath(1, 0);
        if (!handler_error) {
            tag = voBufGetTag(&voBuf);
            if (!tag) {
                frd++;
                ExitHandler();
                return 0;
            }

            sceGsSetHalfOffset((oddeven & 1) ? (&db.draw1) : (&db.draw0), 2048, 2048, oddeven ^ 1);

            if ((oddeven == 0) && (tag->status == VOBUF_STATUS_FULL)) {
                sceGsSwapDBuff(&db, 0);
                sceGsSyncPath(0, 0);
                sceDmaSend(dmaGif_loadimage, (u_long128 *)((u_int)tag->v[0] & 0x0fffffff));

                tag->status = VOBUF_STATUS_TOPDONE;

            } else if ((oddeven == 1) && (tag->status == VOBUF_STATUS_TOPDONE)) {
                sceGsSwapDBuff(&db, 1);
                sceGsSyncPath(0, 0);
                sceDmaSend(dmaGif_loadimage, (u_long128 *)((u_int)tag->v[1] & 0x0fffffff));

                tag->status = VOBUF_STATUS_;
                isFrameEnd = 1;
            }
        }
    }
    ExitHandler();
    return 0;
}

int handler_endimage(int val) {
    if (isFrameEnd) {
        voBufDecCount(&voBuf);
        isFrameEnd = 0;
    }
    ExitHandler();
    return 0;
}

void startDisplay(int waitEven) {
    while (sceGsSyncV(0) == waitEven)
        ;

    frd = 0;
    isCountVblank = 1;
    vblankCount = 0;
}

void endDisplay(void) {
    isCountVblank = 0;
    frd = 0;
}

void *DmaEndAlloc(int size, int align) {
    u_int addr = UsrMemEndAlloc(size + align);
    WorkClear((void *)addr, size + align);
    return (void *)Bound(addr, align);
}

void *movieMemAlign(int size, int align) {
    void *ret = DmaEndAlloc(size, align);
    alloc_count++;
    return ret;
}

void movieMemFree(void) {
    int i;
    UsrMemAllocEndNext();
    for (i = 0; i < alloc_count; i++) {
        UsrMemFree();
    }
    UsrMemAllocEndNext();
    alloc_count = 0;
}

static void initAll(FILE_STR *fstr_pp) {
    *D_CTRL = *D_CTRL | 0x003;
    *D_STAT = 0x4;

    readBuf = movieMemAlign(sizeof(ReadBuf), 0x80);
    readBufCreate(readBuf);
    sceMpegInit();

    mpegWork = movieMemAlign(SCE_MPEG_BUFFER_SIZE(MAX_WIDTH, MAX_HEIGHT), 0x80);
    viBufData = movieMemAlign(VIBUF_SIZE * VIBUF_ELM_SIZE, 0x80);
    viBufTag = movieMemAlign((VIBUF_SIZE + 1) * sizeof(u_long128), 0x80);
    timeStamp = movieMemAlign(VIBUF_TS_SIZE * sizeof(TimeStamp), 0x80);

    printf("videoDecCreate in\n");
    videoDecCreate(&videoDec,
        mpegWork, SCE_MPEG_BUFFER_SIZE(MAX_WIDTH, MAX_HEIGHT),
        viBufData, viBufTag, VIBUF_SIZE, timeStamp, VIBUF_TS_SIZE);
    printf("videoDecCreate out\n");

    audioBuff = movieMemAlign(0x3000 * 4, 0x80);
    audioDecCreate(&audioDec, audioBuff, 0x3000 * 4, 0x6000);

    videoDecSetStream(&videoDec, sceMpegStrM2V, 0, (sceMpegCallback)videoCallback, readBuf);
    if (isWithAudio) {
        videoDecSetStream(&videoDec, sceMpegStrPCM, 0, (sceMpegCallback)pcmCallback, readBuf);
    }

    voBufData = movieMemAlign(2 * sizeof(VoData), 0x80);
    voBufTag = movieMemAlign(2 * sizeof(VoTag), 0x80);
    voBufCreate(&voBuf, UncachedAddr(voBufData), voBufTag, 2);

    th_param.entry = defMain;
    th_param.stack = defaultStack;
    th_param.stackSize = 0x800;
    th_param.initPriority = 17;
    th_param.gpReg = &_gp;
    th_param.option = 0;
    defaultTh = CreateThread(&th_param);
    StartThread(defaultTh, NULL);

    th_param.entry = videoDecMain;
    th_param.stack = videoDecStack;
    th_param.stackSize = 0x4000;
    th_param.initPriority = 17;
    th_param.gpReg = &_gp;
    th_param.option = 0;
    videoDecTh = CreateThread(&th_param);
    StartThread(videoDecTh, &videoDec);

    while (!strFileOpen(&infile, fstr_pp)) {
        printf("Can't Open file %s\n", fstr_pp->fname);
    }

    videoDec.hid_vblank = AddIntcHandler(INTC_VBLANK_S, vblankHandler, 0);
    EnableIntc(INTC_VBLANK_S);

    videoDec.hid_endimage = AddDmacHandler(DMAC_GIF, handler_endimage, 0);
    EnableDmac(DMAC_GIF);
}

static void termAll(void) {
    readBufDelete(readBuf);
    voBufDelete(&voBuf);

    TerminateThread(videoDecTh);
    DeleteThread(videoDecTh);

    TerminateThread(defaultTh);
    DeleteThread(defaultTh);

    DisableDmac(DMAC_GIF);
    RemoveDmacHandler(DMAC_GIF, videoDec.hid_endimage);

    DisableIntc(INTC_VBLANK_S);
    RemoveIntcHandler(INTC_VBLANK_S, videoDec.hid_vblank);

    videoDecDelete(&videoDec);
    audioDecDelete(&audioDec);

    strFileClose(&infile);

    movieMemFree();
}

void func_00140EB0(void) {
    sceGsSetDefDBuffDc(&DBufDc, SCE_GS_PSMCT32, 640, 256, SCE_GS_ZNOUSE, SCE_GS_PSMZ32, SCE_GS_CLEAR);
}

void func_00140ED8(void) {
    sceGsSetDefDBuffDc(&DBufDc, SCE_GS_PSMCT32, 640, 256, SCE_GS_ZGEQUAL, SCE_GS_PSMZ32, SCE_GS_CLEAR);
}

int func_00140F00(FILE_STR *fstr_pp) {
    int ret = 0;

    func_00140EB0();
    initAll(fstr_pp);
    ret = readMpeg(&videoDec, readBuf, &infile);
    termAll();
    func_00140ED8();

    return ret;
}

static int readMpeg(VideoDec *vd, ReadBuf *rb, StrFile *file) {
    u_char *put_ptr;
    u_char *get_ptr;
    int put_size;
    int get_size;
    int readrest = file->size;
    int writerest = file->size;
    int count;
    int proceed;
    int isStarted = 0;
    int isPaused = 0;
    int ret = 0;

    printf(" readMpeg in \n");

    while (isPaused || ((writerest > 4) && (videoDecGetState(vd) != VD_STATE_END))) {
        GPadSysRead();
        GPadRead(pad);

        if (vd->mpeg.frameCount > 10) {
            if (!isPaused && (pad[0].one & 0x800)) {
                ret = 1;
                videoDecAbort(&videoDec);
            } else if (pad[0].one & 0x80) {
                if (isPaused) {
                    startDisplay(1);
                    if (isWithAudio) {
                        audioDecResume(&audioDec);
                    }
                } else {
                    endDisplay();
                    if (isWithAudio) {
                        audioDecPause(&audioDec);
                    }
                }
                isPaused ^= 1;
            }
        }

        put_size = readBufBeginPut(rb, &put_ptr);
        if ((readrest > 0) && (put_size >= READ_UNIT_SIZE)) {
            count = strFileRead(file, put_ptr, READ_UNIT_SIZE);
            readBufEndPut(rb, count);
            readrest -= count;
        }

        switchThread();

        get_size = readBufBeginGet(rb, &get_ptr);
        if (get_size > 0) {
            proceed = sceMpegDemuxPssRing(&vd->mpeg, get_ptr, get_size, rb->data, rb->size);
            readBufEndGet(rb, proceed);
            writerest -= proceed;
        }

        proceedAudio();

        if (!isStarted && voBufIsFull(&voBuf) && isAudioOK()) {
            startDisplay(1);
            if (isWithAudio) {
                audioDecStart(&audioDec);
            }
            isStarted = 1;
        }
    }

    while (!videoDecFlush(vd)) {
        switchThread();
    }

    while (!videoDecIsFlushed(vd) && (videoDecGetState(vd) != VD_STATE_END)) {
        switchThread();
    }

    endDisplay();
    if (isWithAudio) {
        audioDecReset(&audioDec);
    }

    return ret;
}

void switchThread(void) {
    RotateThreadReadyQueue(17);
}

static int isAudioOK(void) {
    return (isWithAudio) ? audioDecIsPreset(&audioDec) : 1;
}

static void defMain(void *dummy) {
    while (1) {
        switchThread();
    }
}

void ErrMessage(char *msg) {
    printf("[ Error ] %s\n", msg);
}

void proceedAudio(void) {
    audioDecSendToIOP(&audioDec);
}

int videoCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data) {
    ReadBuf *rb = (ReadBuf *)data;
    u_char *ps0 = str->data;
    u_char *ps1 = rb->data;
    int s0 = MIN(rb->data + rb->size - str->data, str->len);
    int s1 = str->len - s0;
    u_char *pd0;
    u_char *pd1;
    int d0, d1;
    int len;

    videoDecBeginPut(&videoDec, &pd0, &d0, &pd1, &d1);
    len = copy2area(UncachedAddr(pd0), d0, UncachedAddr(pd1), d1, ps0, s0, ps1, s1);

    if (len > 0) {
        if (videoDecPutTs(&videoDec, str->pts, str->dts, (int)pd0, len) == 0) {
            ErrMessage("pts buffer overflow\n");
        }
    }

    videoDecEndPut(&videoDec, len);

    return (len > 0) ? 1 : 0;
}

int pcmCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data) {
    ReadBuf *rb = (ReadBuf *)data;
    u_char *ps0 = str->data;
    u_char *ps1 = rb->data;
    int s0;
    int s1;
    u_char *pd0;
    u_char *pd1;
    int d0, d1;
    int len;
    int ret;

    ps0 = str->data + 4;
    if (ps0 >= rb->data + rb->size) {
        ps0 -= rb->size;
    }
    len = str->len - 4;

    s0 = MIN(rb->data + rb->size - ps0, len);
    s1 = len - s0;

    audioDecBeginPut(&audioDec, &pd0, &d0, &pd1, &d1);
    ret = copy2area(pd0, d0, pd1, d1, ps0, s0, ps1, s1);
    audioDecEndPut(&audioDec, ret);

    return (ret > 0) ? 1 : 0;
}

static int copy2area(u_char *pd0, int d0, u_char *pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1) {
    if (d0 + d1 < s0 + s1) {
        return 0;
    }

    if (s0 >= d0) {
        memcpy(pd0,           ps0,           d0);
        memcpy(pd1,           ps0 + d0,      s0 - d0);
        memcpy(pd1 + s0 - d0, ps1,           s1);
    } else {
        if (s1 >= d0 - s0) {
            memcpy(pd0,      ps0,           s0);
            memcpy(pd0 + s0, ps1,           d0 - s0);
            memcpy(pd1,      ps1 + d0 - s0, s1 - (d0 - s0));
        } else {
            memcpy(pd0,      ps0,           s0);
            memcpy(pd0 + s0, ps1,           s1);
        }
    }
    return s0 + s1;
}

/* =========================================================================
 * readbuf.c (0x001415C8..0x001416D0)
 * ========================================================================= */

void readBufCreate(ReadBuf *b) {
    b->put = b->count = 0;
    b->size = READBUFF_SIZE;
}

void readBufDelete(ReadBuf *b) {
}

int readBufBeginPut(ReadBuf *b, u_char **ptr) {
    int size = b->size - b->count;
    if (size) {
        *ptr = b->data + b->put;
    }
    return size;
}

int readBufEndPut(ReadBuf *b, int size) {
    int size_can = MIN(b->size - b->count, size);
    b->put = (b->put + size_can) % b->size;
    b->count += size_can;
    return size_can;
}

int readBufBeginGet(ReadBuf *b, u_char **ptr) {
    if (b->count) {
        *ptr = b->data + (b->put - b->count + b->size) % b->size;
    }
    return b->count;
}

int readBufEndGet(ReadBuf *b, int size) {
    int size_can = MIN(b->count, size);
    b->count -= size_can;
    return size_can;
}

/* =========================================================================
 * strfile.c (0x001416D0..0x00141938)
 * ========================================================================= */

int strFileOpen(StrFile *file, FILE_STR *fstr_pp) {
    if (!CdctrlSerch(fstr_pp)) {
        return 0;
    }

    file->fp = fstr_pp->fpCd;
    file->size = fstr_pp->fpCd.size;

    file->isOnCD = (fstr_pp->frmode == FRMODE_CD);

    if (file->isOnCD) {
        sceCdRMode mode;

        file->iopBuf = sceSifAllocIopHeap(2048 * 80 + 16);
        sceCdStInit(80, 5, Bound((u_int)file->iopBuf, 16));

        mode.trycount = 0;
        mode.spindlctrl = SCECdSpinStm;
        mode.datapattern = SCECdSecS2048;
        sceCdStStart(file->fp.lsn, &mode);
    } else {
        file->fd = sceOpen(fstr_pp->fname, SCE_RDONLY);
        if (file->fd < 0) {
            printf("Cannot open '%s'(sceOpen)\n", fstr_pp->fname);
            return 0;
        }
        if (file->size < 0) {
            printf("sceLseek() fails (%s): %d\n", fstr_pp->fname, file->size);
            sceClose(file->fd);
            return 0;
        }

        if (sceLseek(file->fd, 0, SCE_SEEK_SET) < 0) {
            printf("sceLseek() fails (%s)\n", fstr_pp->fname);
            sceClose(file->fd);
            return 0;
        }
    }

    return 1;
}

int strFileClose(StrFile *file) {
    if (file->isOnCD) {
        sceCdStStop();
        sceSifFreeIopHeap((void *)file->iopBuf);
    } else {
        sceClose(file->fd);
    }
    return 1;
}

int strFileRead(StrFile *file, void *buff, int size) {
    int count;
    if (file->isOnCD) {
        u_int err;

        count = sceCdStRead(size >> 11, (u_int *)buff, STMBLK, &err);
        count <<= 11;
    } else {
        count = sceRead(file->fd, buff, size);
    }
    return count;
}

static void loadModule(char *moduleName) {
    while (sceSifLoadModule(moduleName, 0, "") < 0) {
        printf("Cannot load '%s'\n", moduleName);
    }
    printf("load '%s'\n", moduleName);
}

/* =========================================================================
 * vibuf.c (0x00141938..0x001428A0)
 * ========================================================================= */

#define QWC_SIZE 16

#define TAG_ADDR(i)     ((u_int)DmaAddr((i) + f->tag))
#define DATA_ADDR(i)    ((u_int)f->data + VIBUF_ELM_SIZE * (i))
#define WRAP_ADDR(addr) ((u_int)f->data + (((u_int)(addr) - (u_int)f->data) % (f->n * VIBUF_ELM_SIZE)))

#define WrapAdd(x, y, n) (((x) + (y)) % (n))

#define IsInRegion(i, start, len, n) ( \
    (0 <= (((i) + (n) - (start)) % (n))) && \
    ((((i) + (n) - (start)) % (n)) < (len)))

#define IsPtsInRegion(tgt, pos, len, size) \
    ((((tgt) + (size) - (pos)) % (size)) < (len))

static int getFIFOindex(ViBuf *f, void *addr) {
    if (addr == (void *)TAG_ADDR(f->n + 1)) {
        return 0;
    } else {
        return ((u_int)addr - (u_int)f->data) / VIBUF_ELM_SIZE;
    }
}

static void setD3_CHCR(u_int val) {
    DIntr();
    *D_ENABLEW = (*D_ENABLER) | 0x00010000;
    *D3_CHCR = val;
    *D_ENABLEW = (*D_ENABLER) & ~0x00010000;
    EIntr();
}

static void setD4_CHCR(u_int val) {
    DIntr();
    *D_ENABLEW = (*D_ENABLER) | 0x00010000;
    *D4_CHCR = val;
    *D_ENABLEW = (*D_ENABLER) & ~0x00010000;
    EIntr();
}

static void scTag2(
    u_int *p,
    void *addr,
    u_int id,
    u_int qwc
) {
    *(u_long *)p =
        (u_long)(u_int)addr << 32 |
        (u_long)id << 28 |
        (u_long)qwc << 0;
}

int viBufCreate(ViBuf *f, u_long128 *data, u_long128 *tag, int size, TimeStamp *ts, int n_ts) {
    struct SemaParam param;

    f->data = data;
    f->tag = UncachedAddr(tag);
    f->n = size;
    f->buffSize = size * VIBUF_ELM_SIZE;

    f->ts = ts;
    f->n_ts = n_ts;

    param.initCount = 1;
    param.maxCount = 1;
    f->sema = CreateSema(&param);

    viBufReset(f);

    f->totalBytes = 0;

    return 1;
}

int viBufReset(ViBuf *f) {
    int i;

    f->dmaStart = 0;
    f->dmaN = 0;
    f->readBytes = 0;
    f->isActive = 1;

    f->count_ts = 0;
    f->wt_ts = 0;
    for (i = 0; i < f->n_ts; i++) {
        f->ts[i].pts = TS_NONE;
        f->ts[i].dts = TS_NONE;
        f->ts[i].pos = 0;
        f->ts[i].len = 0;
    }

    for (i = 0; i < f->n; i++) {
        scTag2(
            (u_int *)(f->tag + i),
            DmaAddr(DATA_ADDR(i)),
            DMA_ID_REF,
            VIBUF_ELM_SIZE / QWC_SIZE
        );
    }

    scTag2(
        (u_int *)(f->tag + i),
        DmaAddr(f->tag),
        DMA_ID_NEXT,
        0
    );

    *D4_QWC = 0;
    *D4_MADR = (u_int)DmaAddr(f->data);
    *D4_TADR = (u_int)DmaAddr(f->tag);
    setD4_CHCR((0 << 8) | (1 << 2) | 1);

    return 1;
}

void viBufBeginPut(ViBuf *f, u_char **ptr0, int *len0, u_char **ptr1, int *len1) {
    int es;
    int en;

    WaitSema(f->sema);

    es = ((f->dmaStart + f->dmaN) * VIBUF_ELM_SIZE + f->readBytes) % f->buffSize;
    en = (f->n - (f->dmaN + 2)) * VIBUF_ELM_SIZE - f->readBytes;

    if (f->buffSize - es >= en) {
        *ptr0 = (u_char *)f->data + es;
        *len0 = en;
        *ptr1 = NULL;
        *len1 = 0;
    } else {
        *ptr0 = (u_char *)f->data + es;
        *len0 = f->buffSize - es;
        *ptr1 = (u_char *)f->data;
        *len1 = en - (f->buffSize - es);
    }

    SignalSema(f->sema);
}

void viBufEndPut(ViBuf *f, int size) {
    WaitSema(f->sema);

    f->readBytes += size;
    f->totalBytes += size;

    SignalSema(f->sema);
}

int viBufAddDMA(ViBuf *f) {
    int i;
    int index;
    int id;
    int last;
    u_int d4chcr;
    int isNewData = 0;
    int read_n, done;

    WaitSema(f->sema);

    if (!f->isActive) {
        ErrMessage("DMA ADD not active\n");
        return 0;
    }

    setD4_CHCR((DMA_ID_REFE << 28) | (0 << 8) | (1 << 2) | 1);
    d4chcr = *D4_CHCR;

    index = getFIFOindex(f, (void *)*D4_MADR);
    done = (index + f->n - f->dmaStart) % f->n;
    f->dmaStart = (f->dmaStart + done) % f->n;
    f->dmaN -= done;

    index = (f->dmaStart + f->dmaN) % f->n;
    read_n = f->readBytes / VIBUF_ELM_SIZE;
    f->readBytes = f->readBytes - read_n * VIBUF_ELM_SIZE;

    if (read_n > 0) {
        last = (f->dmaStart + f->dmaN + (f->n - 1)) % f->n;
        scTag2(
            (u_int *)(f->tag + last),
            (char *)f->data + VIBUF_ELM_SIZE * last,
            DMA_ID_REF,
            VIBUF_ELM_SIZE / 16
        );
        isNewData = 1;
    }

    for (i = 0; i < read_n; i++) {
        id = (i == read_n - 1) ? DMA_ID_REFE : DMA_ID_REF;
        scTag2(
            (u_int *)(f->tag + index),
            (char *)f->data + VIBUF_ELM_SIZE * index,
            id,
            VIBUF_ELM_SIZE / 16
        );
        index = (index + 1) % f->n;
    }

    f->dmaN += read_n;

    if (f->dmaN) {
        if (isNewData) {
            d4chcr = (d4chcr & 0x0fffffff) | (DMA_ID_REF << 28);
        }
        setD4_CHCR(d4chcr | 0x100);
    }

    SignalSema(f->sema);

    return 1;
}

int viBufStopDMA(ViBuf *f) {
    WaitSema(f->sema);

    f->isActive = 0;

    setD4_CHCR((0 << 8) | (1 << 2) | 1);
    f->env.d4madr = *D4_MADR;
    f->env.d4tadr = *D4_TADR;
    f->env.d4qwc = *D4_QWC;
    f->env.d4chcr = *D4_CHCR;

    while (DGET_IPU_CTRL() & 0xf0)
        ;

    setD3_CHCR((0 << 8) | 0);
    f->env.d3madr = *D3_MADR;
    f->env.d3qwc = *D3_QWC;
    f->env.d3chcr = *D3_CHCR;

    f->env.ipubp = DGET_IPU_BP();
    f->env.ipuctrl = DGET_IPU_CTRL();

    SignalSema(f->sema);

    return 1;
}

int viBufRestartDMA(ViBuf *f) {
    int bp;
    int fp;
    int ifc;
    u_int d4madr_next;
    u_int d4qwc_next;
    u_int d4tadr_next;
    u_int d4chcr_next;
    int index;
    int index_next;
    int id;

    bp = f->env.ipubp & 0x7f;
    fp = (f->env.ipubp >> 16) & 0x3;
    ifc = (f->env.ipubp >> 8) & 0xf;
    d4madr_next = f->env.d4madr - ((fp + ifc) << 4);
    d4qwc_next = f->env.d4qwc + (fp + ifc);
    d4tadr_next = f->env.d4tadr;
    d4chcr_next = f->env.d4chcr | 0x100;

    WaitSema(f->sema);

    if (d4madr_next < (u_int)f->data) {
        d4qwc_next = (DATA_ADDR(0) - d4madr_next) >> 4;
        d4madr_next += (u_int)(f->n * VIBUF_ELM_SIZE);
        d4tadr_next = TAG_ADDR(0);

        id = (f->env.d4madr == DATA_ADDR(0) || f->env.d4madr == DATA_ADDR(f->n))
            ? DMA_ID_REFE : DMA_ID_REF;
        d4chcr_next = (f->env.d4chcr & 0x0fffffff) | (id << 28) | 0x100;

        if (!IsInRegion(0, f->dmaStart, f->dmaN, f->n)) {
            f->dmaStart = f->n - 1;
            f->dmaN++;
        }
    } else if ((index = getFIFOindex(f, (void *)f->env.d4madr))
               != (index_next = getFIFOindex(f, (void *)d4madr_next))) {
        d4tadr_next = TAG_ADDR(index);
        d4qwc_next = (DATA_ADDR(index) - d4madr_next) >> 4;

        id = (WRAP_ADDR(f->env.d4madr) == DATA_ADDR((f->dmaStart + f->dmaN) % f->n))
            ? DMA_ID_REFE : DMA_ID_REF;
        d4chcr_next = (f->env.d4chcr & 0x0fffffff) | (id << 28) | 0x100;

        if (!IsInRegion(index_next, f->dmaStart, f->dmaN, f->n)) {
            f->dmaStart = index_next;
            f->dmaN++;
        }
    }

    if (f->env.d3madr && f->env.d3qwc) {
        *D3_MADR = f->env.d3madr;
        *D3_QWC = f->env.d3qwc;
        setD3_CHCR(f->env.d3chcr | 0x100);
    }

    if (f->dmaN) {
        while (sceIpuIsBusy())
            ;
        sceIpuBCLR(bp);
        while (sceIpuIsBusy())
            ;
    }

    *D4_MADR = d4madr_next;
    *D4_TADR = d4tadr_next;
    *D4_QWC = d4qwc_next;
    if (f->dmaN) {
        setD4_CHCR(d4chcr_next);
    }

    *IPU_CTRL = f->env.ipuctrl;

    f->isActive = 1;

    SignalSema(f->sema);

    return 1;
}

int viBufDelete(ViBuf *f) {
    setD4_CHCR((0 << 8) | (1 << 2) | 1);
    *D4_QWC = 0;
    *D4_MADR = 0;
    *D4_TADR = 0;
    DeleteSema(f->sema);
    return 1;
}

int viBufIsActive(ViBuf *f) {
    int ret;

    WaitSema(f->sema);
    ret = f->isActive;
    SignalSema(f->sema);

    return ret;
}

int viBufCount(ViBuf *f) {
    int ret;

    WaitSema(f->sema);
    ret = f->dmaN * VIBUF_ELM_SIZE + f->readBytes;
    SignalSema(f->sema);

    return ret;
}

void viBufFlush(ViBuf *f) {
    WaitSema(f->sema);

    f->readBytes = Bound(f->readBytes, VIBUF_ELM_SIZE);

    SignalSema(f->sema);
}

int viBufModifyPts(ViBuf *f, TimeStamp *new_ts) {
    TimeStamp *ts;
    int rd = (f->wt_ts - f->count_ts + f->n_ts) % f->n_ts;
    int datasize = VIBUF_ELM_SIZE * f->n;
    int loop = 1;

    if (f->count_ts > 0) {
        while (loop) {
            ts = f->ts + rd;

            if (ts->len == 0 || new_ts->len == 0) {
                break;
            }

            if (IsPtsInRegion(ts->pos, new_ts->pos, new_ts->len, datasize)) {
                int size = MIN(new_ts->pos + new_ts->len - ts->pos, ts->len);

                ts->pos = (ts->pos + size) % datasize;
                ts->len -= size;
                if (ts->len == 0) {
                    if (ts->pts >= 0) {
                        ts->pts = TS_NONE;
                        ts->dts = TS_NONE;
                        ts->pos = 0;
                        ts->len = 0;
                    }
                    f->count_ts = MAX(f->count_ts - 1, 0);
                }
            } else {
                loop = 0;
            }

            rd = (rd + 1) % f->n_ts;
        }
    }

    return 0;
}

int viBufPutTs(ViBuf *f, TimeStamp *ts) {
    int ret = 0;

    WaitSema(f->sema);

    if (f->count_ts < f->n_ts) {
        viBufModifyPts(f, ts);

        if (ts->pts >= 0 || ts->dts >= 0) {
            f->ts[f->wt_ts].pts = ts->pts;
            f->ts[f->wt_ts].dts = ts->dts;
            f->ts[f->wt_ts].pos = ts->pos;
            f->ts[f->wt_ts].len = ts->len;

            f->count_ts++;
            f->wt_ts = (f->wt_ts + 1) % f->n_ts;
        }
        ret = 1;
    }

    SignalSema(f->sema);

    return ret;
}

int viBufGetTs(ViBuf *f, TimeStamp *ts) {
    u_int d4madr = *D4_MADR;
    u_int ipubp = DGET_IPU_BP();
    int bp = f->env.ipubp & 0x7f;
    int fp = (ipubp >> 16) & 0x3;
    int ifc = (ipubp >> 8) & 0xf;
    u_int d4madr_next = d4madr - ((fp + ifc) << 4) + (bp >> 3);
    int datasize = VIBUF_ELM_SIZE * f->n;
    int isEnd = 0;
    int tscount;
    int wt;
    int i;
    int stop;

    WaitSema(f->sema);

    ts->pts = TS_NONE;
    ts->dts = TS_NONE;

    stop = (d4madr_next + datasize - (u_int)f->data) % datasize;

    tscount = f->count_ts;
    wt = f->wt_ts;

    for (i = 0; (i < tscount) && (!isEnd); i++) {
        int rd = (wt - tscount + f->n_ts + i) % f->n_ts;

        if (IsPtsInRegion(stop,
                f->ts[rd].pos,
                f->ts[rd].len,
                datasize)) {
            ts->pts = f->ts[rd].pts;
            ts->dts = f->ts[rd].dts;
            f->ts[rd].pts = TS_NONE;
            f->ts[rd].dts = TS_NONE;

            isEnd = 1;
            f->count_ts -= MIN(1, f->count_ts);
        }
    }

    SignalSema(f->sema);

    return 1;
}

/* =========================================================================
 * videodec.c (0x001428A0..0x00143040)
 * ========================================================================= */

static int cpy2area(u_char *pd0, int d0, u_char *pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1);

int videoDecCreate(VideoDec *vd, u_char *mpegWork, int mpegWorkSize, u_long128 *data, u_long128 *tag, int tagSize, TimeStamp *pts, int n_pts) {
    sceMpegCreate(&vd->mpeg, mpegWork, mpegWorkSize);

    sceMpegAddCallback(&vd->mpeg, sceMpegCbError, (sceMpegCallback)mpegError, NULL);
    sceMpegAddCallback(&vd->mpeg, sceMpegCbNodata, (sceMpegCallback)mpegNodata, NULL);
    sceMpegAddCallback(&vd->mpeg, sceMpegCbStopDMA, (sceMpegCallback)mpegStopDMA, NULL);
    sceMpegAddCallback(&vd->mpeg, sceMpegCbRestartDMA, (sceMpegCallback)mpegRestartDMA, NULL);
    sceMpegAddCallback(&vd->mpeg, sceMpegCbTimeStamp, (sceMpegCallback)mpegTS, NULL);

    videoDecReset(vd);

    viBufCreate(&vd->vibuf, data, tag, tagSize, pts, n_pts);

    return 1;
}

void videoDecSetDecodeMode(VideoDec *vd, int ni, int np, int nb) {
    sceMpegSetDecodeMode(&vd->mpeg, ni, np, nb);
}

int videoDecSetStream(VideoDec *vd, int strType, int ch, sceMpegCallback h, void *data) {
    sceMpegAddStrCallback(&vd->mpeg, strType, ch, h, data);
    return 1;
}

void videoDecBeginPut(VideoDec *vd, u_char **ptr0, int *len0, u_char **ptr1, int *len1) {
    viBufBeginPut(&vd->vibuf, ptr0, len0, ptr1, len1);
}

void videoDecEndPut(VideoDec *vd, int size) {
    viBufEndPut(&vd->vibuf, size);
}

void videoDecReset(VideoDec *vd) {
    vd->state = VD_STATE_NORMAL;
}

int videoDecDelete(VideoDec *vd) {
    viBufDelete(&vd->vibuf);
    sceMpegDelete(&vd->mpeg);
    return 1;
}

void videoDecAbort(VideoDec *vd) {
    vd->state = VD_STATE_ABORT;
}

u_int videoDecGetState(VideoDec *vd) {
    return vd->state;
}

u_int videoDecSetState(VideoDec *vd, u_int state) {
    u_int old = vd->state;
    vd->state = state;
    return old;
}

int videoDecPutTs(VideoDec *vd, long pts_val, long dts_val, int pos, int len) {
    TimeStamp ts;

    ts.pts = pts_val;
    ts.dts = dts_val;
    ts.pos = (u_char *)pos - (u_char *)vd->vibuf.data;
    ts.len = len;
    return viBufPutTs(&videoDec.vibuf, &ts);
}

int videoDecInputCount(VideoDec *vd) {
    return viBufCount(&vd->vibuf);
}

int videoDecInputSpaceCount(VideoDec *vd) {
    u_char *ptr0;
    u_char *ptr1;
    int len0, len1;

    videoDecBeginPut(vd, &ptr0, &len0, &ptr1, &len1);
    return len0 + len1;
}

int videoDecFlush(VideoDec *vd) {
    u_char *pd0;
    u_char *pd1;
    u_char seq_end_code[4] = { 0x00, 0x00, 0x01, 0xb7 };
    int d0, d1;
    u_char *ps0 = seq_end_code;
    u_char *ps1 = NULL;
    int s0 = 4;
    int s1 = 0;
    int count;

    videoDecBeginPut(vd, &pd0, &d0, &pd1, &d1);

    if (d0 + d1 < 4) {
        return 0;
    }

    count = cpy2area(UncachedAddr(pd0), d0, UncachedAddr(pd1), d1, ps0, s0, ps1, s1);

    videoDecEndPut(&videoDec, count);

    viBufFlush(&vd->vibuf);

    if (vd->state == VD_STATE_NORMAL) {
        vd->state = VD_STATE_FLUSH;
    }

    return 1;
}

int videoDecIsFlushed(VideoDec *vd) {
    return (videoDecInputCount(vd) == 0) && sceMpegIsRefBuffEmpty(&vd->mpeg);
}

void videoDecMain(VideoDec *vd) {
    viBufReset(&vd->vibuf);

    voBufReset(&voBuf);

    decBs0(vd);

    while (voBuf.count)
        ;

    videoDecSetState(vd, VD_STATE_END);
}

int decBs0(VideoDec *vd) {
    VoData *voData;
    sceIpuRGB32 *rgb32;
    int ret;
    int status = 1;
    sceMpeg *mp = &vd->mpeg;

    while (!sceMpegIsEnd(&vd->mpeg)) {
        if (videoDecGetState(vd) == VD_STATE_ABORT) {
            status = 0;
            printf("decode thread: aborted\n");
            break;
        }

        while (!(voData = voBufGetData(&voBuf))) {
            switchThread();
        }
        rgb32 = (sceIpuRGB32 *)voData->v;

        if (vd->mpeg.frameCount == 0) {
            int i;
            clearGsMem(0x00, 0x00, 0x00, vd->mpeg.width, vd->mpeg.height);
            for (i = 0; i < voBuf.size; i++) {
                setImageTag(voBuf.tag[i].v[0], voBuf.data[i].v, 0, vd->mpeg.width, vd->mpeg.height);
                setImageTag(voBuf.tag[i].v[1], voBuf.data[i].v, 1, vd->mpeg.width, vd->mpeg.height);
            }
        }

        ret = sceMpegGetPicture(mp, rgb32, MAX_WIDTH / 16 * MAX_HEIGHT / 16);

        if (ret < 0) {
            ErrMessage("sceMpegGetPicture() decode error");
        }

        if (mp->frameCount < 0) {
            continue;
        }

        voBufIncCount(&voBuf);

        switchThread();
    }

    sceMpegReset(mp);

    return status;
}

int mpegError(sceMpeg *mp, sceMpegCbDataError *cberror, void *anyData) {
    printf("%s\n", cberror->errMessage);
    return 1;
}

int mpegNodata(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData) {
    switchThread();
    viBufAddDMA(&videoDec.vibuf);
    return 1;
}

int mpegStopDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData) {
    viBufStopDMA(&videoDec.vibuf);
    return 1;
}

int mpegRestartDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData) {
    viBufRestartDMA(&videoDec.vibuf);
    return 1;
}

int mpegTS(sceMpeg *mp, sceMpegCbDataTimeStamp *cbts, void *anyData) {
    TimeStamp ts;

    viBufGetTs(&videoDec.vibuf, &ts);
    cbts->pts = ts.pts;
    cbts->dts = ts.dts;
    return 1;
}

static int cpy2area(u_char *pd0, int d0, u_char *pd1, int d1, u_char *ps0, int s0, u_char *ps1, int s1) {
    if (d0 + d1 < s0 + s1) {
        return 0;
    }

    if (s0 >= d0) {
        memcpy(pd0,           ps0,           d0);
        memcpy(pd1,           ps0 + d0,      s0 - d0);
        memcpy(pd1 + s0 - d0, ps1,           s1);
    } else {
        if (s1 >= d0 - s0) {
            memcpy(pd0,      ps0,           s0);
            memcpy(pd0 + s0, ps1,           d0 - s0);
            memcpy(pd1,      ps1 + d0 - s0, s1 - (d0 - s0));
        } else {
            memcpy(pd0,      ps0,           s0);
            memcpy(pd0 + s0, ps1,           s1);
        }
    }
    return s0 + s1;
}

/* =========================================================================
 * vobuf.c (0x00143040..0x00143200)
 * ========================================================================= */

void voBufCreate(VoBuf *f, VoData *data, VoTag *tag, int size) {
    int i;

    f->data = data;
    f->tag = tag;
    f->size = size;
    f->count = 0;
    f->write = 0;

    for (i = 0; i < size; i++) {
        f->tag[i].status = VOBUF_STATUS_;
    }
}

void voBufDelete(VoBuf *f) {
}

void voBufReset(VoBuf *f) {
    f->count = 0;
    f->write = 0;
}

int voBufIsFull(VoBuf *f) {
    return f->count == f->size;
}

void voBufIncCount(VoBuf *f) {
    DIntr();
    f->tag[f->write].status = VOBUF_STATUS_FULL;
    f->count++;
    f->write = (f->write + 1) % f->size;
    EIntr();
}

VoData *voBufGetData(VoBuf *f) {
    return voBufIsFull(f) ? (VoData *)NULL : f->data + f->write;
}

int voBufIsEmpty(VoBuf *f) {
    return f->count == 0;
}

VoTag *voBufGetTag(VoBuf *f) {
    return voBufIsEmpty(f) ? (VoTag *)NULL :
        f->tag + ((f->write - f->count + f->size) % f->size);
}

void voBufDecCount(VoBuf *f) {
    if (f->count > 0) {
        f->count--;
    }
}
