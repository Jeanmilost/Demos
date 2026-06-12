/****************************************************************************
 * ==> Copper Bar ----------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - A gradient copper bar that moves            *
 *              sinusoidally from the screen centre                         *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
 * permit persons whom the Software is furnished to do so, subject to       *
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

/*
* HARDWARE BACKGROUND
* ===================
* The Amiga 500 has a custom chip called "Agnus" that contains the Copper,
* a simple co-processor that runs in sync with the video beam. It reads a
* list of instructions from chip RAM and executes them as the beam sweeps
* the screen. There are only three instructions:
*
*   MOVE  - write a value to any custom chip register
*   WAIT  - stall until the beam reaches a given (V, H) position
*   SKIP  - skip the next instruction if the beam is past a position
*
* A "copper bar" effect works by inserting WAIT + MOVE pairs that change
* color register 0 (the background color) at specific scanlines. Because
* the copper executes in hardware during the actual beam scan, the color
* change takes effect on exactly the right line with zero CPU involvement
*
* SINE ANIMATION WITHOUT FPU
* ==========================
* The 68000 has no floating-point unit. A full sine period is precomputed
* as a table of integers using fixed-point arithmetic at startup (when
* still have access to the C runtime). During the demo the IRQ only does
* integer table lookups — no multiply, no divide, no floating point
*
* Fixed-point format used: 16.16 (upper 16 bits = integer, lower 16 = fraction)
* but because amplitude fits in a short, simplify to plain WORD values scaled
* by M_SineScale
*/

// amiga sdk
#include "support/gcc8_c_support.h"
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
#include "AMG_CopperBar.h"

#define M_DisplayTop  44                        // first visible scanline in absolute beam coordinates
#define M_DisplayH    256                       // height of the visible window in scanlines
#define M_DisplayCY   (M_DisplayH / 2)          // Y center of the visible window (relative to display top)
#define M_BarHeight   16                        // thickness of the bar in scanlines. Must be even so the gradient ramp (up then down) divides cleanly in half
#define M_SineSteps   255                       // number of entries in the table (= period of the animation in frames)
#define M_CopBarWords (M_BarHeight * 4 + 4 + 2) // total number of USHORT words the bar block occupies in the copper list:
                                                //   M_BarHeight lines × 4 words each (WAIT + MOVE)
                                                //   + 4 words for the "restore black" WAIT+MOVE after bar
                                                //   + 2 words for the end-of-list marker (0xFFFF/0xFFFE)

//---------------------------------------------------------------------------
// Amiga OS library base pointers. NOTE These three names are mandatory and
// must not be renamed, the Amiga linker and exec stubs resolve them by name
//---------------------------------------------------------------------------
         struct ExecBase*   SysBase;
         struct DosLibrary* DOSBase;
         struct GfxBase*    GfxBase;
volatile struct Custom*     g_pCustom; // direct pointer to the custom chip register block at 0xDFF000. Declared volatile because
                                       // its members are hardware registers that can change independently of the CPU

//---------------------------------------------------------------------------
// System state backup. NOTE Before we take over the hardware we save the OS
// state so FreeSystem() can restore everything cleanly on exit
//---------------------------------------------------------------------------
static          UWORD       g_SystemInts;   // saved INTENA register
static          UWORD       g_SystemDMA;    // saved DMACON register
static          UWORD       g_SystemADKCON; // saved ADKCON register
static volatile APTR        g_pVBR = 0;     // Vector Base Register (020+; 0 on 68000)
static          APTR        g_pSystemIrq;   // saved level-3 autovector handler
struct          View*       g_pActiView;    // saved OS display view

//---------------------------------------------------------------------------
// Copper bar
//---------------------------------------------------------------------------
static USHORT g_BarColors[M_BarHeight]; // precomputed gradient, g_BarColors[i] is the Amiga color register value
                                        // (format 0x0RGB, 4 bits per channel) for scanline i of the bar.
                                        // Filled once by BuildGradient() before the demo starts

//---------------------------------------------------------------------------
// Sine animation
//---------------------------------------------------------------------------
static volatile UWORD g_SineIndex = 0; // current index into g_SineTable, advanced by 1 every VBL interrupt.
                                       // Declared volatile because it is written by the IRQ and read by main().
                                       // Wraps around modulo M_SineSteps (= 255)

