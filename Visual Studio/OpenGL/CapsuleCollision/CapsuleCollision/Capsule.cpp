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

    // this capsule
    const Vector3F firstLineDir       = (m_Top       - m_Bottom).Normalize();
    const Vector3F firstLineEndOffset = firstLineDir * m_Radius;
    const Vector3F firstTop           = m_Top        - firstLineEndOffset;
    const Vector3F firstBottom        = m_Bottom     + firstLineEndOffset;

    // second capsule
    const Vector3F secondLineDir       = (other.m_Top   - other.m_Bottom).Normalize();
    const Vector3F secondLineEndOffset = secondLineDir  * other.m_Radius;
    const Vector3F secondTop           = other.m_Top    - secondLineEndOffset;
    const Vector3F secondBottom        = other.m_Bottom + secondLineEndOffset;

    // vectors between line endpoints
    const Vector3F v0 = secondBottom - firstBottom;
    const Vector3F v1 = secondTop    - firstBottom;
    const Vector3F v2 = secondBottom - firstTop;
    const Vector3F v3 = secondTop    - firstTop;

    // squared distances
    const float d0 = v0.Dot(v0);
    const float d1 = v1.Dot(v1);
    const float d2 = v2.Dot(v2);
    const float d3 = v3.Dot(v3);

    Vector3F bestCandidate;

    // select best candidate for endpoint on first capsule
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
        bestCandidate = firstTop;
    else
        bestCandidate = firstBottom;

    const Line3F line      (firstBottom,  firstTop);
    const Line3F secondLine(secondBottom, secondTop);

    // select best candidate for point on second capsule line segment nearest to best potential endpoint on first capsule
    const Vector3F secondBestCandidate = secondLine.ClosestPoint(bestCandidate);

    // do the same for first capsule segment
    bestCandidate = line.ClosestPoint(secondBestCandidate);

    // calculate penetration normal and length
    Vector3F     penetrationNormal = bestCandidate - secondBestCandidate;
    const float  len               = penetrationNormal.Length();

    if (len == 0.0f)
        return false;

    const float absLen = std::fabsf(len);

    // normalize
    penetrationNormal /= absLen;

    // calculate the penetration depth
    penetrationDepth = (m_Radius + other.m_Radius) - absLen;

    return (penetrationDepth > 0.0f);
}
//---------------------------------------------------------------------------
