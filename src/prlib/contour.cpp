#define NALIB_NAVECTOR_H
#define NALIB_NAMATRIX_H

template <typename T, int t0>
class NaVECTOR {
public:
    NaVECTOR() {}
    NaVECTOR(const NaVECTOR<float, 4>& rhs) {
        Copy(*this, rhs);
    }

    NaVECTOR<T, t0>& Set(const T& x, const T& y, const T& z, const T& w);

public:
    T operator[](int arg0) const {
        return v[arg0];
    }

    bool inl0(const NaVECTOR<T, t0>& arg0) const {
        for (int i = 0; i < t0; i++) {
            if (arg0[i] != v[i]) {
                return true;
            }
        }
        
        return false;
    }

    bool inl1() const {
        for (int i = 0; i < t0; i++) {
            if (v[i] != 0.0f) {
                return true;
            }
        }
        
        return false;
    }

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
    NaMATRIX() {}
    NaMATRIX(const NaMATRIX<float, 4, 4>& rhs) {
        Copy(*this, rhs);
    }

    const NaVECTOR<T, t0>& operator[](int arg0) const {
        return m[arg0];
    }

    bool inl0() const {
        const NaMATRIX<float, 4, 4>& a0 = NaMATRIX<float, 4, 4>::IDENT;
        for (int i = 0; i < t1; i++) {
            if (a0[i].inl0(this->m[i])) {
                return false;
            }
        }

        return true;
    }

    bool inl1() const {
        for (int i = 0; i < t1; i++) {
            if (this->m[i].inl1()) {
                return true;
            }
        }
        
        return false;
    }

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
#include "vu0mat.h"

static PrSPRAM_DATA *prSpramData = (PrSPRAM_DATA*)0x70000000;

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

static inline NaMATRIX<float, 4, 4> operator*(const NaMATRIX<float, 4, 4>& a, const NaMATRIX<float, 4, 4>& b) {
    NaMATRIX<float, 4, 4> ret;
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
        "lqc2       $vf08, 0x10(%2)        \n\t"
        "vmulax     ACC,    $vf04,  $vf08  \n\t"
        "vmadday    ACC,    $vf05,  $vf08  \n\t"
        "vmaddaz    ACC,    $vf06,  $vf08  \n\t"
        "vmaddw     $vf09,  $vf07,  $vf08  \n\t"
        "sqc2       $vf09, 0x10(%0)        \n\t"
        "lqc2       $vf08, 0x20(%2)        \n\t"
        "vmulax     ACC,    $vf04,  $vf08  \n\t"
        "vmadday    ACC,    $vf05,  $vf08  \n\t"
        "vmaddaz    ACC,    $vf06,  $vf08  \n\t"
        "vmaddw     $vf09,  $vf07,  $vf08  \n\t"
        "sqc2       $vf09, 0x20(%0)        \n\t"
        "lqc2       $vf08, 0x30(%2)        \n\t"
        "vmulax     ACC,    $vf04,  $vf08  \n\t"
        "vmadday    ACC,    $vf05,  $vf08  \n\t"
        "vmaddaz    ACC,    $vf06,  $vf08  \n\t"
        "vmaddw     $vf09,  $vf07,  $vf08  \n\t"
        "sqc2       $vf09, 0x30(%0)        \n\t"
    : : "r"(&ret), "r"(&a), "r"(&b));
    return ret;
}

struct SpmContourIndex {
    u_int src;
    u_int dst;
};

struct PrVuContourPacket {
    sceDmaTag m_tag;
    PR_PADDING(unk10, 0x48);
    float unk58;
    float unk5C;
};

struct SpmComplexNodeData {
    PR_PADDING(unk0, 0x140);
    NaVECTOR<float, 4> unk140;
    PR_PADDING(unk150, 0x2c);
    NaVECTOR<float, 4> *unk17C;
    PR_PADDING(unk180, 0x1c);
    u_int unk19C;
    SpmContourIndex *unk1A0;
    PrVuContourPacket *unk1A4;
};

void SpmComplexNode::SaveContour(PrModelObject *model) {
    SpmComplexNodeData *node = (SpmComplexNodeData*)this;
    NaVECTOR<float, 4> *vertices = node->unk17C;
    NaMATRIX<float, 4, 4> *matrix = (NaMATRIX<float, 4, 4>*)((u_char*)vertices + 0x10);

    bool identity = matrix->inl0();
    NaVECTOR<float, 4> *contour = (NaVECTOR<float, 4>*)((u_char*)node->unk1A4 + 0x20);
    u_int num = node->unk19C;

    if (identity) {
        for (u_int i = 0; i < num; i++) {
            u_int src = node->unk1A0[i].src;
            contour[node->unk1A0[i].dst] = vertices[src];
        }
    } else {
        for (u_int i = 0; i < num; i++) {
            contour[node->unk1A0[i].dst] = *matrix * vertices[node->unk1A0[i].src];
        }
    }
}

void PrModelObject::ResetContour() {
    m_flags &= ~2;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/contour", RenderContour__14SpmComplexNodeP13PrModelObject);
#else
void SpmComplexNode::RenderContour(PrModelObject *model) {
    SpmComplexNodeData *node = (SpmComplexNodeData*)this;
    if (!(model->m_flags & 2)) {
        return;
    }

    float alpha0 = model->m_contour_blur_alpha[0];
    if (alpha0 == 0.0f && model->m_contour_blur_alpha[1] == 0.0f) {
        return;
    }

    NaVECTOR<float, 4> *vertices = node->unk17C;
    NaMATRIX<float, 4, 4> *matrix = (NaMATRIX<float, 4, 4>*)((u_char*)vertices + 0x10);

    bool identity = matrix->inl0();
    NaVECTOR<float, 4> *contour = (NaVECTOR<float, 4>*)node->unk1A4;
    u_int num = node->unk19C;

    if (identity) {
        for (u_int i = 0; i < num; i++) {
            u_int src = node->unk1A0[i].src;
            contour[node->unk1A0[i].dst] = vertices[src];
        }
    } else {
        for (u_int i = 0; i < num; i++) {
            contour[node->unk1A0[i].dst] = *matrix * vertices[node->unk1A0[i].src];
        }
    }

    node->unk1A4->unk58 = alpha0;
    node->unk1A4->unk5C = model->m_contour_blur_alpha[1];

    NaVECTOR<float, 4> vec = (prSpramData->m_view_projection_matrix * *matrix) * node->unk140;

    float f12 = vec[2] / vec[3];
    if (vec[3] == 0.0f) {
        f12 = vec[2] * 3.4028235e+38f;
    }

    prRenderStuff.AppendTransmitDmaTag(&node->unk1A4->m_tag, -1, f12);
}
#endif

