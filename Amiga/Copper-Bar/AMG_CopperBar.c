/****************************************************************************
 * ==> AMG_CopperBar -------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - Copper bars functions and resources         *
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

#include "AMG_CopperBar.h"

// amiga sdk
#include "support/gcc8_c_support.h"
#include <hardware/custom.h>

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
static USHORT* g_pBar = NULL;
//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------
USHORT* amgCopperBarGetBar()
{
    return g_pBar;
}
//---------------------------------------------------------------------------
void amgCopperBarSetBar(USHORT* pBar)
{
    g_pBar = pBar;
}
//---------------------------------------------------------------------------
void amgCopperBarBuildRedGradient(USHORT height, USHORT* pBarColors)
{
    const USHORT half = height / 2;

    for (USHORT i = 0; i < height; ++i)
    {
        // distance from the nearest edge: rises 0->half over the first half, falls half->0 over the second half (symmetric)
        const USHORT dist = (i < half) ? i : (height - 1 - i);

        // scale to the 4-bit Amiga color range 0-15
        // Formula is:
        //     for line i, dist = distance from nearest edge of the bar
        //     (0 at first/last line, peaks at the centre)
        //     red = (dist * 15) / (half - 1)  scaled to 0..15
        const UBYTE red = (UBYTE)((dist * 15) / (half - 1));

        // pack into Amiga color register format 0x0RGB (here only the red channel is used so 0x0F00)
        pBarColors[i] = (USHORT)(red << 8);
    }
}
//---------------------------------------------------------------------------
void amgCopperBarBuild(USHORT startLine, USHORT height, USHORT* pBarColors)
{
    USHORT* p = g_pBar;

    // copper instruction encoding:
    //     WAIT instruction (2 words):
    //         Word 0:  VVVVVVVV HHHHHHHX
    //             V[7:0] = vertical beam position to wait for (bits 8:1 of V)
    //             H[6:0] = horizontal position (we use 0x00 = any)
    //             X = BFD flag (blitter-finish disable); set to 1 to ignore blitter
    //             Lower bit of word0 must be 1 (distinguishes WAIT from MOVE).
    //             => (scanline << 8) | 0x01
    //         Word 1:  VVVVVVVV HHHHHHHX  mask bits (1 = compare, 0 = ignore)
    //             0xFFFE = compare all V bits, ignore H and BFD
    //             Lower bit of word1 must be 0.
    //
    //     MOVE instruction (2 words):
    //         Word 0:  custom chip register offset (must be even, < 0x1FE, bit 0 = 0)
    //         Word 1:  16-bit value to write
    //
    //     Layout of the bar block (M_BarHeight × 4 + 4 + 2 words total):
    //         For each scanline i (0 .. M_BarHeight-1):
    //             [i*4+0]  WAIT word0 : (absoluteLine << 8) | 0x01
    //             [i*4+1]  WAIT word1 : 0xFFFE
    //             [i*4+2]  MOVE addr  : offsetof(Custom, color[0])
    //             [i*4+3]  MOVE value : g_BarColors[i]
    //         After bar (restore black):
    //             [N+0]    WAIT word0 : ((absoluteLine + M_BarHeight) << 8) | 0x01
    //             [N+1]    WAIT word1 : 0xFFFE
    //             [N+2]    MOVE addr  : offsetof(Custom, color[0])
    //             [N+3]    MOVE value : 0x0000  (black)
    //         End of list:
    //             0xFFFF, 0xFFFE  (WAIT for line 255+ which never comes = end marker)
    for (USHORT i = 0; i < height; ++i)
    {
        *p++ = (USHORT)(((startLine + i) << 8) | 0x01); // WAIT: line i of bar
        *p++ = 0xFFFE;                                  // WAIT: mask
        *p++ = offsetof(struct Custom, color[0]);       // MOVE: color[0] addr
        *p++ = pBarColors[i];                           // MOVE: gradient value
    }

    // restore background to black on the first line after the bar
    *p++ = (USHORT)(((startLine + height) << 8) | 0x01);
    *p++ = 0xFFFE;
    *p++ = offsetof(struct Custom, color[0]);
    *p++ = 0x0000;

    // copper end-of-list: WAIT for line 0xFF which never arrives in PAL
    *p++ = 0xFFFF;
    *p++ = 0xFFFE;
}
//---------------------------------------------------------------------------
