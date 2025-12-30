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

#include "AMG_Star.h"

// amiga lib
#include <proto/exec.h>

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------
AMG_Star2D* amgCreateStar2D(void)
{
    // create a new star
    AMG_Star2D* pStar = (AMG_Star2D*)AllocMem(sizeof(AMG_Star2D), MEMF_CHIP | MEMF_CLEAR);

    // succeeded?
    if (!pStar)
        return 0;

    // initialize the star content
    pStar->m_X = 0;
    pStar->m_Y = 0;

    return pStar;
}
//---------------------------------------------------------------------------
void amgReleaseStar2D(AMG_Star2D* pStar)
{
    if (!pStar)
        return;

    FreeMem(pStar, sizeof(AMG_Star2D));
}
//---------------------------------------------------------------------------
AMG_Star3D* amgCreateStar3D(void)
{
    // create a new star
    AMG_Star3D* pStar = (AMG_Star3D*)AllocMem(sizeof(AMG_Star3D), MEMF_CHIP | MEMF_CLEAR);

    // succeeded?
    if (!pStar)
        return 0;

    // initialize the star content
    pStar->m_X = 0;
    pStar->m_Y = 0;
    pStar->m_Z = 0;

    return pStar;
}
//---------------------------------------------------------------------------
void amgReleaseStar3D(AMG_Star3D* pStar)
{
    if (!pStar)
        return;

    FreeMem(pStar, sizeof(AMG_Star3D));
}
//---------------------------------------------------------------------------
void amgTransformStar(AMG_Star3D* pStar, AMG_Star2D* pTransformedStar)
{
    if (!pStar || !pTransformedStar)
        return;

    // rasterize the point. NOTE the 16.16 fixed-point conversion
    pTransformedStar->m_X = ((pStar->m_X << 16) / pStar->m_Z) >> 16;
    pTransformedStar->m_Y = ((pStar->m_Y << 16) / pStar->m_Z) >> 16;
}
//---------------------------------------------------------------------------
UBYTE amgStarColorFromZ(LONG z, LONG maxZ)
{
    // z = 1 .. M_Star_Deep → 7 .. 0
    return (UBYTE)(((maxZ - z) * 7) / (maxZ - 1));
}
//---------------------------------------------------------------------------
