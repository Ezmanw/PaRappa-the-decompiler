#include "spadata.h"

#include "animation.h"
#include "model.h"

#include <math.h>

template <>
NaVECTOR<float, 4>* SpaTrack<NaVECTOR<float, 4> >::GetSprineValue(u_int seg, float arg1) const;

u_int SpaTrackBase::SearchSegment(float arg0) const {
    if (this->unk4 == 1) {
        return (u_int)-1;
    }

    if (arg0 <= this->unkC[0]) {
        return (u_int)-1;
    }

    if (arg0 >= this->unkC[this->unk4 - 1]) {
        return this->unk4;
    }

    if (arg0 >= this->unkC[this->unk8]) {
        if (arg0 < this->unkC[this->unk8 + 1]) {
            return this->unk8;
        }

        if ((this->unk8 + 2) < this->unk4) {
            if (arg0 < this->unkC[this->unk8 + 2]) {
                return ++this->unk8;
            }
        }
    } else if (this->unk8 != 0) {
        if (arg0 >= this->unkC[this->unk8 - 1]) {
            return --this->unk8;
        }
    }

    int right = this->unk4 - 2;
    int left = 0;
    int mid = 0;

    while (left <= right) {
        mid = (left + right) / 2;
        if (arg0 < this->unkC[mid]) {
            right = mid - 1;
        } else if (arg0 >= this->unkC[mid + 1]) {
            left = mid + 1;
        } else {
            break;
        }
    }

    this->unk8 = mid;
    return this->unk8;
}

template <>
int* SpaTrack<int>::GetValue(float arg0) const {
    if (this->unk2 & 0x1) {
        float f13 = this->unkC[this->unk4 - 1];
        if (arg0 < 0.0f || arg0 >= f13) {
            arg0 = fmodf(arg0, f13);
        }
    }

    u_int seg = this->SearchSegment(arg0);
    if (seg == (u_int)-1) {
        return (int*)&this->unk10;
    }

    if (seg == this->unk4) {
        return (int*)&this->unkC + seg;
    } else {
        return (int*)&this->unk10 + seg;
    }
}

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", GetMatrix__C12SpaTransformf);

NaMATRIX<float, 4, 4>* SpaNodeAnimation::GetMatrix(float time) const {
    static NaMATRIX<float, 4, 4> return_buffer;

    asm volatile(
        "vmove      $vf16,  $vf00          \n\t"
        "vmr32      $vf15,  $vf00          \n\t"
        "vmr32      $vf14,  $vf15          \n\t"
        "vmr32      $vf13,  $vf14          \n\t"
    );

    for (u_int i = 0; i < this->unk8; i++) {
        NaMATRIX<float, 4, 4> *matrix = this->unkC[i]->GetMatrix(time);
        asm volatile(
            "lqc2       $vf04,  0x0(%0)        \n\t"
            "lqc2       $vf05, 0x10(%0)        \n\t"
            "lqc2       $vf06, 0x20(%0)        \n\t"
            "lqc2       $vf07, 0x30(%0)        \n\t"
            "vmulax     ACC,    $vf04,  $vf13  \n\t"
            "vmadday    ACC,    $vf05,  $vf13  \n\t"
            "vmaddaz    ACC,    $vf06,  $vf13  \n\t"
            "vmaddw     $vf13,  $vf07,  $vf13  \n\t"
            "vmulax     ACC,    $vf04,  $vf14  \n\t"
            "vmadday    ACC,    $vf05,  $vf14  \n\t"
            "vmaddaz    ACC,    $vf06,  $vf14  \n\t"
            "vmaddw     $vf14,  $vf07,  $vf14  \n\t"
            "vmulax     ACC,    $vf04,  $vf15  \n\t"
            "vmadday    ACC,    $vf05,  $vf15  \n\t"
            "vmaddaz    ACC,    $vf06,  $vf15  \n\t"
            "vmaddw     $vf15,  $vf07,  $vf15  \n\t"
            "vmulax     ACC,    $vf04,  $vf16  \n\t"
            "vmadday    ACC,    $vf05,  $vf16  \n\t"
            "vmaddaz    ACC,    $vf06,  $vf16  \n\t"
            "vmaddw     $vf16,  $vf07,  $vf16  \n\t"
        : : "r"(matrix));
    }

    asm volatile(
        "sqc2       $vf13,  0x0(%0)        \n\t"
        "sqc2       $vf14, 0x10(%0)        \n\t"
        "sqc2       $vf15, 0x20(%0)        \n\t"
        "sqc2       $vf16, 0x30(%0)        \n\t"
    : : "r"(&return_buffer) : "memory");

    return &return_buffer;
}

