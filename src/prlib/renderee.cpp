#include "renderstuff.h"

#include "spram.h"

extern PrSPRAM_DATA *prRenderEeSpram;

#include "dma.h"
#include "scene.h"

struct PrEECoreInitPacket {
    sceDmaTag     tag;
    sceGifTag     giftag;
    sceGsFrame    frame;
    u_long        frame_addr;
    sceGsZbuf     zbuf;
    u_long        zbuf_addr;
    u_long        unk40[16];
    sceGsXyoffset xyoffset;
    u_long        xyoffset_addr;
    sceGsScissor  scissor;
    u_long        scissor_addr;
    sceGsDthe     dthe;
    u_long        dthe_addr;
} PR_ALIGNED(128);

extern PrEECoreInitPacket D_00371800;

void PrRenderStuff::InitializeEECore(PrSceneObject *scene) {
    void *tag = &D_00371800;
    PrEECoreInitPacket *packet = (PrEECoreInitPacket*)PR_UNCACHED(tag);
    packet->zbuf     = this->m_zbuf;
    packet->frame    = scene->unk50;
    packet->xyoffset = scene->unk58;
    packet->scissor  = scene->unk70->scissor1;
    packet->dthe     = scene->unk70->dthe;

    PrWaitDmaFinish(SCE_DMA_GIF);

    sceDmaChan *chan = sceDmaGetChan(SCE_DMA_GIF);
    chan->chcr.TTE = 0;

    register void *a1 asm("$5") = tag;
    asm("" : "+r"(a1));
    sceDmaSend(chan, a1);
    asm volatile("");
}

void PrRenderStuff::RenderVertexEECoreBothface() {
    register char *spr asm("$4") = reinterpret_cast<char*>(prRenderEeSpram) + 0x330;
    asm volatile(
        ".set noreorder\n\t"
        "    lqc2 vf12, 0(%0)\n\t"
        "    lqc2 vf13, 16(%0)\n\t"
        "    lqc2 vf14, 32(%0)\n\t"
        "    lui $3, 0x4\n\t"
        "    vmulax.xyzw ACC, vf5, vf12x\n\t"
        "    vmadday.xyzw ACC, vf6, vf12y\n\t"
        "    vmaddaz.xyzw ACC, vf7, vf12z\n\t"
        "    vmaddw.xyzw vf16, vf8, vf12w\n\t"
        "    vmulax.xyzw ACC, vf1, vf12x\n\t"
        "    vmadday.xyzw ACC, vf2, vf12y\n\t"
        "    vmaddaz.xyzw ACC, vf3, vf12z\n\t"
        "    vdiv Q, vf0w, vf16w\n\t"
        "    vmaddw.xyzw vf24, vf4, vf12w\n\t"
        "    vmuly.xyz vf13, vf13, vf10y\n\t"
        "    addi $3, $3, -1\n\t"
        "    vsuba.xyzw ACC, vf0, vf0\n\t"
        "    c2 0x1f8c1ff\n\t"
        "    vaddax.z ACC, vf0z, vf10x\n\t"
        "    vmaddq.xyzw vf16, vf16, Q\n\t"
        "    vmulq.xyzw vf14, vf14, Q\n\t"
        "    vmtir vi6, vf14w\n\t"
        "    cfc2 $2, $18\n\t"
        "    vftoi4.xyzw vf16, vf16\n\t"
        "    and $2, $2, $3\n\t"
        "    vftoi0.xyzw vf13, vf13\n\t"
        "    bnez $2, L149890\n\t"
        "    vnop \n\t"
        "    cfc2 $2, $6\n\t"
        "    beqz $2, L149898\n\t"
        "    vnop \n\t"
        "L149890:\n\t"
        "    vftoi15.w vf16w, vf0w\n\t"
        "    vnop \n\t"
        "L149898:\n\t"
        "    sqc2 vf14, 48(%0)\n\t"
        "    sqc2 vf13, 64(%0)\n\t"
        "    sqc2 vf16, 80(%0)\n\t"
        ".set reorder"
        : : "r"(spr) : "$2", "$3", "memory"
    );
}

