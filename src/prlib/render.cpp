#include "render.h"

#include "dma.h"
#include "gifreg.h"
#include "model.h"
#include "renderstuff.h"
#include "scene.h"
#include "spram.h"
#include "vu0mat.h"

#include <nalib/namatrix.h>
#include "animation.h"
#include "spadata.h"
#include <math.h>

/* float D_FLT_003998E4; */

extern PrSPRAM_DATA *prSpramData;

extern int AwfulStatus;

extern PrVu1InitPacket initVu1DmaPacket;

INCLUDE_ASM("asm/nonmatchings/prlib/render", Render__13PrSceneObject);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/render", InitializeVu1__13PrSceneObject);
#else /* Retail is not a tail call; a0/a1 move order differs */
void PrSceneObject::InitializeVu1() {
    PrVu1InitPacket *packet = (PrVu1InitPacket*)PR_UNCACHED(&initVu1DmaPacket);
    packet->zbuf     = prRenderStuff.m_zbuf;
    packet->frame    = this->unk50;
    packet->xyoffset = this->unk58;
    packet->scissor  = this->unk70->scissor1;

    PrWaitDmaFinish(SCE_DMA_GIF);

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_GIF);
    chan->chcr.TTE = 0; /* Don't transfer the DMAtag */

    sceDmaSend(chan, &initVu1DmaPacket);
    asm volatile("");
}
#endif

void PrSceneObject::PrepareScreenModelRender() {
    prRenderStuff.StartRender(this);
    prRenderStuff.m_transmit_array_size = 0;

    PrDmaStripForSetGifRegister *strip = PrGetDmaStripGifRegister(eGifRegisterMode_Unk3);
    prRenderStuff.AppendDmaTag(&strip->m_tag);

    for (PrModelObject *model = m_screen_model_list; model != NULL; model = model->m_list.next) {
        if (model->m_flags & 0x1) {
            prSpramData->InitializeModel(model);
            model->RenderScreenModelNode();
        }
    }

    strip = PrGetDmaStripGifRegister(eGifRegisterMode_Unk1);
    prRenderStuff.AppendDmaTag(&strip->m_tag);

    prRenderStuff.SortTransmitDmaArray();
    prRenderStuff.MergeRender();
}

static inline void MulVec(NaVECTOR<float, 4>& out, const NaMATRIX<float, 4, 4>& m, const NaVECTOR<float, 4>& v) {
    asm volatile("
        lqc2 $vf4, 0x0(%1)
        lqc2 $vf5, 0x10(%1)
        lqc2 $vf6, 0x20(%1)
        lqc2 $vf7, 0x30(%1)
        lqc2 $vf8, 0x0(%2)
        vmulax.xyzw ACC, $vf4, $vf8
        vmadday.xyzw ACC, $vf5, $vf8
        vmaddaz.xyzw ACC, $vf6, $vf8
        vmaddw.xyzw $vf9, $vf7, $vf8
        sqc2 $vf9, 0x0(%0)
    " : : "r"(&out), "r"(&m), "r"(&v) : "memory");
}

void PrModelObject::CalculateCurrentMatrix() {
    const NaMATRIX<float, 4, 4> *mat;
    SpmFileHeader *spm;
    SpaFileHeader *anim;

    PrSPRAM_DATA *spram = prSpramData;
    spram->unk660 = m_animation_time;
    *(PrModelObject**)spram->unk664 = this;
    anim = m_animation;
    spram->unk668 = anim;
    unkA4 = 1.0f;
    spm = m_spm_image;
    mat = &unk10;

    if (m_position_animation != NULL) {
        float time = m_position_animation_time;
        NaMATRIX<float, 4, 4> m;
        m = *m_position_animation->unk50[0]->GetMatrix(time);
        mat = &m;
    }

    if (anim != NULL) {
        if (spm->m_flags & 0x20) {
            spm->CalculateClusterMatrixAnimation(this, *mat);
            const NaMATRIX<float, 4, 4>& nm = spm->m_nodes[0]->unk40;
            NaVECTOR<float, 4> r;
            NaVECTOR<float, 4> v;
            v.Set(1.0f, 1.0f, 1.0f, 0.0f);
            NaVECTOR<float, 4> t;
            MulVec(t, nm, v);
            r = t;
            float d = m_disturbance;
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += r[i] * r[i];
            }
            unkA4 = d * sqrtf(sum);
        } else {
            spm->CalculateCurrentMatrixAnimation(this, *mat);
        }
    } else {
        if (spm->m_flags & 0x20) {
            spm->CalculateClusterMatrix(this, *mat);
        } else {
            spm->CalculateCurrentMatrix(this, *mat);
        }
    }
}

