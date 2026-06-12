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

#pragma once

// amiga sdk
#include <exec/types.h>

//-------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------

/**
* Gets the copper bar address into the copper list at the first WAIT word of the bar block
*@return the copper bar address
*@note The interrupt handler walks this pointer each VBL to update scanline waits
*/
USHORT* amgCopperBarGetBar();

/**
* Sets the copper bar address into the copper list at the first WAIT word of the bar block
*@param pBar -  the new copper bar address to set
*@note The interrupt handler walks this pointer each VBL to update scanline waits
*/
void amgCopperBarSetBar(USHORT* pBar);

/**
* Precomputes the per-scanline color values for the gradient bar
*@param height - copper bar height in pixels
*@param pBarColors - bar color table
*/
void amgCopperBarBuildRedGradient(USHORT height, USHORT* pBarColors);

/**
* Writes the initial copper bar block into chip RAM starting at bar address
*@param startLine - bar start line in pixels
*@param height - copper bar height in pixels
*@param pBarColors - bar color table
*/
void amgCopperBarBuild(USHORT startLine, USHORT height, USHORT* pBarColors);
