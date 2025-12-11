/****************************************************************************
 * ==> Line3 ---------------------------------------------------------------*
 ****************************************************************************
 * Description: Line represented by a start and an end point                *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
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
#include <math.h>

// classes
#include "MathHelper.h"
#include "Vector3.h"

/**
* Line
*@author Jean-Milost Reymond
*/
template <class T>
class Line3
{
    public:
        Vector3<T> m_Start;
        Vector3<T> m_End;

        /**
        * Constructor
        */
        inline Line3();

        /**
        * Constructor
        *@param start - line start position
        *@param end - line end position
        */
        inline Line3(const Vector3<T>& start, const Vector3<T>& end);

        /**
        * Copy constructor
        *@param other - other matrix to copy from
        */
        inline Line3(const Line3& other);

        /**
        * Destructor
        */
        virtual inline ~Line3();

        /**
        * Gets the closest point on this line
        *@param point - point from which the closest point should be found
        *@return the closest point on this line
        */
        virtual inline Vector3<T> ClosestPoint(const Vector3<T>& point) const;

        /**
        * Calculates distance to line
        *@param point - point from which the distance must be calculated
        *@return distance to line
        */
        virtual inline T DistanceTo(const Vector3<T>& point) const;

        /**
        * Get shortest distance between 2 lines
        *@param line - line to which distance must be calculated
        *@return shortest distance between lines
        */
        virtual inline T GetShortestDistance(const Line3& line) const;
};

typedef Line3<float>  Line3F;
typedef Line3<double> Line3D;

//---------------------------------------------------------------------------
// Line3
//---------------------------------------------------------------------------
template <class T>
Line3<T>::Line3()
{}
//---------------------------------------------------------------------------
template <class T>
Line3<T>::Line3(const Vector3<T>& start, const Vector3<T>& end) :
    m_Start(start),
    m_End(end)
{}
//---------------------------------------------------------------------------
template <class T>
Line3<T>::Line3(const Line3& other) :
    m_Start(other.m_Start),
    m_End(other.m_End)
{}
//---------------------------------------------------------------------------
template <class T>
Line3<T>::~Line3()
{}
//---------------------------------------------------------------------------
template <class T>
Vector3<T> Line3<T>::ClosestPoint(const Vector3<T>& point) const
{
    // calculate line delta
    const Vector3<T> delta = m_End - m_Start;

    // calculate projected point on line
    const T t = (point - m_Start).Dot(delta) / delta.Dot(delta);

    // calculate closest point on line
    return m_Start + (delta * MathHelper::Clamp01(t));
}
//---------------------------------------------------------------------------
template <class T>
T Line3<T>::DistanceTo(const Vector3<T>& point) const
{
    // get the closest point on line
    const Vector3<T> closestPointOnLine = ClosestPoint(point);

    // compute distance between point and closest point on line
    const Vector3<T> d = point - closestPointOnLine;

    // calculate distance
    return sqrt(d.m_X * d.m_X + d.m_Y * d.m_Y + d.m_Z * d.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
T Line3<T>::GetShortestDistance(const Line3<T>& line) const
{
    // calculate p2 - p1, p4 - p3, and p1 - p3 line segments
    const Vector3<T> delta21 = m_End      - m_Start;
    const Vector3<T> delta43 = line.m_End - line.m_Start;
    const Vector3<T> delta13 = m_Start    - line.m_Start;

    // calculate some needed values - a, c and D are always >= 0
    const T a = delta21.Dot(delta21);
    const T b = delta21.Dot(delta43);
    const T c = delta43.Dot(delta43);
    const T d = delta21.Dot(delta13);
    const T e = delta43.Dot(delta13);
    const T D = a * c - b * b;

    // sc = sN / sD, default sD = D >= 0
    T sc, sN, sD = D;

    // tc = tN / tD, default tD = D >= 0
    T tc, tN, tD = D;

    // compute the line parameters of the two closest points
    if (D < M_Epsilon)
    {
        // the lines are almost parallel, force using point P0 on segment S1
        // to prevent possible division by 0 later
        sN = T(0.0);
        sD = T(1.0);
        tN = e;
        tD = c;
    }
    else
    {
        // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);

        // sc < 0 => the s=0 edge is visible
        if (sN < T(0.0))
        {
            sN = T(0.0);
            tN = e;
            tD = c;
        }
        else
        // sc > 1 => the s=1 edge is visible
        if (sN > sD)
        {
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    // tc < 0 => the t=0 edge is visible
    if (tN < T(0.0))
    {
        tN = T(0.0);

        // recompute sc for this edge
        if (-d < T(0.0))
            sN = T(0.0);
        else
        if (-d > a)
            sN = sD;
        else
        {
            sN = -d;
            sD =  a;
        }
    }
    else
    // tc > 1 => the t=1 edge is visible
    if (tN > tD)
    {
        tN = tD;

        // recompute sc for this edge
        if ((-d + b) < T(0.0))
            sN = T(0.0);
        else
        if ((-d + b) > a)
            sN = sD;
        else
        {
            sN = (-d + b);
            sD = a;
        }
    }

    // finally do the division to get sc and tc
    #ifdef _MSC_VER
        if (fabs(sN) < M_Epsilon)
    #else
        if (std::fabs(sN) < M_Epsilon)
    #endif
        sc = T(0.0);
    else
        sc = sN / sD;

    #ifdef _MSC_VER
        if (fabs(tN) < M_Epsilon)
    #else
        if (std::fabs(tN) < M_Epsilon)
    #endif
        tc = T(0.0);
    else
        tc = tN / tD;

    Vector3<T> dP;

    // get the difference of the two closest points
    dP.m_X = delta13.m_X + (sc * delta21.m_X) - (tc * delta43.m_X);
    dP.m_Y = delta13.m_Y + (sc * delta21.m_Y) - (tc * delta43.m_Y);
    dP.m_Z = delta13.m_Z + (sc * delta21.m_Z) - (tc * delta43.m_Z);

    // return the closest distance
    return sqrt(dP.Dot(dP));
}
//---------------------------------------------------------------------------
