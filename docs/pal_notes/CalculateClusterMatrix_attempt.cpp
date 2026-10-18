/* CalculateClusterMatrix PAL attempt: 2/188 positional diffs (loop body: addiu t3,s1,1 and
 * addiu a0,s0,128 scheduled in swapped order). Function-level dst pointer d is what shares the dst
 * pseudo between the two blocks; the two VU0 waits need separate counters. */
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
        d = &NODE_MAT(node, 0xC0);
        if (node->m_flags & 0x1000) {
            MulMV(&node->unk40, &NODE_MAT(node, 0x80), d);
        }
    }
}
