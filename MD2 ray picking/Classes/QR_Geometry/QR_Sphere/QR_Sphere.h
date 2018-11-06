/****************************************************************************
 * ==> QR_Sphere -----------------------------------------------------------*
 ****************************************************************************
 * Description : 3D sphere                                                  *
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

#ifndef QR_SphereH
#define QR_SphereH

// std
#include <cmath>

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"

/**
* Sphere
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Sphere
{
    public:
        QR_Vector3D<T> m_Center;
        T              m_Radius;

        /**
        * Constructor
        */
        QR_Sphere();

        /**
        * Constructor
        *@param center - center
        *@param radius - radius
        */
        QR_Sphere(const QR_Vector3D<T>& center, T radius);

        /**
        * Destructor
        */
        virtual ~QR_Sphere();

        /**
        * Check if a point is inside rectangle
        *@param x - point x coordinate
        *@param y - point y coordinate
        *@return true if point is inside rectangle, otherwise false
        */
        virtual bool Inside(T x, T y, T z) const;

        /**
        * Check if a point is inside the sphere
        *@param point - point coordinate
        *@return true if point is inside the sphere, otherwise false
        */
        virtual bool Inside(const QR_Vector3D<T>& point) const;

        /**
        * Check if sphere intersects with another sphere
        *@param other - other sphere to check
        *@return true if spheres intersect, otherwise false
        */
        virtual bool Intersect(const QR_Sphere& other) const;
};

typedef QR_Sphere<QR_Float>    QR_SphereF;
typedef QR_Sphere<QR_Double>   QR_SphereD;
typedef QR_Sphere<M_Precision> QR_SphereP;

//---------------------------------------------------------------------------
// QR_Sphere
//---------------------------------------------------------------------------
template <class T>
QR_Sphere<T>::QR_Sphere() :
    m_Radius(T(0.0))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Sphere<T>::QR_Sphere(const QR_Vector3D<T>& center, T radius)
{
    m_Center = center;
    m_Radius = radius;
}
//---------------------------------------------------------------------------
template <class T>
QR_Sphere<T>::~QR_Sphere()
{}
//---------------------------------------------------------------------------
template <class T>
bool QR_Sphere<T>::Inside(T x, T y, T z) const
{
    return Inside(QR_Vector3D<T>(x, y, z));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Sphere<T>::Inside(const QR_Vector3D<T>& point) const
{
    // calculate the distance between test point and the center of the sphere
    const QR_Vector3D<T> length   = point - m_Center;
    const T              distance = length.Length();

    // check if distance is shorter than the sphere radius and return result
    return (distance <= m_Radius);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Sphere<T>::Intersect(const QR_Sphere& other) const
{
    const QR_Vector3D<T> dist(std::fabs(m_Center.m_X - other.m_Center.m_X),
                              std::fabs(m_Center.m_Y - other.m_Center.m_Y),
                              std::fabs(m_Center.m_Z - other.m_Center.m_Z));

    const T length = dist.Length();

    return (length <= (m_Radius + other.m_Radius));
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
