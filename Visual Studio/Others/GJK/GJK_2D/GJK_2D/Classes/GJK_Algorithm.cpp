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

#include "GJK_Algorithm.h"

//------------------------------------------------------------------------------
// GJK_Algorithm
//------------------------------------------------------------------------------
GJK_Algorithm::GJK_Algorithm()
{}
//------------------------------------------------------------------------------
GJK_Algorithm::~GJK_Algorithm()
{}
//------------------------------------------------------------------------------
bool GJK_Algorithm::Resolve(const GJK_PolygonCollider& collider1, const GJK_PolygonCollider& collider2)
{
    // not a Center of Gravity but it's ok for GJK
    const GJK_Vector2F position1 = collider1.AveragePoint(true);
    const GJK_Vector2F position2 = collider2.AveragePoint(true);

    // initial direction from the center of 1st body to the center of 2nd body
    GJK_Vector2F dir = (position1 - position2);

    // if initial direction is zero – set it to any arbitrary axis (we choose X)
    if ((dir.m_X == 0.0f) && (dir.m_Y == 0.0f))
        dir.m_X = 1.0f;

    GJK_Vector2F simplex[3];

    // set the first support as initial point of the new simplex
    GJK_Vector2F a = simplex[0] = CalculateMinkowskiDiff(collider1, collider2, dir);

    // no collision?
    if (a.Dot(dir) <= 0)
        return false;

    // the next search direction is always towards the origin, so the next search direction is -a
    dir = -a;

    // index of current vertex of simplex
    std::size_t index = 0;

    while (1)
    {
        ++index;

        // calculate the next support
        a              = CalculateMinkowskiDiff(collider1, collider2, dir);
        simplex[index] = a;

        // no collision?
        if (a.Dot(dir) <= 0)
            return false;

        // from point A to Origin is just negative A
        const GJK_Vector2F ao = -a;

        GJK_Vector2F b;
        GJK_Vector2F ab;

        // simplex has 2 points (a line segment, not a triangle yet)
        if (index < 2)
        {
            b = simplex[0];

            // from point A to B
            ab  = b - a;

            // normal to AB towards Origin
            dir = TripleProduct(ab, ao, ab);

            if (dir.SquaredLength() == 0.0f)
                dir = ab.Perpendicular();

            // skip to next iteration
            continue;
        }

                           b = simplex[1];
        const GJK_Vector2F c = simplex[0];

        // from point A to B
        ab = b - a;

        // from point A to C
        const GJK_Vector2F ac     = c - a;
        const GJK_Vector2F acperp = TripleProduct(ab, ac, ac);

        // new direction is normal to AC towards Origin
        if (acperp.Dot(ao) >= 0.0f)
            dir = acperp;
        else
        {
            const GJK_Vector2F abperp = TripleProduct(ac, ab, ab);

            // collision?
            if (abperp.Dot(ao) < 0.0f)
                return true;

            // swap first element (point C)
            simplex[0] = simplex[1];

            // new direction is normal to AB towards Origin
            dir = abperp;
        }

        // swap element in the middle (point B)
        simplex[1] = simplex[2];

        --index;
    }

    return false;
}
//------------------------------------------------------------------------------
GJK_Vector2F GJK_Algorithm::CalculateMinkowskiDiff(const GJK_PolygonCollider& collider1,
                                                   const GJK_PolygonCollider& collider2,
                                                   const GJK_Vector2F&        dir)
{
    // get first body furthest point along an arbitrary direction
    const GJK_Vector2F furthestPoint1 = collider1.Support(dir);

    // get second body furthest point along an arbitrary direction
    const GJK_Vector2F furthestPoint2 = collider2.Support(-dir);

    // subtract (Minkowski sum) the two points to see if bodies 'overlap'
    return furthestPoint1 - furthestPoint2;
}
//------------------------------------------------------------------------------
GJK_Vector2F GJK_Algorithm::TripleProduct(const GJK_Vector2F& a, const GJK_Vector2F& b, const GJK_Vector2F& c)
{
    GJK_Vector2F result;

    const float ac = a.Dot(c);
    const float bc = b.Dot(c);

    // perform b * a.dot(c) - a * b.dot(c)
    result.m_X = (b.m_X * ac) - (a.m_X * bc);
    result.m_Y = (b.m_Y * ac) - (a.m_Y * bc);

    return result;
}
//------------------------------------------------------------------------------
