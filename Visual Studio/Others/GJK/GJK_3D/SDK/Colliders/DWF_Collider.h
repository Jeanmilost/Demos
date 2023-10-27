/****************************************************************************
 * ==> DWF_Collider --------------------------------------------------------*
 ****************************************************************************
 * Description : Basic collider                                             *
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
#include "DWF_Vector3.h"
#include "DWF_Matrix4x4.h"

#pragma once

namespace DWF_Collider
{
    /**
    * Basic collider
    *@author Jean-Milost Reymond
    */
    class Collider
    {
        public:
            DWF_Math::Vector3F   m_Pos;      // origin in world space
            DWF_Math::Matrix4x4F m_MatRS;    // rotation/scale model matrix
            DWF_Math::Matrix4x4F m_InvMatRS;

            Collider();
            virtual ~Collider();

            /**
            * Support function for the Minkowski difference
            *@param dir - search direction
            *@return the furthest vertex belonging to the polygon along the direction
            */
            virtual DWF_Math::Vector3F Support(const DWF_Math::Vector3F& dir) const = 0;
    };
}
