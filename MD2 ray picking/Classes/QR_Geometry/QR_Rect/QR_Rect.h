/****************************************************************************
 * ==> QR_Rect -------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D rectangle, can be used e.g. to delimit 2D area, ...     *
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

#ifndef QR_RectH
#define QR_RectH

// qr engine
#include "QR_Types.h"
#include "QR_Point.h"
#include "QR_Size.h"

// windows
#if defined (OS_WIN)
    #include <Windows.h>
#endif

/**
* Generic rectangle, can be used e.g. to delimit 2D area, ...
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Rect
{
    public:
        T m_Left;
        T m_Top;
        T m_Right;
        T m_Bottom;

        inline QR_Rect<T>();

        /**
        * Constructor
        *@param left - left edge
        *@param top - top edge
        *@param right - right edge
        *@param bottom - bottom edge
        */
        inline QR_Rect(T left, T top, T right, T bottom);

        /**
        * Constructor
        *@param min - left top corner position
        *@param max - right bottom corner position
        */
        inline QR_Rect(const QR_Point<T>& min, const QR_Point<T>& max);

        /**
        * Constructor
        *@param x - x position
        *@param y - y position
        *@param size - size
        */
        inline QR_Rect(T x, T y, const QR_Size<T>& size);

        /**
        * Constructor
        *@param pos - position
        *@param size - size
        */
        inline QR_Rect(const QR_Point<T>& pos, const QR_Size<T>& size);

        /**
        * Constructor
        *@param rect - Windows rect
        */
        #if defined(OS_WIN)
            inline QR_Rect(const ::RECT& rect);
        #endif

        virtual inline ~QR_Rect<T>();

        /**
        * Addition operator
        *@param value - value to add
        *@returns resulting rect
        */
        virtual inline QR_Rect operator +(const QR_Rect& value) const;
        virtual inline QR_Rect operator +(T value) const;

        /**
        * Subtraction operator
        *@param value - value to subtract
        *@returns resulting rect
        */
        virtual inline QR_Rect operator -(const QR_Rect& value) const;
        virtual inline QR_Rect operator -(T value) const;

        /**
        * Negation operator
        *@returns inverted rect
        */
        virtual inline QR_Rect operator -() const;

        /**
        * Multiplication operator
        *@param value - value to multiply
        *@returns resulting rect
        */
        virtual inline QR_Rect operator *(const QR_Rect& value) const;
        virtual inline QR_Rect operator *(T value) const;

        /**
        * Division operator
        *@param value - value to divide
        *@returns resulting rect
        */
        virtual inline QR_Rect operator /(const QR_Rect& value) const;
        virtual inline QR_Rect operator /(T value) const;

        /**
        * Addition and assignation operator
        *@param value - value to add
        *@returns resulting rect
        */
        virtual inline const QR_Rect& operator +=(const QR_Rect& value);
        virtual inline const QR_Rect& operator +=(T value);

        /**
        * Subtraction and assignation operator
        *@param value - value to subtract
        *@returns resulting rect
        */
        virtual inline const QR_Rect& operator -=(const QR_Rect& value);
        virtual inline const QR_Rect& operator -=(T value);

        /**
        * Multiplication and assignation operator
        *@param value - value to multiply
        *@returns resulting rect
        */
        virtual inline const QR_Rect& operator *=(const QR_Rect& value);
        virtual inline const QR_Rect& operator *=(T value);

        /**
        * Division and assignation operator
        *@param value - value to divide
        *@returns resulting rect
        */
        virtual inline const QR_Rect& operator /=(const QR_Rect& value);
        virtual inline const QR_Rect& operator /=(T value);

        /**
        * Equality operator
        *@param value - value to compare
        *@returns true if values are identical, otherwise false
        */
        virtual inline bool operator == (const QR_Rect& other) const;

        /**
        * Not equality operator
        *@param value - value to compare
        *@returns true if values are not identical, otherwise false
        */
        virtual inline bool operator != (const QR_Rect& other) const;

        /**
        * Get the top and left point
        *@returns top and left point
        */
        virtual inline QR_Point<T> TopLeft() const;

        /**
        * Get the bottom and right point
        *@returns bottom and right point
        */
        virtual inline QR_Point<T> BottomRight() const;

        /**
        * Get rect width
        *@returns rect width
        */
        virtual inline T Width() const;

        /**
        * Get rect height
        *@returns rect height
        */
        virtual inline T Height() const;

        /**
        * Make sure top > left and bottom > top
        */
        virtual inline void Normalize();

        /**
        * Get center point
        *@returns center point
        */
        virtual inline QR_Point<T> GetCenter() const;

        /**
        * Offset rect
        *@param x - value to move on x axis (value can be negative)
        *@param y - value to move on y axis (value can be negative)
        */
        virtual inline void Offset(T x, T y);

        /**
        * Inflate rect
        *@param x - value to inflate on x axis (deflate if value is negative)
        *@param y - value to inflate on y axis (deflate if value is negative)
        */
        virtual inline void Inflate(T x, T y);

        /**
        * Inflate rect
        *@param left - value to inflate on the left (deflate if value is negative)
        *@param top - value to inflate on the top (deflate if value is negative)
        *@param right - value to inflate on the right (deflate if value is negative)
        *@param bottom - value to inflate on the bottom (deflate if value is negative)
        */
        virtual inline void Inflate(T left, T top, T right, T bottom);

        /**
        * Check if rect is empty
        *@returns true if rect is empty, otherwise false
        */
        virtual inline bool IsEmpty() const;

        /**
        * Check if a point is inside the rect
        *@param point - point to check
        *@param includeRightBottom - if true, a point located on the right or bottom edges will be
        *                            considered as inside the rect (the VCL and the Win API exclude it)
        *@returns true if the point is inside the rect, otherwise false
        */
        virtual inline bool PtInRect(const QR_Point<T>& point, bool includeRightBottom) const;

        /**
        * Check if a rect is inside this rect
        *@param rect - rect to check
        *@param includeRightBottom - if true, the points located on the right or bottom edges will be
        *                            considered as inside the rect (the VCL and the Win API exclude them)
        *@returns true if the rect is inside this rect, otherwise false
        */
        virtual inline bool Inside(const QR_Rect& rect, bool includeRightBottom) const;

        /**
        * Check if a rect intersects this rect
        *@param rect - rect to check
        *@param includeRightBottom - if true, the points located on the right or bottom edges will be
        *                            considered as inside the rect (the VCL and the Win API exclude them)
        *@returns true if the rect is intersects this rect, otherwise false
        */
        virtual inline bool Intersect(const QR_Rect &rect, bool includeRightBottom) const;
};

