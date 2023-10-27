/****************************************************************************
 * ==> DWF_AABB ------------------------------------------------------------*
 ****************************************************************************
 * Description : Geometric aligned-Axis Bounding Box                        *
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

#include "DWF_AABB.h"

using namespace DWF_Geometry;

//---------------------------------------------------------------------------
// AABB
//---------------------------------------------------------------------------
AABB::AABB()
{}
//---------------------------------------------------------------------------
AABB::AABB(const DWF_Math::Vector3F& min, const DWF_Math::Vector3F& max) :
    m_Min(min),
    m_Max(max)
{}
//---------------------------------------------------------------------------
AABB::~AABB()
{}
//---------------------------------------------------------------------------
DWF_Math::Vector3F AABB::GetCenter() const
{
    return DWF_Math::Vector3F((m_Min.m_X + m_Max.m_X) / 2.0f,
                              (m_Min.m_Y + m_Max.m_Y) / 2.0f,
                              (m_Min.m_Z + m_Max.m_Z) / 2.0f);
}
//---------------------------------------------------------------------------
void AABB::GetLines(std::vector<LineF>& lines) const
{
    //     xyz *--------* Xyz
    //       / |      / |
    // xyZ *--------* Xy|Z
    //     |   |    |   |
    //    x|Yz *----|---* XYz
    //     | /      | /
    // xYZ *--------* XYZ

    lines.resize(12);
    lines[0]  = LineF(DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    lines[1]  = LineF(DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z));
    lines[2]  = LineF(DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z));
    lines[3]  = LineF(DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z));
    lines[4]  = LineF(DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z));
    lines[5]  = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z));
    lines[6]  = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    lines[7]  = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    lines[8]  = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z));
    lines[9]  = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z));
    lines[10] = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z));
    lines[11] = LineF(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z));
}
//---------------------------------------------------------------------------
void AABB::GetFaces(std::vector<DWF_Math::Vector3F>& faces) const
{
    //     xyz *--------* Xyz
    //       / |      / |
    // xyZ *--------* Xy|Z
    //     |   |    |   |
    //    x|Yz *----|---* XYz
    //     | /      | /
    // xYZ *--------* XYZ

    faces.resize(24);

    faces[0]  = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z);
    faces[1]  = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[2]  = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[3]  = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z);

    faces[4]  = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z);
    faces[5]  = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[6]  = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[7]  = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z);

    faces[8]  = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z);
    faces[9]  = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z);
    faces[10] = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[11] = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z);

    faces[12] = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z);
    faces[13] = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z);
    faces[14] = DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z);
    faces[15] = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z);

    faces[16] = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z);
    faces[17] = DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z);
    faces[18] = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z);
    faces[19] = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z);

    faces[20] = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z);
    faces[21] = DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z);
    faces[22] = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z);
    faces[23] = DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z);
}
//---------------------------------------------------------------------------
void AABB::GetPlanes(std::vector<PlaneF>& planes) const
{
    //     xyz *--------* Xyz
    //       / |      / |
    // xyZ *--------* Xy|Z
    //     |   |    |   |
    //    x|Yz *----|---* XYz
    //     | /      | /
    // xYZ *--------* XYZ

    planes.resize(6);
    planes[0] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    planes[1] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    planes[2] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z));
    planes[3] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z));
    planes[4] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z));
    planes[5] = PlaneF::FromPoints(DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z), DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z), DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z));
}
//---------------------------------------------------------------------------
AABB::IEAxis AABB::GetLongestAxis() const
{
    // calculate each edge length
    const float x = std::fabsf(m_Max.m_X - m_Min.m_X);
    const float y = std::fabsf(m_Max.m_Y - m_Min.m_Y);
    const float z = std::fabsf(m_Max.m_Z - m_Min.m_Z);

    // search for longest axis
    if (x >= y && x >= z)
        return IEAxis::IE_A_X;
    else
    if (y >= x && y >= z)
        return IEAxis::IE_A_Y;

    return IEAxis::IE_A_Z;
}
//---------------------------------------------------------------------------
PlaneF AABB::GetSplittingPlane(IEAxis axis) const
{
    switch (axis)
    {
        case IEAxis::IE_A_X: return PlaneF(1.0f, 0.0f, 0.0f, -GetCenter().m_X);
        case IEAxis::IE_A_Y: return PlaneF(0.0f, 1.0f, 0.0f, -GetCenter().m_Y);
        case IEAxis::IE_A_Z: return PlaneF(0.0f, 0.0f, 1.0f, -GetCenter().m_Z);
        default:             return PlaneF();
    }
}
//---------------------------------------------------------------------------
PlaneF AABB::GetSplittingPlane(IEAxis axis, const DWF_Math::Vector3F& center) const
{
    switch (axis)
    {
        case IEAxis::IE_A_X: return PlaneF(1.0f, 0.0f, 0.0f, -center.m_X);
        case IEAxis::IE_A_Y: return PlaneF(0.0f, 1.0f, 0.0f, -center.m_Y);
        case IEAxis::IE_A_Z: return PlaneF(0.0f, 0.0f, 1.0f, -center.m_Z);
        default:             return PlaneF();
    }
}
//---------------------------------------------------------------------------
void AABB::Cut(AABB& leftBox, AABB& rightBox)
{
    // calculate each edge length
    const float x = std::fabsf(m_Max.m_X - m_Min.m_X);
    const float y = std::fabsf(m_Max.m_Y - m_Min.m_Y);
    const float z = std::fabsf(m_Max.m_Z - m_Min.m_Z);

    IEAxis longestAxis;

    // search for longest axis
    if ((x >= y) && (x >= z))
        longestAxis = IEAxis::IE_A_X;
    else
    if ((y >= x) && (y >= z))
        longestAxis = IEAxis::IE_A_Y;
    else
        longestAxis = IEAxis::IE_A_Z;

    // cut box
    switch (longestAxis)
    {
        case IEAxis::IE_A_X:
            // cut on x axis
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_X = m_Min.m_X + (x * 0.5f);

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_X = leftBox.m_Max.m_X;

            return;

        case IEAxis::IE_A_Y:
            // cut on y axis
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_Y = m_Min.m_Y + (y * 0.5f);

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_Y = leftBox.m_Max.m_Y;

            return;

        case IEAxis::IE_A_Z:
            // cut on z axis
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_Z = m_Min.m_Z + (z * 0.5f);

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_Z = leftBox.m_Max.m_Z;

            return;
    }
}
//---------------------------------------------------------------------------
bool AABB::Inside(const DWF_Math::Vector3F& point) const
{
    return (point.m_X >= m_Min.m_X && point.m_Y >= m_Min.m_Y && point.m_Z >= m_Min.m_Z &&
            point.m_X <= m_Max.m_X && point.m_Y <= m_Max.m_Y && point.m_Z <= m_Max.m_Z);
}
//---------------------------------------------------------------------------
bool AABB::Intersect(const AABB& other) const
{
    return !(m_Min.m_X <= other.m_Max.m_X &&
             m_Max.m_X >= other.m_Min.m_X &&
             m_Min.m_Y <= other.m_Max.m_Y &&
             m_Max.m_Y >= other.m_Min.m_Y &&
             m_Min.m_Z <= other.m_Max.m_Z &&
             m_Max.m_Z >= other.m_Min.m_Z);
}
//---------------------------------------------------------------------------
bool AABB::Intersect(const PlaneF& plane) const
{
    // get the 8 box vertices
    const DWF_Math::Vector3F vertices[8] =
    {
        m_Min,
        DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Min.m_Z),
        DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Min.m_Z),
        DWF_Math::Vector3F(m_Max.m_X, m_Max.m_Y, m_Min.m_Z),
        DWF_Math::Vector3F(m_Min.m_X, m_Min.m_Y, m_Max.m_Z),
        DWF_Math::Vector3F(m_Min.m_X, m_Max.m_Y, m_Max.m_Z),
        DWF_Math::Vector3F(m_Max.m_X, m_Min.m_Y, m_Max.m_Z),
        m_Max
    };

    float refLength = 0.0f;

    // iterate through box vertices
    for (std::size_t i = 0; i < 8; ++i)
    {
        // calculate distance between current box vertex and plane
        const float length = plane.DistanceTo(vertices[i]);

        // is vertex on plane?
        if (length == 0.0f)
            return true;

        // is reference initialized?
        if (refLength == 0.0f)
        {
            refLength = length;
            continue;
        }

        // box cross plane if at least one vertex is on the other plane side
        if ((length < 0.0f && refLength > 0.0f) || (length > 0.0f && refLength < 0.0f))
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool AABB::Intersect(const LineF& line) const
{
    // is line completely outside the box?
    if (line.m_End.m_X < m_Min.m_X && line.m_Start.m_X < m_Min.m_X)
        return false;
    else
    if (line.m_End.m_X > m_Max.m_X && line.m_Start.m_X > m_Max.m_X)
        return false;
    else
    if (line.m_End.m_Y < m_Min.m_Y && line.m_Start.m_Y < m_Min.m_Y)
        return false;
    else
    if (line.m_End.m_Y > m_Max.m_Y && line.m_Start.m_Y > m_Max.m_Y)
        return false;
    else
    if (line.m_End.m_Z < m_Min.m_Z && line.m_Start.m_Z < m_Min.m_Z)
        return false;
    else
    if (line.m_End.m_Z > m_Max.m_Z && line.m_Start.m_Z > m_Max.m_Z)
        return false;

    DWF_Math::Vector3F intersectionPoint;

    // is line start point inside the box?
    if (Inside(line.m_Start))
    {
        intersectionPoint = line.m_Start;
        return true;
    }

    // check if line intersects the box
    return ((GetIntersection(line.m_Start.m_X - m_Min.m_X, line.m_End.m_X - m_Min.m_X, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_X)) ||
            (GetIntersection(line.m_Start.m_Y - m_Min.m_Y, line.m_End.m_Y - m_Min.m_Y, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_Y)) ||
            (GetIntersection(line.m_Start.m_Z - m_Min.m_Z, line.m_End.m_Z - m_Min.m_Z, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_Z)) ||
            (GetIntersection(line.m_Start.m_X - m_Max.m_X, line.m_End.m_X - m_Max.m_X, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_X)) ||
            (GetIntersection(line.m_Start.m_Y - m_Max.m_Y, line.m_End.m_Y - m_Max.m_Y, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_Y)) ||
            (GetIntersection(line.m_Start.m_Z - m_Max.m_Z, line.m_End.m_Z - m_Max.m_Z, line.m_Start, line.m_End, intersectionPoint) && InBox(intersectionPoint, m_Min, m_Max, IEAxis::IE_A_Z)));
}
//---------------------------------------------------------------------------
bool AABB::Intersect(const RayF& ray) const
{
    const DWF_Math::Vector3F rayPos    = ray.GetPos();
    const DWF_Math::Vector3F rayInvDir = ray.GetInvDir();

    float tX1 = 0.0f;

    // calculate nearest point where ray intersects box on x coordinate
    if (rayInvDir.m_X != std::numeric_limits<float>::infinity())
        tX1 = ((m_Min.m_X - rayPos.m_X) * rayInvDir.m_X);
    else
    if ((m_Min.m_X - rayPos.m_X) < 0.0f)
        tX1 = -std::numeric_limits<float>::infinity();
    else
        tX1 =  std::numeric_limits<float>::infinity();

    float tX2 = 0.0f;

    // calculate farthest point where ray intersects box on x coordinate
    if (rayInvDir.m_X != std::numeric_limits<float>::infinity())
        tX2 = ((m_Max.m_X - rayPos.m_X) * rayInvDir.m_X);
    else
    if ((m_Max.m_X - rayPos.m_X) < 0.0f)
        tX2 = -std::numeric_limits<float>::infinity();
    else
        tX2 =  std::numeric_limits<float>::infinity();

    float tY1 = 0.0f;

    // calculate nearest point where ray intersects box on y coordinate
    if (rayInvDir.m_Y != std::numeric_limits<float>::infinity())
        tY1 = ((m_Min.m_Y - rayPos.m_Y) * rayInvDir.m_Y);
    else
    if ((m_Min.m_Y - rayPos.m_Y) < 0.0f)
        tY1 = -std::numeric_limits<float>::infinity();
    else
        tY1 =  std::numeric_limits<float>::infinity();

    float tY2 = 0.0f;

    // calculate farthest point where ray intersects box on y coordinate
    if (rayInvDir.m_Y != std::numeric_limits<float>::infinity())
        tY2 = ((m_Max.m_Y - rayPos.m_Y) * rayInvDir.m_Y);
    else
    if ((m_Max.m_Y - rayPos.m_Y) < 0.0f)
        tY2 = -std::numeric_limits<float>::infinity();
    else
        tY2 =  std::numeric_limits<float>::infinity();

    float tZ1 = 0.0f;

    // calculate nearest point where ray intersects box on z coordinate
    if (rayInvDir.m_Z != std::numeric_limits<float>::infinity())
        tZ1 = ((m_Min.m_Z - rayPos.m_Z) * rayInvDir.m_Z);
    else
    if ((m_Min.m_Z - rayPos.m_Z) < 0.0f)
        tZ1 = -std::numeric_limits<float>::infinity();
    else
        tZ1 =  std::numeric_limits<float>::infinity();

    float tZ2 = 0.0f;

    // calculate farthest point where ray intersects box on z coordinate
    if (rayInvDir.m_Z != std::numeric_limits<float>::infinity())
        tZ2 = ((m_Max.m_Z - rayPos.m_Z) * rayInvDir.m_Z);
    else
    if ((m_Max.m_Z - rayPos.m_Z) < 0.0f)
        tZ2 = -std::numeric_limits<float>::infinity();
    else
        tZ2 =  std::numeric_limits<float>::infinity();

    // calculate near/far intersection on each axis
    const float tXn = std::min(tX1, tX2);
    const float tXf = std::max(tX1, tX2);
    const float tYn = std::min(tY1, tY2);
    const float tYf = std::max(tY1, tY2);
    const float tZn = std::min(tZ1, tZ2);
    const float tZf = std::max(tZ1, tZ2);

    // calculate final near/far intersection point
    const float tNear = std::max(tXn, std::max(tYn, tZn));
    const float tFar  = std::min(tXf, std::min(tYf, tZf));

    // check if ray intersects box
    return (tFar >= tNear);
}
//---------------------------------------------------------------------------