bool SpaNodeAnimation::IsVisible(float arg0) const {
    if (this->unk4 == NULL) {
        return true;
    }

    return *this->unk4->GetValue(arg0);
}

bool SpaFileHeader::IsNodeVisible(SpmNode *arg0, float arg1) const {
    SpaNodeAnimation *a0 = this->unk50[arg0->unk150];
    SpmNode *a2 = arg0->unk164;

    if (a0 != NULL && a0->unk4 == NULL) {
        a0 = NULL;
    }

    if (a2 != NULL) {
        if (!(a2->m_flags & 0x4000)) {
            return false;
        }
        if (a2->m_flags & 0x40000) {
            if (a0 == NULL) {
                arg0->m_flags |= 0x40000;
                if (a2->m_flags & 0x4000) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    if (a0 != NULL) {
        arg0->m_flags |= 0x40000;
        return a0->IsVisible(arg1);
    }

    arg0->m_flags &= ~0x40000;
    return (arg0->m_flags & 0x20000) ? false : true;
}

bool SpaTransform::IsEverIdentical() {
    switch (this->unk0) {
    case 0:
        if (this->unk14 == 1) {
            bool v0 = false;
            if (this->unk20[0][0] == 1.0f) {
                if (this->unk20[0][1] != 1.0f) {
                    return false;
                }
                if (this->unk20[0][2] == 1.0f) {
                    v0 = true;
                }
            }
            return v0;
        }

        return false;

    case 1:
        if (this->unk14 == 1) {
            if (this->unk20[0][3] == 0.0f) {
                return true;
            }
        }

        return false;

    case 2:
    case 3:
    case 4:
        if (this->unk14 == 1) {
            if (this->unk20[0][0] == 0.0f) {
                return true;
            }
        }

        return false;

    case 5:
        if (this->unk14 == 1) {
            bool v0 = false;
            if (this->unk20[0][0] == 0.0f) {
                if (this->unk20[0][1] != 0.0f) {
                    return false;
                }
                if (this->unk20[0][2] == 0.0f) {
                    v0 = true;
                }
            }
            return v0;
        }

        return false;

    case 6:
        if (this->unk14 == 1) {
            return this->unk20.inl0();
        }

        return false;
        
    case 7:
        if (this->unk14 == 1) {
            bool v0 = false;
            if (this->unk20[0][0] == 0.0f) {
                if (this->unk20[0][1] != 0.0f) {
                    return false;
                }
                if (this->unk20[0][2] == 0.0f) {
                    v0 = true;
                }
            }
            return v0;
        }

        return false;

    default:
        NaMATRIX<float, 4, 4>& a2 = NaMATRIX<float, 4, 4>::IDENT;
        return a2.inl1();
    }
}

int SpaNodeAnimation::Optimize() {
    int remove_count = 0;

    for (int i = 0; i < this->unk8; i++) {
        SpaTransform *transform = this->unkC[i];
        if (transform == NULL || transform->IsEverIdentical()) {
            remove_count++;
            continue;
        }
        this->unkC[i - remove_count] = this->unkC[i];
    }

    this->unk8 -= remove_count;
    return remove_count;
}

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", GetLinearValue__Ct8SpaTrack1Zt8NaVECTOR2Zfi4Uif);
#if 0
// TODO: nonmatching (regalloc only: a0/a1/a2 swaps in the address computations).
// Requires NaVECTOR to have a copy constructor that copies with lq/sq.
static inline NaVECTOR<float, 4> operator*(const NaVECTOR<float, 4>& v, float f) {
    NaVECTOR<float, 4> ret;
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "mfc1       $8,     %2             \n\t"
        "qmtc2      $8,     $vf05          \n\t"
        "vmulx.xyzw $vf06,  $vf04,  $vf05  \n\t"
        "sqc2       $vf06,  0x0(%0)        \n\t"
    : : "r"(&ret), "r"(&v), "f"(f) : "$8", "memory");
    return ret;
}

static inline NaVECTOR<float, 4> operator+(const NaVECTOR<float, 4>& a, const NaVECTOR<float, 4>& b) {
    NaVECTOR<float, 4> ret;
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "lqc2       $vf05,  0x0(%2)        \n\t"
        "vadd.xyzw  $vf06,  $vf04,  $vf05  \n\t"
        "sqc2       $vf06,  0x0(%0)        \n\t"
    : : "r"(&ret), "r"(&a), "r"(&b) : "memory");
    return ret;
}

static inline NaVECTOR<float, 4> operator/(const NaVECTOR<float, 4>& v, float f) {
    NaVECTOR<float, 4> ret;
    asm volatile(
        "lqc2       $vf04,  0x0(%1)        \n\t"
        "mfc1       $8,     %2             \n\t"
        "qmtc2      $8,     $vf05          \n\t"
        "vdiv       Q,      $vf00w, $vf05x \n\t"
        "vwaitq                            \n\t"
        "vmulq.xyzw $vf04,  $vf04,  Q      \n\t"
        "sqc2       $vf04,  0x0(%0)        \n\t"
    : : "r"(&ret), "r"(&v), "f"(f) : "$8", "memory");
    return ret;
}

template <>
NaVECTOR<float, 4>* SpaTrack<NaVECTOR<float, 4> >::GetLinearValue(u_int seg, float arg1) const {
    static NaVECTOR<float, 4> return_buffer;

    float f0 = this->unkC[seg + 1] - arg1;
    float f1 = arg1 - this->unkC[seg];
    NaVECTOR<float, 4> ret = (((NaVECTOR<float, 4>*)&this->unk10)[seg] * f0 + ((NaVECTOR<float, 4>*)&this->unk10)[seg + 1] * f1) / (f0 + f1);
    return_buffer = ret;
    return &return_buffer;
}
#endif

template <>
NaVECTOR<float, 4>* SpaTrack<NaVECTOR<float, 4> >::GetValue(float arg0) const {
    if (this->unk2 & 0x1) {
        float f13 = this->unkC[this->unk4 - 1];
        if (arg0 < 0.0f || arg0 >= f13) {
            arg0 = fmodf(arg0, f13);
        }
    }

    u_int seg = this->SearchSegment(arg0);

    if (seg == (u_int)-1) {
        return (NaVECTOR<float, 4>*)&this->unk10;
    }

    if (seg == this->unk4) {
        if (this->unk0 == 0) {
            return (NaVECTOR<float, 4>*)&this->unk10 + ((seg - 1) * 3);
        } else {
            return (NaVECTOR<float, 4>*)this + seg;
        }
    }

    switch (this->unk0) {
    case 0:
        return this->GetSprineValue(seg, arg0);
    case 1:
        return this->GetLinearValue(seg, arg0);
    case 2:
        return (NaVECTOR<float, 4>*)&this->unk10 + seg;
    default:
        break;
    }

    return NULL;
}

/* nalib/navector.h */
float* func_00149168(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) asm("func_00149168");

float* func_00149168(
    float* self,
    const float& a0, const float& a1, const float& a2,
    const float& a3, const float& a4, const float& a5,
    const float& a6, const float& a7, const float& a8
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    self[4] = a4;
    self[5] = a5;
    self[6] = a6;
    self[7] = a7;
    self[8] = a8;
    return self;
}

float* func_001491C0(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) asm("func_001491C0");

float* func_001491C0(
    float* self,
    const float& a0, const float& a1, const float& a2, const float& a3,
    const float& a4, const float& a5, const float& a6, const float& a7,
    const float& a8, const float& a9, const float& a10, const float& a11,
    const float& a12, const float& a13, const float& a14, const float& a15
) {
    self[0] = a0;
    self[1] = a1;
    self[2] = a2;
    self[3] = a3;
    self[4] = a4;
    self[5] = a5;
    self[6] = a6;
    self[7] = a7;
    self[8] = a8;
    self[9] = a9;
    self[10] = a10;
    self[11] = a11;
    self[12] = a12;
    self[13] = a13;
    self[14] = a14;
    self[15] = a15;
    return self;
}

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", func_00149270);

/* prlib/spadata.cpp */
template <>
float* SpaTrack<float>::GetSprineValue(u_int seg, float arg1) const {
    static float return_buffer;

    float f2 = this->unkC[seg + 1] - this->unkC[seg];
    float *a0 = (float*)&this->unk10 + (seg * 3);
    const float& p2 = ((float*)&this->unk10)[seg * 3 + 2];
    const float& p4 = ((float*)&this->unk10)[seg * 3 + 4];
    const float& p3 = ((float*)&this->unk10)[seg * 3 + 3];
    if (f2 == 0.0f) {
        return a0;
    }

    float f0 = (arg1 - this->unkC[seg]) / f2;
    float d = a0[0] - p3;

    return_buffer = f0 * (f0 * (f0 * ((p2 + p4) * f2 + (d + d)) - (p2 + p2 + p4) * f2 - d * 3.0f) + p2 * f2) + a0[0];
    return &return_buffer;
}

template <>
float* SpaTrack<float>::GetLinearValue(u_int seg, float arg1) const {
    static float return_buffer;

    float f2 = this->unkC[seg + 1] - arg1;
    arg1 -= this->unkC[seg];
    u_int next = seg + 1;
    float *values = (float*)&this->unk10;
    float *a = &values[seg];
    float *b = &values[next];

    return_buffer = (f2 * *a + arg1 * *b) / (f2 + arg1);
    return &return_buffer;
}

template <>
float* SpaTrack<float>::GetValue(float arg0) const {
    if (this->unk2 & 0x1) {
        float f13 = this->unkC[this->unk4 - 1];
        if (arg0 < 0.0f || arg0 >= f13) {
            arg0 = fmodf(arg0, f13);
        }
    }

    u_int seg = this->SearchSegment(arg0);

    if (seg == (u_int)-1) {
        return (float*)&this->unk10;
    }

    if (seg == this->unk4) {
        if (this->unk0 == 0) {
            return (float*)&this->unk10 + ((seg - 1) * 3);
        } else {
            return (float*)&this->unkC + seg;
        }
    }

    switch (this->unk0) {
    case 0:
        return this->GetSprineValue(seg, arg0);
    case 1:
        return this->GetLinearValue(seg, arg0);
    case 2:
        return (float*)&this->unk10 + seg;
    default:
        break;
    }

    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", GetSprineValue__Ct8SpaTrack1Zt8NaMATRIX3Zfi4i4Uif);

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", GetLinearValue__Ct8SpaTrack1Zt8NaMATRIX3Zfi4i4Uif);

template <>
NaMATRIX<float, 4, 4>* SpaTrack<NaMATRIX<float, 4, 4> >::GetValue(float arg0) const {
    if (this->unk2 & 0x1) {
        float f13 = this->unkC[this->unk4 - 1];
        if (arg0 < 0.0f || arg0 >= f13) {
            arg0 = fmodf(arg0, f13);
        }
    }

    u_int seg = this->SearchSegment(arg0);

    if (seg == (u_int)-1) {
        return (NaMATRIX<float, 4, 4>*)&this->unk10;
    }

    if (seg == this->unk4) {
        if (this->unk0 == 0) {
            return (NaMATRIX<float, 4, 4>*)&this->unk10 + ((seg - 1) * 3);
        } else {
            return (NaMATRIX<float, 4, 4>*)&this->unk10 + (seg - 1);
        }
    }

    switch (this->unk0) {
    case 0:
        return this->GetSprineValue(seg, arg0);
    case 1:
        return this->GetLinearValue(seg, arg0);
    case 2:
        return (NaMATRIX<float, 4, 4>*)&this->unk10 + seg;
    default:
        break;
    }

    return NULL;
}

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/spadata", func_0014ABE0);

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", func_0014ACE8);

INCLUDE_ASM("asm/nonmatchings/prlib/spadata", func_0014AFE0);


template <>
NaVECTOR<float, 4>* SpaTrack<NaVECTOR<float, 4> >::GetSprineValue(u_int seg, float arg1) const {
    float f2 = this->unkC[seg + 1] - this->unkC[seg];
    NaVECTOR<float, 4> *a0 = (NaVECTOR<float, 4>*)&this->unk10 + (seg * 3);
    if (f2 == 0.0f) {
        return a0;
    }

    float f0 = (arg1 - this->unkC[seg]) / f2;

    /*
     * FIXME(poly): These `tmp_X` symbols aren't real
     * and are product of the static NaVECTOR instances.
     *
     * The solution would be to make these static
     * (when the TU's .bss matches, of course), and
     * the compiler should generate these temp. symbols.
     *
     * The symbols themselves are meant to call the
     * constructor once, though in this case there's
     * no actual call because it is inlined (empty constructor).
     */
    extern NaVECTOR<float, 4> return_buffer;
    extern int tmp_0_1744;
    if (tmp_0_1744 == 0) {
        tmp_0_1744 = 1;
    }

    #if 0
        vf04.xyzw = a0[0];
        vf05.xyzw = a0[3];
        vf01.xyzw = vf04.xyzw - vf05.xyzw;
        
        vf06.xyzw = a0[2];
        vf07.xyzw = a0[4];
        vf02.xyzw = vf06.xyzw + vf07.xyzw;
        
        vf08.x    = f2;
        ACC.xyzw  = vf01.xyzw + vf01.xyzw;
        vf03.xyzw = ACC.xyzw + (vf02.xyzw * vf08.x);
        
        vf09.x    = f0;
        ACC.xyzw  = vf06.xyzw + vf06.xyzw;
        vf02.xyzw = ACC.xyzw + (vf07.xyzw * vf00.w);
        ACC.w     = vf00.w + vf00.w;
        vf08.w    = ACC.w + (vf00.w * vf00.w);
        ACC.xyzw  = vf03.xyzw * vf09.x;
        ACC.xyzw  = ACC.xyzw - (vf02.xyzw * vf08.x);
        vf10.x    = vf09.x * vf08.x;
        vf02.x    = vf09.x * vf09.x;
        vf03.xyzw = ACC.xyzw - (vf01.xyzw * vf08.w);
        ACC.xyzw  = vf04.xyzw + vf00.x;
        ACC.xyzw  = ACC.xyzw + (vf06.xyzw * vf10.x);
        vf01.xyzw = ACC.xyzw + (vf03.xyzw * vf02.x);
        return_buffer = vf01.xyzw;
    #else
    asm volatile(
        "lqc2     $vf04,  0x00(%1)       \n\t"
        "lqc2     $vf05,  0x30(%1)       \n\t"
        "vsub     $vf01,  $vf04,  $vf05  \n\t"
        
        "lqc2     $vf06,  0x20(%1)       \n\t"
        "lqc2     $vf07,  0x40(%1)       \n\t"
        "vadd     $vf02,  $vf06,  $vf07  \n\t"
        
        "qmtc2    %2,     $vf08          \n\t"
        "vadda    ACC,    $vf01,  $vf01  \n\t"
        "vmaddx   $vf03,  $vf02,  $vf08  \n\t"
        
        "qmtc2    %3,     $vf09          \n\t"
        "vadda    ACC,    $vf06,  $vf06  \n\t"
        "vmaddw   $vf02,  $vf07,  $vf00  \n\t"
        "vadda.w  ACC,    $vf00,  $vf00  \n\t"
        "vmadd.w  $vf08,  $vf00,  $vf00  \n\t"
        "vmulax   ACC,    $vf03,  $vf09  \n\t"
        "vmsubax  ACC,    $vf02,  $vf08  \n\t"
        "vmul.x   $vf10,  $vf09,  $vf08  \n\t"
        "vmul.x   $vf02,  $vf09,  $vf09  \n\t"
        "vmsubw   $vf03,  $vf01,  $vf08  \n\t"
        "vaddax   ACC,    $vf04,  $vf00  \n\t"
        "vmaddax  ACC,    $vf06,  $vf10  \n\t"
        "vmaddx   $vf01,  $vf03,  $vf02  \n\t"
        "sqc2     $vf01,  0(%0)          \n\t"
    : : "r"(&return_buffer), "r"(a0), "r"(f2), "r"(f0));
    #endif

    return &return_buffer;
}
