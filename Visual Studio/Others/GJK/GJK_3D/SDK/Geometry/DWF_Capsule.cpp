/****************************************************************************
 * ==> DWF_Capsule ---------------------------------------------------------*
 ****************************************************************************
 * Description: Geometric capsule                                           *
 * Developer:   Jean-Milost Reymond                                         *
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

#include "DWF_Capsule.h"

using namespace DWF_Geometry;

//---------------------------------------------------------------------------
// Capsule
//---------------------------------------------------------------------------
Capsule::Capsule()
{}
//---------------------------------------------------------------------------
Capsule::~Capsule()
{}
//---------------------------------------------------------------------------
bool Capsule::Inside(const DWF_Math::Vector3F& point) const
{
    // todo -cFeature -oJean: If the code below doesn't work, try the one implemented in the CompactStar engine (csrInsideCapsule())

    // calculate the distance between the test point and the top capsule center
    const DWF_Math::Vector3F pToTopC = point - m_Top;
    const float              topDist = pToTopC.Length();

    // is distance shorter than the capsule radius?
    if (topDist <= m_Radius)
        return true;

    // calculate the distance between the test point and the bottom capsule center
    const DWF_Math::Vector3F pToBottomC = point - m_Bottom;
    const float              bottomDist = pToBottomC.Length();

    // is distance shorter than the capsule radius?
    if (bottomDist <= m_Radius)
        return true;

    // create a line between the capsule top and bottom
    LineF capsuleLine(m_Bottom, m_Top);

    // calculate the distance between the line and the point to test
    const float centerDist = capsuleLine.GetShortestDistance(point);

    // is distance shorter than the capsule radius?
    return (std::fabsf(centerDist) <= m_Radius);
}
//---------------------------------------------------------------------------
bool Capsule::Intersect(const Capsule& other) const
{
    // perfect collision (rare, but may happen)
    if (m_Top == other.m_Top || m_Bottom == other.m_Bottom)
        return true;

    // calculate first capsule top and bottom positions from which the calculation should be applied
    const DWF_Math::Vector3F firstLineDir       = (m_Top       - m_Bottom).Normalize();
    const DWF_Math::Vector3F firstLineEndOffset = firstLineDir * m_Radius;
    const DWF_Math::Vector3F firstTop           = m_Top        - firstLineEndOffset;
    const DWF_Math::Vector3F firstBottom        = m_Bottom     + firstLineEndOffset;

    // calculate second capsule top and bottom positions from which the calculation should be applied
    const DWF_Math::Vector3F secondLineDir       = (other.m_Top   - other.m_Bottom).Normalize();
    const DWF_Math::Vector3F secondLineEndOffset = secondLineDir  * other.m_Radius;
    const DWF_Math::Vector3F secondTop           = other.m_Top    - secondLineEndOffset;
    const DWF_Math::Vector3F secondBottom        = other.m_Bottom + secondLineEndOffset;

    // get lines in the center of each capsules
    const LineF firstLine (firstTop,  firstBottom);
    const LineF secondLine(secondTop, secondBottom);

    // check if closest distance between lines composed by capsules top and bottom points
    // is smaller than capsules radius
    const float len = firstLine.GetShortestDistance(secondLine);

    // calculate the penetration depth
    const float penetrationDepth = (m_Radius + other.m_Radius) - std::fabsf(len);

    // are capsules colliding?
    return (penetrationDepth > 0.0f);
}
//---------------------------------------------------------------------------
bool Capsule::Intersect(const PlaneF& plane) const
{
    const float d1 = plane.DistanceTo(m_Top);
    const float d2 = plane.DistanceTo(m_Bottom);

    // is capsule crossing plane?
    if ((d1 <= 0.0f && d2 >= 0.0f) || (d1 >= 0.0f && d2 <= 0.0f))
        return true;

    // is one of the capsule extremity close to the plane?
    if ((std::fabsf(d1) <= m_Radius) || (std::fabsf(d2) <= m_Radius))
        return true;

    return false;
}
//---------------------------------------------------------------------------
bool Capsule::Intersect(const LineF& line) const
{
    // get the capsule center line
    const LineF capsuleLine(m_Top, m_Bottom);

    return (std::fabsf(line.GetShortestDistance(capsuleLine)) <= m_Radius);
}
//---------------------------------------------------------------------------
bool Capsule::Intersect(const RayF& ray) const
{
    // get the closest line on the ray from the both capsule extremity
    const LineF closestLine(ray.ClosestPoint(m_Top), ray.ClosestPoint(m_Bottom));

    return Intersect(closestLine);
}
//---------------------------------------------------------------------------