typedef QR_Rect<int>         QR_RectI;
typedef QR_Rect<float>       QR_RectF;
typedef QR_Rect<M_Precision> QR_RectP;

//---------------------------------------------------------------------------
// QR_Rect
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::QR_Rect() :
    m_Left(T(0)),
    m_Top(T(0)),
    m_Right(T(0)),
    m_Bottom(T(0))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::QR_Rect(T left, T top, T right, T bottom) :
    m_Left(left),
    m_Top(top),
    m_Right(right),
    m_Bottom(bottom)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::QR_Rect(const QR_Point<T>& min, const QR_Point<T>& max) :
    m_Left(min.m_X),
    m_Top(min.m_Y),
    m_Right(max.m_X),
    m_Bottom(max.m_Y)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::QR_Rect(T x, T y, const QR_Size<T>& size) :
    m_Left(x),
    m_Top(y),
    m_Right(x + size.m_Width),
    m_Bottom(y + size.m_Height)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::QR_Rect(const QR_Point<T>& pos, const QR_Size<T>& size) :
    m_Left(pos.m_X),
    m_Top(pos.m_Y),
    m_Right(pos.m_X + size.m_Width),
    m_Bottom(pos.m_Y + size.m_Height)
{}
//---------------------------------------------------------------------------
#if defined(OS_WIN)
    template <class T>
    QR_Rect<T>::QR_Rect(const ::RECT& rect):
        m_Left(rect.left),
        m_Top(rect.top),
        m_Right(rect.right),
        m_Bottom(rect.bottom)
    {}
