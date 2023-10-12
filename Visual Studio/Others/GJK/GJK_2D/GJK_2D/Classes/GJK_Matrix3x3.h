/****************************************************************************
 * ==> GJK_Matrix3x3 -------------------------------------------------------*
 ****************************************************************************
 * Description: 3x3 matrix                                                  *
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
#include <cmath>

// classes
#include "GJK_Vector2.h"

/**
* 3x3 matrix
*@author Jean-Milost Reymond
*/
template <class T>
struct GJK_Matrix3x3
{
    T m_Table[3][3];

    /**
    * Constructor
    *@note Thus created matrix is an identity matrix
    */
    inline GJK_Matrix3x3();

    /**
    * Constructor
    *@param _11 - matrix value at [0][0]
    *@param _12 - matrix value at [1][0]
    *@param _13 - matrix value at [2][0]
    *@param _21 - matrix value at [0][1]
    *@param _22 - matrix value at [1][1]
    *@param _23 - matrix value at [2][1]
    *@param _31 - matrix value at [0][2]
    *@param _32 - matrix value at [1][2]
    *@param _33 - matrix value at [2][2]
    */
    inline GJK_Matrix3x3(const T& _11, const T& _12, const T& _13,
                         const T& _21, const T& _22, const T& _23,
                         const T& _31, const T& _32, const T& _33);

    /**
    * Copy constructor
    *@param other - other matrix to copy from
    */
    inline GJK_Matrix3x3(const GJK_Matrix3x3& other);

    virtual inline ~GJK_Matrix3x3();

    /**
    * Copy operator
    *@param other - other matrix to copy from
    *@return copy of itself
    */
    virtual inline GJK_Matrix3x3& operator =(const GJK_Matrix3x3& other);

    /**
    * Equality operator
    *@param other - other matrix to compare with
    *@return true if matrices are equals, otherwise false
    */
    virtual inline bool operator ==(const GJK_Matrix3x3& other) const;

    /**
    * Difference operator
    *@param other - other matrix to compare with
    *@return true if matrices differ, otherwise false
    */
    virtual inline bool operator !=(const GJK_Matrix3x3& other) const;

    /**
    * Lower than operator (used for sorting operations)
    *@param other - other matrix to compare with
    *@return true if matrix memory is lower than other matrix, otherwise false
    */
    virtual bool operator < (const GJK_Matrix3x3& other) const;

    /**
    * Copies matrix
    *@param other - other matrix to copy from
    */
    virtual inline void Copy(const GJK_Matrix3x3& other);

    /**
    * Sets matrix as identity
    */
    virtual inline void SetIdentity();

    /**
    * Checks if matrix and other matrix are equals
    *@param other - other matrix to compare
    *@return true if matrix are equals, otherwise false
    */
    virtual bool IsEqual(const GJK_Matrix3x3& other) const;

    /**
    * Checks if matrix is the identity matrix (meaning thus this matrix is "empty")
    *@return true if matrix is the identity matrix, otherwise false
    */
    virtual inline bool IsIdentity() const;

    /**
    * Multiplies a matrix by another matrix
    *@param other - other matrix to multiply with
    *@return multiplied resulting matrix
    */
    virtual GJK_Matrix3x3 Multiply(const GJK_Matrix3x3& other) const;

    /**
    * Translates matrix
    *@param t - translation vector
    *@return copy of translated matrix
    */
    virtual inline GJK_Matrix3x3<T> Translate(const GJK_Vector2<T>& t);

    /**
    * Rotates matrix
    *@param angle - rotation angle in radians
    *@param r - rotation direction (e.g. [0.0f, 1.0f] for a y-axis rotation)
    *@return copy of rotated matrix
    *@note Rotation direction vector should be normalized before calling this function
    */
    virtual inline GJK_Matrix3x3<T> Rotate(T angle, const GJK_Vector2<T>& r);

    /**
    * Rotates matrix based on a rotation center point
    *@param angle - rotation angle in radians
    *@param center - rotation center point
    *@return copy of rotated matrix
    */
    virtual inline GJK_Matrix3x3<T> RotateCenter(T angle, const GJK_Vector2<T>& center);

    /**
    * Scales matrix
    *@param s - scale vector
    *@return copy of scaled matrix
    */
    virtual inline GJK_Matrix3x3<T> Scale(const GJK_Vector2<T>& s);

    /**
    * Transforms a vector using matrix
    *@param vector - vector to transform
    *@return transformed vector
    */
    virtual inline GJK_Vector2<T> Transform(const GJK_Vector2<T>& vector) const;
};

typedef GJK_Matrix3x3<float>  GJK_Matrix3x3F;
typedef GJK_Matrix3x3<double> GJK_Matrix3x3D;

