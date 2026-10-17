#ifndef PRLIB_VU0MAT_H
#define PRLIB_VU0MAT_H

#include "common.h"

#include <nalib/navector.h>
#include <nalib/namatrix.h>

/* Stall loop emitted around VU0 macro-mode matrix operations. */
#define PR_VU0_WAIT()                                \
    do {                                             \
        int _wait = 3;                               \
        do {                                         \
            _wait--;                                 \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
            asm volatile("nop");                     \
        } while (_wait != -1);                       \
    } while (0)

/* dst = b * a (row vectors of b transformed by matrix a) */
static inline void PrMulMatrix(NaMATRIX<float, 4, 4> *dst, const NaMATRIX<float, 4, 4> *b, const NaMATRIX<float, 4, 4> *a) {
    PR_VU0_WAIT();

    asm volatile(
        "lqc2     $vf4,   0x0(%0)       \n\t"
        "lqc2     $vf5,  0x10(%0)       \n\t"
        "lqc2     $vf6,  0x20(%0)       \n\t"
        "lqc2     $vf7,  0x30(%0)       \n\t"
    : : "r"(a));

    asm volatile(
        "lqc2     $vf8,   0x0(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,   0x0(%0)       \n\t"
        "lqc2     $vf8,  0x10(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x10(%0)       \n\t"
        "lqc2     $vf8,  0x20(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x20(%0)       \n\t"
        "lqc2     $vf8,  0x30(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,  0x30(%0)       \n\t"
    : : "r"(dst), "r"(b));

    PR_VU0_WAIT();
}

/* dst = v * m */
static inline void PrMulVector(NaVECTOR<float, 4> *dst, const NaVECTOR<float, 4> *v, const NaMATRIX<float, 4, 4> *m) {
    asm volatile(
        "lqc2     $vf4,   0x0(%0)       \n\t"
        "lqc2     $vf5,  0x10(%0)       \n\t"
        "lqc2     $vf6,  0x20(%0)       \n\t"
        "lqc2     $vf7,  0x30(%0)       \n\t"
    : : "r"(m));

    asm volatile(
        "lqc2     $vf8,   0x0(%1)       \n\t"
        "vmulax   ACC,    $vf4,   $vf8  \n\t"
        "vmadday  ACC,    $vf5,   $vf8  \n\t"
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t"
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t"
        "sqc2     $vf9,   0x0(%0)       \n\t"
    : : "r"(dst), "r"(v));
}

#endif /* PRLIB_VU0MAT_H */