#endif
//---------------------------------------------------------------------------
template <class T>
QR_Rect<T>::~QR_Rect()
{}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator +(const QR_Rect& value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   + value.m_Left;
    rect.m_Top    = m_Top    + value.m_Top;
    rect.m_Right  = m_Right  + value.m_Right;
    rect.m_Bottom = m_Bottom + value.m_Bottom;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator +(T value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   + value;
    rect.m_Top    = m_Top    + value;
    rect.m_Right  = m_Right  + value;
    rect.m_Bottom = m_Bottom + value;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator -(const QR_Rect& value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   - value.m_Left;
    rect.m_Top    = m_Top    - value.m_Top;
    rect.m_Right  = m_Right  - value.m_Right;
    rect.m_Bottom = m_Bottom - value.m_Bottom;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator -(T value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   - value;
    rect.m_Top    = m_Top    - value;
    rect.m_Right  = m_Right  - value;
    rect.m_Bottom = m_Bottom - value;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator -() const
{
    return QR_Rect<T>(-m_Right, -m_Bottom, -m_Left, -m_Top);
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator *(const QR_Rect& value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   * value.m_Left;
    rect.m_Top    = m_Top    * value.m_Top;
    rect.m_Right  = m_Right  * value.m_Right;
    rect.m_Bottom = m_Bottom * value.m_Bottom;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator *(T value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   * value;
    rect.m_Top    = m_Top    * value;
    rect.m_Right  = m_Right  * value;
    rect.m_Bottom = m_Bottom * value;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator /(const QR_Rect& value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   / value.m_Left;
    rect.m_Top    = m_Top    / value.m_Top;
    rect.m_Right  = m_Right  / value.m_Right;
    rect.m_Bottom = m_Bottom / value.m_Bottom;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
QR_Rect<T> QR_Rect<T>::operator /(T value) const
{
    // calculate resulting rect
    QR_Rect rect;
    rect.m_Left   = m_Left   / value;
    rect.m_Top    = m_Top    / value;
    rect.m_Right  = m_Right  / value;
    rect.m_Bottom = m_Bottom / value;

    return rect;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator +=(const QR_Rect& value)
{
    m_Left   += value.m_Left;
    m_Top    += value.m_Top;
    m_Right  += value.m_Right;
    m_Bottom += value.m_Bottom;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator +=(T value)
{
    m_Left   += value;
    m_Top    += value;
    m_Right  += value;
    m_Bottom += value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator -=(const QR_Rect& value)
{
    m_Left   -= value.m_Left;
    m_Top    -= value.m_Top;
    m_Right  -= value.m_Right;
    m_Bottom -= value.m_Bottom;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator -=(T value)
{
    m_Left   -= value;
    m_Top    -= value;
    m_Right  -= value;
    m_Bottom -= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator *=(const QR_Rect& value)
{
    m_Left   *= value.m_Left;
    m_Top    *= value.m_Top;
    m_Right  *= value.m_Right;
    m_Bottom *= value.m_Bottom;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator *=(T value)
{
    m_Left   *= value;
    m_Top    *= value;
    m_Right  *= value;
    m_Bottom *= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator /=(const QR_Rect& value)
{
    m_Left   /= value.m_Left;
    m_Top    /= value.m_Top;
    m_Right  /= value.m_Right;
    m_Bottom /= value.m_Bottom;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Rect<T>& QR_Rect<T>::operator /=(T value)
{
    m_Left   /= value;
    m_Top    /= value;
    m_Right  /= value;
    m_Bottom /= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::operator == (const QR_Rect& other) const
{
    return ((m_Left   == other.m_Left)  &&
            (m_Top    == other.m_Top)   &&
            (m_Right  == other.m_Right) &&
            (m_Bottom == other.m_Bottom));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::operator != (const QR_Rect& other) const
{
    return ((m_Left   != other.m_Left)  ||
            (m_Top    != other.m_Top)   ||
            (m_Right  != other.m_Right) ||
            (m_Bottom != other.m_Bottom));
}
//---------------------------------------------------------------------------
template <class T>
QR_Point<T> QR_Rect<T>::TopLeft() const
{
    return QR_Point<T>(m_Left, m_Top);
}
//---------------------------------------------------------------------------
template <class T>
QR_Point<T> QR_Rect<T>::BottomRight() const
{
    return QR_Point<T>(m_Right, m_Bottom);
}
//---------------------------------------------------------------------------
template <class T>
T QR_Rect<T>::Width() const
{
    return (m_Right - m_Left);
}
//---------------------------------------------------------------------------
template <class T>
T QR_Rect<T>::Height() const
{
    return (m_Bottom - m_Top);
}
//---------------------------------------------------------------------------
template <class T>
inline void QR_Rect<T>::Normalize()
{
    if (m_Left > m_Right)
        std::swap(m_Left, m_Right);

    if (m_Top > m_Bottom)
        std::swap(m_Top, m_Bottom);
}
//---------------------------------------------------------------------------
template <class T>
QR_Point<T> QR_Rect<T>::GetCenter()const
{
    return QR_Point<T>(((m_Left + m_Right) / T(2)), ((m_Top + m_Bottom) / T(2)));
}
//---------------------------------------------------------------------------
template <class T>
void QR_Rect<T>::Offset(T x, T y)
{
    m_Left   += x;
    m_Top    += y;
    m_Right  += x;
    m_Bottom += y;
}
//---------------------------------------------------------------------------
template <class T>
void QR_Rect<T>::Inflate(T x, T y)
{
    m_Left   -= x;
    m_Top    -= y;
    m_Right  += x;
    m_Bottom += y;
}
//---------------------------------------------------------------------------
template <class T>
void QR_Rect<T>::Inflate(T left, T top, T right, T bottom)
{
    m_Left   -= left;
    m_Top    -= top;
    m_Right  += right;
    m_Bottom += bottom;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::IsEmpty() const
{
    return ((m_Right == m_Left) || (m_Bottom == m_Top));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::PtInRect(const QR_Point<T>& point, bool includeRightBottom) const
{
    if (includeRightBottom)
        return (point.m_X >= m_Left  &&
                point.m_Y >= m_Top   &&
                point.m_X <= m_Right &&
                point.m_Y <= m_Bottom);
    else
        return (point.m_X >= m_Left  &&
                point.m_Y >= m_Top   &&
                point.m_X <  m_Right &&
                point.m_Y <  m_Bottom);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::Inside(const QR_Rect& rect, bool includeRightBottom) const
{
    return (PtInRect(rect.TopLeft(),     includeRightBottom) &&
            PtInRect(rect.BottomRight(), includeRightBottom));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Rect<T>::Intersect(const QR_Rect& rect, bool includeRightBottom) const
{
    if (includeRightBottom)
        return !(m_Left   > rect.m_Right  ||
                 m_Right  < rect.m_Left   ||
                 m_Top    > rect.m_Bottom ||
                 m_Bottom < rect.m_Top);
    else
        return !(rect.m_Right  < m_Left      ||
                 m_Right       < rect.m_Left ||
                 rect.m_Bottom < m_Top       ||
                 m_Bottom      < rect.m_Top);
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
