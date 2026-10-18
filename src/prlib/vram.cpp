#include "common.h"

struct NaGifPacket;

struct PrVRAM_RECT {
    int x;
    unsigned int y;
    int w;
    int h;
    unsigned int fbw;
};

extern "C" {
void func_0014CD48(NaGifPacket *p, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
void func_0014CDA0(NaGifPacket *p, int reg, unsigned long value);
void func_0014D668(NaGifPacket *p, int ofx, int ofy);
void func_0014D680(NaGifPacket *p, int lcm, int mxl, int mmag, int mmin, int mtba, int l, int k);
void func_0014D6C0(NaGifPacket *p, int tbp, int tbw, int psm, int tw, int th, int tcc, int tfx, int cbp, int cpsm, int csm, int csa, int cld);
void func_0014D7A0(NaGifPacket *p, int x0, int x1, int y0, int y1);
void func_0014D7D0(NaGifPacket *p, int prim, int iip, int tme, int fge, int abe, int aa1, int fst, int ctxt, int fix);
void func_0014D838(NaGifPacket *p, int fbp, int fbw, int psm, int fbmask);
void func_0014D868(NaGifPacket *p, int wms, int wmt, int minu, int maxu, int minv, int maxv);
}

INCLUDE_ASM("asm/nonmatchings/prlib/vram", PrAddDrawAreaDefinition__FP11NaGifPacketRC11PrVRAM_RECTbbUl);

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
INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D668);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D680);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D6C0);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D748);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D7A0);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D7D0);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D838);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D868);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D8A8);

INCLUDE_ASM("asm/nonmatchings/prlib/vram", func_0014D8D8);
