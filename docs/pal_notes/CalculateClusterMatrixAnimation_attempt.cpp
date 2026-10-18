/* CalculateClusterMatrixAnimation PAL draft: 1114 vs 1124 instrs, 45 opcode-aligned diffs, frame 336 vs 352.
 * Same body as CalculateCurrentMatrixAnimation_attempt.cpp plus the 0x1000 cluster multiply after each ComposeAnim;
 * same remaining causes (1.0f hoisted into f21 instead of int 1 into s7; movn vs movz; MulMV regalloc). */
static inline int NoXf(const SpaNodeAnimation *na) { if (na->unk8 == 0) return 1; return 0; }

static inline void MulMV2(const PrMat44 *a, const PrMat44 *b, PrMat44 *out) {
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

static inline void MulMVc(const PrMat44 *a, const PrMat44 *bs, PrMat44 *out) {
    PrRawMat m;
    PrRawMat t;
    PrRawMat r;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((PrMat44*)&m, bs);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    LDA(a);
    MUL((PrMat44*)&r, (PrMat44*)&m);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((PrMat44*)&t, (PrMat44*)&r);
    MatCopy(out, (PrMat44*)&t);
}

static inline void MulMVd(const PrMat44 *a, const PrMat44 *bs, PrMat44 *out) {
    PrRawMat m;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((PrMat44*)&m, bs);
    MulMV(a, (PrMat44*)&m, out);
}

static inline void ComposeNoVis(SpmNode *node, PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    if (model->unk7C != NULL) {
        PrSPRAM_DATA *spram;
        if (node->m_flags & 1) {
            spram = prSpramData;
            *(PrMat44*)spram = PrMat44::IDENT;
        } else {
            spram = prSpramData;
            *(PrMat44*)spram = NODE_MAT(node, 0);
        }
        if (spram->m_model_transaction_blend_ratio != 1.0f) {
            node->BlendTransitionMatrix(model, *(PrMat44*)spram);
            spram = prSpramData;
        }
        MulMV(&arg1, (PrMat44*)spram, &node->unk40);
        NaMATRIX<float, 4, 4> **mats = &model->unk7C;
        mats[model->m_active_transition][node->unk150] = *(PrMat44*)spram;
    } else if (node->m_flags & 1) {
        node->unk40 = arg1;
    } else {
        MulMV(&arg1, &NODE_MAT(node, 0), &node->unk40);
    }

    if (node->m_flags & 0x8000) {
        node->ApplyBillboardMatrix();
    }
}

static inline void ComposeAnim(SpmNode *node, PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    
    PrSPRAM_DATA *spram = prSpramData;
    float time = spram->unk660;
    SpaFileHeader *anim = spram->unk668;

    if ((node->unk164 == NULL || (node->unk164->m_flags & 0x4000)) && anim->IsNodeVisible(node, time)) {
        node->m_flags |= 0x4000;
    } else {
        node->m_flags &= ~0x4000;
    }

    if (node->m_flags & 0x4000) {
        SpaNodeAnimation *na = anim->unk50[node->unk150];
        if (na == NULL) {
            ComposeNoVis(node, model, arg1);
        } else {
            if (model->unk7C != NULL) {
                PrSPRAM_DATA *sp;
                if (NoXf(na)) {
                    sp = prSpramData;
                    *(PrMat44*)sp = PrMat44::IDENT;
                } else {
                    PrMat44 *m = na->GetMatrix(time);
                    sp = prSpramData;
                    *(PrMat44*)sp = *m;
                }
                if (sp->m_model_transaction_blend_ratio != 1.0f) {
                    node->BlendTransitionMatrix(model, *(PrMat44*)sp);
                    sp = prSpramData;
                }
                MulMV(&arg1, (PrMat44*)sp, &node->unk40);
                NaMATRIX<float, 4, 4> **mats = &model->unk7C;
                mats[model->m_active_transition][node->unk150] = *(PrMat44*)sp;
            } else if (NoXf(na)) {
                node->unk40 = arg1;
            } else {
                PrMat44 *src = na->GetMatrix(time);
                PrRawMat m;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
                MatCopy((PrMat44*)&m, src);
                PrRawMat t;
                PrRawMat r;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
                LDA(&arg1);
                MUL((PrMat44*)&r, (PrMat44*)&m);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
                MatCopy((PrMat44*)&t, (PrMat44*)&r);
                MatCopy(&node->unk40, (PrMat44*)&t);
            }

            if (node->m_flags & 0x8000) {
                node->ApplyBillboardMatrix();
            }
        }
    }
}

static inline void ComposeAnimCluster(SpmNode *node, PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    ComposeAnim(node, model, arg1);
    if (node->m_flags & 0x1000) {
        const PrMat44 *b = &NODE_MAT(node, 0x80);
        const PrMat44 *a = &node->unk40;
        PrMat44 *d = &NODE_MAT(node, 0xC0);
        MulMV(a, b, d);
    }
}

void SpmFileHeader::CalculateClusterMatrixAnimation(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    ComposeAnimCluster(m_nodes[0], model, arg1);

    for (u_int i = 1; i < m_node_num; i++) {
        SpmNode *node = m_nodes[i];
        ComposeAnimCluster(node, model, node->unk164->unk40);
    }
}
