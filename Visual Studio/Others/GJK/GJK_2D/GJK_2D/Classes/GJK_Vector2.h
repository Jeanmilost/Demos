/****************************************************************************
 * ==> GJK_Vector2 ---------------------------------------------------------*
 ****************************************************************************
 * Description: 2D vector                                                   *
 * Developer:   Jean-Milost Reymond                                         *
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

// std
#include <algorithm>

/**
* 2D vector
*@author Jean-Milost Reymond
*/
template <class T>
class GJK_Vector2
{
    public:
        T m_X; // vector x coordinate
        T m_Y; // vector y coordinate

        inline GJK_Vector2();

        /**
        * Constructor
        *@param x - x coordinate
        *@param y - y coordinate
        */
        inline GJK_Vector2(T x, T y);

        /**
        * Copy constructor
        *@param other - other vector to copy from
        */
        inline GJK_Vector2(const GJK_Vector2& other);

        virtual ~GJK_Vector2();

        /**
        * Copy operator
        *@param other - other vector to copy from
        *@return this vector
        */
        virtual inline GJK_Vector2<T>& operator = (const GJK_Vector2<T>& other);

        /**
        * Addition operator
        *@param value - value to add
        *@return resulting vector
        */
        virtual inline GJK_Vector2<T> operator + (const GJK_Vector2<T>& value) const;
        virtual inline GJK_Vector2<T> operator + (const T&              value) const;

        /**
        * Subtraction operator
        *@param value - value to subtract
        *@return resulting vector
        */
        virtual inline GJK_Vector2<T> operator - (const GJK_Vector2<T>& value) const;
        virtual inline GJK_Vector2<T> operator - (const T&              value) const;

        /**
        * Negation operator
        *@return inverted vector
        */
        virtual inline GJK_Vector2<T> operator - () const;

        /**
        * Multiplication operator
        *@param value - value to multiply
        *@return resulting vector
        */
        virtual inline GJK_Vector2<T> operator * (const GJK_Vector2<T>& value) const;
        virtual inline GJK_Vector2<T> operator * (const T&              value) const;

        /**
        * Division operator
        *@param value - value to divide
        *@return resulting vector
        */
        virtual inline GJK_Vector2<T> operator / (const GJK_Vector2<T>& value) const;
        virtual inline GJK_Vector2<T> operator / (const T&              value) const;

        /**
        * Addition and assignation operator
        *@param value - value to add
        *@return resulting vector
        */
        virtual inline const GJK_Vector2<T>& operator += (const GJK_Vector2<T>& value);
        virtual inline const GJK_Vector2<T>& operator += (const T&              value);

        /**
        * Subtraction and assignation operator
        *@param value - value to subtract
        *@return resulting vector
        */
        virtual inline const GJK_Vector2<T>& operator -= (const GJK_Vector2<T>& value);
        virtual inline const GJK_Vector2<T>& operator -= (const T&              value);

        /**
        * Multiplication and assignation operator
        *@param value - value to multiply
        *@return resulting vector
        */
        virtual inline const GJK_Vector2<T>& operator *= (const GJK_Vector2<T>& value);
        virtual inline const GJK_Vector2<T>& operator *= (const T&              value);

        /**
        * Division and assignation operator
        *@param value - value to divide
        *@return resulting vector
        */
        virtual inline const GJK_Vector2<T>& operator /= (const GJK_Vector2<T>& value);
        virtual inline const GJK_Vector2<T>& operator /= (const T&              value);

        /**
        * Equality operator
        *@param value - value to compare
        *@return true if values are identical, otherwise false
        */
        virtual inline bool operator == (const GJK_Vector2<T>& value) const;

        /**
        * Not equality operator
        *@param value - value to compare
        *@return true if values are not identical, otherwise false
        */
        virtual inline bool operator != (const GJK_Vector2<T>& value) const;

        /**
        * Calculates the vector length
        *@return vector length
        */
        virtual inline T Length() const;

        /**
        * Calculates the vector squared length
        *@return vector squared length
        */
        virtual inline T SquaredLength() const;

        /**
        * Calculates the perpendicular vector of this vector
        *@return the perpendicular vector of this vector
        */
        virtual inline GJK_Vector2<T> Perpendicular() const;

        /**
        * Normalizes the vector
        *@return normalized vector
        */
        virtual inline GJK_Vector2<T> Normalize() const;

        /**
        * Calculates cross product between 2 vectors
        *@param vector - other vector to cross with
        *@return the resulting vector
        */
        virtual inline GJK_Vector2 Cross(const GJK_Vector2& vector) const;

        /**
        * Calculates dot product between 2 vectors
        *@param vector - other vector to dot with
        *@return resulting angle
        */
        virtual inline T Dot(const GJK_Vector2& vector) const;
};

typedef GJK_Vector2<float>  GJK_Vector2F;
typedef GJK_Vector2<double> GJK_Vector2D;

