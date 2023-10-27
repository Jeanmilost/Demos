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

#pragma once

// std
#include <vector>

// classes
#include "DWF_Vector3.h"
#include "DWF_AABB.h"
#include "DWF_Capsule.h"
#include "DWF_Polygon.h"
#include "DWF_Sphere.h"

namespace DWF_Geometry
{
    /**
    * Helper class for geometry
    *@author Jean-Milost Reymond
    */
    class Helper
    {
        public:
            typedef std::vector<DWF_Math::Vector3F> IVertices;

            /**
            * Checks if a point is inside a convex polygon for which all vertices are located on the same plane
            *@param point - point to test
            *@param vertices - polygon vertices
            *@return true if point is inside the polygon, otherwise false
            *@note The polygon should contain at least 3 vertices, otherwise it's not a polygon and the function
            *      will return false
            */
            static bool PointInsidePolygon(const DWF_Math::Vector3F point, const IVertices& vertices);

            /**
            * Checks if a box intersects a sphere
            *@param c1 - box collider to check
            *@param c2 - sphere collider to check
            *@return true if colliders are in collision, otherwise false
            */
            static bool Intersect(const AABB& c1, const Sphere& c2);

            /**
            * Checks if a capsule intersects a sphere
            *@param c1 - capsule collider to check
            *@param c2 - sphere collider to check
            *@return true if colliders are in collision, otherwise false
            */
            static bool Intersect(const Capsule& c1, const Sphere& c2);

            /**
            * Checks if a polygon intersects a sphere, and calculates the sliding plane
            *@param c1 - polygon to check
            *@param c2 - sphere to check
            *@param[out] slidingPlane - sliding plane, in case of collision
            *@return true if sphere intersects polygon, otherwise false
            */
            static bool Intersect(const Polygon& c1, const Sphere& c2, PlaneF& slidingPlane);

            /**
            * Checks if a polygon intersects a box
            *@param c1 - polygon collider to check
            *@param c2 - box collider to check
            *@return true if colliders are in collision, otherwise false
            */
            static bool Intersect(const Polygon& c1, const AABB& c2);
    };
}