// full precalculated sine period as signed scanline offsets
static const short g_SineTable[M_SineSteps] =
{
     0,   2,   4,   6,   7,   9,   11,  13,  15,  16,  18,  20,  22,  24,  25,  27,
     29,  31,  32,  34,  35,  37,  39,  40,  42,  43,  45,  46,  48,  49,  51,  52,
     53,  54,  56,  57,  58,  59,  60,  61,  63,  64,  64,  65,  66,  67,  68,  69,
     69,  70,  71,  71,  72,  72,  73,  73,  74,  74,  74,  74,  75,  75,  75,  75,
     75,  75,  75,  75,  75,  74,  74,  74,  73,  73,  73,  72,  72,  71,  70,  70,
     69,  68,  68,  67,  66,  65,  64,  63,  62,  61,  60,  59,  58,  56,  55,  54,
     53,  51,  50,  48,  47,  46,  44,  43,  41,  39,  38,  36,  35,  33,  31,  30,
     28,  26,  24,  23,  21,  19,  17,  16,  14,  12,  10,  8,   6,   5,   3,   1,
    -1,  -3,  -5,  -6,  -8,  -10, -12, -14, -16, -17, -19, -21, -23, -24, -26, -28,
    -30, -31, -33, -35, -36, -38, -39, -41, -43, -44, -46, -47, -48, -50, -51, -53,
    -54, -55, -56, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -68, -69,
    -70, -70, -71, -72, -72, -73, -73, -73, -74, -74, -74, -75, -75, -75, -75, -75,
    -75, -75, -75, -75, -74, -74, -74, -74, -73, -73, -72, -72, -71, -71, -70, -69,
    -69, -68, -67, -66, -65, -64, -64, -63, -61, -60, -59, -58, -57, -56, -54, -53,
    -52, -51, -49, -48, -46, -45, -43, -42, -40, -39, -37, -35, -34, -32, -31, -29,
    -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9,  -7,  -6,  -4,  -2
};

