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

#pragma once

// std
#include <vector>

// classes
#include "DWF_Vector3.h"
#include "DWF_Plane.h"

namespace DWF_Geometry
{
    /**
    * Geometric aligned-Axis Bounding Box
    *@author Jean-Milost Reymond
    */
    class AABB
    {
        public:
            /**
            * Longest axis enumeration
            */
            enum class IEAxis
            {
                IE_A_X,
                IE_A_Y,
                IE_A_Z,
            };

            DWF_Math::Vector3F m_Min;
            DWF_Math::Vector3F m_Max;

            AABB();

            /**
            * Constructor
            *@param min - min corner
            *@param max - max corner
            */
            AABB(const DWF_Math::Vector3F& min, const DWF_Math::Vector3F& max);

            virtual ~AABB();

            /**
            * Gets center
            *@return center
            */
            virtual DWF_Math::Vector3F GetCenter() const;

            /**
            * Gets lines joining each vertex
            *@param[out] lines - vector to fill with lines
            */
            virtual void GetLines(std::vector<LineF>& lines) const;

            /**
            * Gets box faces
            *@param[out] faces - box face vertices, each face is composed by a group of 4 vertices
            */
            virtual void GetFaces(std::vector<DWF_Math::Vector3F>& faces) const;

            /**
            * Gets planes composing each faces
            *@param[out] planes - vector to fill with planes
            *@note the planes are in the same order as the faces
            */
            virtual void GetPlanes(std::vector<PlaneF>& planes) const;

            /**
            * Gets longest axis
            *@return longest axis, @see IE_Axis enumeration
            */
            virtual IEAxis GetLongestAxis() const;

            /**
            * Gets splitting plane
            *@param axis - axis for which splitting plane should be get
            *@return splitting plane
            */
            virtual PlaneF GetSplittingPlane(IEAxis axis) const;

            /**
            * Gets splitting plane
            *@param axis - axis for which splitting plane should be get
            *@param center - box center
            *@return splitting plane
            */
            virtual PlaneF GetSplittingPlane(IEAxis axis, const DWF_Math::Vector3F& center) const;

            /**
            * Cuts the box into 2 sub-boxes
            *@param[out] leftBox - left cut sub-box
            *@param[out] rightBox - right cut sub-box
            */
            virtual void Cut(AABB& leftBox, AABB& rightBox);

            /**
            * Checks if a point is inside the aabb
            *@param point - point coordinate
            *@return true if point is inside the aabb, otherwise false
            */
            virtual bool Inside(const DWF_Math::Vector3F& point) const;

            /**
            * Checks if aabb intersects with another aabb
            *@param other - other aabb to check
            *@return true if aabbs are in collision, otherwise false
            */
            virtual bool Intersect(const AABB& other) const;

            /**
            * Checks if aabb intersects with a plane
            *@param plane - plane to check
            *@return true if aabb intersects plane, otherwise false
            */
            virtual bool Intersect(const PlaneF& plane) const;

            /**
            * Checks if aabb intersects with a line
            *@param line - line
            *@return true if aabb intersects line, otherwise false
            */
            virtual bool Intersect(const LineF& line) const;

            /**
            * Checks if aabb intersects with a ray
            *@param ray - ray
            *@return true if aabb intersects ray, otherwise false
            */
            virtual bool Intersect(const RayF& ray) const;

        private:
            /**
            * Gets the intersection point between the box and 2 points composing a line
            *@param fDst1 - distance between the first point and the box on a given axis
            *@param fDst2 - distance between the second point and the box on a given axis
            *@param p1 - line start point
            *@param p2 - line end point
            *@param[out] intersectionPoint - resulting intersection point, if an intersection was found
            *@return true if an intersection was found, otherwise false
            */
            bool inline GetIntersection(float               fDst1,
                                        float               fDst2,
                                  const DWF_Math::Vector3F& p1,
                                  const DWF_Math::Vector3F& p2,
                                        DWF_Math::Vector3F& intersectionPoint) const;

            /**
            * Checks if a point is inside bounding box limits on a given axis
            *@param point - point to test
            *@param b1 - first bounding point to test
            *@param b2 - second bounding point to test
            *@param axis - axis
            */
            bool inline InBox(const DWF_Math::Vector3F& point,
                              const DWF_Math::Vector3F& b1,
                              const DWF_Math::Vector3F& b2,
                                    IEAxis              axis) const;
    };

    //---------------------------------------------------------------------------
    // AABB
    //---------------------------------------------------------------------------
    bool inline AABB::GetIntersection(float               fDst1,
                                      float               fDst2,
                                const DWF_Math::Vector3F& p1,
                                const DWF_Math::Vector3F& p2,
                                      DWF_Math::Vector3F& intersectionPoint) const
    {
        if ((fDst1 * fDst2) >= 0.0f)
            return false;

        if (fDst1 == fDst2)
            return false;

        intersectionPoint = p1 + (p2 - p1) * (-fDst1 / (fDst2 - fDst1));
        return true;
    }
    //---------------------------------------------------------------------------
    bool inline AABB::InBox(const DWF_Math::Vector3F& point,
                            const DWF_Math::Vector3F& b1,
                            const DWF_Math::Vector3F& b2,
                                  IEAxis              axis) const
    {
        switch (axis)
        {
            case IEAxis::IE_A_X: return (point.m_Z > b1.m_Z && point.m_Z < b2.m_Z && point.m_Y > b1.m_Y && point.m_Y < b2.m_Y);
            case IEAxis::IE_A_Y: return (point.m_Z > b1.m_Z && point.m_Z < b2.m_Z && point.m_X > b1.m_X && point.m_X < b2.m_X);
            case IEAxis::IE_A_Z: return (point.m_X > b1.m_X && point.m_X < b2.m_X && point.m_Y > b1.m_Y && point.m_Y < b2.m_Y);
            default:             return false;
        }
    }
    //---------------------------------------------------------------------------
}
