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

static inline int ClampZero(int x) {
    if (x <= 0) {
        return 0;
    }
    return x;
}

static inline int ClampZeroFirst(int x) {
    register int zero asm("$0");
    if (x <= zero) {
        return 0;
    }
    return x;
}

int PrAddSpriteDefinition(NaGifPacket *p, const PrVRAM_RECT &r0, const PrVRAM_RECT &r1, bool abe) {
    register int s4_zero asm("s4") = 0;
    u_int x0 = r0.x;
    u_int y0 = r0.y & 0x1f;
    u_int w0 = r0.w;
    u_int h0 = r0.h;
    int x1 = r1.x;
    u_int y1 = r1.y & 0x1f;
    int w1 = r1.w;
    int h1 = r1.h;

    func_0014D7D0(p, 6, 0, true, false, abe, false, 1, 0, false);

    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZeroFirst((x1 << 4) + 8), (y1 << 4) + 8));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, g_SpriteDefinitionZ << 4));

    int u1 = ((x1 + w1) << 4) + 8;
    int v1 = ((y1 + h1) << 4) + 8;
    if (u1 <= s4_zero) u1 = 0;
    if (v1 <= s4_zero) v1 = 0;
    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(u1, v1));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((x0 + w0) << 4, (y0 + h0) << 4, g_SpriteDefinitionZ << 4));

    return 5;
}

int PrAddSpriteDefinitionSuperSampled(NaGifPacket *p, const PrVRAM_RECT &r0, const PrVRAM_RECT &r1) {
    u_int x0 = r0.x;
    u_int y0 = r0.y & 0x1f;
    u_int w0 = r0.w;
    u_int h0 = r0.h;
    int x1 = r1.x;
    u_int y1 = r1.y & 0x1f;
    int w1 = r1.w;
    int h1 = r1.h;

    u_int du = (w1 << 2) / w0;
    u_int dv = (h1 << 1) / h0;

    int u0 = x1 << 4;
    int v0 = y1 << 4;
    int u_step = (du >> 2) + 8;
    int v_step = dv + 8;

    func_0014D7D0(p, 6, 0, true, false, true, false, 1, 0, false);
    func_0014D8A8(p, 0, 2, 2, 2, 33);

    int u_off0 = u_step - du;
    int v_off0 = v_step - dv;

    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u0 + u_off0), ClampZero(v0 + v_off0)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, g_SpriteDefinitionZ << 4));

    int u1 = (x1 + w1) << 4;
    int v1 = (y1 + h1) << 4;
    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u1 + u_off0), ClampZero(v1 + v_off0)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((x0 + w0) << 4, (y0 + h0) << 4, g_SpriteDefinitionZ << 4));

    int u_off1 = u_step + du;
    int v_off1 = v_step + dv;

    func_0014D8A8(p, 0, 2, 2, 1, 33);

    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u0 + u_off1), ClampZero(v0 + v_off0)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, g_SpriteDefinitionZ << 4));
    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u1 + u_off1), ClampZero(v1 + v_off0)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((x0 + w0) << 4, (y0 + h0) << 4, g_SpriteDefinitionZ << 4));

    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u0 + u_off0), ClampZero(v0 + v_off1)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, g_SpriteDefinitionZ << 4));
    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u1 + u_off0), ClampZero(v1 + v_off1)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((x0 + w0) << 4, (y0 + h0) << 4, g_SpriteDefinitionZ << 4));

    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u0 + u_off1), ClampZero(v0 + v_off1)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2(x0 << 4, y0 << 4, g_SpriteDefinitionZ << 4));
    func_0014D8D8(p, SCE_GS_UV, SCE_GS_SET_UV(ClampZero(u1 + u_off1), ClampZero(v1 + v_off1)));
    func_0014D8D8(p, SCE_GS_XYZ2, SCE_GS_SET_XYZ2((x0 + w0) << 4, (y0 + h0) << 4, g_SpriteDefinitionZ << 4));

    return 19;
}

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
