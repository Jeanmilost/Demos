/****************************************************************************
 * ==> Capsule -------------------------------------------------------------*
 ****************************************************************************
 * Description: Geometric capsule                                           *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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

#include "Capsule.h"

// std
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

// classes
#include "Line3.h"

//---------------------------------------------------------------------------
// Capsule
//---------------------------------------------------------------------------
Capsule::Capsule()
{}
//---------------------------------------------------------------------------
Capsule::~Capsule()
{}
//---------------------------------------------------------------------------
bool Capsule::Intersect(const Capsule& other, float& penetrationDepth) const
{
    // perfect collision (rare, but may happen)
    if (m_Top == other.m_Top || m_Bottom == other.m_Bottom)
        return true;

    // calculate first capsule top and bottom positions from which the calculation should be applied
    const Vector3F firstLineDir       = (m_Top - m_Bottom).Normalize();
    const Vector3F firstLineEndOffset = firstLineDir * m_Radius;
    const Vector3F firstTop           = m_Top        - firstLineEndOffset;
    const Vector3F firstBottom        = m_Bottom     + firstLineEndOffset;

    // calculate second capsule top and bottom positions from which the calculation should be applied
    const Vector3F secondLineDir       = (other.m_Top - other.m_Bottom).Normalize();
    const Vector3F secondLineEndOffset = secondLineDir  * other.m_Radius;
    const Vector3F secondTop           = other.m_Top    - secondLineEndOffset;
    const Vector3F secondBottom        = other.m_Bottom + secondLineEndOffset;

    // get lines in the center of each capsules
    const Line3F firstLine(firstTop, firstBottom);
    const Line3F secondLine(secondTop, secondBottom);

    // check if closest distance between lines composed by capsules top and bottom points
    // is smaller than capsules radius
    const float len = firstLine.GetShortestDistance(secondLine);

    // calculate the penetration depth
    penetrationDepth = (m_Radius + other.m_Radius) - std::fabsf(len);

    // are capsules colliding?
    return (penetrationDepth > 0.0f);
}
//---------------------------------------------------------------------------
