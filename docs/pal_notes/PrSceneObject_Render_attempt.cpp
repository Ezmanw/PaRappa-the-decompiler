/* PrSceneObject::Render PAL draft: 253/253 instrs, opcode-identical, 13 reg diffs (s3/s4 swapped: &xyoffset temp vs &prRenderStuff).
 * rs2 variant (AwfulStatus store before the loop) gives correct regs but sw lands in the beqz delay slot (7 diffs).
 * Needs at top of render.cpp: #include "mfifo.h" and PR_EXTERN float PrGetMendererRatio(); */
#include "mfifo.h"
PR_EXTERN float PrGetMendererRatio();
#define PR_T1_COUNT (*(volatile u_int*)0x10001800)

void PrDrawAwfulBackground(sceGsFrame frame);
void PrWaitMendererTexture(sceGsDrawEnv1 *env, const sceGsFrame& frame, const sceGsXyoffset& xyoffset);

void PrSceneObject::Render() {
    FlushCache(0);

    if (PrGetMendererRatio() >= 1.5f && m_model_set.m_head != NULL) {
        prRenderStuff.InitializeEECore(this);
        PrDrawAwfulBackground(unk50);
    }

    int awful = 1;
    prRenderStuff.ResetStatistics();
    prRenderStuff.m_statistics.render_time0 = PR_T1_COUNT;
    prSpramData->Initialize(this);

    PrModelObject *model = m_model_set.m_head;
    if (!(PrGetMendererRatio() >= 1.5f)) {
        awful = 0;
    }
    if (model != NULL) {
        AwfulStatus = awful;
        do {
            if ((model->m_flags & 1) && (!awful || (model->m_spm_image->m_flags & 0x100))) {
                prSpramData->InitializeModel(model);
                model->CalculateCurrentMatrix();
            }
            model = model->m_list.next;
        } while (model != NULL);
    } else {
        AwfulStatus = awful;
    }

    prRenderStuff.m_statistics.render_time1 = PR_T1_COUNT;
    sceGsFrame frame = unk50;
    sceGsXyoffset xyoffset = unk58;
    PrWaitMendererTexture(unk70, frame, xyoffset);
    prRenderStuff.m_statistics.render_time2 = PR_T1_COUNT;

    prRenderStuff.InitializeEECore(this);
    InitializeVu1();
    prSpramData->SendDisplayHeader();
    prRenderStuff.StartRender(this);
    prRenderStuff.m_transmit_array_size = 0;
    PrStartMfifo();

    model = m_model_set.m_head;
    if (model != NULL && (model->m_spm_image->m_flags & 0x200)) {
        prRenderStuff.AppendDmaTag(&PrGetDmaStripGifRegister((PrSetGifRegisterMode)4)->m_tag);
        do {
            if ((model->m_flags & 1) && (!awful || (model->m_spm_image->m_flags & 0x100))) {
                prSpramData->InitializeModel(model);
                model->RenderBackgroundScreenModel();
            }
            model = model->m_list.next;
        } while (model != unk98);
    }

    if (model != NULL && (model->m_spm_image->m_flags & 0x400)) {
        prRenderStuff.AppendDmaTag(&PrGetDmaStripGifRegister((PrSetGifRegisterMode)5)->m_tag);
        do {
            if ((model->m_flags & 1) && (!awful || (model->m_spm_image->m_flags & 0x100))) {
                prSpramData->InitializeModel(model);
                model->RenderContext1Model();
            }
            model = model->m_list.next;
        } while (model != unk9C);
    }

    prRenderStuff.AppendDmaTag(&PrGetDmaStripGifRegister((PrSetGifRegisterMode)0)->m_tag);
    for (; model != m_screen_model_list; model = model->m_list.next) {
        if ((model->m_flags & 1) && (!awful || (model->m_spm_image->m_flags & 0x100))) {
            prSpramData->InitializeModel(model);
            model->RenderContext1Model();
        }
    }
    for (model = unk9C; model != m_screen_model_list; model = model->m_list.next) {
        if ((model->m_flags & 1) && (!awful || (model->m_spm_image->m_flags & 0x100))) {
            prSpramData->InitializeModel(model);
            model->RenderContext2Model();
        }
    }

    prRenderStuff.m_statistics.render_time3 = PR_T1_COUNT;
    prRenderStuff.SortTransmitDmaArray();
    prRenderStuff.m_statistics.render_time4 = PR_T1_COUNT;

    if (prCurrentStage != 19) {
        prRenderStuff.AppendDmaTag(&PrGetDmaStripGifRegister((PrSetGifRegisterMode)1)->m_tag);
    }

    FlushCache(0);
    PrWaitMfifo();
    prRenderStuff.MergeRender();
    prRenderStuff.m_statistics.render_time5 = PR_T1_COUNT;
}