//---------------------------------------------------------------------------
// GJK_Matrix3x3
//---------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T>::GJK_Matrix3x3()
{
    SetIdentity();
}
//---------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T>::GJK_Matrix3x3(const T& _11, const T& _12, const T& _13,
                                const T& _21, const T& _22, const T& _23,
                                const T& _31, const T& _32, const T& _33)
{
    m_Table[0][0] = _11; m_Table[1][0] = _12; m_Table[2][0] = _13;
    m_Table[0][1] = _21; m_Table[1][1] = _22; m_Table[2][1] = _23;
    m_Table[0][2] = _31; m_Table[1][2] = _32; m_Table[2][2] = _33;
}
//---------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T>::GJK_Matrix3x3(const GJK_Matrix3x3& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T>::~GJK_Matrix3x3()
{}
//---------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T>& GJK_Matrix3x3<T>::operator =(const GJK_Matrix3x3& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool GJK_Matrix3x3<T>::operator ==(const GJK_Matrix3x3& other) const
{
    return IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
bool GJK_Matrix3x3<T>::operator !=(const GJK_Matrix3x3& other) const
{
    return !IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
bool GJK_Matrix3x3<T>::operator < (const GJK_Matrix3x3& other) const
{
    return std::memcmp(&m_Table[0][0], &other.m_Table[0][0], sizeof(m_Table)) < 0;
}
//---------------------------------------------------------------------------
template <class T>
void GJK_Matrix3x3<T>::Copy(const GJK_Matrix3x3& other)
{
    std::memcpy(m_Table, other.m_Table, sizeof(m_Table));
}
//---------------------------------------------------------------------------
template <class T>
void GJK_Matrix3x3<T>::SetIdentity()
{
    m_Table[0][0] = 1.0f; m_Table[1][0] = 0.0f; m_Table[2][0] = 0.0f;
    m_Table[0][1] = 0.0f; m_Table[1][1] = 1.0f; m_Table[2][1] = 0.0f;
    m_Table[0][2] = 0.0f; m_Table[1][2] = 0.0f; m_Table[2][2] = 1.0f;
}
//---------------------------------------------------------------------------
template <class T>
bool GJK_Matrix3x3<T>::IsEqual(const GJK_Matrix3x3& other) const
{
    // compare each matrix element with other matrix. NOTE don't use std::memcmp here because the
    // comparison will fail in the following cases:
    // 1. if 0.0f == -0.0f, the equality operator will return true whereas the memory cmp will fail
    // 2. if NaN, the memory compare will always fail
    for (std::size_t i = 0; i < 3; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            if (m_Table[i][j] != other.m_Table[i][j])
                return false;

    return true;
}
//---------------------------------------------------------------------------
template <class T>
bool GJK_Matrix3x3<T>::IsIdentity() const
{
    return IsEqual(GJK_Matrix3x3<T>());
}
//------------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T> GJK_Matrix3x3<T>::Multiply(const GJK_Matrix3x3<T>& other) const
{
    GJK_Matrix3x3<T> matrix;

    for (std::size_t i = 0; i < 3; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            matrix.m_Table[i][j] = m_Table[i][0] * other.m_Table[0][j] +
                                   m_Table[i][1] * other.m_Table[1][j] +
                                   m_Table[i][2] * other.m_Table[2][j];

    return matrix;
}
//------------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T> GJK_Matrix3x3<T>::Translate(const GJK_Vector2<T>& t)
{
    m_Table[2][0] += m_Table[0][0] * t.m_X + m_Table[1][0] * t.m_Y;
    m_Table[2][1] += m_Table[0][1] * t.m_X + m_Table[1][1] * t.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T> GJK_Matrix3x3<T>::Rotate(T angle, const GJK_Vector2<T>& r)
{
    // calculate sinus and cosine values
    const T c = std::cos(angle);
    const T s = std::sin(angle);

    // create rotation matrix
    GJK_Matrix3x3<T> matrix;
    matrix.SetIdentity();
    matrix.m_Table[0][0] = c * r.m_X; matrix.m_Table[1][0] = -s * r.m_Y;
    matrix.m_Table[0][1] = s * r.m_X; matrix.m_Table[1][1] =  c * r.m_Y;

    // combine current matrix with rotation matrix
    *this = matrix.Multiply(*this);

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T> GJK_Matrix3x3<T>::RotateCenter(T angle, const GJK_Vector2<T>& center)
{
    const T c = std::cos(angle);
    const T s = std::sin(angle);

    // create rotation matrix
    GJK_Matrix3x3<T> matrix;
    matrix.SetIdentity();

    // equivalent to translate(center) * rotate(angle) * translate(-center)
    matrix.m_Table[0][0] = c; matrix.m_Table[1][0] = -s; matrix.m_Table[2][0] = center.m_X - center.m_X * c + center.m_Y * s;
    matrix.m_Table[0][1] = s; matrix.m_Table[1][1] =  c; matrix.m_Table[2][1] = center.m_Y - center.m_X * s - center.m_Y * c;

    // combine current matrix with rotation matrix
    *this = matrix.Multiply(*this);

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
GJK_Matrix3x3<T> GJK_Matrix3x3<T>::Scale(const GJK_Vector2<T>& s)
{
    m_Table[0][0] *= s.m_X; m_Table[1][0] *= s.m_Y;
    m_Table[0][1] *= s.m_X; m_Table[1][1] *= s.m_Y;
    m_Table[0][2] *= s.m_X; m_Table[1][2] *= s.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
GJK_Vector2<T> GJK_Matrix3x3<T>::Transform(const GJK_Vector2<T>& vector) const
{
    // calculates x and y coordinates and returns transformed vector
    return GJK_Vector2<T>((vector.m_X * m_Table[0][0] + vector.m_Y * m_Table[1][0] + m_Table[2][0]),
                          (vector.m_X * m_Table[0][1] + vector.m_Y * m_Table[1][1] + m_Table[2][1]));
}
//---------------------------------------------------------------------------