//---------------------------------------------------------------------------
// Functions
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

    amgWaitVbl();
    amgWaitVbl();

    OwnBlitter();
    WaitBlit();
    Disable();

    // writing 0x7FFF to INTENA/INTREQ/DMACON with the SET bit (bit 15) clear, clears all 15 enable/request bits simultaneously
    g_pCustom->intena = 0x7fff;
    g_pCustom->intreq = 0x7fff;
    g_pCustom->dmacon = 0x7fff;

    // zero all 32 color registers so no leftover palette bleeds through
    for (int a = 0; a < 32; ++a)
        g_pCustom->color[a] = 0;

    amgWaitVbl();
    amgWaitVbl();

    g_pVBR       = amgGetVBR();
    g_pSystemIrq = amgGetInterruptHandler(g_pVBR);
}
//---------------------------------------------------------------------------
void FreeSystem()
{
    amgWaitVbl();
    WaitBlit();

    g_pCustom->intena = 0x7fff;
    g_pCustom->intreq = 0x7fff;
    g_pCustom->dmacon = 0x7fff;

    // restore the original level-3 interrupt handler
    amgSetInterruptHandler(g_pVBR, g_pSystemIrq);

    // restore the OS copper lists so graphics.library regains the display
    g_pCustom->cop1lc  = (ULONG)GfxBase->copinit;
    g_pCustom->cop2lc  = (ULONG)GfxBase->LOFlist;
    g_pCustom->copjmp1 = 0x7fff;  // strobe: restart copper from cop1lc

    // re-enable saved interrupts, DMA and audio/disk control. Setting bit 15 (SETCLR) alongside the saved bits re-enables them
    g_pCustom->intena = g_SystemInts   | 0x8000;
    g_pCustom->dmacon = g_SystemDMA    | 0x8000;
    g_pCustom->adkcon = g_SystemADKCON | 0x8000;

    WaitBlit();
    DisownBlitter();
    Enable();

    // restore the OS display view and wait for it to settle
    LoadView(g_pActiView);
    WaitTOF();
    WaitTOF();

    Permit();
}
//---------------------------------------------------------------------------
static __attribute__((interrupt)) void interruptHandler(void)
{
    // level-3 (VBL) interrupt service routine, called once per frame at 50 Hz
    //
    // Responsibilities:
    //   1. Acknowledge the interrupt so the hardware deasserts the IRQ line
    //   2. Advance the sine index
    //   3. Compute the new bar top scanline from the sine table
    //   4. Update only the WAIT words in the copper list (colors never change)
    //
    // Why update only WAIT words?
    //   The copper is executing the list continuously. At VBL it resets to the
    //   top of cop1lc. The CPU can safely write WAIT words[0] of each entry
    //   because by the time the copper re-reads them (next frame) we will have
    //   finished. The MOVE words (address + value) are fixed and never touched
    //
    // Performance note:
    //   This routine runs every frame. It does only integer additions, one table
    //   lookup, and M_BarHeight+1 word writes. No branches inside the hot loop

    // acknowledge the vertical blank interrupt
    g_pCustom->intreq = (1 << INTB_VERTB);
    g_pCustom->intreq = (1 << INTB_VERTB); // A4000 bug

    if (!amgCopperBarGetBar())
        return;

    // advance the sine phase
    g_SineIndex += 2;

    if (g_SineIndex >= M_SineSteps)
        g_SineIndex -= M_SineSteps;

    // compute the new bar top scanline in absolute beam coordinates
    short  sineOffset = g_SineTable[g_SineIndex];
    short  barTopRel  = (short)(M_DisplayCY - M_BarHeight / 2) + sineOffset;
    USHORT startLine  = (USHORT)(M_DisplayTop + barTopRel);

    // update the WAIT words in the copper list
    USHORT* p = amgCopperBarGetBar();

    for (int i = 0; i < M_BarHeight; ++i)
    {
        p[0]  = (USHORT)(((startLine + i) << 8) | 0x01);
        p    += 4;
    }

    // update the "restore black" WAIT that sits immediately after the bar
    p[0] = (USHORT)(((startLine + M_BarHeight) << 8) | 0x01);
}
//---------------------------------------------------------------------------
int main()
{
    // library initialisation
    // - SysBase lives at address 4 (the Exec "rombase" pointer)
    // - graphics.library is needed for LoadView/WaitTOF and to save/restore
    // - GfxBase->ActiView and GfxBase->copinit
    // - dos.library is opened for completeness; needed if you add Printf etc
    SysBase   = *((struct ExecBase**)4UL);
    g_pCustom =   (struct Custom*)0xdff000;
    GfxBase   =   (struct GfxBase*)OpenLibrary((CONST_STRPTR)"graphics.library", 0);

    if (!GfxBase)
        Exit(0);

    DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);

    if (!DOSBase)
        Exit(0);

    // take the hardware, must happen before any register writes. Also disables OS interrupts
    TakeSystem();
    amgWaitVbl();

    // build gradient palette
    amgCopperBarBuildRedGradient(M_BarHeight, g_BarColors);

    // allocate and build the copper list in chip RAM. The copper DMA can only read from chip RAM (the lower 512 KB on A500),
    // MEMF_CHIP ensures AllocMem gives us chip RAM
    const int copWords = 2 + M_CopBarWords;
    const int copSize  = copWords * (int)sizeof(USHORT);

    USHORT* pCopper1 = (USHORT*)AllocMem(copSize, MEMF_CHIP);

    if (!pCopper1)
    {
        FreeSystem();
        CloseLibrary((struct Library*)DOSBase);
        CloseLibrary((struct Library*)GfxBase);
        Exit(0);
    }

    USHORT* pCopPtr = pCopper1;

    // first copper instruction, unconditionally set color[0] to black. This runs at the very top of every frame (beam position 0,0)
    // before any WAIT, guaranteeing the background is black above the bar
    *pCopPtr++ = offsetof(struct Custom, color[0]);
    *pCopPtr++ = 0x0000;

    // save the pointer to the start of the bar block so the IRQ handler, can update the WAIT scanlines each frame without searching
    amgCopperBarSetBar(pCopPtr);

    // write the initial bar at the screen center
    // startLine = M_DisplayTop + M_DisplayCY - M_BarHeight/2
    //           = 44 + 128 - 8 = 164  (bar centred at line 168 absolute)
    amgCopperBarBuild((USHORT)(M_DisplayTop + M_DisplayCY - M_BarHeight / 2), M_BarHeight, g_BarColors);

    // start the copper DMA
    // cop1lc:  set the primary copper list pointer to our list
    // copjmp1: strobing this register forces the copper to restart from
    //          cop1lc immediately (equivalent to a copper reset)
    // dmacon:  enable copper DMA (DMAF_COPPER) and the DMA master (DMAF_MASTER)
    // NOTE do NOT enable DMAF_RASTER or DMAF_BLITTER here because there is no bitplanes or blitter operations in this demo
    g_pCustom->cop1lc  = (ULONG)pCopper1;
    g_pCustom->copjmp1 = 0x7fff;
    g_pCustom->dmacon  = DMAF_SETCLR | DMAF_MASTER | DMAF_COPPER;

    // install VBL interrupt handler
    // - SetInterruptHandler patches the level-3 autovector directly
    // - INTF_SETCLR | INTF_INTEN enables the master interrupt gate
    // - INTF_VERTB enables the vertical blank interrupt specifically
    // - Writing INTB_VERTB to INTREQ pre-clears any stale request
    amgSetInterruptHandler(g_pVBR, (APTR)interruptHandler);
    g_pCustom->intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB;
    g_pCustom->intreq = (1 << INTB_VERTB);

    // main loop. Nothing to do on the CPU — the copper and the IRQ handle everything.
    // Simply poll the left mouse button and exit when it is pressed
    while (!amgMouseLeft())
    {}

    // teardown
    g_pCustom->intena = 0x7fff;

    FreeMem(pCopper1, copSize);

    FreeSystem();

    CloseLibrary((struct Library*)DOSBase);
    CloseLibrary((struct Library*)GfxBase);
}
//---------------------------------------------------------------------------
