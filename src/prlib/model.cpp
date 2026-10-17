#include "model.h"
#include "scene.h"
#include "spadata.h"
#include "spram.h"
#include "animation.h"

#include <nalib/navector.h>

static PrSPRAM_DATA *prSpramData = (PrSPRAM_DATA*)0x70000000;

static u_int prPostureWorkArea = 0;
static int prPostureWorkAreaSize;

PR_EXTERN
void PrSetPostureWorkArea(u_int area_top, int area_size) {
    while (area_top & 0xf) {
        area_top++;
        area_size--;
    }
    prPostureWorkArea = area_top;
    prPostureWorkAreaSize = area_size;
}

static void* AllocateFromWorkArea(u_int size) {
    void *ret = (void*)prPostureWorkArea;
    if (ret == NULL) {
        return NULL;
    }
    size = (size + 15) / 16 * 16;
    if (prPostureWorkAreaSize < size) {
        return NULL;
    }
    prPostureWorkAreaSize -= size;
    prPostureWorkArea += size;
    return ret;
}

PrModelObject::PrModelObject(SpmFileHeader *spm) {
    m_list.next = NULL;
    m_list.prev = NULL;
    m_obj_set = NULL;
    m_linked_scene = NULL;
    unk10 = NaMATRIX<float, 4, 4>::IDENT;
    unk50 = 0x55668899;
    unk54 = 0;
    m_spm_image = spm;
    m_flags = 0;
    m_animation_time = 0.0f;
    m_position_animation_time = 0.0f;
    m_animation = NULL;
    m_position_animation = NULL;
    unk7C = NULL;
    unk80 = NULL;
    unk74 = NULL;
    unk78 = NULL;
    m_rendered_once = 0;
    m_active_transition = 0;
    unk88 = NULL;
    unk8C = NULL;
    unk90 = 0;
    m_contour_blur_alpha[0] = 0.0f;
    m_contour_blur_alpha[1] = 0.0f;
    m_transaction_blend_ratio = 1.0f;
    m_disturbance = 0.0f;
    unkA4 = 1.0f;
    spm->m_linked_model = this;
}

PrModelObject::~PrModelObject() {
    m_linked_scene->m_model_set.Remove(this);
    m_linked_scene = NULL;

    CleanupAnimation();
    CleanupPositionAnimation();

    m_spm_image->m_linked_model = NULL;

    delete unk88;
    delete unk8C;

    if ((m_flags & 0x8) && unk74 != NULL) {
        delete[] unk74;
    }
    if ((m_flags & 0x10) && unk7C != NULL) {
        delete[] unk7C;
    }
}

void PrModelObject::Initialize() {
    SpmFileHeader *spm = m_spm_image;
    spm->ChangePointer();

    if (spm->m_flags & 0x40) {
        u_int node_num = spm->m_node_num;
        if (node_num != 0) {
            u_int size = node_num * sizeof(NaMATRIX<float, 4, 4>) * 2;
            void *area = AllocateFromWorkArea(size);
            if (area == NULL) {
                unk7C = new NaMATRIX<float, 4, 4>[node_num * 2];
                m_flags |= 0x10;
            } else {
                unk7C = (NaMATRIX<float, 4, 4>*)area;
            }
            unk80 = unk7C + node_num;

            u_int num = spm->unk6C;
            if (num != 0) {
                size = num * sizeof(int) * 2;
                area = AllocateFromWorkArea(size);
                if (area == NULL) {
                    unk74 = new int[num * 2];
                    m_flags |= 0x8;
                } else {
                    unk74 = (int*)area;
                }
                unk78 = unk74 + num;
            }
        }
    }

    m_flags &= ~0x4;
}

void PrModelObject::LinkAnimation(SpaFileHeader *animation) {
    if (animation == NULL) {
        CleanupAnimation();
    } else {
        m_animation = animation;
        m_animation_time = 0.0f;
    }
}

void PrModelObject::CleanupAnimation() {
    m_animation = NULL;
}

