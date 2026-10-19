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

INCLUDE_ASM("asm/nonmatchings/prlib/renderee", RenderNodeEECore__13PrRenderStuffP23PrVuNodeHeaderDmaPacket);

INCLUDE_ASM("asm/nonmatchings/prlib/renderee", RenderChunkEECore__13PrRenderStuffP25PrVuDataChunkPacketHeaderf);

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
