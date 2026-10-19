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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/shape", RenderShapeNode__12SpmShapeNodeP13PrModelObject);
#else
#include "animation.h"
#include "renderstuff.h"
#include "spadata.h"
#include "spram.h"

static PrSPRAM_DATA *prSpramData = (PrSPRAM_DATA*)0x70000000;

float BlendTransactionWeight(SpmShapeNode *self, PrModelObject *model, float weight, u_int idx) asm("BlendTransactionWeight__12SpmShapeNodeP13PrModelObjectfUi");

/* dst = b * a (each row of b transformed by matrix a) */
static void ShapeMulMat(const float *b, const float *a, float *dst) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dst[i * 4 + j] = b[i * 4 + 0] * a[0 * 4 + j] + b[i * 4 + 1] * a[1 * 4 + j]
                           + b[i * 4 + 2] * a[2 * 4 + j] + b[i * 4 + 3] * a[3 * 4 + j];
        }
    }
}

void SpmShapeNode::RenderShapeNode(PrModelObject *model) {
    char *self = reinterpret_cast<char*>(this);
    char *mdl = reinterpret_cast<char*>(model);
    PrSPRAM_DATA *spram = prSpramData;

    /* Animation of this node (if any) */
    SpaNodeAnimation *na = NULL;
    SpaFileHeader *anim = spram->unk668;
    if (anim != NULL) {
        na = anim->unk50[*reinterpret_cast<int*>(self + 0x150)];
    }

    u_int track_num = 0;
    if (na != NULL) {
        track_num = na->unk2C;
    }

    /* Write the base shape position of every vertex group */
    u_int base = *reinterpret_cast<u_int*>(self + 0x17C) | 0x30000000;
    u_int group_num = *reinterpret_cast<u_int*>(self + 0x194);
    int *stream = *reinterpret_cast<int**>(self + 0x198);
    NaVECTOR<float, 4> *shape = *reinterpret_cast<NaVECTOR<float, 4>**>(self + 0x1B4);

    for (u_int i = 0; i < group_num; i++) {
        NaVECTOR<float, 4> *group_pos = shape++;
        u_int vert_num = *stream++;
        for (u_int j = 0; j < vert_num; j++) {
            NaVECTOR<float, 4> *dst = reinterpret_cast<NaVECTOR<float, 4>*>(*stream++ * 0x10 + base);
            *dst = *group_pos;
        }
    }

    /* Add the animated shape tracks */
    if (*reinterpret_cast<void**>(mdl + 0x7c) != NULL) {
        for (u_int i = 0; i < track_num; i++) {
            float weight = *na->unk30[i]->GetValue(prSpramData->unk660);
            if (prSpramData->m_model_transaction_blend_ratio != 1.0f) {
                weight = BlendTransactionWeight(this, model, weight, i);
            }
            if (weight != 0.0f) {
                AddShapePosition(i, weight);
            }
            float **weights = reinterpret_cast<float**>(mdl + 0x74);
            int active = *reinterpret_cast<int*>(mdl + 0x70);
            weights[active][*reinterpret_cast<u_int*>(self + 0x1B8) + i] = weight;
        }
    } else {
        for (u_int i = 0; i < track_num; i++) {
            float weight = *na->unk30[i]->GetValue(prSpramData->unk660);
            if (weight != 0.0f) {
                AddShapePosition(i, weight);
            }
        }
    }

    /* Opaque packet */
    u_int packet0 = *reinterpret_cast<u_int*>(self + 0x16C);
    if (packet0 != 0) {
        packet0 |= 0x30000000;
        *reinterpret_cast<NaMATRIX<float, 4, 4>*>(packet0 + 0x10) = *reinterpret_cast<NaMATRIX<float, 4, 4>*>(self + 0x40);
        *reinterpret_cast<float*>(packet0 + 0x68) = prSpramData->m_disturbance;
        prRenderStuff.RenderNodeEECore(reinterpret_cast<PrVuNodeHeaderDmaPacket*>(packet0 & 0x0fffffff));
    }

    /* Transmit packet */
    u_int packet1 = *reinterpret_cast<u_int*>(self + 0x170);
    if (packet1 != 0) {
        packet1 |= 0x30000000;
        *reinterpret_cast<NaMATRIX<float, 4, 4>*>(packet1 + 0x10) = *reinterpret_cast<NaMATRIX<float, 4, 4>*>(self + 0x40);
        *reinterpret_cast<float*>(packet1 + 0x68) = prSpramData->m_disturbance;

        /* Depth of the node: z / w of (unk140 row 0) * (unk40 * camera) * ... */
        float view[16];
        float vec[16];
        const float *world = reinterpret_cast<const float*>(self + 0x40);
        const float *camera = reinterpret_cast<const float*>(&prSpramData->m_camera_matrix);
        ShapeMulMat(world, camera, view);
        ShapeMulMat(reinterpret_cast<const float*>(self + 0x140), view, vec);
        float depth;
        if (vec[3] != 0.0f) {
            depth = vec[2] / vec[3];
        } else {
            depth = vec[2] * 3.4028234663852886e+38f;
        }

        prRenderStuff.AppendTransmitDmaTag(reinterpret_cast<const sceDmaTag*>(packet1 & 0x0fffffff),
                                           *reinterpret_cast<u_int*>(self + 0x188), depth);
    }
}
#endif
