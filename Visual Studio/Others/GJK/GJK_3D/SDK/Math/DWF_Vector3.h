/****************************************************************************
 * ==> DWF_Vector3 ---------------------------------------------------------*
 ****************************************************************************
 * Description: 3D vector                                                   *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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

namespace DWF_Math
{
    /**
    * 3D vector
    *@author Jean-Milost Reymond
    */
    template <class T>
    class Vector3
    {
        public:
            T m_X; // vector x coordinate
            T m_Y; // vector y coordinate
            T m_Z; // vector z coordinate

            inline Vector3();

            /**
            * Constructor
            *@param x - x coordinate
            *@param y - y coordinate
            *@param z - z coordinate
            */
            inline Vector3(T x, T y, T z);

            /**
            * Copy constructor
            *@param other - other vector to copy from
            */
            inline Vector3(const Vector3& other);

            virtual ~Vector3();

            /**
            * Copy operator
            *@param other - other vector to copy from
            *@return this vector
            */
            virtual inline Vector3& operator = (const Vector3& other);

            /**
            * Addition operator
            *@param value - value to add
            *@return resulting vector
            */
            virtual inline Vector3 operator + (const Vector3& value) const;
            virtual inline Vector3 operator + (const T& value) const;

            /**
            * Subtraction operator
            *@param value - value to subtract
            *@return resulting vector
            */
            virtual inline Vector3 operator - (const Vector3& value) const;
            virtual inline Vector3 operator - (const T& value) const;

            /**
            * Negation operator
            *@return inverted vector
            */
            virtual inline Vector3 operator - () const;

            /**
            * Multiplication operator
            *@param value - value to multiply
            *@return resulting vector
            */
            virtual inline Vector3 operator * (const Vector3& value) const;
            virtual inline Vector3 operator * (const T& value) const;

            /**
            * Division operator
            *@param value - value to divide
            *@return resulting vector
            */
            virtual inline Vector3 operator / (const Vector3& value) const;
            virtual inline Vector3 operator / (const T& value) const;

            /**
            * Addition and assignation operator
            *@param value - value to add
            *@return resulting vector
            */
            virtual inline const Vector3& operator += (const Vector3& value);
            virtual inline const Vector3& operator += (const T& value);

            /**
            * Subtraction and assignation operator
            *@param value - value to subtract
            *@return resulting vector
            */
            virtual inline const Vector3& operator -= (const Vector3& value);
            virtual inline const Vector3& operator -= (const T& value);

            /**
            * Multiplication and assignation operator
            *@param value - value to multiply
            *@return resulting vector
            */
            virtual inline const Vector3& operator *= (const Vector3& value);
            virtual inline const Vector3& operator *= (const T& value);

            /**
            * Division and assignation operator
            *@param value - value to divide
            *@return resulting vector
            */
            virtual inline const Vector3& operator /= (const Vector3& value);
            virtual inline const Vector3& operator /= (const T& value);

            /**
            * Equality operator
            *@param value - value to compare
            *@return true if values are identical, otherwise false
            */
            virtual inline bool operator == (const Vector3& value) const;

            /**
            * Not equality operator
            *@param value - value to compare
            *@return true if values are not identical, otherwise false
            */
            virtual inline bool operator != (const Vector3& value) const;

            /**
            * Calculates the vector length
            *@return vector length
            */
            virtual inline T Length() const;

            /**
            * Normalizes the vector
            *@return normalized vector
            */
            virtual inline Vector3 Normalize() const;

            /**
            * Calculates cross product between 2 vectors
            *@param vector - other vector to cross with
            *@return the resulting vector
            */
            virtual inline Vector3 Cross(const Vector3& vector) const;

            /**
            * Calculates dot product between 2 vectors
            *@param vector - other vector to dot with
            *@return resulting angle
            */
            virtual inline T Dot(const Vector3& vector) const;
    };

    typedef Vector3<float>  Vector3F;
    typedef Vector3<double> Vector3D;

    //---------------------------------------------------------------------------
    // Vector3
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T>::Vector3() :
        m_X(T(0.0)),
        m_Y(T(0.0)),
        m_Z(T(0.0))
    {}
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T>::Vector3(T x, T y, T z) :
        m_X(x),
        m_Y(y),
        m_Z(z)
    {}
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T>::Vector3(const Vector3& other) :
        m_X(other.m_X),
        m_Y(other.m_Y),
        m_Z(other.m_Z)
    {}
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T>::~Vector3()
    {}
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T>& Vector3<T>::operator = (const Vector3& other)
    {
        m_X = other.m_X;
        m_Y = other.m_Y;
        m_Z = other.m_Z;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator + (const Vector3& value) const
    {
        return Vector3(m_X + value.m_X, m_Y + value.m_Y, m_Z + value.m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator + (const T& value) const
    {
        return Vector3(m_X + value, m_Y + value, m_Z + value);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator - (const Vector3& value) const
    {
        return Vector3(m_X - value.m_X, m_Y - value.m_Y, m_Z - value.m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator - (const T& value) const
    {
        return Vector3(m_X - value, m_Y - value, m_Z - value);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator - () const
    {
        return Vector3<T>(-m_X, -m_Y, -m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator * (const Vector3& value) const
    {
        return Vector3(m_X * value.m_X, m_Y * value.m_Y, m_Z * value.m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator * (const T& value) const
    {
        return Vector3(m_X * value, m_Y * value, m_Z * value);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator / (const Vector3& value) const
    {
        if (!value.m_X)
            throw new std::exception("Division by 0 error - invalid m_X value");

        if (!value.m_Y)
            throw new std::exception("Division by 0 error - invalid m_Y value");

        if (!value.m_Z)
            throw new std::exception("Division by 0 error - invalid m_Z value");

        return Vector3(m_X / value.m_X, m_Y / value.m_Y, m_Z / value.m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    Vector3<T> Vector3<T>::operator / (const T& value) const
    {
        if (!value)
            throw new std::exception("Division by 0 error - invalid value");

        return Vector3(m_X / value, m_Y / value, m_Z / value);
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator += (const Vector3& value)
    {
        m_X += value.m_X;
        m_Y += value.m_Y;
        m_Z += value.m_Z;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator += (const T& value)
    {
        m_X += value;
        m_Y += value;
        m_Z += value;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator -= (const Vector3& value)
    {
        m_X -= value.m_X;
        m_Y -= value.m_Y;
        m_Z -= value.m_Z;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator -= (const T& value)
    {
        m_X -= value;
        m_Y -= value;
        m_Z -= value;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator *= (const Vector3& value)
    {
        m_X *= value.m_X;
        m_Y *= value.m_Y;
        m_Z *= value.m_Z;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator *= (const T& value)
    {
        m_X *= value;
        m_Y *= value;
        m_Z *= value;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator /= (const Vector3& value)
    {
        if (!value.m_X)
            throw new std::exception("Division by 0 error - invalid m_X value");

        if (!value.m_Y)
            throw new std::exception("Division by 0 error - invalid m_Y value");

        if (!value.m_Z)
            throw new std::exception("Division by 0 error - invalid m_Z value");

        m_X /= value.m_X;
        m_Y /= value.m_Y;
        m_Z /= value.m_Z;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    const Vector3<T>& Vector3<T>::operator /= (const T& value)
    {
        if (!value)
            throw new std::exception("Division by 0 error - invalid value");

        m_X /= value;
        m_Y /= value;
        m_Z /= value;

        return *this;
    }
    //---------------------------------------------------------------------------
    template<class T>
    bool Vector3<T>::operator == (const Vector3& value) const
    {
        return (m_X == value.m_X && m_Y == value.m_Y && m_Z == value.m_Z);
    }
    //---------------------------------------------------------------------------
    template<class T>
    bool Vector3<T>::operator !=(const Vector3& value) const
    {
        return (m_X != value.m_X || m_Y != value.m_Y || m_Z != value.m_Z);
    }
    //---------------------------------------------------------------------------
    template <class T>
    T Vector3<T>::Length() const
    {
        return T(sqrt((double(m_X) * double(m_X)) + (double(m_Y) * double(m_Y)) + (double(m_Z) * double(m_Z))));
    }
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T> Vector3<T>::Normalize() const
    {
        // calculate vector length
        const T len = Length();

        // no vector length?
        if (!len)
            return Vector3();

        // normalize vector (thus values will always be between 0.0f, and 1.0f)
        return Vector3(m_X / len, m_Y / len, m_Z / len);
    }
    //---------------------------------------------------------------------------
    template <class T>
    Vector3<T> Vector3<T>::Cross(const Vector3& vector) const
    {
        return Vector3((m_Y * vector.m_Z) - (vector.m_Y * m_Z),
                       (m_Z * vector.m_X) - (vector.m_Z * m_X),
                       (m_X * vector.m_Y) - (vector.m_X * m_Y));
    }
    //---------------------------------------------------------------------------
    template <class T>
    T Vector3<T>::Dot(const Vector3& vector) const
    {
        return ((m_X * vector.m_X) + (m_Y * vector.m_Y) + (m_Z * vector.m_Z));
    }
    //---------------------------------------------------------------------------
}
