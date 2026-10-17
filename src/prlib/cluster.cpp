#ifndef NON_MATCHING
#include "common.h"

INCLUDE_ASM("asm/nonmatchings/prlib/cluster", RenderClusterNode__22SpmClusterGeometryNodeP13PrModelObject);
#else
#define NALIB_NAVECTOR_H
#define NALIB_NAMATRIX_H

#include "common.h"
#include <eetypes.h>

template <typename T, int t0>
class NaVECTOR {
public:
    NaVECTOR() {}

    static NaVECTOR<float, 4>& Copy(NaVECTOR<float, 4>& lhs, const NaVECTOR<float, 4>& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(&lhs), "r"(&rhs)
        : "$6");
        return lhs;
    }

    NaVECTOR<float, 4>& operator=(const NaVECTOR<float, 4>& rhs) {
        return Copy(*this, rhs);
    }

private:
    T v[t0];
};

template <typename T, int t0, int t1>
class NaMATRIX {
public:
    static NaMATRIX<float, 4, 4>& Copy(NaMATRIX<float, 4, 4>& lhs, const NaMATRIX<float, 4, 4>& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(&lhs), "r"(&rhs)
        : "$6", "$7", "$8", "$9");
        return lhs;
    }

    NaMATRIX<float, 4, 4>& operator=(const NaMATRIX<float, 4, 4>& rhs) {
        return Copy(*this, rhs);
    }

private:
    NaVECTOR<T, t0> m[t1];

public:
    static NaMATRIX<float, 4, 4> IDENT;
};

#include "model.h"
#include "renderstuff.h"
#include "spram.h"

extern PrSPRAM_DATA *prSpramData;

struct SpmClusterNodeInternal {
    u_char pad0[0xC0];
    NaMATRIX<float, 4, 4> unkC0;
    NaVECTOR<float, 4> unk100[4];
    NaVECTOR<float, 4> unk140;
    int unk150;
    u_int m_flags;
    SpmFileHeader *unk158;
    SpmNode *unk15C;
    SpmNode *unk160;
    SpmNode *unk164;
    int unk168;
    PrVuNodeHeaderDmaPacket *unk16C[2];
    int unk174[2];
    NaVECTOR<float, 4> *unk17C;
    float unk180;
    float unk184;
    u_int unk188;
    int unk18C[2];
    u_int unk194;
    int *unk198;
    u_int unk19C;
    int *unk1A0;
    PrVuNodeHeaderDmaPacket *unk1A4;
    int unk1A8[3];
    u_int *unk1B4;
    NaVECTOR<float, 4> *unk1B8;
};

