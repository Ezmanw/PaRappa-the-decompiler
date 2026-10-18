#include "common.h"

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", ApplyDepthOfField__13PrSceneObject);

/* nalib/napacket.h */
struct NaGifPacket;

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

#include <libgifpk.h>

extern "C" void func_0014CD48(NaGifPacket *p, bool ate, int atst, u_char aref, int afail, bool date, int datm, bool zte, int ztst) {
    func_0014CDA8(p, SCE_GS_TEST_1, SCE_GS_SET_TEST_1(ate, atst, aref, afail, date, datm, zte, ztst));
}

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDA0);

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDA8);

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDB0);

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDB8);

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDC0);

INCLUDE_ASM("asm/nonmatchings/prlib/depthfield", func_0014CDC8);
