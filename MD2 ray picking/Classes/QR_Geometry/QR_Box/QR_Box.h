/****************************************************************************
 * ==> QR_Box --------------------------------------------------------------*
 ****************************************************************************
 * Description : 3D box                                                     *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - QR Engine                                                  *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sublicense, and/or sell copies of the Software, and to       *
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

#ifndef QR_BoxH
#define QR_BoxH

// std
#include <cmath>

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"
#include "QR_Plane.h"

/**
* Box
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Box
{
    public:
        /**
        * Longest axis enumeration
        */
        enum IE_Axis
        {
            IE_X,
            IE_Y,
            IE_Z,
        };

        QR_Vector3D<T> m_Min;
        QR_Vector3D<T> m_Max;

        /**
        * Constructor
        */
        QR_Box();

        /**
        * Constructor
        *@param min - min corner
        *@param max - max corner
        */
        QR_Box(const QR_Vector3D<T>& min, const QR_Vector3D<T>& max);

        /**
        * Destructor
        */
        virtual ~QR_Box();

        /**
        * Gets center
        *@return center
        */
        virtual QR_Vector3D<T> GetCenter() const;

        /**
        * Gets longest axis
        *@return longest axis, @see IE_Axis enumeration
        */
        virtual IE_Axis GetLongestAxis() const;

        /**
        * Gets splitting plane
        *@param axis - axis for which splitting plane should be get
        *@return splitting plane
        */
        virtual QR_Plane<T> GetSplittingPlane(IE_Axis axis) const;

        /**
        * Gets splitting plane
        *@param axis - axis for which splitting plane should be get
        *@param center - box center
        *@return splitting plane
        */
        virtual QR_Plane<T> GetSplittingPlane(      IE_Axis         axis,
                                              const QR_Vector3D<T>& center) const;

        /**
        * Cuts the box into 2 sub-boxes
        *@param[out] leftBox - left cut sub-box
        *@param[out] rightBox - right cut sub-box
        */
        virtual void Cut(QR_Box<T>& leftBox, QR_Box<T>& rightBox);

        /**
        * Check if a point is inside the box
        *@param x - point x coordinate
        *@param y - point y coordinate
        *@param z - point z coordinate
        *@return true if point is inside the box, otherwise false
        */
        virtual bool Inside(T x, T y, T z) const;

        /**
        * Check if a point is inside the box
        *@param point - point coordinate
        *@return true if point is inside the box, otherwise false
        */
        virtual bool Inside(const QR_Vector3D<T>& point) const;

        /**
        * Check if box intersects with another box
        *@param other - other box to check
        *@return true if boxes intersect, otherwise false
        */
        virtual bool Intersect(const QR_Box& other) const;
};

typedef QR_Box<QR_Float>    QR_BoxF;
typedef QR_Box<QR_Double>   QR_BoxD;
typedef QR_Box<M_Precision> QR_BoxP;

