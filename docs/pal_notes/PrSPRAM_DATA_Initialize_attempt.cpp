/* Best non-matching PAL attempt for PrSPRAM_DATA::Initialize (PAL 0x14a4f8, 733 instrs).
 * Drop in place of the INCLUDE_ASM in src/prlib/spram.cpp; also needs, before it:
 *   extern NaMATRIX<float, 4, 4> screenClipMatrix; extern NaMATRIX<float, 4, 4> screenPrimitiveMatrix;
 *   #include "scene.h"
 * Status: 744 vs 733 instrs, 589/733 mnemonic-aligned equal. Semantics fully decoded (below is
 * believed to be functionally exact): camera copy, zdir = normalize(interest - position) (w=1),
 * sceVu0CameraMatrix(cam, pos, zdir, -up) [0x166498], three sceVu0ViewScreenMatrix [0x166670]
 * into this+0x1A0/0x1E0/0x220 each multiplied by the camera matrix into 0xE0/0x120/0x160,
 * screenClipMatrix/screenPrimitiveMatrix setup via func_00148248/00147FB8/00148140/00147E38.
 * Remaining diffs are register allocation / spill-slot order: target uses the hoisted address
 * registers (s7=&camera_matrix, s8=&view_screen[0]) directly as asm operands of the inlined
 * matrix multiply, ours copies them into fresh pseudos (inline-parameter copies made by
 * integrate.c when the actual arg is not a REG_USERVAR_P pseudo) -> +1 frame qword, different
 * spill slots. Making the operands reference locals (SprMat& cm = ...) removes the copies for the
 * operands but adds others (585/733). Same root cause as SpaTrack<NaMATRIX> GetLinear/GetSprine.
 */
#include "renderstuff.h"
#include <math.h>

