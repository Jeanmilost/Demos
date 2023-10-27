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

#pragma once

 // classes
#include "DWF_Vector3.h"
#include "DWF_Plane.h"
#include "DWF_Line.h"
#include "DWF_Ray.h"

namespace DWF_Geometry
{
    /**
    * Geometric capsule
    *@author Jean-Milost Reymond
    */
    class Capsule
    {
        public:
            DWF_Math::Vector3F m_Top;
            DWF_Math::Vector3F m_Bottom;
            float              m_Radius = 0.0f;

            Capsule();
            virtual ~Capsule();

            /**
            * Checks if a point is inside the capsule
            *@param point - point coordinate
            *@return true if point is inside the capsule, otherwise false
            */
            virtual bool Inside(const DWF_Math::Vector3F& point) const;

            /**
            * Checks if capsule intersects with another capsule
            *@param other - other capsule to check
            *@return true if capsules are in collision, otherwise false
            */
            virtual bool Intersect(const Capsule& other) const;

            /**
            * Checks if capsule intersects with a plane
            *@param plane - plane to check
            *@return true if capsule intersects plane, otherwise false
            */
            virtual bool Intersect(const PlaneF& plane) const;

            /**
            * Checks if capsule intersects with a line
            *@param line - line
            *@return true if capsule intersects line, otherwise false
            */
            virtual bool Intersect(const LineF& line) const;

            /**
            * Checks if capsule intersects with a ray
            *@param ray - ray
            *@return true if capsule intersects ray, otherwise false
            */
            virtual bool Intersect(const RayF& ray) const;
    };
}
