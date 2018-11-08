/****************************************************************************
 * ==> QR_Size -------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D size (i.e. width and height)                            *
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

#ifndef QR_SizeH
#define QR_SizeH

// qr engine
#include "QR_Types.h"

/**
* Generic size structure
*@author JMR
*/
template <class T>
class QR_Size
{
    public:
        T m_Width;
        T m_Height;

        inline QR_Size();

        /**
        * Constructor
        *@param width - width
        *@param height - height
        */
        inline QR_Size(T width, T height);

        virtual inline ~QR_Size();

        /**
        * Addition operator
        *@param value - value to add
        *@returns resulting size
        */
        virtual inline QR_Size operator +(const QR_Size& value) const;
        virtual inline QR_Size operator +(T value) const;

        /**
        * Subtraction operator
        *@param value - value to subtract
        *@returns resulting size
        */
        virtual inline QR_Size operator -(const QR_Size& value) const;
        virtual inline QR_Size operator -(T value) const;

        /**
        * Negation operator
        *@returns inverted size
        */
        virtual inline QR_Size operator -() const;

        /**
        * Multiplication operator
        *@param value - value to multiply
        *@returns resulting size
        */
        virtual inline QR_Size operator *(const QR_Size& value) const;
        virtual inline QR_Size operator *(T value) const;

        /**
        * Division operator
        *@param value - value to divide
        *@returns resulting size
        */
        virtual inline QR_Size operator /(const QR_Size& value) const;
        virtual inline QR_Size operator /(T value) const;

        /**
        * Addition and assignation operator
        *@param value - value to add
        *@returns resulting size
        */
        virtual inline const QR_Size& operator +=(const QR_Size& value);
        virtual inline const QR_Size& operator +=(T value);

        /**
        * Subtraction and assignation operator
        *@param value - value to subtract
        *@returns resulting size
        */
        virtual inline const QR_Size& operator -=(const QR_Size& value);
        virtual inline const QR_Size& operator -=(T value);

        /**
        * Multiplication and assignation operator
        *@param value - value to multiply
        *@returns resulting size
        */
        virtual inline const QR_Size& operator *=(const QR_Size& value);
        virtual inline const QR_Size& operator *=(T value);

        /**
        * Division and assignation operator
        *@param value - value to divide
        *@returns resulting size
        */
        virtual inline const QR_Size& operator /=(const QR_Size& value);
        virtual inline const QR_Size& operator /=(T value);

        /**
        * Equality operator
        *@param value - value to compare
        *@returns true if values are identical, otherwise false
        */
        virtual inline bool operator == (const QR_Size& other) const;

        /**
        * Not equality operator
        *@param value - value to compare
        *@returns true if values are not identical, otherwise false
        */
        virtual inline bool operator != (const QR_Size& other) const;

        /**
        * Check if size is set to [0, 0]
        *@returns true if size is set to [0, 0], otherwise false
        */
        virtual inline bool IsZero() const;
};

typedef QR_Size<int>         QR_SizeI;
typedef QR_Size<float>       QR_SizeF;
typedef QR_Size<M_Precision> QR_SizeP;

//---------------------------------------------------------------------------
// QR_Size
//---------------------------------------------------------------------------
template <class T>
QR_Size<T>::QR_Size() :
    m_Width(T(0)),
    m_Height(T(0))
{}
//---------------------------------------------------------------------------
template <class T>
QR_Size<T>::QR_Size(T width, T height) :
    m_Width(width),
    m_Height(height)
{}
//---------------------------------------------------------------------------
template <class T>
QR_Size<T>::~QR_Size()
{}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator +(const QR_Size& value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  + value.m_Width;
    size.m_Height = m_Height + value.m_Height;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator +(T value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  + value;
    size.m_Height = m_Height + value;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator -(const QR_Size& value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  - value.m_Width;
    size.m_Height = m_Height - value.m_Height;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator -(T value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  - value;
    size.m_Height = m_Height - value;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator -() const
{
    return QR_Size<T>(-m_Width, -m_Height);
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator *(const QR_Size& value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  * value.m_Width;
    size.m_Height = m_Height * value.m_Height;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator *(T value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  * value;
    size.m_Height = m_Height * value;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator /(const QR_Size& value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  / value.m_Width;
    size.m_Height = m_Height / value.m_Height;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
QR_Size<T> QR_Size<T>::operator /(T value) const
{
    // calculate resulting size
    QR_Size size;
    size.m_Width  = m_Width  / value;
    size.m_Height = m_Height / value;

    return size;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator +=(const QR_Size& value)
{
    m_Width  += value.m_Width;
    m_Height += value.m_Height;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator +=(T value)
{
    m_Width  += value;
    m_Height += value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator -=(const QR_Size& value)
{
    m_Width  -= value.m_Width;
    m_Height -= value.m_Height;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator -=(T value)
{
    m_Width  -= value;
    m_Height -= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator *=(const QR_Size& value)
{
    m_Width  *= value.m_Width;
    m_Height *= value.m_Height;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator *=(T value)
{
    m_Width  *= value;
    m_Height *= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator /=(const QR_Size& value)
{
    m_Width  /= value.m_Width;
    m_Height /= value.m_Height;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const QR_Size<T>& QR_Size<T>::operator /=(T value)
{
    m_Width  /= value;
    m_Height /= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Size<T>::operator == (const QR_Size& other) const
{
    return ((m_Width == other.m_Width) && (m_Height == other.m_Height));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Size<T>::operator != (const QR_Size& other) const
{
    return ((m_Width != other.m_Width) || (m_Height != other.m_Height));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Size<T>::IsZero() const
{
    return !m_Width && !m_Height;
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
