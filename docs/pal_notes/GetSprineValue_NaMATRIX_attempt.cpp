/* Best non-matching PAL attempt. Drop into src/prlib/spadata.cpp in place of the
 * INCLUDE_ASM (spadata.cpp needs `#include <float.h>` and `#include "vu0mat.h"`).
 * SpaTrack<NaMATRIX<float,4,4>>::GetSprineValue (0x14cd48) and GetLinearValue (0x14dc38), both OK addresses.
 * Requires the header changes below (all under VERSION_PAL). With them the structure is right:
 * the PR_VU0_WAIT stall loops are just the R5900 short-loop-padded array-ctor loop for
 * NaVECTOR m[4] (empty NaVECTOR ctor) -> emitted for every NaMATRIX construction, incl. the
 * static return_buffer guard. Frame sizes match (1536 for Sprine; Linear 496 vs 480).
 * GetSprineValue: 982 vs 956 instrs, ~425 differ (mnemonic-aligned 838 equal).
 * GetLinearValue:  284 vs 274 instrs, ~156 differ (mnemonic-aligned 268 equal).
 * Cause (both): every matrix-returning inline operator gets an extra pseudo for its hidden
 * return-slot pointer and for &ret in the copy-ctor; GCSE turns them into copies of the hoisted
 * sp+N address regs (extra `move tX,tY` before each 4x lq/sq copy) -> one extra saved reg and
 * cascading regalloc. Target uses the hoisted address regs directly. Tried: member vs free ops,
 * copy-ctor via Copy / *this=rhs / direct asm, NRV (`return ret`), by-value params: no change
 * (NRV/by-value remove the copies entirely, which is wrong).
 * NaVECTOR::Copy must lose its "memory" clobber under PAL (as NaMATRIX::Copy already did),
 * otherwise the float scale is reloaded each row.
 *
 * Header diff used:
 * diff --git a/src/nalib/namatrix.h b/src/nalib/namatrix.h
 * index da6ddec..d4df1f5 100644
 * --- a/src/nalib/namatrix.h
 * +++ b/src/nalib/namatrix.h
 * @@ -6,6 +6,54 @@
 *  template <typename T, int t0, int t1>
 *  class NaMATRIX {
 *  public:
 * +#ifdef VERSION_PAL
 * +    NaMATRIX() {}
 * +
 * +    NaMATRIX(const NaMATRIX<float, 4, 4>& rhs) {
 * +        asm volatile("
 * +            lq $6, 0(%1)
 * +            lq $7, 0x10(%1)
 * +            lq $8, 0x20(%1)
 * +            lq $9, 0x30(%1)
 * +            sq $6, 0(%0)
 * +            sq $7, 0x10(%0)
 * +            sq $8, 0x20(%0)
 * +            sq $9, 0x30(%0)
 * +        " : : "r"(m), "r"(rhs.m) : "$6", "$7", "$8", "$9");
 * +    }
 * +
 * +    NaMATRIX<T, t0, t1> operator*(const T& f) const {
 * +        NaMATRIX<T, t0, t1> ret;
 * +        for (int i = 0; i < t1; i++) {
 * +            ret.m[i] = m[i] * f;
 * +        }
 * +        return ret;
 * +    }
 * +
 * +    NaMATRIX<T, t0, t1> operator/(const T& f) const {
 * +        NaMATRIX<T, t0, t1> ret;
 * +        for (int i = 0; i < t1; i++) {
 * +            ret.m[i] = m[i] / f;
 * +        }
 * +        return ret;
 * +    }
 * +
 * +    NaMATRIX<T, t0, t1> operator+(const NaMATRIX<T, t0, t1>& rhs) const {
 * +        NaMATRIX<T, t0, t1> ret;
 * +        for (int i = 0; i < t1; i++) {
 * +            ret.m[i] = m[i] + rhs.m[i];
 * +        }
 * +        return ret;
 * +    }
 * +
 * +    NaMATRIX<T, t0, t1> operator-(const NaMATRIX<T, t0, t1>& rhs) const {
 * +        NaMATRIX<T, t0, t1> ret;
 * +        for (int i = 0; i < t1; i++) {
 * +            ret.m[i] = m[i] - rhs.m[i];
 * +        }
 * +        return ret;
 * +    }
 * +#endif
 *      const NaVECTOR<T, t0>& operator[](int arg0) const {
 *          return m[arg0];
 *      }
 * diff --git a/src/nalib/navector.h b/src/nalib/navector.h
 * index ac42934..6697ac5 100644
 * --- a/src/nalib/navector.h
 * +++ b/src/nalib/navector.h
 * @@ -5,6 +5,59 @@ template <typename T, int t0>
 *  class NaVECTOR {
 *  public:
 *      NaVECTOR() {}
 * +#ifdef VERSION_PAL
 * +    NaVECTOR(const NaVECTOR<float, 4>& rhs) {
 * +        Copy(*this, rhs);
 * +    }
 * +
 * +    NaVECTOR<T, t0> operator*(const T& f) const {
 * +        NaVECTOR<T, t0> ret;
 * +        asm volatile("
 * +            lqc2       $vf04,  0x0(%1)
 * +            mfc1       $8,     %2
 * +            qmtc2      $8,     $vf05
 * +            vmulx.xyzw $vf06,  $vf04,  $vf05
 * +            sqc2       $vf06,  0x0(%0)
 * +        " : : "r"(&ret), "r"(this), "f"(f) : "$8");
 * +        return ret;
 * +    }
 * +
 * +    NaVECTOR<T, t0> operator/(const T& f) const {
 * +        NaVECTOR<T, t0> ret;
 * +        asm volatile("
 * +            lqc2       $vf04,  0x0(%1)
 * +            mfc1       $8,     %2
 * +            qmtc2      $8,     $vf05
 * +            vdiv       Q,      $vf00w, $vf05x
 * +            vwaitq
 * +            vmulq.xyzw $vf04,  $vf04,  Q
 * +            sqc2       $vf04,  0x0(%0)
 * +        " : : "r"(&ret), "r"(this), "f"(f) : "$8");
 * +        return ret;
 * +    }
 * +
 * +    NaVECTOR<T, t0> operator+(const NaVECTOR<T, t0>& rhs) const {
 * +        NaVECTOR<T, t0> ret;
 * +        asm volatile("
 * +            lqc2       $vf04,  0x0(%1)
 * +            lqc2       $vf05,  0x0(%2)
 * +            vadd.xyzw  $vf06,  $vf04,  $vf05
 * +            sqc2       $vf06,  0x0(%0)
 * +        " : : "r"(&ret), "r"(this), "r"(&rhs));
 * +        return ret;
 * +    }
 * +
 * +    NaVECTOR<T, t0> operator-(const NaVECTOR<T, t0>& rhs) const {
 * +        NaVECTOR<T, t0> ret;
 * +        asm volatile("
 * +            lqc2       $vf04,  0x0(%1)
 * +            lqc2       $vf05,  0x0(%2)
 * +            vsub.xyzw  $vf06,  $vf04,  $vf05
 * +            sqc2       $vf06,  0x0(%0)
 * +        " : : "r"(&ret), "r"(this), "r"(&rhs));
 * +        return ret;
 * +    }
 * +#endif
 *  
 *      NaVECTOR<T, t0>& Set(const T& x, const T& y, const T& z, const T& w);
 *  
 * @@ -38,7 +91,11 @@ public:
 *              lq $6, 0(%1)
 *              sq $6, 0(%0)
 *          " : : "r"(&lhs), "r"(&rhs)
 * +#ifdef VERSION_PAL
 * +        : "$6");
 * +#else
 *          : "$6", "memory");
 * +#endif
 *          return lhs;
 *      }
 *  
 */
