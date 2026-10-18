/* CalculateClusterMatrix PAL attempt: 44/188 positional diffs (check.py), same instruction count.
 * Remaining: register allocation only (a/b/r pointer regs, t-reg numbering of dst/m_node_num). */
typedef NaMATRIX<float, 4, 4> PrMat44;
#define NODE_MAT(node, off) (*(PrMat44*)((char*)(node) + (off)))

static inline void MulMRaw(PrMat44 *dst, const PrMat44 *b, const PrMat44 *a) {
    asm volatile(
        "lqc2     $vf4,   0x0(%0)       \n\t"
        "lqc2     $vf5,  0x10(%0)       \n\t"
        "lqc2     $vf6,  0x20(%0)       \n\t"
        "lqc2     $vf7,  0x30(%0)       \n\t"
    : : "r"(a));
    asm volatile(
        "lqc2     $vf8,   0x0(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,   0x0(%0)       \n\t"
        "lqc2     $vf8,  0x10(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x10(%0)       \n\t"
        "lqc2     $vf8,  0x20(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x20(%0)       \n\t"
        "lqc2     $vf8,  0x30(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x30(%0)       \n\t"
    : : "r"(dst), "r"(b));
}

static inline void MulM(PrMat44& out, const PrMat44& a, const PrMat44& b) {
    u_long128 t[4];
    u_long128 r[4];
    PrMulMatrix((PrMat44*)r, &b, &a);
    PrMat44::Copy(*(PrMat44*)t, *(PrMat44*)r);
    out = *(PrMat44*)t;
}

void SpmFileHeader::CalculateClusterMatrix(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    SpmNode *node = m_nodes[0];
    node->ComposeGlobalMatrix(model, arg1);
    if (node->m_flags & 0x1000) {
        MulM(NODE_MAT(node, 0xC0), node->unk40, NODE_MAT(node, 0x80));
    }

    for (u_int i = 1; i < m_node_num; i++) {
        node = m_nodes[i];
        node->ComposeGlobalMatrix(model, node->unk164->unk40);
        if (node->m_flags & 0x1000) {
            MulM(NODE_MAT(node, 0xC0), node->unk40, NODE_MAT(node, 0x80));
        }
    }
}

