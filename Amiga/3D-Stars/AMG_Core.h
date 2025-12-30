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

#pragma once

// amiga lib
#include "support/gcc8_c_support.h"
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <exec/execbase.h>
#include <exec/types.h>
#include <graphics/gfxmacros.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

/*
* Amiga-standard fixed-width types:
*
* BYTE  (8-bit signed)
* UBYTE (8-bit unsigned)
* WORD  (16-bit signed)
* UWORD (16-bit unsigned)
* LONG  (32-bit signed)
* ULONG (32-bit unsigned)
*/

//-------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------

/**
* Clears the draw buffer
*@param lineSize - line size
*@param bitPlanes - number of used bitplanes
*@param[out] pDrawBuffer - output draw buffer
*/
void amgClearDrawBuffer(const USHORT lineSize, UBYTE bitPlanes, UBYTE* pDrawBuffer[]);

/**
* Requests a buffer swap (will happen at next VBL)
*@param bitPlanes - number of used bitplanes
*@param pDrawBuffer - draw buffer
*@param pDisplayBuffer - display buffer
*@param[in, out] pSwapRequested - swap requested flag
*/
void amgSwapBuffers(UBYTE bitPlanes, UBYTE* pDrawBuffer[], UBYTE* pDisplayBuffer[], volatile short* pSwapRequested);

/**
* Draws a pixel in the draw buffer
*@param x - pixel x position
*@param y - pixel y position
*@param lineSize - line size
*@param color - pixel color
*@param bitPlanes - number of used bitplanes
*@param[out] pDrawBuffer - output draw buffer
*/
void amgDrawPixel(USHORT x, USHORT y, USHORT lineSize, UBYTE color, UBYTE bitPlanes, UBYTE* pDrawBuffer[]);
