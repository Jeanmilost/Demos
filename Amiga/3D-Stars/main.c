/****************************************************************************
 * ==> 3D Stars ------------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - A 3D starfield                              *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

// amiga lib
#include "support/gcc8_c_support.h" //REM?
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <exec/execbase.h>
#include <graphics/gfxmacros.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

// classes
#include "AMG_Core.h"
#include "AMG_Star.h"

/*
* This provides the Amiga-standard fixed-width types:
*
* BYTE  (8-bit signed)
* UBYTE (8-bit unsigned)
* WORD  (16-bit signed)
* UWORD (16-bit unsigned)
* LONG  (32-bit signed)
* ULONG (32-bit unsigned)
*/

#define M_BitPlanes  3 // 3 bitplanes = 8 colors
#define M_Star_Count 64
#define M_Star_Deep  100

//---------------------------------------------------------------------------
// Amiga specific structures
//---------------------------------------------------------------------------
// proto header structures, names cannot be changed nor prefixed (i.e. no g_p... here)
struct          ExecBase*   SysBase;
struct          DosLibrary* DOSBase;
struct          GfxBase*    GfxBase;

volatile struct Custom*     g_pCustom;
//---------------------------------------------------------------------------
// Backup
//---------------------------------------------------------------------------
static          UWORD       g_SystemInts;
static          UWORD       g_SystemDMA;
static          UWORD       g_SystemADKCON;
static volatile APTR        g_pVBR = 0;
static          APTR        g_pSystemIrq;
struct          View*       g_pActiView;
//---------------------------------------------------------------------------
// Double buffering
//---------------------------------------------------------------------------
static          UBYTE*      g_pBuffer1[M_BitPlanes];
static          UBYTE*      g_pBuffer2[M_BitPlanes];
static          UBYTE*      g_pDrawBuffer[M_BitPlanes];
static          UBYTE*      g_pDisplayBuffer[M_BitPlanes];
static volatile short       g_SwapRequested  = 0;
static          USHORT*     g_pBplPtrLoc     = NULL; // location in copper list where bitplane pointers are
//---------------------------------------------------------------------------
// Randomization
//---------------------------------------------------------------------------
static          ULONG       g_RandSeed       = 1;
//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------
void srand(ULONG seed)
{
    g_RandSeed = seed;
}
//---------------------------------------------------------------------------
ULONG rand(void)
{
    g_RandSeed = g_RandSeed * 1103515245UL + 12345UL;
    return (g_RandSeed >> 16) & 0x7FFF;
}
//---------------------------------------------------------------------------
static APTR GetVBR()
{
    APTR  pVbr      = 0;
    UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 }; // MOVEC.L VBR,D0 RTE

    if (SysBase->AttnFlags & AFF_68010) 
        pVbr = (APTR)Supervisor((ULONG(*)())getvbr);

    return pVbr;
}
//---------------------------------------------------------------------------
void SetInterruptHandler(APTR pInterrupt)
{
    *(volatile APTR*)(((UBYTE*)g_pVBR) + 0x6c) = pInterrupt;
}
//---------------------------------------------------------------------------
APTR GetInterruptHandler()
{
    return *(volatile APTR*)(((UBYTE*)g_pVBR) + 0x6c);
}
//---------------------------------------------------------------------------
void WaitVbl()
{
    while (1)
    {
        volatile ULONG vPos = *(volatile ULONG*)0xDFF004;
        vPos               &= 0x1ff00;

        if (vPos != (311 << 8))
            break;
    }

    while (1)
    {
        volatile ULONG vPos = *(volatile ULONG*)0xDFF004;
        vPos               &= 0x1ff00;

        if (vPos == (311 << 8))
            break;
    }
}
//---------------------------------------------------------------------------
__attribute__((always_inline)) inline void WaitBlt()
{
    UWORD tst = *(volatile UWORD*)&g_pCustom->dmaconr;
    (void)tst;

    while (*(volatile UWORD*)&g_pCustom->dmaconr&(1 << 14))
    {}
}
//---------------------------------------------------------------------------
void TakeSystem()
{
    Forbid();

    g_SystemADKCON = g_pCustom->adkconr;
    g_SystemInts   = g_pCustom->intenar;
    g_SystemDMA    = g_pCustom->dmaconr;
    g_pActiView    = GfxBase->ActiView;

    LoadView(0);
    WaitTOF();
    WaitTOF();

    WaitVbl();
    WaitVbl();

    OwnBlitter();
    WaitBlit();
    Disable();
    
    g_pCustom->intena = 0x7fff;
    g_pCustom->intreq = 0x7fff;
    g_pCustom->dmacon = 0x7fff;

    for (int a = 0; a < 32; ++a)
        g_pCustom->color[a]=0;

    WaitVbl();
    WaitVbl();

    g_pVBR       = GetVBR();
    g_pSystemIrq = GetInterruptHandler();
}
//---------------------------------------------------------------------------
void FreeSystem()
{
    WaitVbl();
    WaitBlit();

    g_pCustom->intena = 0x7fff;
    g_pCustom->intreq = 0x7fff;
    g_pCustom->dmacon = 0x7fff;

    SetInterruptHandler(g_pSystemIrq);

    g_pCustom->cop1lc  = (ULONG)GfxBase->copinit;
    g_pCustom->cop2lc  = (ULONG)GfxBase->LOFlist;
    g_pCustom->copjmp1 = 0x7fff;

    g_pCustom->intena = g_SystemInts   | 0x8000;
    g_pCustom->dmacon = g_SystemDMA    | 0x8000;
    g_pCustom->adkcon = g_SystemADKCON | 0x8000;

    WaitBlit();
    DisownBlitter();
    Enable();

    LoadView(g_pActiView);
    WaitTOF();
    WaitTOF();

    Permit();
}
//---------------------------------------------------------------------------
__attribute__((always_inline)) inline short MouseLeft()
{
    return !((*(volatile UBYTE*)0xbfe001) & 64);
}
//---------------------------------------------------------------------------
static __attribute__((interrupt)) void interruptHandler(void)
{
    g_pCustom->intreq = (1 << INTB_VERTB);
    g_pCustom->intreq = (1 << INTB_VERTB); // A4000 bug

    if (g_SwapRequested && g_pBplPtrLoc)
    {
        USHORT* cop = g_pBplPtrLoc;

        for (int p = 0; p < M_BitPlanes; ++p)
        {
            ULONG addr = (ULONG)g_pDisplayBuffer[p];

            cop[1] = (UWORD)(addr >> 16); // update BPLxPTH value
            cop[3] = (UWORD)(addr);       // update BPLxPTL value

            cop += 4;                     // move to next plane
        }

        g_SwapRequested = 0;
    }
}
//---------------------------------------------------------------------------
__attribute__((always_inline)) inline USHORT* copSetPlanes(UBYTE blPtrStart, USHORT* pCopListEnd, const UBYTE** pPlanes, int numPlanes)
{
    for (USHORT i = 0; i < numPlanes; ++i)
    {
        ULONG addr     = (ULONG)pPlanes[i];
        *pCopListEnd++ = offsetof(struct Custom, bplpt[0]) + (i + blPtrStart) * sizeof(APTR);
        *pCopListEnd++ = (UWORD)(addr>>16);
        *pCopListEnd++ = offsetof(struct Custom, bplpt[0]) + (i + blPtrStart) * sizeof(APTR) + 2;
        *pCopListEnd++ = (UWORD)addr;
    }

    return pCopListEnd;
}
//---------------------------------------------------------------------------
__attribute__((always_inline)) inline USHORT* screenScanDefault(USHORT* pCopListEnd)
{
    const USHORT x      = 129;
    const USHORT width  = 320;
    const USHORT height = 256;
    const USHORT y      = 44;
    const USHORT RES    = 8;
    USHORT       xstop  = x + width;
    USHORT       ystop  = y + height;
    USHORT       fw     = (x >> 1) - RES;

    *pCopListEnd++ = offsetof(struct Custom, ddfstrt);
    *pCopListEnd++ = fw;
    *pCopListEnd++ = offsetof(struct Custom, ddfstop);
    *pCopListEnd++ = fw + (((width >> 4) - 1) << 3);
    *pCopListEnd++ = offsetof(struct Custom, diwstrt);
    *pCopListEnd++ = x + (y << 8);
    *pCopListEnd++ = offsetof(struct Custom, diwstop);
    *pCopListEnd++ = (xstop - 256) + ((ystop - 256) << 8);

    return pCopListEnd;
}
//---------------------------------------------------------------------------
int main()
{
    SysBase   = *((struct ExecBase**)4UL);
    g_pCustom =   (struct Custom*)0xdff000;
    GfxBase   =   (struct GfxBase*)OpenLibrary((CONST_STRPTR)"graphics.library", 0);

    if (!GfxBase)
        Exit(0);

    DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);

    if (!DOSBase)
        Exit(0);

    TakeSystem();
    WaitVbl();

    const USHORT lineSize = 320 / 8;

    // allocate bitplanes for double buffering (320x256 = 10240 bytes each)
    for (int i = 0; i < M_BitPlanes; ++i)
    {
        g_pBuffer1[i] = (UBYTE*)AllocMem(lineSize * 256, MEMF_CHIP | MEMF_CLEAR);
        g_pBuffer2[i] = (UBYTE*)AllocMem(lineSize * 256, MEMF_CHIP | MEMF_CLEAR);
    }

    if (!g_pBuffer1 || !g_pBuffer2)
    {
        FreeSystem();

        if (g_pBuffer1)
            FreeMem(g_pBuffer1, lineSize * 256);

        if (g_pBuffer2)
            FreeMem(g_pBuffer2, lineSize * 256);

        CloseLibrary((struct Library*)DOSBase);
        CloseLibrary((struct Library*)GfxBase);

        Exit(0);
    }

    // initialize double buffering
    for (int i = 0; i < M_BitPlanes; ++i)
    {
        g_pDisplayBuffer[i] = g_pBuffer1[i];
        g_pDrawBuffer[i]    = g_pBuffer2[i];
    }

    USHORT* pCopper1 = (USHORT*)AllocMem(1024, MEMF_CHIP);
    USHORT* pCopPtr  = pCopper1;

    pCopPtr = screenScanDefault(pCopPtr);

    // enable the bitplanes
    *pCopPtr++ = offsetof(struct Custom, bplcon0);
    *pCopPtr++ = (0 << 10) | (1 << 9) | (M_BitPlanes << 12);
    *pCopPtr++ = offsetof(struct Custom, bplcon1);
    *pCopPtr++ = 0;
    *pCopPtr++ = offsetof(struct Custom, bplcon2);
    *pCopPtr++ = 0;
    *pCopPtr++ = offsetof(struct Custom, bplcon3);
    *pCopPtr++ = 0;
    *pCopPtr++ = offsetof(struct Custom, bplcon4);
    *pCopPtr++ = 0;

    // set bitplane modulo
    *pCopPtr++ = offsetof(struct Custom, bpl1mod);
    *pCopPtr++ = 0;
    *pCopPtr++ = offsetof(struct Custom, bpl2mod);
    *pCopPtr++ = 0;

    // set bitplane pointer, save location for double buffering
    g_pBplPtrLoc = pCopPtr;

    const UBYTE* pPlanes[M_BitPlanes];

    for (int i = 0; i < M_BitPlanes; ++i)
        pPlanes[i] = g_pDisplayBuffer[i];

    pCopPtr = copSetPlanes(0, pCopPtr, pPlanes, M_BitPlanes);

    // generate the color palette
    for (int i = 0; i < 8; ++i)
    {
        // between 0 and 15
        UBYTE  level = (i * 15) / 7;
        USHORT gray  = (level << 8) | (level << 4) | level;

        *pCopPtr++ = offsetof(struct Custom, color[i]);
        *pCopPtr++ = gray;
    }

    // end copper list
    *pCopPtr++ = 0xFFFF;
    *pCopPtr++ = 0xFFFE;

    g_pCustom->cop1lc  = (ULONG)pCopper1;
    g_pCustom->dmacon  = DMAF_BLITTER;
    g_pCustom->copjmp1 = 0x7fff;
    g_pCustom->dmacon  = DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_COPPER | DMAF_BLITTER;

    // set up VBL interrupt for buffer swapping
    SetInterruptHandler((APTR)interruptHandler);
    g_pCustom->intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB;
    g_pCustom->intreq = (1 << INTB_VERTB);

    // initialize random seed using amiga timer
    srand(0xFE4dF5FF);

    AMG_Star3D stars3D[M_Star_Count];
    AMG_Star2D star2D;

    for (UWORD i = 0; i < M_Star_Count; ++i)
    {
        stars3D[i].m_X = ((rand() % 3200) - 1600) * 2;
        stars3D[i].m_Y = ((rand() % 2560) - 1280) * 2;
        stars3D[i].m_Z =  (rand() % M_Star_Deep) + 1;
    }

    // animation loop
    while (!MouseLeft())
    {
        // clear the draw buffer
        amgClearDrawBuffer(lineSize, M_BitPlanes, g_pDrawBuffer);

        for (UWORD i = 0; i < M_Star_Count; ++i)
        {
            // transform the 3D star to 2D
            amgTransformStar(&stars3D[i], &star2D);

            // calculate the star color
            UBYTE color = amgStarColorFromZ(stars3D[i].m_Z, M_Star_Deep);

            // draw the star
            amgDrawPixel(star2D.m_X + 160, star2D.m_Y + 128, lineSize, color, M_BitPlanes, g_pDrawBuffer);

            // move star to next position
            --stars3D[i].m_Z;

            // limit the position, reset the z position if min position is reached
            if (stars3D[i].m_Z <= 1)
                stars3D[i].m_Z = M_Star_Deep;
        }

        // swap buffers (waits for VBL internally)
        amgSwapBuffers(M_BitPlanes, g_pDrawBuffer, g_pDisplayBuffer, &g_SwapRequested);
    }

    // clean up
    g_pCustom->intena = 0x7fff;

    // free the bitplanes
    for (int i = 0; i < M_BitPlanes; ++i)
    {
        FreeMem(g_pBuffer1[i], lineSize * 256);
        FreeMem(g_pBuffer2[i], lineSize * 256);
    }

    // free the copper
    FreeMem(pCopper1, 1024);

    FreeSystem();

    CloseLibrary((struct Library*)DOSBase);
    CloseLibrary((struct Library*)GfxBase);
}
//---------------------------------------------------------------------------