void SpmClusterGeometryNode::RenderClusterNode(PrModelObject *model) {
    SpmClusterNodeInternal *self = reinterpret_cast<SpmClusterNodeInternal*>(this);
    NaVECTOR<float, 4> *dst_verts = reinterpret_cast<NaVECTOR<float, 4>*>(reinterpret_cast<u_int>(self->unk17C) | 0x30000000);
    u_int *weight_stream = self->unk1B4;
    SpmClusterNodeInternal **nodes = reinterpret_cast<SpmClusterNodeInternal**>(self->unk158->m_nodes);
    int *dst_stream = self->unk198;
    NaVECTOR<float, 4> *src_verts = self->unk1B8;
    u_int group_num = self->unk194;

    for (u_int i = 0; i < group_num; i++) {
        u_int weight_num = *weight_stream++;
        asm volatile("vsub.xyzw vf17, vf00, vf00");
        NaVECTOR<float, 4> *src_vec = src_verts++;
        asm volatile("lqc2 vf18, 0x0(%0)" : : "r"(src_vec));

        for (u_int j = 0; j < weight_num; j++) {
            int node_idx = *reinterpret_cast<int*>(weight_stream++);
            float weight = *reinterpret_cast<float*>(weight_stream++);

            asm volatile(
                "lqc2 vf13,  0x0(%0) \n\t"
                "lqc2 vf14, 0x10(%0) \n\t"
                "lqc2 vf15, 0x20(%0) \n\t"
                "lqc2 vf16, 0x30(%0) \n\t"
                : : "r"(&nodes[node_idx]->unkC0)
            );
            asm volatile(
                "qmtc2.ni   %0,   vf04             \n\t"
                "vmulx.xyzw vf04, vf18, vf04x      \n\t"
                "vmulax.xyzw ACC, vf13, vf04x      \n\t"
                "vmadday.xyzw ACC, vf14, vf04y     \n\t"
                "vmaddaz.xyzw ACC, vf15, vf04z     \n\t"
                "vmaddaw.xyzw ACC, vf16, vf04w     \n\t"
                "vmaddw.xyzw vf17, vf17, vf00w     \n\t"
                : : "r"(weight)
            );
        }

        u_int dst_num = *dst_stream++;
        for (u_int j = 0; j < dst_num; j++) {
            int dst_idx = *dst_stream++;
            asm volatile("sqc2 vf17, 0x0(%0)" : : "r"(&dst_verts[dst_idx]));
        }
    }

    PrVuNodeHeaderDmaPacket *packet = self->unk16C[0];
    if (packet != NULL) {
        packet = reinterpret_cast<PrVuNodeHeaderDmaPacket*>(reinterpret_cast<u_int>(packet) | 0x30000000);
        packet->unk10 = NaMATRIX<float, 4, 4>::IDENT;
        packet->unk68 = prSpramData->m_disturbance * model->unkA4;
        prRenderStuff.RenderNodeEECore(reinterpret_cast<PrVuNodeHeaderDmaPacket*>(reinterpret_cast<u_int>(packet) & 0x0FFFFFFF));
    }

    packet = self->unk16C[1];
    if (packet != NULL) {
        packet = reinterpret_cast<PrVuNodeHeaderDmaPacket*>(reinterpret_cast<u_int>(packet) | 0x30000000);
        u_int *w_stream = self->unk1B4;
        u_int weight_num = *w_stream++;
        asm volatile("vsub.xyzw vf17, vf00, vf00");
        asm volatile("lqc2 vf18, 0x0(%0)" : : "r"(&self->unk140));

        for (u_int j = 0; j < weight_num; j++) {
            int node_idx = *reinterpret_cast<int*>(w_stream++);
            float weight = *reinterpret_cast<float*>(w_stream++);

            asm volatile(
                "lqc2 vf13,  0x0(%0) \n\t"
                "lqc2 vf14, 0x10(%0) \n\t"
                "lqc2 vf15, 0x20(%0) \n\t"
                "lqc2 vf16, 0x30(%0) \n\t"
                : : "r"(&nodes[node_idx]->unkC0)
            );
            asm volatile(
                "qmtc2.ni   %0,   vf04             \n\t"
                "vmulx.xyzw vf04, vf18, vf04x      \n\t"
                "vmulax.xyzw ACC, vf13, vf04x      \n\t"
                "vmadday.xyzw ACC, vf14, vf04y     \n\t"
                "vmaddaz.xyzw ACC, vf15, vf04z     \n\t"
                "vmaddaw.xyzw ACC, vf16, vf04w     \n\t"
                "vmaddw.xyzw vf17, vf17, vf00w     \n\t"
                : : "r"(weight)
            );
        }

        packet->unk10 = NaMATRIX<float, 4, 4>::IDENT;
        packet->unk68 = prSpramData->m_disturbance * model->unkA4;

        float sp0[4];
        asm volatile(
            "lqc2     vf13,     0(%1)       \n\t"
            "lqc2     vf14,  0x10(%1)       \n\t"
            "lqc2     vf15,  0x20(%1)       \n\t"
            "lqc2     vf16,  0x30(%1)       \n\t"
            "vmulax   ACC,   vf13,    vf17  \n\t"
            "vmadday  ACC,   vf14,    vf17  \n\t"
            "vmaddaz  ACC,   vf15,    vf17  \n\t"
            "vmaddw   vf17,  vf16,    vf17  \n\t"
            "sqc2     vf17,   0x0(%0)       \n\t"
            : : "r"(sp0), "r"(&prSpramData->m_view_projection_matrix)
        );

        float z = (sp0[3] == 0.0f) ? (sp0[2] * 3.4028235e+38f) : (sp0[2] / sp0[3]);
        prRenderStuff.AppendTransmitDmaTag(
            reinterpret_cast<sceDmaTag*>(reinterpret_cast<u_int>(packet) & 0x0FFFFFFF),
            self->unk188,
            z
        );
    }
}
#endif
