/****************************************************************************
 * ==> DWF_CapsuleCollider -------------------------------------------------*
 ****************************************************************************
 * Description : Capsule collider                                           *
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

#include "DWF_CapsuleCollider.h"

using namespace DWF_Collider;

//---------------------------------------------------------------------------
// Capsule_Collider
//---------------------------------------------------------------------------
Capsule_Collider::Capsule_Collider()
{}
//---------------------------------------------------------------------------
Capsule_Collider::~Capsule_Collider()
{}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Capsule_Collider::Support(const DWF_Math::Vector3F& dir) const
{
    // find support in model space
    const DWF_Math::Vector3F invDir = m_InvMatRS.Transform(dir);

    DWF_Math::Vector3F result  = invDir.Normalize() * m_Radius;
    result.m_Y                += (dir.m_Y > 0) ? m_yCap : m_yBase;

    // convert support to world space
    return m_MatRS.Transform(result) + m_Pos;
}
//---------------------------------------------------------------------------
