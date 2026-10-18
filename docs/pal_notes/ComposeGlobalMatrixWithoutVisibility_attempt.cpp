/* ComposeGlobalMatrixWithoutVisibility PAL draft: 222/222 instrs, frame ok, opcode-aligned 4 diffs / 12 arg diffs.
 * Remaining: scheduling of &unk40 vs &r before the first wait loop (target: li 3; addiu t3,sp,64; addiu t4,s0,64),
 * flags reload before spram reload after BlendTransitionMatrix, addu operand order for mats[act]. */
void SpmNode::ComposeGlobalMatrixWithoutVisibility(PrModelObject *model, const NaMATRIX<float, 4, 4>& arg1) {
    PrMat44 *d;
    if (model->unk7C != NULL) {
        PrSPRAM_DATA *spram;
        if (m_flags & 1) {
            spram = prSpramData;
            *(PrMat44*)spram = PrMat44::IDENT;
        } else {
            spram = prSpramData;
            *(PrMat44*)spram = NODE_MAT(this, 0);
        }
        if (spram->m_model_transaction_blend_ratio != 1.0f) {
            BlendTransitionMatrix(model, *(PrMat44*)spram);
            spram = prSpramData;
        }
        d = &unk40;
        MulMV(&arg1, (PrMat44*)spram, d);
        u_int idx = unk150; int act = model->m_active_transition; NaMATRIX<float, 4, 4> **mats = &model->unk7C;
        mats[act][idx] = *(PrMat44*)spram;
    } else if (m_flags & 1) {
        unk40 = arg1;
    } else {
        d = &unk40;
        MulMV(&arg1, &NODE_MAT(this, 0), d);
    }

    if (m_flags & 0x8000) {
        ApplyBillboardMatrix();
    }
}
