#define NALIB_NAVECTOR_H

#include "common.h"
#include <eetypes.h>

template <typename T, int t0>
class NaVECTOR {
public:
    NaVECTOR() {}

    NaVECTOR(const NaVECTOR<float, 4>& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(this), "r"(&rhs) : "$6");
    }

    NaVECTOR(const T& val) {
        for (int i = 0; i < t0; i++) {
            v[i] = val;
        }
    }

    NaVECTOR(const T& x, const T& y, const T& z, const T& w) {
        SetInl(x, y, z, w);
    }

    NaVECTOR<T, t0>& Set(const T& x, const T& y, const T& z, const T& w);

    NaVECTOR<T, t0>& SetInl(const T& x, const T& y, const T& z, const T& w) asm("func_00149368");

    const T& operator[](int arg0) const {
        return v[arg0];
    }

private:
    T v[t0];
};

#include "scene.h"

#include "camera.h"
#include "model.h"

#include <float.h>

PrSceneObject::PrSceneObject(sceGsDrawEnv1 *draw_env, const char *name, u_int fbp) {
    unkC = 0x19832b1a;
    unk94 = fbp;
    m_camera_time = 0.0f;
    m_name = "(noname)";
    m_default_depth_level = 3;
    m_list.next = NULL;
    m_list.prev = NULL;
    m_obj_set = NULL;
    m_camera = NULL;
    m_default_focal_len = 0.0f;
    m_default_defocus_len = 0.0f;
    unk90 = NULL;
    unk70 = draw_env;
    unk50 = draw_env->frame1;
    unk58 = draw_env->xyoffset1;
    unk74 = unk50.FBW * 64;
    unk78 = (2048 - (int)(draw_env->xyoffset1.OFY >> 4)) * 2;

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

static inline void VecCopy(NaVECTOR<float, 4>* dst, const NaVECTOR<float, 4>* src) {
    asm volatile(
        "lq $6, 0(%1)\n\t"
        "sq $6, 0(%0)\n\t"
    : : "r"(dst), "r"(src) : "$6");
}

static inline void VecAdd(NaVECTOR<float, 4>* ret, const NaVECTOR<float, 4>* a, const NaVECTOR<float, 4>* b) {
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "lqc2       $vf05,  0x0(%2)        \n\t"
        "vadd.xyzw  $vf06,  $vf04,  $vf05  \n\t"
        "sqc2       $vf06,  0x0(%0)        \n\t"
    : : "r"(ret), "r"(a), "r"(b));
}

static inline void VecSub(NaVECTOR<float, 4>* ret, const NaVECTOR<float, 4>* a, const NaVECTOR<float, 4>* b) {
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "lqc2       $vf05,  0x0(%2)        \n\t"
        "vsub.xyzw  $vf06,  $vf04,  $vf05  \n\t"
        "sqc2       $vf06,  0x0(%0)        \n\t"
    : : "r"(ret), "r"(a), "r"(b));
}

static inline void VecMul(NaVECTOR<float, 4>* ret, const NaVECTOR<float, 4>* v, float f) {
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "mfc1       $8,     %2             \n\t"
        "qmtc2      $8,     $vf05          \n\t"
        "vmulx.xyzw $vf06,  $vf04,  $vf05  \n\t"
        "sqc2       $vf06,  0x0(%0)        \n\t"
    : : "r"(ret), "r"(v), "f"(f) : "$8");
}

static inline void VecDiv(NaVECTOR<float, 4>* ret, const NaVECTOR<float, 4>* v, float f) {
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "mfc1       $8,     %2             \n\t"
        "qmtc2      $8,     $vf05          \n\t"
        "vdiv       Q,      $vf00w, $vf05x \n\t"
        "vwaitq                            \n\t"
        "vmulq.xyzw $vf04,  $vf04,  Q      \n\t"
        "sqc2       $vf04,  0x0(%0)        \n\t"
    : : "r"(ret), "r"(v), "f"(f) : "$8");
}

void PrSceneObject::SetAppropriateDefaultCamera() {
    NaVECTOR<float, 4> min_pos(FLT_MAX);
    NaVECTOR<float, 4> max_pos(-FLT_MAX);

    for (PrModelObject *model = m_model_set.m_head; model != NULL; model = model->m_list.next) {
        if (model->m_flags & 0x1) {
            u_short flags = model->m_spm_image->m_flags;
            if (flags & eSpmIsScreenModel) {
                continue;
            }
            if (flags & 0x200) {
                continue;
            }
            model->UnionBoundaryBox(&min_pos, &max_pos);
        }
    }

    NaVECTOR<float, 4> half_size;
    {
        NaVECTOR<float, 4> tmp1;
        NaVECTOR<float, 4> tmp2;
        VecSub(&tmp2, &max_pos, &min_pos);
        VecCopy(&tmp1, &tmp2);
        VecDiv(&tmp2, &tmp1, 2.0f);
        VecCopy(&half_size, &tmp2);
    }
    if (half_size[0] < 0.0f || half_size[1] < 0.0f || half_size[2] < 0.0f) {
        return;
    }

    NaVECTOR<float, 4> center;
    {
        NaVECTOR<float, 4> tmp1;
        NaVECTOR<float, 4> tmp2;
        VecAdd(&tmp2, &min_pos, &max_pos);
        VecCopy(&tmp1, &tmp2);
        VecDiv(&tmp2, &tmp1, 2.0f);
        VecCopy(&center, &tmp2);
    }
    NaVECTOR<float, 4> dir(0.0f, 0.0f, -1.0f, 1.0f);

    float max_xy = (half_size[2] <= half_size[1]) ? half_size[1] : half_size[2];
    float dist = half_size[2] + max_xy * 1.7320508f * 1.3f;

    NaVECTOR<float, 4> cam_pos;
    {
        NaVECTOR<float, 4> tmp1;
        NaVECTOR<float, 4> tmp2;
        VecMul(&tmp2, &dir, dist);
        VecCopy(&tmp1, &tmp2);
        VecSub(&tmp2, &center, &tmp1);
        VecCopy(&cam_pos, &tmp2);
    }

    m_default_camera.position.Set(cam_pos[0], cam_pos[1], cam_pos[2], 1.0f);
    m_default_camera.interest.Set(center[0], center[1], center[2], 1.0f);

    float d_z = cam_pos[2] - center[2];
    m_default_camera.up.Set(0.0f, 1.0f, 0.0f, 1.0f);

    m_default_camera.aspect = 1.0f;
    m_default_camera.field_of_view = 1.0471976f;
    m_default_camera.near_clip = d_z / 30000.0f;
    m_default_camera.far_clip = d_z * 30.0f;
}

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