struct SprVec {
    SprVec() {}
    SprVec(const SprVec& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "memory");
    }
    SprVec& operator=(const SprVec& rhs) {
        asm volatile("
            lq $6, 0(%1)
            sq $6, 0(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "memory");
        return *this;
    }
    SprVec operator-(const SprVec& rhs) const {
        SprVec ret;
        asm volatile("
            lqc2       $vf04,  0x0(%1)
            lqc2       $vf05,  0x0(%2)
            vsub.xyzw  $vf06,  $vf04,  $vf05
            sqc2       $vf06,  0x0(%0)
        " : : "r"(&ret), "r"(this), "r"(&rhs) : "memory");
        return ret;
    }
    SprVec operator-() const {
        SprVec ret;
        for (int i = 0; i < 4; i++) {
            ret.v[i] = 0.0f - v[i];
        }
        return ret;
    }
    SprVec Normalize() const {
        SprVec ret;
        asm volatile("
            lqc2       $vf04,  0x0(%1)
            vmul.xyz   $vf05,  $vf04,  $vf04
            vaddy.x    $vf05,  $vf05,  $vf05
            vaddz.x    $vf05,  $vf05,  $vf05
            vsqrt      Q,      $vf05x
            vwaitq
            vaddq.x    $vf05,  $vf00,  Q
            vdiv       Q,      $vf00w, $vf05x
            vsub.xyzw  $vf06,  $vf00,  $vf00
            vwaitq
            vmulq.xyz  $vf06,  $vf04,  Q
            sqc2       $vf06,  0x0(%0)
        " : : "r"(&ret), "r"(this) : "memory");
        ret.v[3] = 1.0f;
        return ret;
    }
    float v[4];
} __attribute__((aligned(16)));

struct SprMat {
    SprMat() {}
    SprMat(const SprMat& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(this), "r"(&rhs) : "$6", "$7", "$8", "$9");
    }
    static SprMat& Copy(SprMat& lhs, const SprMat& rhs) {
        asm volatile("
            lq $6, 0(%1)
            lq $7, 0x10(%1)
            lq $8, 0x20(%1)
            lq $9, 0x30(%1)
            sq $6, 0(%0)
            sq $7, 0x10(%0)
            sq $8, 0x20(%0)
            sq $9, 0x30(%0)
        " : : "r"(&lhs), "r"(&rhs) : "$6", "$7", "$8", "$9");
        return lhs;
    }
    SprMat& operator=(const SprMat& rhs) {
        return Copy(*this, rhs);
    }
    SprMat operator*(const SprMat& rhs) const {
        SprMat ret;
        asm volatile(
            "lqc2     $vf4,   0x0(%0)       \n\t"
            "lqc2     $vf5,  0x10(%0)       \n\t"
            "lqc2     $vf6,  0x20(%0)       \n\t"
            "lqc2     $vf7,  0x30(%0)       \n\t"
        : : "r"(&rhs));
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
        : : "r"(&ret), "r"(this));
        return ret;
    }
    SprVec m[4];
};

struct SprCamera {
    SprVec position;
    SprVec interest;
    SprVec up;
    float aspect;
    float field_of_view;
    float near_clip;
    float far_clip;
};

struct SprData {
    char unk0[0x40];
    u_int unk40;
    u_int unk44;
    u_int unk48;
    char unk4C[0x4];
    SprCamera camera;
    SprVec zdir;
    SprMat camera_matrix;
    SprMat view_projection_matrix;
    SprMat unk120;
    SprMat unk160;
    SprMat view_screen[3];
    char unk260[0x40];
    sceDmaTag end_dmatag;
    char unk2B0[0x3BC];
    u_int unk66C;
    u_int unk670;
    u_int unk674;
    u_int unk678;
};

extern "C" void sceVu0CameraMatrix(SprMat* m, SprVec* p, SprVec* zd, SprVec* yd);
extern "C" void sceVu0ViewScreenMatrix(SprMat* m, float scrz, float ax, float ay,
    float cx, float cy, float zmin, float zmax, float nearz, float farz);

SprMat Spr_func_00148248(const float& x, const float& y, const float& z) asm("func_00148248");
SprMat* Spr_func_00147FB8(SprMat* self, const float& x, const float& y, const float& z) asm("func_00147FB8");
SprMat* Spr_func_00148140(SprMat* self, const float& x, const float& y, const float& z) asm("func_00148140");
SprMat* Spr_func_00147E38(SprMat* self, const float& x, const float& y, const float& z) asm("func_00147E38");

void PrSPRAM_DATA::Initialize(PrSceneObject *scene) {
    static const sceDmaTag endTag = { 0, 0, 0x70, NULL, { 0, 0 } };
    SprData* d = (SprData*)this;

    d->unk40 = (u_int)this + 0x1000;
    d->unk44 = (u_int)this + 0x2000;
    d->unk48 = (u_int)this + 0x3000;
    d->end_dmatag = endTag;

    d->camera = *(SprCamera*)scene->GetCurrentCamera();
    d->zdir = (d->camera.interest - d->camera.position).Normalize();
    sceVu0CameraMatrix(&d->camera_matrix, &d->camera.position, &d->zdir, &-d->camera.up);

    u_int width = (u_int)scene->unk74;
    u_int height = (u_int)scene->unk78;
    float zmax = (float)(0xFFFFFFFFu >> (37 - prRenderStuff.GetZbufBits()));
    float zmin = zmax * 0.000002f;
    zmax = zmax * 0.999998f;
    float aspect = (float)width / (float)height * 3.0f * 0.25f;
    float scrz = 1.0f / tanf(d->camera.field_of_view * 0.5f);

    sceVu0ViewScreenMatrix(&d->view_screen[0], scrz,
        (float)height * 0.5f * aspect, (float)height * 0.5f,
        2048.0f, 2048.0f, zmin, zmax, d->camera.near_clip, d->camera.far_clip);
    zmax -= zmin;
    d->view_projection_matrix = d->camera_matrix * d->view_screen[0];

    sceVu0ViewScreenMatrix(&d->view_screen[1], scrz,
        (float)height * 1.01f * 0.5f * aspect * (1.0f / 2048.0f), (float)height * 1.01f * 0.5f * (1.0f / 2048.0f),
        0.0f, 0.0f, -1.0f, 1.0f, d->camera.near_clip, d->camera.far_clip);
    d->unk120 = d->camera_matrix * d->view_screen[1];

    sceVu0ViewScreenMatrix(&d->view_screen[2], scrz,
        4096.0f / (float)(width + 2) * 0.99f * (float)height * 0.5f * aspect * (1.0f / 2048.0f),
        4096.0f / (float)(height + 2) * 0.99f * (float)height * 0.5f * (1.0f / 2048.0f),
        0.0f, 0.0f, -1.0f, 1.0f, d->camera.near_clip, d->camera.far_clip);
    d->unk160 = d->camera_matrix * d->view_screen[2];

    SprMat m = Spr_func_00148248((float)width * 0.5f, -(float)height * 0.5f, 5010.0f);
    *(SprMat*)&screenClipMatrix = m;

    Spr_func_00147FB8(&m, 1.01f / 2048.0f, 1.01f / 2048.0f, 1.0f / 5010.0f);
    Spr_func_00148140(&m, 1.0f, -0.5f, zmax / 10020.0f);
    *(SprMat*)&screenPrimitiveMatrix = m;
    Spr_func_00147E38((SprMat*)&screenPrimitiveMatrix,
        2048.0f - (float)width * 0.5f, 2048.0f - (float)height * 0.5f,
        zmin - zmax * -10010.0f / 10020.0f);

    d->unk66C = 0;
    d->unk670 = 0;
    d->unk674 = 0;
    d->unk678 = 0;
}