void PrRenderStuff::RenderVertexEECoreNormal() {
    register char *spr asm("$4") = reinterpret_cast<char*>(prRenderEeSpram) + 0x330;
    asm volatile(
        ".set noreorder\n\t"
        "    lqc2 vf12, 0(%0)\n\t"
        "    lqc2 vf13, 16(%0)\n\t"
        "    lqc2 vf14, 32(%0)\n\t"
        "    lui $3, 0x4\n\t"
        "    vmulax.xyzw ACC, vf5, vf12x\n\t"
        "    vmadday.xyzw ACC, vf6, vf12y\n\t"
        "    vmaddaz.xyzw ACC, vf7, vf12z\n\t"
        "    vmaddw.xyzw vf16, vf8, vf12w\n\t"
        "    vmulax.xyzw ACC, vf1, vf12x\n\t"
        "    vmadday.xyzw ACC, vf2, vf12y\n\t"
        "    vmaddaz.xyzw ACC, vf3, vf12z\n\t"
        "    vdiv Q, vf0w, vf16w\n\t"
        "    vmaddw.xyzw vf24, vf4, vf12w\n\t"
        "    vmuly.xyz vf13, vf13, vf10y\n\t"
        "    addi $3, $3, -1\n\t"
        "    vsuba.xyzw ACC, vf0, vf0\n\t"
        "    c2 0x1f8c1ff\n\t"
        "    vaddax.z ACC, vf0z, vf10x\n\t"
        "    vmaddq.xyzw vf16, vf16, Q\n\t"
        "    vmulq.xyzw vf14, vf14, Q\n\t"
        "    vmtir vi6, vf14w\n\t"
        "    cfc2 $2, $18\n\t"
        "    vsub.xyzw vf18, vf16, vf17\n\t"
        "    vftoi4.xyzw vf24, vf16\n\t"
        "    and $2, $2, $3\n\t"
        "    vftoi0.xyzw vf13, vf13\n\t"
        "    vopmula.xyz ACC, vf18, vf19\n\t"
        "    vopmsub.xyz vf0, vf19, vf18\n\t"
        "    bnez $2, L14994c\n\t"
        "    cfc2 $2, $6\n\t"
        "    bnez $2, L14994c\n\t"
        "    vnop \n\t"
        "    vnop \n\t"
        "    cfc2 $2, $17\n\t"
        "    andi $2, $2, 0x20\n\t"
        "    beqz $2, L149950\n\t"
        "    vnop \n\t"
        "L14994c:\n\t"
        "    vftoi15.w vf24w, vf0w\n\t"
        "L149950:\n\t"
        "    vmulz.xyzw vf19, vf18, vf11z\n\t"
        "    vsub.z vf11z, vf0z, vf11z\n\t"
        "    sqc2 vf14, 48(%0)\n\t"
        "    sqc2 vf13, 64(%0)\n\t"
        "    sqc2 vf24, 80(%0)\n\t"
        "    vmove.xyzw vf17, vf16\n\t"
        ".set reorder"
        : : "r"(spr) : "$2", "$3", "memory"
    );
}

void PrRenderStuff::RenderVertexEECoreRefmap() {
    /* Empty */
}

void PrRenderStuff::RenderVertexEECoreContour() {
    /* Empty */
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/prlib/renderee", RenderNodeEECore__13PrRenderStuffP23PrVuNodeHeaderDmaPacket);

INCLUDE_ASM("asm/nonmatchings/prlib/renderee", RenderChunkEECore__13PrRenderStuffP25PrVuDataChunkPacketHeaderf);
#else
#include "mfifo.h"
#include "model.h"
#include "vu0mat.h"

struct PrVuDataChunkPacketHeader {
    sceDmaTag m_tag;
    u_int unk10;
    u_int unk14;
    u_int unk18;
    u_int unk1C;
    u_int unk20[4];
    u_int unk30[8];
    NaVECTOR<float, 4> unk50[0];
};

extern sceDmaTag D_00371910;
extern NaVECTOR<float, 4> D_00371920;
extern float D_01C74270[4];
extern void (*D_003718F0[])();

typedef NaMATRIX<float, 4, 4> PrMat44;

#define LDA(A) asm volatile( \
        "lqc2     $vf4,   0x0(%0)       \n\t" \
        "lqc2     $vf5,  0x10(%0)       \n\t" \
        "lqc2     $vf6,  0x20(%0)       \n\t" \
        "lqc2     $vf7,  0x30(%0)       \n\t" \
    : : "r"(A))

#define ROW(o) \
        "lqc2     $vf8,  " #o "(%1)       \n\t" \
        "vmulax   ACC,    $vf4,   $vf8  \n\t" \
        "vmadday  ACC,    $vf5,   $vf8  \n\t" \
        "vmaddaz  ACC,    $vf6,   $vf8  \n\t" \
        "vmaddw   $vf9,   $vf7,   $vf8  \n\t" \
        "sqc2     $vf9,   " #o "(%0)       \n\t"

