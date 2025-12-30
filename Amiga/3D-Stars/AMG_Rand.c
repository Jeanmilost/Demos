/****************************************************************************
 * ==> AMG_Rand ------------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - Random functions                            *
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

#include "AMG_Rand.h"

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
static ULONG g_RandSeed = 1;
//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------
void amgSrand(ULONG seed)
{
    g_RandSeed = seed;
}
//---------------------------------------------------------------------------
ULONG amgRand(void)
{
    g_RandSeed = g_RandSeed * 1103515245UL + 12345UL;

    return (g_RandSeed >> 16) & 0x7FFF;
}
//---------------------------------------------------------------------------
