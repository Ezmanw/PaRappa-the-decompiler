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
unsigned int sceSifSetDma(sceSifDmaData *sdd, int len) asm("func_0015D320");
int sceSifDmaStat(unsigned int id) asm("func_0015D300");
void *sceSifAllocIopHeap(unsigned int) asm("func_00163D50");
int sceSifFreeIopHeap(void *) asm("func_00163EB8");
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

#define MIN(x, y)       min(x, y)
#define MAX(x, y)       max(x, y)

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
        int hdr_add = MIN(size, (int)(AU_HDR_SIZE - ad->hdrCount));
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
