/****************************************************************************
 * ==> GJK_Algorithm -------------------------------------------------------*
 ****************************************************************************
 * Description : GJK algorithm                                              *
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
#include "GJK_PolygonCollider.h"

/**
* GJK algorithm
*@author Jean-Milost Reymond
*/
class GJK_Algorithm
{
    public:
        GJK_Algorithm();
        virtual ~GJK_Algorithm();

        virtual bool Resolve(const GJK_PolygonCollider& collider1, const GJK_PolygonCollider& collider2);

    private:
        /**
        * Calculates the Minkowski difference between 2 colliders along an arbitrary direction
        *@param collider1 - first collider to use for calculation
        *@param collider2 - second collider to use for calculation
        *@param dir - direction along with the Minkowski difference should be calculated
        *@return the Minkowski difference
        */
        GJK_Vector2F CalculateMinkowskiDiff(const GJK_PolygonCollider& collider1,
                                            const GJK_PolygonCollider& collider2,
                                            const GJK_Vector2F&        dir);

        /**
        * Calculates perpendicular normal vectors which kinda 'prefer' pointing towards the origin in Minkowski space
        *@param a - first vertex
        *@param b - second vertex
        *@param c - third vertex
        *@return the perpendicular normal vectors pointing towards the origin in Minkowski space
        */
        GJK_Vector2F TripleProduct(const GJK_Vector2F& a, const GJK_Vector2F& b, const GJK_Vector2F& c);
};
