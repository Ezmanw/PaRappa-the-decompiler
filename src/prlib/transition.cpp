#include "model.h"
#include "spram.h"

static PrSPRAM_DATA *prSpramData = (PrSPRAM_DATA*)0x70000000;

void PrModelObject::SavePosture() {
    if (!(m_spm_image->m_flags & 0x40)) {
        return;
    }

    if (m_rendered_once) {
        m_active_transition = 1 - m_active_transition;
        m_flags |= 4;
    }
}

void PrModelObject::ResetPosture() {
    m_flags &= ~4;
}

INCLUDE_ASM("asm/nonmatchings/prlib/transition", BlendTransitionMatrix__7SpmNodeP13PrModelObjectRt8NaMATRIX3Zfi4i4);

struct PrModelObjectTransitionOverlay {
    char pad0[0x5c];
    u_int m_flags;
    char pad60[0x10];
    int m_active_transition;
    float *m_weights[2];
    NaMATRIX<float, 4, 4> *m_matrices[2];
};

float BlendTransactionWeight(SpmNode *self, PrModelObjectTransitionOverlay *model, float weight, u_int idx) asm("BlendTransactionWeight__12SpmShapeNodeP13PrModelObjectfUi");
float BlendTransactionWeight(SpmNode *self, PrModelObjectTransitionOverlay *model, float weight, u_int idx) {
    float ratio = prSpramData->m_model_transaction_blend_ratio;
    if (!(model->m_flags & 4)) {
        return weight;
    }
    return (1.0f - ratio) * model->m_weights[1 - model->m_active_transition][(u_int)self->unk1B8 + idx] + ratio * weight;
}
