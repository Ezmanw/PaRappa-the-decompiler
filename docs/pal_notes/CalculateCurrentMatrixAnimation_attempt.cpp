/* CalculateCurrentMatrixAnimation PAL draft: structure right (975 vs 980 instrs), 483/980 positional diffs.
 * Remaining: frame 400 vs 352 (extra 64B temp in GetMatrix branch), 1.0f hoisted into f21, MulMV r/limit regalloc. Uses MulMV/MatCopy helpers in render.cpp. */
static inline void ComposeNoVis(SpmNode *node, PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    PrMat44 *d;
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
const PrMat44 *a = &arg1;
        d = &node->unk40;
        MulMV(a, (PrMat44*)spram, d);
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
        PrMat44 *d;
        if (model->unk7C != NULL) {
            PrSPRAM_DATA *sp;
            if (na->unk8 == 0) {
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
        } else if (na->unk8 == 0) {
            node->unk40 = arg1;
        } else {
            PrMat44 *src = na->GetMatrix(time);
            PrMat44 m;
            m = *src;
const PrMat44 *b = &m;
        const PrMat44 *a = &arg1;
        d = &node->unk40;
        MulMV(a, b, d);
        }

        if (node->m_flags & 0x8000) {
            node->ApplyBillboardMatrix();
        }
        }
    }
}

void SpmFileHeader::CalculateCurrentMatrixAnimation(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    ComposeAnim(m_nodes[0], model, arg1);

    for (u_int i = 1; i < m_node_num; i++) {
        SpmNode *node = m_nodes[i];
        ComposeAnim(node, model, node->unk164->unk40);
    }
}