void PrModelObject::LinkPositionAnimation(SpaFileHeader *animation) {
    if (animation == NULL) {
        CleanupPositionAnimation();
    } else {
        m_position_animation = animation;
        m_position_animation_time = 0.0f;
    }
}

void PrModelObject::CleanupPositionAnimation() {
    m_position_animation = NULL;
}

void PrModelObject::UnionBoundaryBox(NaVECTOR<float, 4> *arg0, NaVECTOR<float, 4> *arg1) {
    asm volatile(
        "lqc2       $vf13,   0x0(%0)       \n\t"
        "lqc2       $vf14,  0x10(%0)       \n\t"
        "lqc2       $vf15,  0x20(%0)       \n\t"
        "lqc2       $vf16,  0x30(%0)       \n\t"
    : : "r"(&this->unk10));

    asm volatile(
        "lqc2       $vf17,  0x0(%0)        \n\t"
        "vmulax     ACC,    $vf13,  $vf17  \n\t"
        "vmadday    ACC,    $vf14,  $vf17  \n\t"
        "vmaddaz    ACC,    $vf15,  $vf17  \n\t"
        "vmaddw     $vf17,  $vf16,  $vf17  \n\t"
    : : "r"(&m_spm_image->unk30));

    asm volatile(
        "lqc2       $vf04,  0x0(%0)        \n\t"
        "vmini.xyz  $vf04,  $vf04,  $vf17  \n\t"
        "sqc2       $vf04,  0x0(%0)        \n\t"
    : : "r"(arg0));

    asm volatile(
        "lqc2       $vf17,  0x0(%0)        \n\t"
        "vmulax     ACC,    $vf13,  $vf17  \n\t"
        "vmadday    ACC,    $vf14,  $vf17  \n\t"
        "vmaddaz    ACC,    $vf15,  $vf17  \n\t"
        "vmaddw     $vf17,  $vf16,  $vf17  \n\t"
    : : "r"(&m_spm_image->unk40));

    asm volatile(
        "lqc2       $vf04,  0x0(%0)        \n\t"
        "vmax.xyz   $vf04,  $vf04,  $vf17  \n\t"
        "sqc2       $vf04,  0x0(%0)        \n\t"
    : : "r"(arg1));
}

#define APPLY_MATRIX(vector, matrix)                \
    asm volatile(                                   \
        "lqc2       $vf04,  0x0(%0)        \n\t"     \
        "lqc2       $vf05, 0x10(%0)        \n\t"     \
        "lqc2       $vf06, 0x20(%0)        \n\t"     \
        "lqc2       $vf07, 0x30(%0)        \n\t"     \
    : : "r"(matrix));                               \
    asm volatile(                                   \
        "lqc2       $vf08,  0x0(%0)        \n\t"     \
        "vmulax     ACC,    $vf04,  $vf08  \n\t"     \
        "vmadday    ACC,    $vf05,  $vf08  \n\t"     \
        "vmaddaz    ACC,    $vf06,  $vf08  \n\t"     \
        "vmaddw     $vf09,  $vf07,  $vf08  \n\t"     \
    : : "r"(vector));                               \
    asm volatile(                                   \
        "sqc2       $vf09,  0x0(%0)        \n\t"     \
    : : "r"(vector) : "memory")

void PrModelObject::GetPrimitivePosition(NaVECTOR<float, 4> *position) {
    position->Set(0.0f, 0.0f, 0.0f, 1.0f);

    if (m_position_animation != NULL) {
        APPLY_MATRIX(position, m_position_animation->unk50[0]->GetMatrix(m_position_animation_time));
    } else {
        APPLY_MATRIX(position, &unk10);
    }

    APPLY_MATRIX(position, &prSpramData->m_view_projection_matrix);
}

void PrModelObject::GetScreenPosition(NaVECTOR<float, 4> *position) {
    GetPrimitivePosition(position);

    float *v = (float*)position;
    float w = v[3];
    v[0] = v[0] / w - 1728.0f;
    v[1] = v[1] / w - 1920.0f;
    v[3] = 1.0f;
}

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/model", func_00140E38);
