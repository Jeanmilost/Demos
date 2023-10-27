/****************************************************************************
 * ==> DWF_CylinderCollider ------------------------------------------------*
 ****************************************************************************
 * Description : Cylinder collider                                          *
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

#include "DWF_CylinderCollider.h"

using namespace DWF_Collider;

//---------------------------------------------------------------------------
// Cylinder_Collider
//---------------------------------------------------------------------------
Cylinder_Collider::Cylinder_Collider()
{}
//---------------------------------------------------------------------------
Cylinder_Collider::~Cylinder_Collider()
{}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Cylinder_Collider::Support(const DWF_Math::Vector3F& dir) const
{
    // find support in model space
    const DWF_Math::Vector3F invDir = m_InvMatRS.Transform(dir);

    // calculate the furthest point on the XZ direction
    const DWF_Math::Vector3F dirXZ(invDir.m_X, 0.0f, invDir.m_Z);

    // now calculate the furthest point on the Y direction
    DWF_Math::Vector3F result = dirXZ.Normalize() * m_Radius;
    result.m_Y                = (invDir.m_Y > 0) ? m_yCap : m_yBase;

    // convert support to world space
    return m_MatRS.Transform(result) + m_Pos; 
}
//---------------------------------------------------------------------------
