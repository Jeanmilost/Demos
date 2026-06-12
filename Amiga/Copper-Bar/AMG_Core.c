/****************************************************************************
 * ==> AMG_Core ------------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - Core and system functions                   *
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

#include "AMG_Core.h"

// amiga sdk
#include "support/gcc8_c_support.h"
#include <proto/exec.h>

// Direct pointer to the custom chip register block at 0xDFF000.
// Declared volatile because its members are hardware registers that can
// change independently of the CPU.
extern volatile struct Custom* g_pCustom;

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------
APTR amgGetInterruptHandler(volatile APTR pVBR)
{
    // the level-3 autovector (address VBR + 0x6C) is the vertical blank IRQ. We patch it directly to install our own handler,
    // bypassing the OS interrupt server chain for maximum performance
    return *(volatile APTR*)(((UBYTE*)pVBR) + 0x6c);
}
//---------------------------------------------------------------------------
void amgSetInterruptHandler(volatile APTR pVBR, APTR pInterrupt)
{
    // the level-3 autovector (address VBR + 0x6C) is the vertical blank IRQ. We patch it directly to install our own handler,
    // bypassing the OS interrupt server chain for maximum performance
    *(volatile APTR*)(((UBYTE*)pVBR) + 0x6c) = pInterrupt;
}
//---------------------------------------------------------------------------
APTR amgGetVBR()
{
    APTR pVbr = 0;

    // inline 68010 opcodes: MOVEC.L VBR,D0 (4E7A 0801) followed by RTE (4E73).
    UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 };

    // Supervisor() switches to supervisor mode, executes the routine, returns D0. NOTE on a plain 68000 the VBR is always 0
    // (interrupt vectors live at address 0). On 68010 and later the VBR can be relocated, so use the MOVEC instruction via
    // Supervisor() to read it safely
    if (SysBase->AttnFlags & AFF_68010)
        pVbr = (APTR)Supervisor((ULONG(*)())getvbr);

    return pVbr;
}
//---------------------------------------------------------------------------
ULONG amgBeamLine()
{
    // the 32-bit read gives: bits[24:16] = bit 8 of V, bits[15:8] = bits[7:0] of V, bits[7:0] = horizontal position.
    // Masking with 0x1FF00 isolates the 9-bit vertical counter, left-shifted by 8 so it can be directly compared with
    // (lineNumber << 8)
    return (*(volatile ULONG*)0xDFF004) & 0x1ff00;
}
//---------------------------------------------------------------------------
void amgWaitVbl()
{
    // two-phase approach to guarantee we catch the *next* VBL and not the current one if called during VBL:
    // - Phase 1 - spin until beam has LEFT line 311 (handle already-in-VBL case)
    // - Phase 2 - spin until beam REACHES line 311 again (the true next VBL)
    while (amgBeamLine() == (311 << 8)); // phase 1: leave current VBL if on it
    while (amgBeamLine() != (311 << 8)); // phase 2: wait for the next VBL
}
//---------------------------------------------------------------------------
void amgWaitBlt()
{
    UWORD tst = *(volatile UWORD*)&g_pCustom->dmaconr;
    (void)tst; // discard value, side-effect of the read is what matters

    // bit 14 of DMACONR is the "blitter busy" flag. The dummy read before the loop is required on some Agnus revisions
    // to ensure the busy flag has been set before we start polling it
    while (*(volatile UWORD*)&g_pCustom->dmaconr & (1 << 14))
    {}
}
//---------------------------------------------------------------------------
void amgClearDrawBuffer(const USHORT lineSize, UBYTE bitPlanes, UBYTE* pDrawBuffer[])
{
    for (int p = 0; p < bitPlanes; ++p)
        memset(pDrawBuffer[p], 0, lineSize * 256);
}
//---------------------------------------------------------------------------
void amgSwapBuffers(UBYTE bitPlanes, UBYTE* pDrawBuffer[], UBYTE* pDisplayBuffer[], volatile short* pSwapRequested)
{
    // swap draw / display buffers for ALL bitplanes
    for (int p = 0; p < bitPlanes; ++p)
    {
        UBYTE* pTmp       = pDrawBuffer[p];
        pDrawBuffer[p]    = pDisplayBuffer[p];
        pDisplayBuffer[p] = pTmp;
    }

    // request copper update in VBL
    *pSwapRequested = 1;

    // wait until interrupt handler updates copper
    while (*pSwapRequested)
    {
        // busy wait
    }
}
//---------------------------------------------------------------------------
void amgDrawPixel(USHORT x, USHORT y, USHORT lineSize, UBYTE color, UBYTE bitPlanes, UBYTE* pDrawBuffer[])
{
    // is pixel out of screen?
    if (x >= 320 || y >= 256)
        return;

    ULONG offset = y * lineSize + (x >> 3);
    UBYTE mask   = 1 << (7 - (x & 7));

    for (int p = 0; p < bitPlanes; ++p)
        if (color & (1 << p))
            pDrawBuffer[p][offset] |= mask;
        else
            pDrawBuffer[p][offset] &= ~mask;
}
//---------------------------------------------------------------------------
