/****************************************************************************
 * ==> DWF_Polygon ---------------------------------------------------------*
 ****************************************************************************
 * Description: Geometric polygon                                           *
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

#include "DWF_Polygon.h"

using namespace DWF_Geometry;

//---------------------------------------------------------------------------
// Polygon
//---------------------------------------------------------------------------
Polygon::Polygon()
{}
//---------------------------------------------------------------------------
Polygon::Polygon(const DWF_Math::Vector3F& v1,
                 const DWF_Math::Vector3F& v2,
                 const DWF_Math::Vector3F& v3)
{
    m_Vertex[0] = v1;
    m_Vertex[1] = v2;
    m_Vertex[2] = v3;
}
//---------------------------------------------------------------------------
Polygon::~Polygon()
{}
//---------------------------------------------------------------------------
Polygon* Polygon::GetClone() const
{
    // copies the polygon, then returns the copy
    return new Polygon(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
PlaneF Polygon::GetPlane() const
{
    // calculates the plane from the values of the 3 vertices of the polygon
    return PlaneF::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Polygon::GetCenter() const
{
    // calculates then returns the value of the midpoint of the polygon
    return DWF_Math::Vector3F(((m_Vertex[0].m_X + m_Vertex[1].m_X + m_Vertex[2].m_X) / 3.0f),
                              ((m_Vertex[0].m_Y + m_Vertex[1].m_Y + m_Vertex[2].m_Y) / 3.0f),
                              ((m_Vertex[0].m_Z + m_Vertex[1].m_Z + m_Vertex[2].m_Z) / 3.0f));
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Polygon::GetNormal() const
{
    // get polygon plane
    const PlaneF plane = GetPlane();

    // extract the polygon normal
    return DWF_Math::Vector3F(plane.m_A, plane.m_B, plane.m_C);
}
//---------------------------------------------------------------------------
void Polygon::GetLines(std::vector<LineF>& lines) const
{
    lines.resize(3);
    lines[0] = LineF(m_Vertex[0], m_Vertex[1]);
    lines[1] = LineF(m_Vertex[1], m_Vertex[2]);
    lines[2] = LineF(m_Vertex[2], m_Vertex[0]);
}
//---------------------------------------------------------------------------
bool Polygon::Inside(float x, float y, float z) const
{
    return Inside(DWF_Math::Vector3F(x, y, z));
}
//---------------------------------------------------------------------------
bool Polygon::Inside(const DWF_Math::Vector3F& point) const
{
    /*
    * check if the point p is inside the polygon in the following manner:
    *
    *                  V1                         V1
    *                  /\                         /\
    *                 /  \                       /  \
    *                / *p \                  *P /    \
    *               /      \                   /      \
    *            V2 -------- V3             V2 -------- V3
    *
    * calculate the vectors between the point p and each polygon vertex, then
    * calculate the angle formed by each of these vectors. If the sum of the
    * angles is equal to a complete circle, i.e. 2 * pi in radians, then the
    * point p is inside the polygon limits, otherwise the point is outside. It
    * is assumed that the point to check belongs to the polygon's plane
    */
    const DWF_Math::Vector3F nPToV1 = (m_Vertex[0] - point).Normalize();
    const DWF_Math::Vector3F nPToV2 = (m_Vertex[1] - point).Normalize();
    const DWF_Math::Vector3F nPToV3 = (m_Vertex[2] - point).Normalize();

    // calculate the angles using the dot product of each vectors. Limit range
    // to values between -1.0f and 1.0f
    const float a1 = DWF_Math::Helper::Clamp(nPToV1.Dot(nPToV2), -1.0f, 1.0f);
    const float a2 = DWF_Math::Helper::Clamp(nPToV2.Dot(nPToV3), -1.0f, 1.0f);
    const float a3 = DWF_Math::Helper::Clamp(nPToV3.Dot(nPToV1), -1.0f, 1.0f);

    // calculate the sum of all angles
    #if defined(__APPLE__)
        const float angleResult = acosf(a1) + acosf(a2) + acosf(a3);
    #else
        const float angleResult = std::acos(a1) + std::acos(a2) + std::acos(a3);
    #endif

    // if sum is equal to 2 PI radians then point p is inside polygon. NOTE can
    // be higher due to precision errors in calculations
    return (angleResult >= (float)(M_PI * 2.0));
}
//---------------------------------------------------------------------------
bool Polygon::Intersect(const Polygon& other) const
{
    const float tolerance = (float)M_Epsilon;

    // get planes from polygons
    const PlaneF plane1 = GetPlane();
    const PlaneF plane2 = other.GetPlane();

    // are planes merged?
    if (plane1 == plane2 || plane1 == -plane2)
    {
        // is any vertex inside other polygon?
        if (Inside(other.m_Vertex[0]) || Inside(other.m_Vertex[1]) || Inside(other.m_Vertex[2]) ||
            other.Inside(m_Vertex[0]) || other.Inside(m_Vertex[1]) || other.Inside(m_Vertex[2]))
            return true;

        std::vector<LineF> lines;
        std::vector<LineF> otherLines;

        // get polygons lines
        GetLines(lines);
        other.GetLines(otherLines);

        // is shortest distance between lines equal to 0?
        if (lines[0].GetShortestDistance(otherLines[0]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[1].GetShortestDistance(otherLines[0]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[2].GetShortestDistance(otherLines[0]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[0].GetShortestDistance(otherLines[1]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[1].GetShortestDistance(otherLines[1]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[2].GetShortestDistance(otherLines[1]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[0].GetShortestDistance(otherLines[2]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[1].GetShortestDistance(otherLines[2]) < tolerance)
            return true;

        // is shortest distance between lines equal to 0?
        if (lines[2].GetShortestDistance(otherLines[2]) < tolerance)
            return true;

        return false;
    }

    // get plane normals
    DWF_Math::Vector3F normal1(plane1.m_A, plane1.m_B, plane1.m_C);
    DWF_Math::Vector3F normal2(plane2.m_A, plane2.m_B, plane2.m_C);

    // calculate angle between planes
    const float planesDot = normal1.Dot(normal2);

    // are plane parallels?
    if (planesDot == 1.0f || planesDot == -1.0f)
        // planes are parallels but not merged, no collision is possible
        return false;

    // calculate distance from each first polygon vertex to second polygon plane
    const float dist1v1 = plane2.DistanceTo(m_Vertex[0]);
    const float dist1v2 = plane2.DistanceTo(m_Vertex[1]);
    const float dist1v3 = plane2.DistanceTo(m_Vertex[2]);

    // get polygon lines
    std::vector<LineF> lines;
    GetLines(lines);

    // prepare list containing first polygon intersection points
    std::vector<DWF_Math::Vector3F> p1pts;
    p1pts.reserve(2);

    // is first polygon V1 to V2 line segment intersects second polygon plane?
    if ((dist1v1 >= 0.0f && dist1v2 < 0.0f) || (dist1v1 < 0.0f && dist1v2 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane2.Intersect(lines[0], p))
            p1pts.push_back(p);
    }

    // is first polygon V2 to V3 line segment intersects second polygon plane?
    if ((dist1v2 >= 0.0f && dist1v3 < 0.0f) || (dist1v2 < 0.0f && dist1v3 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane2.Intersect(lines[1], p))
            p1pts.push_back(p);
    }

    // is first polygon V3 to V1 line segment intersects second polygon plane?
    if ((dist1v3 >= 0.0f && dist1v1 < 0.0f) || (dist1v3 < 0.0f && dist1v1 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane2.Intersect(lines[2], p))
            p1pts.push_back(p);
    }

    // were the first polygon 2 intersection point found?
    if (p1pts.size() != 2)
        return false;

    // calculate distance from each second polygon vertex to first polygon plane
    const float dist2v1 = plane1.DistanceTo(other.m_Vertex[0]);
    const float dist2v2 = plane1.DistanceTo(other.m_Vertex[1]);
    const float dist2v3 = plane1.DistanceTo(other.m_Vertex[2]);

    // get other polygon lines
    std::vector<LineF> otherLines;
    other.GetLines(otherLines);

    // prepare list containing second polygon intersection points
    std::vector<DWF_Math::Vector3F> p2pts;
    p2pts.reserve(2);

    // is second polygon V1 to V2 line segment intersects first polygon plane?
    if ((dist2v1 >= 0.0f && dist2v2 < 0.0f) || (dist2v1 < 0.0f && dist2v2 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane1.Intersect(otherLines[0], p))
            p2pts.push_back(p);
    }

    // is second polygon V2 to V3 line segment intersects first polygon plane?
    if ((dist2v2 >= 0.0f && dist2v3 < 0.0f) || (dist2v2 < 0.0f && dist2v3 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane1.Intersect(otherLines[1], p))
            p2pts.push_back(p);
    }

    // is second polygon V3 to V1 line segment intersects first polygon plane?
    if ((dist2v3 >= 0.0f && dist2v1 < 0.0f) || (dist2v3 < 0.0f && dist2v1 >= 0.0f))
    {
        DWF_Math::Vector3F p;

        // calculate intersection point and add to list on success
        if (plane1.Intersect(otherLines[2], p))
            p2pts.push_back(p);
    }

    // were the second polygon 2 intersection point found?
    if (p2pts.size() != 2)
        return false;

    // first and second polygon intersection points are on the same line, so
    // check if calculated first and second polygon segments intersect
    return (DWF_Math::Helper::IsBetween(p1pts[0], p2pts[0], p2pts[1], tolerance) ||
            DWF_Math::Helper::IsBetween(p1pts[1], p2pts[0], p2pts[1], tolerance) ||
            DWF_Math::Helper::IsBetween(p2pts[0], p1pts[0], p1pts[1], tolerance) ||
            DWF_Math::Helper::IsBetween(p2pts[1], p1pts[0], p1pts[1], tolerance));
}
//---------------------------------------------------------------------------
bool Polygon::Intersect(const PlaneF& plane, std::vector<DWF_Math::Vector3F>& intersectionPoints) const
{
    std::vector<LineF> lines;

    // get lines based on polygon vertices
    GetLines(lines);

    bool foundIntersection = false;

    // iterate through polygon edges
    for (std::size_t i = 0; i < lines.size(); ++i)
    {
        DWF_Math::Vector3F intersectionPoint;

        // found an intersection?
        if (plane.Intersect(lines[i], intersectionPoint))
        {
            intersectionPoints.push_back(intersectionPoint);
            foundIntersection = true;
        }
    }

    return foundIntersection;
}
//---------------------------------------------------------------------------
bool Polygon::Intersect(const LineF& line, DWF_Math::Vector3F& intersectionPoint) const
{
    // create a plane using the 3 vertices of the polygon
    PlaneF polygonPlane = PlaneF::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);

    if (!polygonPlane.Intersect(line, intersectionPoint))
        return false;

    return Inside(intersectionPoint);
}
//---------------------------------------------------------------------------
bool Polygon::Intersect(const RayF& ray, DWF_Math::Vector3F& intersectionPoint) const
{
    // create a plane using the 3 vertices of the polygon
    PlaneF polygonPlane = PlaneF::FromPoints(m_Vertex[0], m_Vertex[1], m_Vertex[2]);

    if (!polygonPlane.Intersect(ray, intersectionPoint))
        return false;

    return Inside(intersectionPoint);
}
//---------------------------------------------------------------------------
float Polygon::DistanceTo(const DWF_Math::Vector3F& point) const
{
    float result     = std::numeric_limits<float>::infinity();
    bool  isNegative = false;

    // check each line belonging to polygon
    for (std::size_t i = 0; i < 3; ++i)
    {
        // get previous line index to check with
        const std::size_t previousIndex = i ? i - 1 : 2;

        // get line to check
        LineF line(m_Vertex[i], m_Vertex[previousIndex]);

        // calculate distance between point and polygon line
        const float dist    = line.GetShortestDistance(point);
        const float absDist = std::fabsf(dist);

        // keep shortest distance
        if (absDist < result)
        {
            result     = absDist;
            isNegative = (dist < 0.0f);
        }
    }

    return isNegative ? -result : result;
}
//---------------------------------------------------------------------------
DWF_Math::Vector3F Polygon::ClosestPoint(const DWF_Math::Vector3F& point) const
{
    std::vector<LineF> lines;

    // get lines based on polygon vertices
    GetLines(lines);

    // calculate the projections points on each edge of the triangle
    DWF_Math::Vector3F rab = lines[0].ClosestPoint(point);
    DWF_Math::Vector3F rbc = lines[1].ClosestPoint(point);
    DWF_Math::Vector3F rca = lines[2].ClosestPoint(point);

    // calculate the distances between points below and test point
    DWF_Math::Vector3F vAB = point - rab;
    DWF_Math::Vector3F vBC = point - rbc;
    DWF_Math::Vector3F vCA = point - rca;

    // calculate the length of each segments
    float dAB = vAB.Length();
    float dBC = vBC.Length();
    float dCA = vCA.Length();

    // calculate the shortest distance
    float min = dAB;

    DWF_Math::Vector3F result = rab;

    // check if dBC is shortest
    if (dBC < min)
    {
        min    = dBC;
        result = rbc;
    }

    // check if dCA is shortest
    if (dCA < min)
        result = rca;

    return result;
}
//---------------------------------------------------------------------------
Polygon* Polygon::ApplyMatrix(const DWF_Math::Matrix4x4F& matrix) const
{
    // build a new polygon transforming all vertices of the polygon using
    // given matrix, and return new built polygon
    return new Polygon(matrix.Transform(m_Vertex[0]),
                       matrix.Transform(m_Vertex[1]),
                       matrix.Transform(m_Vertex[2]));
}
//---------------------------------------------------------------------------