typedef NaVECTOR<float, 4> Vec4;
typedef NaMATRIX<float, 4, 4> Mat44;

template <>
Mat44* SpaTrack<Mat44>::GetSprineValue(u_int seg, float arg1) const {
    static Mat44 return_buffer;

    float f2 = this->unkC[seg + 1] - this->unkC[seg];
    Mat44 *a0 = (Mat44*)&this->unk10 + (seg * 3);
    const Mat44& p2 = ((Mat44*)&this->unk10)[seg * 3 + 2];
    const Mat44& p4 = ((Mat44*)&this->unk10)[seg * 3 + 4];
    const Mat44& p3 = ((Mat44*)&this->unk10)[seg * 3 + 3];
    if (f2 == 0.0f) {
        return a0;
    }

    float f0 = (arg1 - this->unkC[seg]) / f2;
    Mat44 d = a0[0] - p3;

    return_buffer = ((((p2 + p4) * f2 + d * 2.0f) * f0 - (p2 * 2.0f + p4) * f2 - d * 3.0f) * f0 + p2 * f2) * f0 + a0[0];
    return &return_buffer;
}

template <>
Mat44* SpaTrack<Mat44>::GetLinearValue(u_int seg, float arg1) const {
    static Mat44 return_buffer;

    float f1 = arg1 - this->unkC[seg];
    float f0 = this->unkC[seg + 1] - arg1;
    Mat44 ret = (((Mat44*)&this->unk10)[seg] * f0 + ((Mat44*)&this->unk10)[seg + 1] * f1) / (f0 + f1);
    return_buffer = ret;
    return &return_buffer;
}