#define MUL(R, B) asm volatile(ROW(0x0) ROW(0x10) ROW(0x20) ROW(0x30) : : "r"(R), "r"(B))

struct PrRawMat { float m[4][4]; };

static inline PrMat44 *MatCopy(PrMat44 *lhs, const PrMat44 *rhs) {
    return &PrMat44::Copy(*lhs, *rhs);
}

static inline void MulMV(const PrMat44 *a, const PrMat44 *b, PrMat44 *out) {
    PrRawMat t;
    PrRawMat r;
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    LDA(a);
    MUL((PrMat44*)&r, b);
    {
        int w = 3;
        do { w--; } while (w != -1);
    }
    MatCopy((PrMat44*)&t, (PrMat44*)&r);
    MatCopy(out, (PrMat44*)&t);
}

void PrRenderStuff::RenderNodeEECore(PrVuNodeHeaderDmaPacket *packet) {
    if (PrMfifoUnsentDataSize() != 0 || packet[1].m_tag.id == 0x10 || (u_int)packet->unk60 >= 2) {
        AppendDmaTag(&packet->m_tag);
        return;
    }

    register PrSPRAM_DATA *spr asm("$5") = prRenderEeSpram;
    packet = (PrVuNodeHeaderDmaPacket*)((u_int)packet | 0x30000000);
    {
        PrRawMat t;
        PrRawMat r;
        register PrMat44 *rp asm("$10") = (PrMat44*)&r;
        register PrVuNodeHeaderDmaPacket *uncached asm("$11") = packet;
        register PrMat44 *m1 asm("$6") = &uncached->unk10;
        register PrMICRO_PROGRAM_MODULE prog asm("$3") = uncached->unk60;
        int w = 3;
        asm("" : "+r"(w), "+r"(m1), "+r"(prog));
        register char *spr_work asm("$12") = spr->unk2B0;
        register PrMat44 *m2 asm("$13") = m1;
        register PrMat44 *vp1 asm("$7") = &spr->m_view_projection_matrix;
        register char *spr_work2 asm("$14") = spr_work + 0x40;
        do { w--; } while (w != -1);
        LDA(vp1);
        MUL(rp, m1);
        {
            int w2 = 3;
            do { w2--; } while (w2 != -1);
        }
        MatCopy((PrMat44*)&t, rp);
        MatCopy((PrMat44*)spr_work, (PrMat44*)&t);

        asm("" : "+r"(spr));
        PrMat44 *vp2 = &spr->unk120;
        {
            int w3 = 3;
            do { w3--; } while (w3 != -1);
        }
        LDA(vp2);
        MUL(rp, m2);
        {
            int w4 = 3;
            do { w4--; } while (w4 != -1);
        }
        MatCopy((PrMat44*)&t, rp);
        MatCopy((PrMat44*)spr_work2, (PrMat44*)&t);

        asm("" : "+r"(prog));
        void (*fn)() = D_003718F0[prog];
        packet = (PrVuNodeHeaderDmaPacket*)((u_int)uncached & 0x0fffffff);
        *(void(**)())(spr_work + 0xF0) = fn;

        void *unk50_ptr = &packet->unk50;

        asm volatile(
            "lqc2 $vf5,  0x00(%0)\n\t"
            "lqc2 $vf6,  0x10(%0)\n\t"
            "lqc2 $vf7,  0x20(%0)\n\t"
            "lqc2 $vf8,  0x30(%0)\n\t"
            "lqc2 $vf1,  0x40(%0)\n\t"
            "lqc2 $vf2,  0x50(%0)\n\t"
            "lqc2 $vf3,  0x60(%0)\n\t"
            "lqc2 $vf4,  0x70(%0)\n\t"
            "lqc2 $vf10, 0x00(%1)\n\t"
            : : "r"(spr_work), "r"(unk50_ptr)
        );
    }

    float disturbance = packet->unk68;
    PrVuDataChunkPacketHeader *chunk = (PrVuDataChunkPacketHeader*)(packet + 1);
    do {
        RenderChunkEECore(chunk, disturbance);
        if (chunk->m_tag.id == 0x10) {
            chunk = (PrVuDataChunkPacketHeader*)((u_int)chunk + (chunk->m_tag.qwc + 1) * 16);
        } else {
            chunk = NULL;
        }
    } while (chunk != NULL);
}

