/****************************************************************************
 * ==> DWF_TriangleCollider ------------------------------------------------*
 ****************************************************************************
 * Description : Triangle collider                                          *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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

#include "DWF_TriangleCollider.h"

using namespace DWF_Collider;

//---------------------------------------------------------------------------
// Triangle_Collider
//---------------------------------------------------------------------------
Triangle_Collider::Triangle_Collider()
{}
//---------------------------------------------------------------------------
Triangle_Collider::~Triangle_Collider()
{}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Triangle_Collider::Support(const DWF_Math::Vector3F& dir) const
{
    float              dot0          = m_Vertices[0].Dot(dir);
    float              dot1          = m_Vertices[1].Dot(dir);
    float              dot2          = m_Vertices[2].Dot(dir);
    DWF_Math::Vector3F furthestPoint = m_Vertices[0];

    // find which triangle vertex is furthest along dir
    if (dot1 > dot0)
    {
        furthestPoint = m_Vertices[1];

        if (dot2 > dot1)
            furthestPoint = m_Vertices[2];
    }
    else
    if (dot2 > dot0)
        furthestPoint = m_Vertices[2];

    // fake some depth behind triangle so we have volume
    if (dir.Dot(m_Normal) < 0)
        furthestPoint -= m_Normal;

    return furthestPoint;
}
//---------------------------------------------------------------------------
