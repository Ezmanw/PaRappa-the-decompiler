#include "common.h"
#include <libgifpk.h>

struct NaGifPacket;

struct PrVRAM_RECT {
    int x;
    unsigned int y;
    int w;
    int h;
    unsigned int fbw;
};

extern "C" {
void func_0015B770(NaGifPacket *p, unsigned int reg, unsigned long value);
void func_0014D668(NaGifPacket *p, u_int ofx, u_int ofy);
void func_0014D680(NaGifPacket *p, int lcm, u_int mxl, int mmag, int mmin, int mtba, u_int l, int k);
void func_0014D6C0(NaGifPacket *p, u_int tbp, u_int tbw, int psm, u_int tw, u_int th, int tcc, int tfx, u_int cbp, int cpsm, int csm, u_int csa, int cld);
void func_0014D748(NaGifPacket *p, bool ate, int atst, u_char aref, int afail, bool date, int datm, bool zte, int ztst);
void func_0014D7A0(NaGifPacket *p, u_int x0, u_int x1, u_int y0, u_int y1);
void func_0014D7D0(NaGifPacket *p, int prim, int iip, bool tme, bool fge, bool abe, bool aa1, int fst, int ctxt, bool fix);
void func_0014D838(NaGifPacket *p, u_int fbp, u_int fbw, int psm, u_int fbmask);
void func_0014D868(NaGifPacket *p, int wms, int wmt, u_int minu, u_int maxu, u_int minv, u_int maxv);
void func_0014D8A8(NaGifPacket *p, int a, int b, int c, int d, u_char fix);
void func_0014D8D8(NaGifPacket *p, u_int reg, u_long value);
}

int PrAddDrawAreaDefinition(NaGifPacket *p, const PrVRAM_RECT &r, bool a, bool b, u_long c) {
    int ret = 2;
    u_int x0 = r.x;
    u_int y = r.y;
    u_int fbw = r.fbw >> 6;
    u_int y0 = y & 0x1f;
    u_int x1 = x0 + r.w;
    u_int y1 = y0 + r.h;
    func_0014D838(p, (y >> 5) * fbw, fbw, 0, 0);
    func_0014D7A0(p, x0, x1, y0, y1);
    if (a) {
        func_0014D668(p, 0, 0);
        ret = 3;
    }
    if (b) {
        func_0014D748(p, false, 0, 0, 0, false, 0, false, 0);
        ret++;
    }
    if (c != 0) {
        func_0014D7D0(p, 6, 0, false, false, false, false, 0, 0, false);
        ret += 4;
        func_0014D8D8(p, SCE_GS_RGBAQ, c);
        func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, 0));
        func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x1 << 4, y1 << 4, 0));
    }
    return ret;
}

int PrAddTextureAreaDefinition(NaGifPacket *p, const PrVRAM_RECT &r, bool a) {
    int w = r.w;
    unsigned int y = r.y;
    unsigned int fbw = r.fbw >> 6;
    int x = r.x;
    int h = r.h;
    unsigned int tbp = ((y >> 5) * fbw) << 5;
    int yy = y & 0x1f;
    func_0014D6C0(p, tbp, fbw, 0, 10, 10, 1, 1, 0, 0, 0, 0, 0);
    func_0014D868(p, 2, 2, x, x + w - 1, yy, yy + h - 1);
    if (!a) {
        return 2;
    }
    func_0014D680(p, 0, 0, 1, 1, 0, 0, 0);
    return 3;
}

static unsigned int g_SpriteDefinitionZ;

void PrSetSpriteDefinitionZ(unsigned int z) {
    g_SpriteDefinitionZ = z;
}

INCLUDE_ASM("asm/nonmatchings/prlib/vram", PrAddSpriteDefinition__FP11NaGifPacketRC11PrVRAM_RECTT1b);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", PrAddSpriteDefinitionSuperSampled__FP11NaGifPacketRC11PrVRAM_RECTT1);

/* nalib/napacket.h */
extern "C" void func_0014D668(NaGifPacket *p, u_int ofx, u_int ofy) {
    func_0014D8D8(p, SCE_GS_XYOFFSET_1, SCE_GS_SET_XYOFFSET_1(ofx, ofy));
}

extern "C" void func_0014D680(NaGifPacket *p, int lcm, u_int mxl, int mmag, int mmin, int mtba, u_int l, int k) {
    func_0014D8D8(p, SCE_GS_TEX1_1, SCE_GS_SET_TEX1_1(lcm, mxl, mmag, mmin, mtba, l, k));
}

extern "C" void func_0014D6C0(NaGifPacket *p, u_int tbp, u_int tbw, int psm, u_int tw, u_int th, int tcc, int tfx, u_int cbp, int cpsm, int csm, u_int csa, int cld) {
    func_0014D8D8(p, SCE_GS_TEX0_1, SCE_GS_SET_TEX0_1(tbp, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld));
}

extern "C" void func_0014D748(NaGifPacket *p, bool ate, int atst, u_char aref, int afail, bool date, int datm, bool zte, int ztst) {
    func_0014D8D8(p, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(ate, atst, aref, afail, date, datm, zte, ztst));
}

extern "C" void func_0014D7A0(NaGifPacket *p, u_int x0, u_int x1, u_int y0, u_int y1) {
    func_0014D8D8(p, SCE_GS_SCISSOR_1, SCE_GS_SET_SCISSOR_1(x0, x1, y0, y1));
}

extern "C" void func_0014D7D0(NaGifPacket *p, int prim, int iip, bool tme, bool fge, bool abe, bool aa1, int fst, int ctxt, bool fix) {
    func_0014D8D8(p, SCE_GS_PRIM, SCE_GS_SET_PRIM(prim, iip, tme, fge, abe, aa1, fst, ctxt, fix));
}

extern "C" void func_0014D838(NaGifPacket *p, u_int fbp, u_int fbw, int psm, u_int fbmask) {
    func_0014D8D8(p, SCE_GS_FRAME_1, SCE_GS_SET_FRAME_1(fbp, fbw, psm, fbmask));
}

extern "C" void func_0014D868(NaGifPacket *p, int wms, int wmt, u_int minu, u_int maxu, u_int minv, u_int maxv) {
    func_0014D8D8(p, SCE_GS_CLAMP_1, SCE_GS_SET_CLAMP_1(wms, wmt, minu, maxu, minv, maxv));
}

extern "C" void func_0014D8A8(NaGifPacket *p, int a, int b, int c, int d, u_char fix) {
    func_0014D8D8(p, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA_1(a, b, c, d, fix));
}

extern "C" void func_0014D8D8(NaGifPacket *p, u_int reg, u_long value) {
    func_0015B770(p, reg, value);
}
