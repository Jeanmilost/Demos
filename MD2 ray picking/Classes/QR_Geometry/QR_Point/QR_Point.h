/****************************************************************************
 * ==> QR_Point ------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D point                                                   *
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

#ifndef QR_PointH
#define QR_PointH

// qr engine
#include "QR_Types.h"

/**
* Generic point structure
*@author JMR
*/
template <class T>
class QR_Point
{
    public:
        T m_X;
        T m_Y;

        inline QR_Point();

        /**
        * Constructor
        *@param x - x coordinate
        *@param y - y coordinate
        */
        inline QR_Point(T x, T y);

        virtual inline ~QR_Point();

        /**
        * Addition operator
        *@param value - value to add
        *@returns resulting point
        */
        virtual inline QR_Point operator +(const QR_Point& value) const;
        virtual inline QR_Point operator +(T value) const;

        /**
        * Subtraction operator
        *@param value - value to subtract
        *@returns resulting point
        */
        virtual inline QR_Point operator -(const QR_Point& value) const;
        virtual inline QR_Point operator -(T value) const;

        /**
        * Negation operator
        *@returns inverted point
        */
        virtual inline QR_Point operator -() const;

        /**
        * Multiplication operator
        *@param value - value to multiply
        *@returns resulting point
        */
        virtual inline QR_Point operator *(const QR_Point& value) const;
        virtual inline QR_Point operator *(T value) const;

        /**
        * Division operator
        *@param value - value to divide
        *@returns resulting point
        */
        virtual inline QR_Point operator /(const QR_Point& value) const;
        virtual inline QR_Point operator /(T value) const;

        /**
        * Addition and assignation operator
        *@param value - value to add
        *@returns resulting point
        */
        virtual inline const QR_Point& operator +=(const QR_Point& value);
        virtual inline const QR_Point& operator +=(T value);

        /**
        * Subtraction and assignation operator
        *@param value - value to subtract
        *@returns resulting point
        */
        virtual inline const QR_Point& operator -=(const QR_Point& value);
        virtual inline const QR_Point& operator -=(T value);

        /**
        * Multiplication and assignation operator
        *@param value - value to multiply
        *@returns resulting point
        */
        virtual inline const QR_Point& operator *=(const QR_Point& value);
        virtual inline const QR_Point& operator *=(T value);

        /**
        * Division and assignation operator
        *@param value - value to divide
        *@returns resulting point
        */
        virtual inline const QR_Point& operator /=(const QR_Point& value);
        virtual inline const QR_Point& operator /=(T value);

        /**
        * Equality operator
        *@param value - value to compare
        *@returns true if values are identical, otherwise false
        */
        virtual inline bool operator == (const QR_Point& other) const;

        /**
        * Not equality operator
        *@param value - value to compare
        *@returns true if values are not identical, otherwise false
        */
        virtual inline bool operator != (const QR_Point& other) const;

        /**
        * Check if point is set to [0, 0] coordinate
        *@returns true if point is set to [0, 0] coordinate, otherwise false
        */
        virtual inline bool IsZero() const;
};

typedef QR_Point<int>         QR_PointI;
typedef QR_Point<float>       QR_PointF;
typedef QR_Point<M_Precision> QR_PointP;

//---------------------------------------------------------------------------
// QR_Point
//---------------------------------------------------------------------------
template <class T>
QR_Point<T>::QR_Point() :
    m_X(T(0)),
    m_Y(T(0))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Point<T>::QR_Point(T x, T y) :
    m_X(x),
    m_Y(y)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Point<T>::~QR_Point()
{}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator +(const QR_Point& value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X + value.m_X;
    point.m_Y = m_Y + value.m_Y;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator +(T value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X + value;
    point.m_Y = m_Y + value;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator -(const QR_Point& value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X - value.m_X;
    point.m_Y = m_Y - value.m_Y;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator -(T value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X - value;
    point.m_Y = m_Y - value;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator -() const
{
    return QR_Point<T>(-m_X, -m_Y);
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator *(const QR_Point& value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X * value.m_X;
    point.m_Y = m_Y * value.m_Y;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator *(T value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X * value;
    point.m_Y = m_Y * value;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator /(const QR_Point& value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X / value.m_X;
    point.m_Y = m_Y / value.m_Y;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
QR_Point<T> QR_Point<T>::operator /(T value) const
{
    // calculate resulting point
    QR_Point point;
    point.m_X = m_X / value;
    point.m_Y = m_Y / value;

    return point;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator +=(const QR_Point& value)
{
    m_X += value.m_X;
    m_Y += value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator +=(T value)
{
    m_X += value;
    m_Y += value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator -=(const QR_Point& value)
{
    m_X -= value.m_X;
    m_Y -= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator -=(T value)
{
    m_X -= value;
    m_Y -= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator *=(const QR_Point& value)
{
    m_X *= value.m_X;
    m_Y *= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator *=(T value)
{
    m_X *= value;
    m_Y *= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator /=(const QR_Point& value)
{
    m_X /= value.m_X;
    m_Y /= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Point<T>& QR_Point<T>::operator /=(T value)
{
    m_X /= value;
    m_Y /= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Point<T>::operator == (const QR_Point& other) const
{
    return (m_X == other.m_X && m_Y == other.m_Y);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Point<T>::operator != (const QR_Point& other) const
{
    return (m_X != other.m_X || m_Y != other.m_Y);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Point<T>::IsZero() const
{
    return !m_X && !m_Y;
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
