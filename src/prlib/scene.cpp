#include "scene.h"

#include "camera.h"
#include "model.h"

#include <float.h>

INCLUDE_ASM("asm/nonmatchings/prlib/scene", __13PrSceneObjectP13sceGsDrawEnv1PCcUi);
#if 0
// TODO: nonmatching (instruction scheduling / store order)
PrSceneObject::PrSceneObject(sceGsDrawEnv1 *draw_env, const char *name, u_int fbp) {
    unkC = 0x19832b1a;
    unk94 = fbp;
    m_default_depth_level = 3;
    m_camera_time = 0.0f;
    m_name = "(noname)";
    m_list.next = NULL;
    m_list.prev = NULL;
    m_obj_set = NULL;
    m_camera = NULL;
    m_default_focal_len = 0.0f;
    m_default_defocus_len = 0.0f;
    unk90 = NULL;
    unk70 = draw_env;
    unk50 = draw_env->frame1;
    unk74 = unk50.FBW * 64;
    unk58 = draw_env->xyoffset1;
    unk78 = (2048 - ((u_int)draw_env->xyoffset1.OFY >> 4)) * 2;

    m_default_camera.position.Set(0.0f, 0.0f, 1000.0f, 1.0f);
    m_default_camera.interest.Set(0.0f, 0.0f, 0.0f, 1.0f);
    m_default_camera.up.Set(0.0f, 1.0f, 0.0f, 1.0f);
    m_default_camera.aspect = 1.0f;
    m_default_camera.field_of_view = 0.7853982f;
    m_default_camera.near_clip = 100.0f;
    m_default_camera.far_clip = 100000.0f;

    unk9C = NULL;
    m_screen_model_list = NULL;
    unk98 = NULL;
}
#endif

PrSceneObject::~PrSceneObject() {
}

void PrSceneObject::SelectCamera(SpcFileHeader *camera) {
    m_camera = camera;
    m_camera_time = 0.0f;
}

PrPERSPECTIVE_CAMERA* PrSceneObject::GetCurrentCamera() {    
    if (m_camera != NULL) {
        return m_camera->GetCamera(m_camera_time);  
    } else {
        return &m_default_camera;
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/scene", SetAppropriateDefaultCamera__13PrSceneObject);

float PrSceneObject::GetFocalLength() const {
    SpcFileHeader *camera = m_camera;
    if (camera == NULL || !(camera->m_flags & 0x8) ) {
        return m_default_focal_len;
    } else {
        float focal_length = *camera->m_focal_len_track->GetValue(m_camera_time);
        if (focal_length <= 0.0f) {
            focal_length = FLT_EPSILON;
        }
        return focal_length;
    }
}

float PrSceneObject::GetDefocusLength() const {
    SpcFileHeader *camera = m_camera;
    if (camera == NULL || !(camera->m_flags & 0x8)) {
        return m_default_defocus_len;
    } else {
        return *camera->m_defocus_len_track->GetValue(m_camera_time);
    }
}

u_int PrSceneObject::GetDepthLevel() const {
    SpcFileHeader *camera = m_camera;
    if (camera == NULL || !(camera->m_flags & 0x8)) {
        return m_default_depth_level;
    } else {
        return camera->m_depth_level;
    }
}

void PrSceneObject::PreprocessModel() {
    PrModelObject *sp = NULL;
    PrModelObject *model = m_model_set.m_head;

    PrModelObject *model_list = NULL;
    PrModelObject *screen_list = NULL;
    PrModelObject *t1 = NULL;

    while (model != NULL) {
        SpmFileHeader *spm = model->m_spm_image;
        PrModelObject *next = model->m_list.next;
        if (spm->m_flags & eSpmIsScreenModel) {
            model->m_list.next = screen_list;
            screen_list = model;
        } else if (spm->m_flags & 0x200) {
            PrModelObject *a1 = sp;
            PrModelObject **a3 = &sp;
            u_int t0_1 = spm->unk78;
            while (a1 != NULL && a1->m_spm_image->unk78 < t0_1) {
                a3 = (PrModelObject**)a1;
                a1 = *a3;
            }
            model->m_list.next = a1;
            *a3 = model;
        } else if (spm->m_flags & 0x400) {
            model->m_list.next = t1;
            t1 = model;
        } else {
            model->m_list.next = model_list;
            model_list = model;
        }
        model = next;
    }

    PrModelObject *head = NULL;
    PrModelObject *tail = NULL;

    m_screen_model_list = screen_list;
    if (model_list != NULL) {
        this->unk9C = model_list;
    } else {
        this->unk9C = screen_list;
    }

    if (t1 != NULL) {
        this->unk98 = t1;
    } else {
        this->unk98 = this->unk9C;
    }

    PrModelObject *v1 = sp;
    if (v1 != NULL) {
        head = v1;
        while (sp != NULL) {
            PrModelObject *v0;
            sp = v1->m_list.next;
            v1->m_list.prev = tail;
            tail = v1;
            v0 = sp;
            v1 = v0;
        }
    }

    if (t1 != NULL) {
        if (head == NULL) {
            head = t1;
        } else {
            tail->m_list.next = t1;
        }

        do {
            PrModelObject *model = t1;
            t1 = t1->m_list.next;
            model->m_list.prev = tail;
            tail = model;
        } while (t1 != NULL);
    }

    if (model_list != NULL) {
        if (head == NULL) {
            head = model_list;
        } else {
            tail->m_list.next = model_list;
        }

        while (model_list != NULL) {
            PrModelObject *model = model_list;
            model_list = model_list->m_list.next;
            model->m_list.prev = tail;
            tail = model;
        }
    }

    if (screen_list != NULL) {
        if (head == NULL) {
            head = screen_list;
        } else {
            tail->m_list.next = screen_list;
        }

        do {
            PrModelObject *model = screen_list;
            screen_list = screen_list->m_list.next;
            model->m_list.prev = tail;
            tail = model;
        } while (screen_list != NULL);
    }

    m_model_set.m_head = head;
    m_model_set.m_tail = tail;
}

/* nalib/navector.h */
float* func_0014B988(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) asm("func_0014B988");

float* func_0014B988(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}

float* func_0014B9B0(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) asm("func_0014B9B0");

float* func_0014B9B0(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    return self;
}

/* prlib/objectset.h */
/* _$_t11PrObjectSet1Z13PrModelObject is emitted here by the compiler */
