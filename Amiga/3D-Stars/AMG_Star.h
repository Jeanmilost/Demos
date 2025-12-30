/****************************************************************************
 * ==> AMG_Star ------------------------------------------------------------*
 ****************************************************************************
 * Description: Amiga project - Star belonging to a starfield               *
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
#include <exec/types.h>

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

/**
* Star belonging to a starfield
*/
typedef struct
{
    LONG m_X;
    LONG m_Y;
    LONG m_Z;
} AMG_Star3D;

/**
* Rasterized star, ready to draw on the screen
*/
typedef struct
{
    LONG m_X;
    LONG m_Y;
} AMG_Star2D;

//-------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------

/**
* Creates a 2D star
*@return newly created star, 0 on error
*@note The star must be released when no longer used, see agaReleaseStar2D()
*/
AMG_Star2D* amgCreateStar2D(void);

/**
* Releases a 2D star
*@param pStar - 2D star to release
*/
void amgReleaseStar2D(AMG_Star2D* pStar);

/**
* Creates a 3D star
*@return newly created star, 0 on error
*@note The star must be released when no longer used, see agaReleaseStar3D()
*/
AMG_Star3D* amgCreateStar3D(void);

/**
* Releases a 3D star
*@param pStar - 3D star to release
*/
void amgReleaseStar3D(AMG_Star3D* pStar);

/**
* Transforms a 3D star to a 2D star
*@param pStar - 3D star to transform
*@param[out] pTransformedStar - 2D transformed star
*/
void amgTransformStar(AMG_Star3D* pStar, AMG_Star2D* pTransformedStar);

/**
* Calculates the star color regarding its z position
*@param z - 3D star z position
*@param maxZ - max 3D star z position
*/
UBYTE amgStarColorFromZ(LONG z, LONG maxZ);