//---------------------------------------------------------------------------
// GJK_Vector2
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T>::GJK_Vector2() :
    m_X(T(0.0)),
    m_Y(T(0.0))
{}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T>::GJK_Vector2(T x, T y) :
    m_X(x),
    m_Y(y)
{}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T>::GJK_Vector2(const GJK_Vector2& other) :
    m_X(other.m_X),
    m_Y(other.m_Y)
{}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T>::~GJK_Vector2()
{}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T>& GJK_Vector2<T>::operator = (const GJK_Vector2& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator + (const GJK_Vector2& value) const
{
    return GJK_Vector2(m_X + value.m_X, m_Y + value.m_Y);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator + (const T& value) const
{
    return GJK_Vector2(m_X + value, m_Y + value);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator - (const GJK_Vector2& value) const
{
    return GJK_Vector2(m_X - value.m_X, m_Y - value.m_Y);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator - (const T& value) const
{
    return GJK_Vector2(m_X - value, m_Y - value);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator - () const
{
    return GJK_Vector2<T>(-m_X, -m_Y);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator * (const GJK_Vector2& value) const
{
    return GJK_Vector2(m_X * value.m_X, m_Y * value.m_Y);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator * (const T& value) const
{
    return GJK_Vector2(m_X * value, m_Y * value);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator / (const GJK_Vector2& value) const
{
    if (!value.m_X)
        throw new std::exception("Division by 0 error - invalid m_X value");

    if (!value.m_Y)
        throw new std::exception("Division by 0 error - invalid m_Y value");

    return GJK_Vector2(m_X / value.m_X, m_Y / value.m_Y);
}
//---------------------------------------------------------------------------
template<class T>
GJK_Vector2<T> GJK_Vector2<T>::operator / (const T& value) const
{
    if (!value)
        throw new std::exception("Division by 0 error - invalid value");

    return GJK_Vector2(m_X / value, m_Y / value);
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator += (const GJK_Vector2& value)
{
    m_X += value.m_X;
    m_Y += value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator += (const T& value)
{
    m_X += value;
    m_Y += value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator -= (const GJK_Vector2& value)
{
    m_X -= value.m_X;
    m_Y -= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator -= (const T& value)
{
    m_X -= value;
    m_Y -= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator *= (const GJK_Vector2& value)
{
    m_X *= value.m_X;
    m_Y *= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator *= (const T& value)
{
    m_X *= value;
    m_Y *= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator /= (const GJK_Vector2& value)
{
    if (!value.m_X)
        throw new std::exception("Division by 0 error - invalid m_X value");

    if (!value.m_Y)
        throw new std::exception("Division by 0 error - invalid m_Y value");

    m_X /= value.m_X;
    m_Y /= value.m_Y;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
const GJK_Vector2<T>& GJK_Vector2<T>::operator /= (const T& value)
{
    if (!value)
        throw new std::exception("Division by 0 error - invalid value");

    m_X /= value;
    m_Y /= value;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
bool GJK_Vector2<T>::operator == (const GJK_Vector2& value) const
{
    return (m_X == value.m_X && m_Y == value.m_Y);
}
//---------------------------------------------------------------------------
template<class T>
bool GJK_Vector2<T>::operator !=(const GJK_Vector2& value) const
{
    return (m_X != value.m_X || m_Y != value.m_Y);
}
//---------------------------------------------------------------------------
template <class T>
T GJK_Vector2<T>::Length() const
{
    return (T)sqrt((T)(m_X * m_X) + (T)(m_Y * m_Y));
}
//---------------------------------------------------------------------------
template <class T>
T GJK_Vector2<T>::SquaredLength() const
{
    return ((T)(m_X * m_X) + (T)(m_Y * m_Y));
}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T> GJK_Vector2<T>::Perpendicular() const
{
    return GJK_Vector2<T>(m_Y, -m_X);
}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T> GJK_Vector2<T>::Normalize() const
{
    // calculate vector length
    const T len = Length();

    // no vector length?
    if (!len)
        return GJK_Vector2();

    // normalize vector (thus values will always be between 0.0f, and 1.0f)
    return GJK_Vector2(m_X / len, m_Y / len);
}
//---------------------------------------------------------------------------
template <class T>
GJK_Vector2<T> GJK_Vector2<T>::Cross(const GJK_Vector2& vector) const
{
    return GJK_Vector2((m_Y * vector.m_X) - (vector.m_Y * m_X),
                       (m_X * vector.m_Y) - (vector.m_X * m_Y));
}
//---------------------------------------------------------------------------
template <class T>
T GJK_Vector2<T>::Dot(const GJK_Vector2& vector) const
{
    return ((m_X * vector.m_X) + (m_Y * vector.m_Y));
}
//---------------------------------------------------------------------------
