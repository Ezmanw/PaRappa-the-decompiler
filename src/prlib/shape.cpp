#include "common.h"

#include "model.h"

void SpmShapeNode::AddShapePosition(u_int arg0, float arg1) {
    char *self = reinterpret_cast<char*>(this);
    u_int base = *reinterpret_cast<u_int*>(self + 0x17C);
    u_int group_num = *reinterpret_cast<u_int*>(self + 0x194);
    int *stream = *reinterpret_cast<int**>(self + 0x198);
    u_int stride = *reinterpret_cast<u_int*>(self + 0x1B0);
    char *shape = self + 0x1C0 + arg0 * 0x10;
    base |= 0x30000000;

    for (u_int i = 0; i < group_num; i++) {
        asm volatile("lqc2 vf17, 0x0(%0)" : : "r"(shape));
        u_int vert_num = *stream++;
        shape += stride * 0x10;
        asm volatile(
            "qmtc2 %0, vf04 \n\t"
            "vmulx.xyz vf17, vf17, vf04x"
            : : "r"(arg1)
        );
        for (u_int j = 0; j < vert_num; j++) {
            char *dst = reinterpret_cast<char*>(*stream++ * 0x10 + base);
            asm volatile(
                "lqc2 vf04, 0x0(%0)       \n\t"
                "vadd.xyz vf04, vf04, vf17 \n\t"
                "sqc2 vf04, 0x0(%0)"
                : : "r"(dst)
            );
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/shape", RenderShapeNode__12SpmShapeNodeP13PrModelObject);
