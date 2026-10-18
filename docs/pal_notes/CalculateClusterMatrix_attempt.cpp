/* CalculateClusterMatrix PAL attempt: 9/188 positional diffs. Remaining: first-block dst reg (t3 vs t5)
 * and li order of the post-multiply wait loop in the first block. */
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
#define WAIT() do { int _w = 3; do { _w--; } while (_w != -1); } while (0)

struct PrRawMat { float m[4][4]; };
#define CPY(l, r) asm volatile("lq $6, 0(%1)\n\tlq $7, 0x10(%1)\n\tlq $8, 0x20(%1)\n\tlq $9, 0x30(%1)\n\tsq $6, 0(%0)\n\tsq $7, 0x10(%0)\n\tsq $8, 0x20(%0)\n\tsq $9, 0x30(%0)" : : "r"(l), "r"(r) : "$6", "$7", "$8", "$9")
static inline PrMat44 *MatCopy(PrMat44 *lhs, const PrMat44 *rhs) {
    return &PrMat44::Copy(*lhs, *rhs);
}

static inline void MulMV(const PrMat44 *b, const PrMat44 *a, PrMat44 *out) {
    PrRawMat t;
    PrRawMat r;
    int w;
    w = 3;
    do { w--; } while (w != -1);
    LDA(a);
    MUL((PrMat44*)&r, b);
    w = 3;
    do { w--; } while (w != -1);
    MatCopy((PrMat44*)&t, (PrMat44*)&r);
    MatCopy(out, (PrMat44*)&t);
}

#define CALLMV(o, b, a) MulMV(&(b), &(a), &(o))

void SpmFileHeader::CalculateClusterMatrix(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    SpmNode *node = m_nodes[0];
    node->ComposeGlobalMatrix(model, arg1);
    if (node->m_flags & 0x1000) {
        CALLMV(NODE_MAT(node, 0xC0), NODE_MAT(node, 0x80), node->unk40);
    }

    for (u_int i = 1; i < m_node_num; i++) {
        node = m_nodes[i];
        node->ComposeGlobalMatrix(model, node->unk164->unk40);
        if (node->m_flags & 0x1000) {
            CALLMV(NODE_MAT(node, 0xC0), NODE_MAT(node, 0x80), node->unk40);
        }
    }
}