void SpmFileHeader::CalculateCurrentMatrix(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    m_nodes[0]->ComposeGlobalMatrix(model, arg1);

    for (u_int i = 1; i < m_node_num; i++) {
        SpmNode *node = m_nodes[i];
        node->ComposeGlobalMatrix(model, node->unk164->unk40);
    }
}

typedef NaMATRIX<float, 4, 4> PrMat44;
#define NODE_MAT(node, off) (*(PrMat44*)((char*)(node) + (off)))

#define LDA(A) asm volatile( \
        "lqc2     $vf4,   0x0(%0)       \n\t" \
        "lqc2     $vf5,  0x10(%0)       \n\t" \
        "lqc2     $vf6,  0x20(%0)       \n\t" \
        "lqc2     $vf7,  0x30(%0)       \n\t" \
    : : "r"(A))

#define ROW(o) \
        "lqc2     $vf8,  " #o "(%1)       \n\t" \
        "vmulax   ACC,    $vf4,   $vf8  \n\t" \
        "vmadday  ACC,    $vf5,   $vf8  \n\t" \
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t" \
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t" \
        "sqc2     $vf9,   " #o "(%0)       \n\t"

#define MUL(R, B) asm volatile(ROW(0x0) ROW(0x10) ROW(0x20) ROW(0x30) : : "r"(R), "r"(B))

struct PrRawMat { float m[4][4]; };

static inline PrMat44 *MatCopy(PrMat44 *lhs, const PrMat44 *rhs) {
    return &PrMat44::Copy(*lhs, *rhs);
}

/* out = b * a via VU0 macro mode, staged through two stack temporaries */
static inline void MulMV(const PrMat44 *a, const PrMat44 *b, PrMat44 *out) {
    PrRawMat t;
    PrRawMat r;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    LDA(a);
    MUL((PrMat44*)&r, b);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((PrMat44*)&t, (PrMat44*)&r);
    MatCopy(out, (PrMat44*)&t);
}

INCLUDE_ASM("asm/nonmatchings/prlib/render", CalculateCurrentMatrixAnimation__13SpmFileHeaderP13PrModelObjectRCt8NaMATRIX3Zfi4i4);

