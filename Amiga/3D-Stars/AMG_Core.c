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

// amiga lib
#include "support/gcc8_c_support.h"

//---------------------------------------------------------------------------
// Functions
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
