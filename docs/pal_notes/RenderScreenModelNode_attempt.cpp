/* RenderScreenModelNode PAL draft: 199/199 instrs, frame 176 (matches), 171/199 eq (regalloc of &r in MulMV: a1 vs v1). */
void SpmNode::RenderScreenModelNode() {
    prRenderStuff.m_statistics.node_num++;

    if (!(this->m_flags & 0x4000)) {
        return;
    }

    PrVuNodeHeaderDmaPacket *packet = this->unk16C[0];
    if (packet != NULL) {
        prRenderStuff.m_statistics.opaque_context1_node_num++;
        packet = (PrVuNodeHeaderDmaPacket*)((u_int)packet | 0x30000000);
        PrMat44 *d = &packet->unk10;
        const PrMat44 *b = &this->unk40;
        const PrMat44 *a = &D_01C74230;
        MulMV(a, b, d);
        prRenderStuff.AppendDmaTag((sceDmaTag*)((u_int)packet & 0x0fffffff));
    }

    PrVuNodeHeaderDmaPacket *packet2 = this->unk16C[1];
    if (packet2 != NULL) {
        prRenderStuff.m_statistics.transmit_context1_node_num++;
        packet2 = (PrVuNodeHeaderDmaPacket*)((u_int)packet2 | 0x30000000);
        {
            PrMat44 *d = &packet2->unk10;
            const PrMat44 *b = &this->unk40;
            const PrMat44 *a = &D_01C74230;
            MulMV(a, b, d);
        }

        NaVECTOR<float, 4> v;
        {
            NaVECTOR<float, 4> t;
            asm volatile(
                "lqc2     $vf4,   0x0(%1)       \n\t"
                "lqc2     $vf5,  0x10(%1)       \n\t"
                "lqc2     $vf6,  0x20(%1)       \n\t"
                "lqc2     $vf7,  0x30(%1)       \n\t"
                "lqc2     $vf8,   0x0(%2)       \n\t"
                "vmulax.xyzw   ACC,    $vf4,   $vf8  \n\t"
                "vmadday.xyzw  ACC,    $vf5,   $vf8  \n\t"
                "vmaddaz.xyzw  ACC,    $vf6,   $vf8  \n\t"
                "vmaddw.xyzw   $vf9,   $vf7,   $vf8  \n\t"
                "sqc2     $vf9,   0x0(%0)       \n\t"
            : : "r"(&t), "r"(&this->unk40), "r"(&this->unk140));
            asm volatile("lq $6, 0(%1)\n\tsq $6, 0(%0)" : : "r"(&v), "r"(&t) : "$6");
        }

        u_int tm = this->unk188;
        prRenderStuff.AppendTransmitDmaTag((sceDmaTag*)((u_int)packet2 & 0x0fffffff), tm, -v[2]);
    }
}
