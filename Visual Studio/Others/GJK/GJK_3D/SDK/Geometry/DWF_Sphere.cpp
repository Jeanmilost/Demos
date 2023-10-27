/****************************************************************************
 * ==> DWF_Sphere ----------------------------------------------------------*
 ****************************************************************************
 * Description : Geometric sphere                                           *
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

#include "DWF_Sphere.h"

using namespace DWF_Geometry;

//---------------------------------------------------------------------------
// Sphere
//---------------------------------------------------------------------------
Sphere::Sphere()
{}
//---------------------------------------------------------------------------
Sphere::~Sphere()
{}
//---------------------------------------------------------------------------
bool Sphere::Inside(const DWF_Math::Vector3F& point) const
{
    // calculate the distance between the test point and the sphere center
    const DWF_Math::Vector3F pToC     = point - m_Center;
    const float              distance = pToC.Length();

    // check if distance is shorter than the sphere radius and return result
    return (distance <= m_Radius);
}
//---------------------------------------------------------------------------
bool Sphere::Intersect(const Sphere& other) const
{
    DWF_Math::Vector3F dist;

    // calculate the distance between the both sphere centers
    dist.m_X = std::fabsf(m_Center.m_X - other.m_Center.m_X);
    dist.m_Y = std::fabsf(m_Center.m_Y - other.m_Center.m_Y);
    dist.m_Z = std::fabsf(m_Center.m_Z - other.m_Center.m_Z);

    // calculate the length between the both sphere centers
    const float length = dist.Length();

    // calculate the penetration depth
    const float penetrationDepth = (m_Radius + other.m_Radius) - length;

    // the spheres are in collision if the length between their centers is lower than
    // or equal to the sum of the both sphere radius
    return (penetrationDepth > 0.0f);
}
//---------------------------------------------------------------------------
bool Sphere::Intersect(const PlaneF& plane) const
{
    // calculate distance between sphere center and plane
    const float sphereToPlane = plane.DistanceTo(m_Center);

    DWF_Math::Vector3F sphereNormal;

    // is sphere before or after plane?
    if (sphereToPlane < 0.0f)
        // get sphere normal
        sphereNormal = DWF_Math::Vector3F( plane.m_A,  plane.m_B,  plane.m_C);
    else
        // get sphere normal, invert values to conserve correct direction
        sphereNormal = DWF_Math::Vector3F(-plane.m_A, -plane.m_B, -plane.m_C);

    // calculate where vector starting from sphere center toward the plane
    // crosses the sphere border
    const DWF_Math::Vector3F pointOnSphere(m_Center.m_X + (m_Radius * sphereNormal.m_X),
                                           m_Center.m_Y + (m_Radius * sphereNormal.m_Y),
                                           m_Center.m_Z + (m_Radius * sphereNormal.m_Z));

    // calculate where vector starting from sphere center toward the plane
    // crosses the plane
    const float radiusToPlane = plane.DistanceTo(pointOnSphere);

    // sphere cross plane if both points are located on each side of the plane
    // or if one point on the plane
    return ((sphereToPlane <  0.0f && radiusToPlane >  0.0f) ||
            (sphereToPlane >  0.0f && radiusToPlane <  0.0f) ||
             sphereToPlane == 0.0f || radiusToPlane == 0.0f);
}
//---------------------------------------------------------------------------
bool Sphere::Intersect(const LineF& line) const
{
    return (std::fabsf(line.GetShortestDistance(m_Center)) <= m_Radius);
}
//---------------------------------------------------------------------------
bool Sphere::Intersect(const RayF& ray) const
{
    return (std::fabsf(ray.GetShortestDistance(m_Center)) <= m_Radius);
}
//---------------------------------------------------------------------------
