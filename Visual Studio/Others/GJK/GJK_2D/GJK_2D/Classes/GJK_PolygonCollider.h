/****************************************************************************
 * ==> GJK_PolygonCollider -------------------------------------------------*
 ****************************************************************************
 * Description : Polygon collider                                           *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - GJK 2D                                                     *
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
#include "GJK_Collider.h"

/**
* Polygon collider
*@author Jean-Milost Reymond
*/
class GJK_PolygonCollider : public GJK_Collider
{
    public:
        IVertices m_Vertices;

        GJK_PolygonCollider();
        virtual ~GJK_PolygonCollider();

        /**
        * Computes the average center point (roughly)
        *@param transform - if true, transform will be applied
        *@return the average center point
        *@note Average center might be different from the Center of Gravity, especially for bodies with nonuniform density,
        *      but this is ok as initial direction of simplex search in GJK
        */
        GJK_Vector2F AveragePoint(bool transform) const;

        /**
        * Support function for the Minkowski difference, gets the furthest vertex belonging to the polygon
        * along a direction. This index will then used to perform the Minkowski difference
        *@param dir - simplex direction
        */
        virtual GJK_Vector2F Support(const GJK_Vector2F& dir) const;
};
