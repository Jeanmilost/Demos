/****************************************************************************
 * ==> DWF_GeometryHelper --------------------------------------------------*
 ****************************************************************************
 * Description: Helper class for geometry                                   *
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

#include "DWF_GeometryHelper.h"

// std
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

// classes
#include "DWF_MathHelper.h"

using namespace DWF_Geometry;

 //---------------------------------------------------------------------------
 // Helper
 //---------------------------------------------------------------------------
bool Helper::PointInsidePolygon(const DWF_Math::Vector3F point, const IVertices& vertices)
{
    const std::size_t vertexCount = vertices.size();

    // should contain at least 3 vertices, otherwise it's not a polygon
    if (vertexCount < 3)
        return false;

    IVertices nPToVx;
    nPToVx.reserve(vertexCount);

    // calculate vector from point to each vertex, and normalize them
    for (std::size_t i = 0; i < vertexCount; ++i)
        nPToVx.push_back((vertices[i] - point).Normalize());

    std::vector<float> angles;
    angles.reserve(vertexCount);

    // calculate the angles using the dot product of each vectors. Limit range
    // to values between -1.0f and 1.0f
    for (std::size_t i = 0; i < vertexCount; ++i)
        if (i == vertexCount - 1)
            angles[i] = DWF_Math::Helper::Clamp(nPToVx[i].Dot(nPToVx[0]),     -1.0f, 1.0f);
        else
            angles[i] = DWF_Math::Helper::Clamp(nPToVx[i].Dot(nPToVx[i + 1]), -1.0f, 1.0f);

    float angleResult = 0.0f;

    // calculate the sum of all angles
    for (std::size_t i = 0; i < vertexCount; ++i)
        #if defined(__APPLE__)
            angleResult += acosf(angles[i]);
        #else
            angleResult += std::acosf(angles[i]);
        #endif

    // if sum is equal to 2 PI radians then point p is inside polygon. NOTE can
    // be higher due to precision errors in calculations
    return (angleResult >= (float)(M_PI * 2.0));
}
//---------------------------------------------------------------------------
bool Helper::Intersect(const AABB& c1, const Sphere& c2)
{
    float d = 0.0f;

    // find the square of the distance from the sphere to the box on the x axis
    if (c2.m_Center.m_X < c1.m_Min.m_X)
        d += powf(c2.m_Center.m_X - c1.m_Min.m_X, 2.0f);
    else
    if (c2.m_Center.m_X > c1.m_Max.m_X)
        d += powf(c2.m_Center.m_X - c1.m_Max.m_X, 2.0f);

    // find the square of the distance from the sphere to the box on the y axis
    if (c2.m_Center.m_Y < c1.m_Min.m_Y)
        d += powf(c2.m_Center.m_Y - c1.m_Min.m_Y, 2.0f);
    else
    if (c2.m_Center.m_Y > c1.m_Max.m_Y)
        d += powf(c2.m_Center.m_Y - c1.m_Max.m_Y, 2.0f);

    // find the square of the distance from the sphere to the box on the z axis
    if (c2.m_Center.m_Z < c1.m_Min.m_Z)
        d += powf(c2.m_Center.m_Z - c1.m_Min.m_Z, 2.0f);
    else
    if (c2.m_Center.m_Z > c1.m_Max.m_Z)
        d += powf(c2.m_Center.m_Z - c1.m_Max.m_Z, 2.0f);

    return (d <= powf(c2.m_Radius, 2.0f));
}
//---------------------------------------------------------------------------
bool Helper::Intersect(const Capsule& c1, const Sphere& c2)
{
    // calculate capsule top and bottom positions from which the calculation should be applied
    const DWF_Math::Vector3F capsLineDir       = (c1.m_Top   - c1.m_Bottom).Normalize();
    const DWF_Math::Vector3F capsLineEndOffset = capsLineDir * c1.m_Radius;
    const DWF_Math::Vector3F capsTop           = c1.m_Top    - capsLineEndOffset;
    const DWF_Math::Vector3F capsBottom        = c1.m_Bottom + capsLineEndOffset;

    // get capsule center line
    const LineF capsLine(capsTop, capsBottom);

    // calculate the closest point on line located in the capsule center, then use this point
    // to calculate the length between the sphere and this line
    const DWF_Math::Vector3F closestPoint = capsLine.ClosestPoint(c2.m_Center);
    const DWF_Math::Vector3F sphereToLine = c2.m_Center - sphereToLine;
    const float              len          = sphereToLine.Length();

    // calculate the penetration depth
    const float penetrationDepth = (c1.m_Radius + c2.m_Radius) - std::fabsf(len);

    return (penetrationDepth > 0.0f);
}
//---------------------------------------------------------------------------
bool Helper::Intersect(const Polygon& c1, const Sphere& c2, PlaneF& slidingPlane)
{
    // create a plane using the 3 vertices of the polygon
    const PlaneF polygonPlane = c1.GetPlane();

    // calculate the distance between the center of the sphere and the plane
    const float testPoint1 = polygonPlane.DistanceTo(c2.m_Center);

    DWF_Math::Vector3F sphereNormal;

    // calculate the normal of the distance sphere-plane using the positive or
    // negative value of the calculated distance between plane and position
    if (testPoint1 < 0.0f)
        sphereNormal = DWF_Math::Vector3F(polygonPlane.m_A, polygonPlane.m_B, polygonPlane.m_C);
    else
        sphereNormal = DWF_Math::Vector3F(-polygonPlane.m_A, -polygonPlane.m_B, -polygonPlane.m_C);

    // calculate the point who the segment from center of sphere in the
    // direction of the plane will cross the border of the sphere
    DWF_Math::Vector3F pointOnSphere = DWF_Math::Vector3F(c2.m_Center.m_X + (c2.m_Radius * sphereNormal.m_X),
                                                          c2.m_Center.m_Y + (c2.m_Radius * sphereNormal.m_Y),
                                                          c2.m_Center.m_Z + (c2.m_Radius * sphereNormal.m_Z));

    // calculate the distance between the border of the sphere and the plane
    const float testPoint2 = polygonPlane.DistanceTo(pointOnSphere);

    // if the test points are on each side of the plane, then the sphere cross
    // the plane. We assume that the segment from the center of the sphere to
    // the direction of the plane can never be co-planar
    if ((testPoint1 <= 0.0f && testPoint2 >= 0.0f) || (testPoint2 <= 0.0f && testPoint1 >= 0.0f))
    {
        DWF_Math::Vector3F pointOnPlane;

        // calculate who the segment cross the plane
        if (testPoint1 == 0.0f)
            // if testPoint1 is equal to 0, the center of the sphere cross the plane
            pointOnPlane = c2.m_Center;
        else
        if (testPoint2 == 0.0f)
            // if testPoint2 is equal to 0, the border of the sphere cross the plane
            pointOnPlane = pointOnSphere;
        else
            // calculate the intersection point
            polygonPlane.Intersect(LineF(c2.m_Center, pointOnSphere), pointOnPlane);

        // check if calculated point is inside the polygon
        if (c1.Inside(pointOnPlane))
        {
            // if yes, the sphere collide the polygon. In this case, we copy
            // the plane and we returns true
            slidingPlane = polygonPlane;
            return true;
        }
        else
        {
            // otherwise check if the sphere collide the border of the polygon.
            // First we calculate the test point on the border of the polygon
            DWF_Math::Vector3F pointOnTriangle = c1.ClosestPoint(pointOnPlane);

            // check if calculated point is inside the sphere
            if (c2.Inside(pointOnTriangle))
            {
                // if yes, the sphere collide the polygon. In this case, we copy
                // the plane and we returns true
                slidingPlane = polygonPlane;
                return true;
            }
        }
    }

    // no collision was found
    return false;
}
//---------------------------------------------------------------------------
bool Helper::Intersect(const Polygon& c1, const AABB& c2)
{
    // one polygon vertex is inside the box?
    if (c2.Inside(c1.m_Vertex[0]) || c2.Inside(c1.m_Vertex[1]) || c2.Inside(c1.m_Vertex[2]))
        return true;

    std::vector<LineF> polygonLines;
    c1.GetLines(polygonLines);

    // one of the polygon edge crosses the box?
    for (std::size_t i = 0; i < polygonLines.size(); ++i)
        if (c2.Intersect(polygonLines[i]))
            return true;

    std::vector<LineF> boxLines;
    c2.GetLines(boxLines);

    DWF_Math::Vector3F intersectionPoint;

    // one of the box edge crosses the polygon?
    for (std::size_t i = 0; i < boxLines.size(); ++i)
        if (c1.Intersect(boxLines[i], intersectionPoint))
            return true;

    return false;
}
//---------------------------------------------------------------------------
