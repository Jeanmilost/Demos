/****************************************************************************
 * ==> DWF_SphereCollider --------------------------------------------------*
 ****************************************************************************
 * Description : Sphere collider                                            *
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

// classes
#include "DWF_Collider.h"

#pragma once

namespace DWF_Collider
{
    /**
    * Sphere collider
    *@author Jean-Milost Reymond
    */
    class Sphere_Collider : public Collider
    {
        public:
            float m_Radius = 0.0f;

            Sphere_Collider();
            virtual ~Sphere_Collider();

            /**
            * Support function for the Minkowski difference
            *@param dir - search direction
            *@return the furthest vertex belonging to the polygon along the direction
            */
            virtual DWF_Math::Vector3F Support(const DWF_Math::Vector3F& dir) const;
    };
}
