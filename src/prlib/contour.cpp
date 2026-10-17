#include "model.h"

void PrModelObject::SaveContour() {
    SpmFileHeader *spm = m_spm_image;
    if (spm->unk70 == 0 || !m_rendered_once) {
        return;
    }

    u_int node_num = spm->m_node_num;
    for (int i = 0; i < node_num; i++) {
        SpmNode *node = spm->m_nodes[i];
        if (node->m_flags & 0x40) {
            SpmComplexNode *complex = reinterpret_cast<SpmComplexNode*>(node);
            complex->SaveContour(this);
        }
    }

    m_flags |= 2;
}

INCLUDE_ASM("asm/nonmatchings/prlib/contour", SaveContour__14SpmComplexNodeP13PrModelObject);
#if 0
// TODO: nonmatching (close: regalloc/scheduling). Needs a NaVECTOR copy constructor
// (lq/sq asm) and NaVECTOR::Copy without the "memory" clobber.
static inline NaVECTOR<float, 4> operator*(const NaMATRIX<float, 4, 4>& m, const NaVECTOR<float, 4>& v) {
    NaVECTOR<float, 4> ret;
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "lqc2       $vf05, 0x10(%1)        \n\t"
        "lqc2       $vf06, 0x20(%1)        \n\t"
        "lqc2       $vf07, 0x30(%1)        \n\t"
        "lqc2       $vf08,  0x0(%2)        \n\t"
        "vmulax     ACC,    $vf04,  $vf08  \n\t"
        "vmadday    ACC,    $vf05,  $vf08  \n\t"
        "vmaddaz    ACC,    $vf06,  $vf08  \n\t"
        "vmaddw     $vf09,  $vf07,  $vf08  \n\t"
        "sqc2       $vf09,  0x0(%0)        \n\t"
    : : "r"(&ret), "r"(&m), "r"(&v));
    return ret;
}

struct SpmContourIndex {
    u_int src;
    u_int dst;
};

void SpmComplexNode::SaveContour(PrModelObject *model) {
    SpmNode *node = (SpmNode*)this;
    NaVECTOR<float, 4> *vertices = (NaVECTOR<float, 4>*)node->unk17C;
    NaMATRIX<float, 4, 4> *matrix = (NaMATRIX<float, 4, 4>*)((u_char*)node->unk17C + 0x10);
    PrVuNodeHeaderDmaPacket *packet = node->unk1A4;
    u_int num = node->unk19C;

    bool identity = matrix->inl0();
    NaVECTOR<float, 4> *contour = (NaVECTOR<float, 4>*)((u_char*)packet + 0x20);

    if (identity) {
        for (u_int i = 0; i < num; i++) {
            SpmContourIndex *index = &((SpmContourIndex*)node->unk1A0)[i];
            contour[index->dst] = vertices[index->src];
        }
    } else {
        for (u_int i = 0; i < num; i++) {
            SpmContourIndex *index = &((SpmContourIndex*)node->unk1A0)[i];
            NaVECTOR<float, 4> v = *matrix * vertices[index->src];
            contour[index->dst] = v;
        }
    }
}
#endif

void PrModelObject::ResetContour() {
    m_flags &= ~2;
}

INCLUDE_ASM("asm/nonmatchings/prlib/contour", RenderContour__14SpmComplexNodeP13PrModelObject);
