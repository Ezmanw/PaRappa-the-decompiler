#include "common.h"

#if defined(PRD_SYORI)
#include "dbug/syori.h"
#endif

#include "renderstuff.h"
#include "utility.h"

#include <eeregs.h>

/* sdata */
extern float prMendererRatio;
extern int prMendererDirection;
extern int prMendererColorModulation;
extern u_int mendererTbp;
extern u_int mendererFbp;
extern int mendererInitialized;

void PrInitializeTextureCreation(u_int arg0, u_int arg1, u_int arg2, u_int arg3);
void PrInitializeAlphaModulation();
void PrInitializeAwfulBackground(void *picture);
void PrInitializeNoodlePolygonPosition();

void InitializeNoodleStripRendering(u_int arg0, u_int arg1, u_int arg2, u_int arg3);
void StartNoodleRotation();

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", InitializeNoodleStripRendering__FUiUiUiUi);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", GetRandom__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", StageIndexForColor__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PushNoodleColor__FPUl);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawNoodleStripChunk__FRCt8NaMATRIX3Zfi4i4);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", SetNoodleRotationMatrix__FRt8NaMATRIX3Zfi4i4f);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PreDrawNoodleStrip__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawNoodleStrip__Fff);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", StartNoodleRotation__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", UpdateNoodleRotation__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PrUpdateMendererSpeed__Fv);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PrDecelerateMenderer);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", PrRestartMenderer);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", DrawMenderer__Fv);
void DrawMenderer();

PR_EXTERN
void PrSetMendererRatio(float ratio) {
    prMendererRatio = ratio;
}

PR_EXTERN
float PrGetMendererRatio() {
    return prMendererRatio;
}

PR_EXTERN
void PrSetMendererDirection(int direction) {
    prMendererDirection = direction;
}

PR_EXTERN
int PrGetMendererDirection() {
    return prMendererDirection;
}

PR_EXTERN
void PrSetMendererColorModulation(int color) {
    prMendererColorModulation = color;
}

PR_EXTERN
int PrIsMendererColorModulation() {
    return prMendererColorModulation;
}

PR_EXTERN
void PrInitializeMenderer(u_int tbp, void *noodlePicture, u_int fbp) {
    sceGsZbuf zbuf = prRenderStuff.m_zbuf;

    prMendererColorModulation = 1;
    mendererTbp = tbp;
    mendererFbp = fbp;

    u_int width = PrGetBitSize(256);
    u_int height = PrGetBitSize(16);

    PrInitializeTextureCreation(tbp, zbuf.ZBP, width, height);
    InitializeNoodleStripRendering(tbp, fbp, width, height);
    PrInitializeAlphaModulation();
    PrInitializeAwfulBackground(noodlePicture);
    PrInitializeNoodlePolygonPosition();
    StartNoodleRotation();

    mendererInitialized = 1;
}

PR_EXTERN
void PrRenderMenderer() {
    DrawMenderer();
    prRenderStuff.m_statistics.render_time8 = *T3_COUNT;
#if defined(PRD_SYORI)
    SyoriUpdateStats(&prRenderStuff.mStatistics);
#endif
}

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F3B8);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F410);

/* prlib/menderer.cpp */
PR_EXTERN
void _GLOBAL_$I$prMendererRatio(void) {
}

/* nalib/navector.h */
INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F4C8);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F5D0);

INCLUDE_ASM("asm/nonmatchings/prlib/menderer", func_0014F6D8);

/* prlib/menderer.cpp */
INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967E0);

INCLUDE_RODATA("asm/nonmatchings/prlib/menderer", D_003967F0);