void SpmFileHeader::CalculateClusterMatrix(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    SpmNode *node = m_nodes[0];
    PrMat44 *d;

    node->ComposeGlobalMatrix(model, arg1);
    d = &NODE_MAT(node, 0xC0);
    if (node->m_flags & 0x1000) {
        MulMV(&node->unk40, &NODE_MAT(node, 0x80), d);
    }

    for (u_int i = 1; i < m_node_num; i++) {
        node = m_nodes[i];
        node->ComposeGlobalMatrix(model, node->unk164->unk40);
        if (node->m_flags & 0x1000) {
            const PrMat44 *b = &NODE_MAT(node, 0x80);
            const PrMat44 *a = &node->unk40;
            d = &NODE_MAT(node, 0xC0);
            MulMV(a, b, d);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/render", CalculateClusterMatrixAnimation__13SpmFileHeaderP13PrModelObjectRCt8NaMATRIX3Zfi4i4);

void PrModelObject::RenderContext1Model() {
    m_spm_image->RenderContext1Model(this);
    m_rendered_once = 1;
}

void SpmFileHeader::RenderContext1Model(PrModelObject *model) {
    if (!(m_flags & 0x10)) {
        for (u_int i = 0; i < m_node_num; i++) {
            m_nodes[i]->RenderContext1Node(model);
        }
    }
}

void SpmNode::ModifySimpleDmaPacket(PrVuNodeHeaderDmaPacket *packet) {
    packet = (PrVuNodeHeaderDmaPacket*)((u_int)packet | 0x30000000);

    packet->unk10 = this->unk40;
    packet->unk68 = prSpramData->m_disturbance;

    if (!AwfulStatus) {
        float a = this->unk180;
        float b = this->unk184;
        if (a != 0.0f) {
            packet->unk70 += a;
            if (packet->unk70 > 1.0f) {
                packet->unk70 -= 1.0f;
            } else if (packet->unk70 < 0.0f) {
                packet->unk70 += 1.0f;
            }
        }

        if (b != 0.0f) {
            packet->unk74 += b;
            if (packet->unk74 > 1.0f) {
                packet->unk74 -= 1.0f;
            } else if (packet->unk74 < 0.0f) {
                packet->unk74 += 1.0f;
            }
        }
    }
}

void SpmNode::RenderContext1Node(PrModelObject *model) {
    prRenderStuff.m_statistics.node_num++;

    if (this->m_flags & 0x2000) {
        return;
    }

    if ((this->m_flags & 0x4000) && (!AwfulStatus || (this->m_flags & 0x400000))) {
        PrVuNodeHeaderDmaPacket *packet = this->unk16C[0];
        if (packet != NULL) {
            prRenderStuff.m_statistics.opaque_context1_node_num++;
            ModifySimpleDmaPacket(packet);
            prRenderStuff.AppendDmaTag(&packet->m_tag);
        }

        packet = this->unk16C[1];
        if (packet != NULL) {
            prRenderStuff.m_statistics.transmit_context1_node_num++;
            ModifySimpleDmaPacket(packet);

            float sp0[4];

            asm volatile(
                "lqc2     vf13,   0x0(%0)       \n\t"
                "lqc2     vf14,  0x10(%0)       \n\t"
                "lqc2     vf15,  0x20(%0)       \n\t"
                "lqc2     vf16,  0x30(%0)       \n\t"
            : : "r"(&this->unk40));

            asm volatile(
                "lqc2     vf04,  0x0(%0)        \n\t"
                "vmulax   ACC,   vf13,    vf04  \n\t"
                "vmadday  ACC,   vf14,    vf04  \n\t"
                "vmaddaz  ACC,   vf15,    vf04  \n\t"
                "vmaddw   vf17,  vf16,    vf04  \n\t"
            : : "r"(&this->unk140));

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
            : : "r"(sp0), "r"(&prSpramData->m_view_projection_matrix));

            float f12 = sp0[2] / sp0[3];
            if (sp0[3] == 0.0f) {
                f12 = sp0[2] * 3.4028235e+38f;
            }

            prRenderStuff.AppendTransmitDmaTag(&packet->m_tag, this->unk188, f12);
        }

        if (this->m_flags & 0x40) {
            SpmComplexNode *complex = reinterpret_cast<SpmComplexNode*>(this);
            complex->RenderContour(model);
        }
    }
}

void PrModelObject::RenderScreenModelNode() {
    m_spm_image->RenderScreenModelNode();
    m_rendered_once = 1;
}

void SpmFileHeader::RenderScreenModelNode() {
    for (u_int i = 0; i < m_node_num; i++) {
        m_nodes[i]->RenderScreenModelNode();
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/render", RenderScreenModelNode__7SpmNode);

void PrModelObject::RenderBackgroundScreenModel() {
    m_spm_image->RenderBackgroundScreenModel();
    m_rendered_once = 1;
}

void SpmFileHeader::RenderBackgroundScreenModel() {
    for (u_int i = 0; i < m_node_num; i++) {
        m_nodes[i]->RenderBackgroundScreenModel();
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/render", RenderBackgroundScreenModel__7SpmNode);

void PrModelObject::RenderContext2Model() {
    PrSPRAM_DATA *spram = prSpramData;
    SpmFileHeader *spm = m_spm_image;
    spram->unk668 = this->m_animation;
    spram->unk660 = this->m_animation_time;
    spm->RenderContext2Model(this);
    m_rendered_once = 1;
}

void SpmFileHeader::RenderContext2Model(PrModelObject *model) {
    if (!(m_flags & 0x8)) {
        for (u_int i = 0; i < m_node_num; i++) {
            m_nodes[i]->RenderContext2Node(model);
        }
    }
}

void SpmNode::RenderContext2Node(PrModelObject *model) {
    if (!(m_flags & 0x2000)) {
        return;
    }
    if (!(m_flags & 0x4000)) {
        return;
    }
    if (AwfulStatus && !(m_flags & 0x400000)) {
        return;
    }

    prRenderStuff.m_statistics.opaque_context2_node_num++;

    if (m_flags & 0x10) {
        reinterpret_cast<SpmClusterGeometryNode*>(this)->RenderClusterNode(model);
    } else if (m_flags & 0x20) {
        reinterpret_cast<SpmShapeNode*>(this)->RenderShapeNode(model);
    } else {
        PrVuNodeHeaderDmaPacket *packet = unk16C[0];
        if (packet != NULL) {
            PrVuNodeHeaderDmaPacket *p = (PrVuNodeHeaderDmaPacket*)((u_int)packet | 0x30000000);
            p->unk10 = unk40;
            p->unk68 = prSpramData->m_disturbance;
            prRenderStuff.RenderNodeEECore((PrVuNodeHeaderDmaPacket*)((u_int)p & 0x0fffffff));
        }

        PrVuNodeHeaderDmaPacket *packet2 = unk16C[1];
        if (packet2 != NULL) {
            packet2 = (PrVuNodeHeaderDmaPacket*)((u_int)packet2 | 0x30000000);
            packet2->unk10 = unk40;
            packet2->unk68 = prSpramData->m_disturbance;

            NaVECTOR<float, 4> v;
            PrRawMat m;
            {
                PrRawMat r;
                const PrMat44 *vp = &prSpramData->m_view_projection_matrix;
                {
                    int w = 3;
                    do { w--; } while (w != -1);
                }
                LDA(vp);
                MUL((PrMat44*)&r, &unk40);
                {
                    int w = 3;
                    do { w--; } while (w != -1);
                }
                MatCopy((PrMat44*)&m, (PrMat44*)&r);
            }
            {
                NaVECTOR<float, 4> t;
                asm volatile(
                    "lqc2     $vf4,   0x0(%1)       \n\t"
                    "lqc2     $vf5,  0x10(%1)       \n\t"
                    "lqc2     $vf6,  0x20(%1)       \n\t"
                    "lqc2     $vf7,  0x30(%1)       \n\t"
                    "lqc2     $vf8,   0x0(%2)       \n\t"
                    "vmulax.xyzw   ACC,    $vf4,   $vf8  \n\t"
                    "vmadday.xyzw  ACC,    $vf5,   $vf8  \n\t"
                    "vmaddaz.xyzw  ACC,    $vf6,   $vf8  \n\t"
                    "vmaddw.xyzw   $vf9,   $vf7,   $vf8  \n\t"
                    "sqc2     $vf9,   0x0(%0)       \n\t"
                : : "r"(&t), "r"(&m), "r"(&unk140));
                asm volatile("lq $6, 0(%1)\n\tsq $6, 0(%0)" : : "r"(&v), "r"(&t) : "$6");
            }

            float z = v[2] / v[3];
            u_int tm = unk188;
            if (v[3] == 0.0f) {
                z = v[2] * 3.4028235e+38f;
            }

            prRenderStuff.AppendTransmitDmaTag((sceDmaTag*)((u_int)packet2 & 0x0fffffff), tm, z);
        }
    }

    if (m_flags & 0x40) {
        reinterpret_cast<SpmComplexNode*>(this)->RenderContour(model);
    }
}


/* nalib/navector.h */
extern "C" float *func_00145DB0(float *d, const float& a0, const float& a1, const float& a2, const float& a3, const float& a4, const float& a5, const float& a6, const float& a7, const float& a8, const float& a9, const float& a10, const float& a11, const float& a12, const float& a13, const float& a14, const float& a15) {
    d[0] = a0;
    d[1] = a1;
    d[2] = a2;
    d[3] = a3;
    d[4] = a4;
    d[5] = a5;
    d[6] = a6;
    d[7] = a7;
    d[8] = a8;
    d[9] = a9;
    d[10] = a10;
    d[11] = a11;
    d[12] = a12;
    d[13] = a13;
    d[14] = a14;
    d[15] = a15;
    return d;
}

/* prlib/render.cpp */
void SpmNode::ComposeGlobalMatrix(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    if (unk164 != NULL && !(unk164->m_flags & 0x4000)) {
        m_flags &= ~0x4000;
    } else if ((((int)m_flags >> 17) ^ 1) & 1) {
        m_flags |= 0x4000;
    } else {
        m_flags &= ~0x4000;
    }

    if (m_flags & 0x4000) {
        ComposeGlobalMatrixWithoutVisibility(model, arg1);
    }
}

/* prlib/renderstuff.h */
extern "C" void func_00145E50(PrDmaQueue *queue, void *tag) {
    queue->Append(tag);
}

/* prlib/render.cpp */
INCLUDE_ASM("asm/nonmatchings/prlib/render", ComposeGlobalMatrixWithoutVisibility__7SpmNodeP13PrModelObjectRCt8NaMATRIX3Zfi4i4);
