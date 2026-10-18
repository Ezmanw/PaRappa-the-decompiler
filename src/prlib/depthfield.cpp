#include "common.h"
#include "scene.h"
#include "renderstuff.h"
#include "spram.h"
#include <libgifpk.h>
#include <libdma.h>
#include <eekernel.h>

static PrSPRAM_DATA *prSpramData = (PrSPRAM_DATA*)0x70000000;

struct PrVRAM_RECT {
    int x;
    unsigned int y;
    int w;
    int h;
    unsigned int fbw;
};

/* nalib/napacket.h */
struct NaGifPacket {
    NaGifPacket(u_long128 *buf) {
        Init(buf);
    }
    void Init(u_long128 *buf) asm("func_0014CDC0");
    void End(int opt1, int opt2, int flag) asm("func_0014CDB8");
    void OpenGifTag(u_long128 tag) asm("func_0014CDB0");
    void CloseGifTag() asm("func_0014CDA8");
    void AddGsAD(unsigned int reg, unsigned long value) asm("func_0014CDA0");
    void AddTest(bool ate, int atst, u_char aref, int afail, bool date, int datm, bool zte, int ztst) asm("func_0014CD48");
    void AddTexflush() asm("func_0014CD38");
    void Terminate() asm("sceGifPkTerminate");
    sceGifPacket pkt;
};

int PrAddDrawAreaDefinition(NaGifPacket *p, const PrVRAM_RECT &r, bool a, bool b, u_long c);
int PrAddTextureAreaDefinition(NaGifPacket *p, const PrVRAM_RECT &r, bool a);
void PrSetSpriteDefinitionZ(unsigned int z);
int PrAddSpriteDefinition(NaGifPacket *p, const PrVRAM_RECT &r0, const PrVRAM_RECT &r1, bool abe);
int PrAddSpriteDefinitionSuperSampled(NaGifPacket *p, const PrVRAM_RECT &r0, const PrVRAM_RECT &r1);

void PrSceneObject::ApplyDepthOfField() {
    static u_long128 s_work;

    u_int depth_level = GetDepthLevel();
    if (depth_level == 0) {
        return;
    }

    float focal_len = GetFocalLength();
    if (focal_len <= 0.0f) {
        return;
    }

    float defocus_len = GetDefocusLength();
    if (defocus_len <= focal_len) {
        return;
    }

    if (unk94 == 0xffffffff) {
        return;
    }

    sceGsZbuf zbuf = prRenderStuff.m_zbuf;

    PrVRAM_RECT rect0;
    u_int w = unk74;
    rect0.fbw = w;
    rect0.x = 0;
    rect0.y = (u_long)((unk50.FBP & 0x1ff) << 11) / w;
    u_int h = unk78;
    rect0.w = w;
    rect0.h = h;

    PrVRAM_RECT rect1;
    rect1.fbw = w;
    rect1.x = 0;
    rect1.y = (unk94 << 11) / w;
    rect1.w = w >> 1;
    rect1.h = h;

    NaGifPacket packet((u_long128*)((u_int)prSpramData + 0x690));
    packet.End(0, 0, 0);

    sceGifTag *tag = (sceGifTag*)&s_work;
    tag->NLOOP = 0;
    tag->EOP = 1;
    tag->PRE = 0;
    tag->FLG = 0;
    tag->NREG = 1;
    tag->REGS0 = 0xe;
    packet.OpenGifTag(s_work);

    zbuf.ZMSK = 1;
    packet.AddGsAD(SCE_GS_ZBUF_1, *(u_long*)&zbuf);

    bool first = true;
    for (u_int i = 1; i <= depth_level; i++) {
        packet.AddTexflush();
        PrAddDrawAreaDefinition(&packet, rect1, first, true, 0);
        PrAddTextureAreaDefinition(&packet, rect0, first);
        PrAddSpriteDefinitionSuperSampled(&packet, rect1, rect0);

        float z_dist = focal_len + (defocus_len - focal_len) * (float)i * 0.25f;
        NaMATRIX<float, 4, 4> *vp = (NaMATRIX<float, 4, 4>*)((u_int)prSpramData + 0x1A0);
        float z_val = ((*vp)[2][2] * z_dist + (*vp)[3][2]) / ((*vp)[2][3] * z_dist + (*vp)[3][3]);
        PrSetSpriteDefinitionZ((u_int)z_val);

        packet.AddTexflush();
        packet.AddTest(false, 0, 0, 0, false, 0, true, 2);
        PrAddDrawAreaDefinition(&packet, rect0, false, false, 0);
        PrAddTextureAreaDefinition(&packet, rect1, false);
        packet.AddGsAD(SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA_1(0, 1, 2, 1, 0x60));
        PrAddSpriteDefinition(&packet, rect0, rect1, true);

        if (first) {
            rect1.w = w >> 2;
            rect1.h = h >> 1;
        }
        first = false;
    }

    zbuf.ZMSK = 0;
    packet.AddGsAD(SCE_GS_ZBUF_1, *(u_long*)&zbuf);
    sceGsDrawEnv1 *env = unk70;
    packet.AddGsAD(SCE_GS_SCISSOR_1, *(u_long*)&env->scissor1);
    packet.AddGsAD(SCE_GS_XYOFFSET_1, *(u_long*)&unk58);
    packet.AddGsAD(SCE_GS_TEST_1, *(u_long*)&env->test1);
    packet.AddGsAD(SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA_1(0, 1, 0, 1, 0x80));
    packet.CloseGifTag();
    packet.Terminate();

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_GIF);
    chan->chcr.TTE = 0;
    FlushCache(0);
    sceDmaSend(chan, (void*)(((u_int)packet.pkt.pBase & 0x3fff) | 0x80000000));
}

extern "C" {
void func_0014CD38(NaGifPacket *p);
void func_0014CDA8(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0014CDB8(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0015B390(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0015B620(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0015B6B0(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0015B6C8(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0015B770(NaGifPacket *p, unsigned int reg, unsigned long value);
}

extern "C" void func_0014CD38(NaGifPacket *p) {
    func_0014CDA8(p, 63, 0);
}

extern "C" void func_0014CD48(NaGifPacket *p, bool ate, int atst, u_char aref, int afail, bool date, int datm, bool zte, int ztst) {
    func_0014CDA8(p, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(ate, atst, aref, afail, date, datm, zte, ztst));
}

extern "C" void func_0014CDA0(NaGifPacket *p, unsigned int reg, unsigned long value) {
    func_0015B770(p, reg, value);
}

extern "C" void func_0014CDA8(NaGifPacket *p, unsigned int reg, unsigned long value) {
    func_0015B6C8(p, reg, value);
}

extern "C" void func_0014CDB0(NaGifPacket *p, unsigned int reg, unsigned long value) {
    func_0015B6B0(p, reg, value);
}

extern "C" void func_0014CDB8(NaGifPacket *p, unsigned int reg, unsigned long value) {
    func_0015B620(p, reg, value);
}

extern "C" void func_0014CDC0(NaGifPacket *p, unsigned int reg, unsigned long value) {
    func_0015B390(p, reg, value);
}