void PrRenderStuff::RenderChunkEECore(PrVuDataChunkPacketHeader *chunk, float disturbance) {
    PrSPRAM_DATA *spr = prRenderEeSpram;
    char *spr_work = spr->unk2B0;
    NaVECTOR<float, 4> *src = chunk->unk50;

    sceDmaTag *tag;
    NaVECTOR<float, 4> *dst;
    tag = *(sceDmaTag**)&spr->unk0[0x48];
    dst = (NaVECTOR<float, 4>*)(tag + 1);
    *tag = D_00371910;

    u_int vtx_hdr_num = chunk->unk10 * 3 + 1;
    u_int vtx_num = chunk->unk1C;
    tag->qwc = vtx_hdr_num + vtx_num * 3;

    for (u_int i = 0; i < vtx_hdr_num; i++) {
        dst[i] = src[i];
    }
    chunk = (PrVuDataChunkPacketHeader*)((char*)chunk + 0x10);
    src += vtx_hdr_num;
    dst += vtx_hdr_num;

    asm volatile(
        "ctc2.ni $0, $vi18\n\t"
        "lqc2    $vf11, 0x0(%0)\n\t"
        : : "r"(chunk)
    );

    if (disturbance != 0.0f) {
        u_int param = prRenderEeSpram->m_disturbance_param;
        D_01C74270[2] = disturbance;
        *(u_int*)&D_01C74270[0] = param;
        for (u_int i = 0; i < vtx_num; i++) {
            *(NaVECTOR<float, 4>*)(spr_work + 0x80) = *src++;
            *(NaVECTOR<float, 4>*)(spr_work + 0x90) = *src++;
            *(NaVECTOR<float, 4>*)(spr_work + 0xA0) = *src++;

            asm volatile(
                "lqc2        $vf23, 0x0(%0)\n\t"
                : : "r"(&D_00371920)
            );
            asm volatile(
                "lqc2        $vf24, 0x0(%0)\n\t"
                : : "r"(D_01C74270)
            );
            asm volatile(
                "lqc2        $vf25, 0x0(%0)\n\t"
                "vrinit      R, $vf24x\n\t"
                "vaddax.xyz  ACC, $vf25, $vf0x\n\t"
                "vrxor       R, $vf25x\n\t"
                "vrnext.x    $vf26, R\n\t"
                "vrxor       R, $vf25y\n\t"
                "vrnext.y    $vf26, R\n\t"
                "vrxor       R, $vf25z\n\t"
                "vrnext.z    $vf26, R\n\t"
                "vmsubaz.xyz ACC, $vf23, $vf24z\n\t"
                "vmaddz.xyz  $vf25, $vf26, $vf24z\n\t"
                "sqc2        $vf25, 0x0(%0)\n\t"
                : : "r"((NaVECTOR<float, 4>*)(spr_work + 0x80)) : "memory"
            );

            (*(void(**)())(spr_work + 0xF0))();

            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xB0);
            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xC0);
            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xD0);
        }
    } else {
        for (u_int i = 0; i < vtx_num; i++) {
            *(NaVECTOR<float, 4>*)(spr_work + 0x80) = *src++;
            *(NaVECTOR<float, 4>*)(spr_work + 0x90) = *src++;
            *(NaVECTOR<float, 4>*)(spr_work + 0xA0) = *src++;

            (*(void(**)())(spr_work + 0xF0))();

            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xB0);
            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xC0);
            *dst++ = *(NaVECTOR<float, 4>*)(spr_work + 0xD0);
        }
    }

    PrSendMfifo(tag);

    PrSPRAM_DATA *spr2 = prRenderEeSpram;
    u_int a1 = *(u_int*)&spr2->unk0[0x40];
    u_int v1 = *(u_int*)&spr2->unk0[0x44];
    u_int a0 = *(u_int*)&spr2->unk0[0x48];
    *(u_int*)&spr2->unk0[0x40] = v1;
    *(u_int*)&spr2->unk0[0x44] = a0;
    *(u_int*)&spr2->unk0[0x48] = a1;
}
#endif

/* prlib/renderstuff.h */
asm("
.text
.globl func_00146A08
.ent func_00146A08
func_00146A08:
    .set noreorder
    j Append__10PrDmaQueuePv
    nop
    .set reorder
.end func_00146A08
");
