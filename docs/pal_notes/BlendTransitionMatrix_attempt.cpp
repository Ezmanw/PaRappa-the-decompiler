// Draft attempt for SpmNode::BlendTransitionMatrix in src/prlib/transition.cpp
// Reached 122/187 instruction match (diffscore 84); remaining difference is
// NaMATRIX<float, 4, 4> return-value copy constructor NRVO/temp stack slot
// elimination in ee-gcc 2.95.3 vs 2.9-ee.

void SpmNode::BlendTransitionMatrix(PrModelObject *model, NaMATRIX<float, 4, 4>& arg1) {
    float ratio = prSpramData->m_model_transaction_blend_ratio;
    if (!(model->m_flags & 4)) {
        return;
    }
    arg1 = model->m_matrices[1 - model->m_active_transition][this->unk150] * (1.0f - ratio) + arg1 * ratio;
}