//---------------------------------------------------------------------------
// QR_Box
//---------------------------------------------------------------------------
template <class T>
QR_Box<T>::QR_Box()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Box<T>::QR_Box(const QR_Vector3D<T>& min, const QR_Vector3D<T>& max) :
    m_Min(min),
    m_Max(max)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Box<T>::~QR_Box()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Box<T>::GetCenter() const
{
    return QR_Vector3DP((m_Min.m_X + m_Max.m_X) / 2.0f,
                        (m_Min.m_Y + m_Max.m_Y) / 2.0f,
                        (m_Min.m_Z + m_Max.m_Z) / 2.0f);
}
//---------------------------------------------------------------------------
template <class T>
QR_Box<T>::IE_Axis QR_Box<T>::GetLongestAxis() const
{
    // calculate each edge length
    T x = std::fabs(m_Max.m_X - m_Min.m_X);
    T y = std::fabs(m_Max.m_Y - m_Min.m_Y);
    T z = std::fabs(m_Max.m_Z - m_Min.m_Z);

    // search for longest axis
    if (x >= y && x >= z)
        return IE_X;
    else
    if (y >= x && y >= z)
        return IE_Y;
    else
        return IE_Z;
}
//---------------------------------------------------------------------------
template <class T>
QR_Plane<T> QR_Box<T>::GetSplittingPlane(IE_Axis axis) const
{
    switch (axis)
    {
        case IE_X: return QR_Plane<T>(1.0f, 0.0f, 0.0f, -GetCenter().m_X);
        case IE_Y: return QR_Plane<T>(0.0f, 1.0f, 0.0f, -GetCenter().m_Y);
        case IE_Z: return QR_Plane<T>(0.0f, 0.0f, 1.0f, -GetCenter().m_Z);
        default:   return QR_Plane<T>();
    }
}
//---------------------------------------------------------------------------
template <class T>
QR_Plane<T> QR_Box<T>::GetSplittingPlane(IE_Axis axis, const QR_Vector3D<T>& center) const
{
    switch (axis)
    {
        case IE_X: return QR_Plane<T>(1.0f, 0.0f, 0.0f, -center.m_X);
        case IE_Y: return QR_Plane<T>(0.0f, 1.0f, 0.0f, -center.m_Y);
        case IE_Z: return QR_Plane<T>(0.0f, 0.0f, 1.0f, -center.m_Z);
        default:   return QR_Plane<T>();
    }
}
//-----------------------------------------------------------------------------------------
template <class T>
void QR_Box<T>::Cut(QR_Box<T>& leftBox, QR_Box<T>& rightBox)
{
    // calculate each edge length
    const T x = std::fabs(m_Max.m_X - m_Min.m_X);
    const T y = std::fabs(m_Max.m_Y - m_Min.m_Y);
    const T z = std::fabs(m_Max.m_Z - m_Min.m_Z);

    QR_SizeT longestAxis;

    // search for longest axis
    if ((x >= y) && (x >= z))
        longestAxis = 0;
    else
    if ((y >= x) && (y >= z))
        longestAxis = 1;
    else
        longestAxis = 2;

    // cut box
    switch (longestAxis)
    {
        // cut on x axis
        case 0:
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_X = m_Min.m_X + (x * T(0.5));

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_X = leftBox.m_Max.m_X;

            return;

        // cut on y axis
        case 1:
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_Y = m_Min.m_Y + (y * T(0.5));

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_Y = leftBox.m_Max.m_Y;

            return;

        // cut on z axis
        case 2:
            leftBox.m_Min     = m_Min;
            leftBox.m_Max     = m_Max;
            leftBox.m_Max.m_Z = m_Min.m_Z + (z * T(0.5));

            rightBox.m_Min     = m_Min;
            rightBox.m_Max     = m_Max;
            rightBox.m_Min.m_Z = leftBox.m_Max.m_Z;

            return;
    }
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Box<T>::Inside(T x, T y, T z) const
{
    return Inside(QR_Vector3DP(x, y, z));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Box<T>::Inside(const QR_Vector3D<T>& point) const
{
    return (point.m_X >= m_Min.m_X &&
            point.m_Y >= m_Min.m_Y &&
            point.m_Z >= m_Min.m_Z &&
            point.m_X <= m_Max.m_X &&
            point.m_Y <= m_Max.m_Y &&
            point.m_Z <= m_Max.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Box<T>::Intersect(const QR_Box& other) const
{
    return !(m_Min.m_X > other.m_Max.m_X || m_Max.m_X < other.m_Min.m_X ||
             m_Min.m_Y > other.m_Max.m_Y || m_Max.m_Y < other.m_Min.m_Y ||
             m_Min.m_Z > other.m_Max.m_Z || m_Max.m_Z < other.m_Min.m_Z);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// RAD studio
//---------------------------------------------------------------------------
#if defined(CP_EMBARCADERO)
    // needed to avoid the W8058 error "Cannot create pre-compiled header: header incomplete"
    // warning in BCC compilers
    ;
#endif
//---------------------------------------------------------------------------

#endif
