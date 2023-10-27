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

#pragma once

// std
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>
#include <cstring>
#include <limits>

// classes
#include "DWF_Matrix4x4.h"
#include "DWF_Plane.h"
#include "DWF_Line.h"
#include "DWF_Ray.h"

namespace DWF_Geometry
{
    /**
    * Geometric polygon
    *@author Jean-Milost Reymond
    */
    class Polygon
    {
        public:
            DWF_Math::Vector3F m_Vertex[3];

            /**
            * Constructor
            */
            Polygon();

            /**
            * Constructor
            *@param v1 - first vertex of the polygon
            *@param v2 - second vertex of the polygon
            *@param v3 - third vertex of the polygon
            */
            Polygon(const DWF_Math::Vector3F& v1,
                    const DWF_Math::Vector3F& v2,
                    const DWF_Math::Vector3F& v3);

            /**
            * Destructor
            */
            virtual ~Polygon();

            /**
            * Creates and returns a clone of the polygon
            *@return a clone of the polygon
            *@note The returned polygon should be deleted when useless
            */
            virtual Polygon* GetClone() const;

            /**
            * Gets the polygon plane
            *@return the polygon plane
            */
            virtual PlaneF GetPlane() const;

            /**
            * Calculates and returns the center point of the polygon
            *@return the center point of the polygon
            */
            virtual DWF_Math::Vector3F GetCenter() const;

            /**
            * Gets polygon normal
            *@return polygon normal
            *@note This normal is equivalent for each polygon vertices
            */
            virtual DWF_Math::Vector3F GetNormal() const;

            /**
            * Gets lines joining each vertex
            *@param[out] lines - vector to fill with lines
            */
            virtual void GetLines(std::vector<LineF>& lines) const;

            /**
            * Checks if a point is inside polygon
            *@param x - point x coordinate
            *@param y - point y coordinate
            *@param z - point z coordinate
            *@return true if point is inside polygon, otherwise false
            */
            virtual bool Inside(float x, float y, float z) const;

            /**
            * Checks if a point is inside polygon
            *@param point - point coordinate
            *@return true if point is inside polygon, otherwise false
            */
            virtual bool Inside(const DWF_Math::Vector3F& point) const;

            /**
            * Checks if polygon intersects with another polygon
            *@param other - other polygon to check
            *@return true if polygons are in collision, otherwise false
            */
            virtual bool Intersect(const Polygon& other) const;

            /**
            * Checks if polygon intersects with a plane
            *@param plane - plane to check
            *@param[out] intersectionPoints - the intersection points, if polygon intersects plane
            *@return true if polygon intersects plane, otherwise false
            */
            virtual bool Intersect(const PlaneF& plane, std::vector<DWF_Math::Vector3F>& intersectionPoints) const;

            /**
            * Checks if polygon intersects with a line
            *@param line - line
            *@param[out] intersectionPoint - the intersection point, if polygon intersects line
            *@return true if polygon intersects line, otherwise false
            */
            virtual bool Intersect(const LineF& line, DWF_Math::Vector3F& intersectionPoint) const;

            /**
            * Checks if polygon intersects with a ray
            *@param ray - ray
            *@param[out] intersectionPoint - the intersection point, if polygon intersects ray
            *@return true if polygon intersects ray, otherwise false
            */
            virtual bool Intersect(const RayF& ray, DWF_Math::Vector3F& intersectionPoint) const;

            /**
            * Calculates distance to polygon
            *@param point - point from which the distance must be calculated
            *@return distance to polygon
            */
            virtual float DistanceTo(const DWF_Math::Vector3F& point) const;

            /**
            * Calculates and gets the projection of a given point on the polygon
            *@param point - point for which projection must be calculated
            *@return the calculated point
            */
            virtual DWF_Math::Vector3F ClosestPoint(const DWF_Math::Vector3F& point) const;

            /**
            * Applies the given matrix to the polygon
            *@param matrix - matrix to apply
            *@return transformed polygon
            *@note The returned polygon should be deleted when useless
            */
            virtual Polygon* ApplyMatrix(const DWF_Math::Matrix4x4F& matrix) const;
    };

    /**
    * Polygon list
    *@author Jean-Milost Reymond
    */
    typedef std::list<Polygon*> Polygons;
}
