/****************************************************************************
 * ==> QR_Matrix16 ---------------------------------------------------------*
 ****************************************************************************
 * Description : 4x4 matrix                                                 *
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

#ifndef QR_Matrix16H
#define QR_Matrix16H

// std
#include <memory>
#include <cmath>
#if defined (OS_WIN)
    // needed for some std::memset implementation, e.g. in CodeBlocks where
    // compiler will not found it otherwise
    #include <cstring>
#endif

// qr engine
#include "QR_Types.h"
#include "QR_Vector3D.h"

/**
* 4x4 matrix
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Matrix16
{
    public:
        T m_Table[4][4]; // 4x4 matrix array

        /**
        * Constructor
        */
        inline QR_Matrix16();

        /**
        * Constructor
        *@param _11 - matrix value
        *@param _12 - matrix value
        *@param _13 - matrix value
        *@param _14 - matrix value
        *@param _21 - matrix value
        *@param _22 - matrix value
        *@param _23 - matrix value
        *@param _24 - matrix value
        *@param _31 - matrix value
        *@param _32 - matrix value
        *@param _33 - matrix value
        *@param _34 - matrix value
        *@param _41 - matrix value
        *@param _42 - matrix value
        *@param _43 - matrix value
        *@param _44 - matrix value
        */
        inline QR_Matrix16(T _11, T _12, T _13, T _14,
                           T _21, T _22, T _23, T _24,
                           T _31, T _32, T _33, T _34,
                           T _41, T _42, T _43, T _44);

        /**
        * Copy constructor
        *@param other - other matrix to copy from
        */
        inline QR_Matrix16(const QR_Matrix16& other);

        /**
        * Destructor
        */
        virtual inline ~QR_Matrix16();

        /**
        * Assignation operator
        *@param other - other matrix to copy from
        */
        virtual inline QR_Matrix16& operator =(const QR_Matrix16& other);

        /**
        * Equality operator
        *@param other - other matrix to compare
        *@return true if both matrix are equals, otherwise false
        */
        virtual inline bool operator ==(const QR_Matrix16& other);

        /**
        * Not equality operator
        *@param other - other matrix to compare
        *@return true if both matrix are not equals, otherwise false
        */
        virtual inline bool operator !=(const QR_Matrix16& other);

        /**
        * Copies matrix from another
        *@param other - other matrix to copy from
        */
        virtual inline void Copy(const QR_Matrix16& other);

        /**
        * Checks if matrix and other matrix are equals
        *@param other - other matrix to compare
        *@return true if matrix are equals, otherwise false
        */
        virtual inline bool IsEqual(const QR_Matrix16& other) const;

        /**
        * Checks if matrix is an identity matrix
        *@return true if matrix is an identity matrix, otherwise false
        */
        virtual inline bool IsIdentity() const;

        /**
        * Gets an identity matrix
        *@return identity matrix
        */
        static inline QR_Matrix16 Identity();

        /**
        * Inverses a matrix
        *@param[out] determinant - matrix determinant
        */
        virtual inline QR_Matrix16 Inverse(float& determinant);

        /**
        * Multiplies matrix by another matrix
        *@param other - other matrix to multiply with
        *@return multiplied resulting matrix
        */
        virtual inline QR_Matrix16 Multiply(const QR_Matrix16<T>& other) const;

        /**
        * Translates matrix
        *@param t - translation vector
        *@return copy of translated matrix
        */
        virtual inline QR_Matrix16 Translate(const QR_Vector3D<T>& t);

        /**
        * Rotates matrix
        *@param angle - rotation angle in radians
        *@param r - rotation direction (e.g. {0.0f, 0.0f, 1.0f} for a z-axis rotation)
        *@return copy of rotated matrix
        *@note rotation direction vector should be normalized before calling
        *      this function
        */
        virtual inline QR_Matrix16 Rotate(T              angle,
                                          const QR_Vector3D<T>& r);

        /**
        * Scales matrix
        *@param s - scale vector
        *@return copy of scaled matrix
        */
        virtual inline QR_Matrix16 Scale(const QR_Vector3D<T>& s);

        /**
        * Swaps matrix lines and columns
        *@return swapped matrix
        */
        virtual inline QR_Matrix16 Swap() const;

        /**
        * Applies a transformation matrix to a vector
        *@param vector - vector to transform
        *@return transformed vector
        */
        virtual inline QR_Vector3D<T> Transform(const QR_Vector3D<T>& vector) const;

        /**
        * Applies a transformation matrix to a normal
        *@param normal - normal to transform
        *@return transformed normal
        */
        virtual inline QR_Vector3D<T> TransformNormal(const QR_Vector3D<T>& normal) const;

        /**
        * Gets table pointer
        *@return pointer
        */
        virtual inline const T* GetPtr() const;
};

typedef QR_Matrix16<QR_Float>    QR_Matrix16F;
typedef QR_Matrix16<QR_Double>   QR_Matrix16D;
typedef QR_Matrix16<M_Precision> QR_Matrix16P;

//---------------------------------------------------------------------------
// QR_Matrix16
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T>::QR_Matrix16()
{
    // initialize matrix table
    m_Table[0][0] = 1.0f; m_Table[1][0] = 0.0f; m_Table[2][0] = 0.0f; m_Table[3][0] = 0.0f;
    m_Table[0][1] = 0.0f; m_Table[1][1] = 1.0f; m_Table[2][1] = 0.0f; m_Table[3][1] = 0.0f;
    m_Table[0][2] = 0.0f; m_Table[1][2] = 0.0f; m_Table[2][2] = 1.0f; m_Table[3][2] = 0.0f;
    m_Table[0][3] = 0.0f; m_Table[1][3] = 0.0f; m_Table[2][3] = 0.0f; m_Table[3][3] = 1.0f;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T>::QR_Matrix16(T _11, T _12, T _13, T _14,
                            T _21, T _22, T _23, T _24,
                            T _31, T _32, T _33, T _34,
                            T _41, T _42, T _43, T _44)
{
    m_Table[0][0] = _11; m_Table[1][0] = _12; m_Table[2][0] = _13; m_Table[3][0] = _14;
    m_Table[0][1] = _21; m_Table[1][1] = _22; m_Table[2][1] = _23; m_Table[3][1] = _24;
    m_Table[0][2] = _31; m_Table[1][2] = _32; m_Table[2][2] = _33; m_Table[3][2] = _34;
    m_Table[0][3] = _41; m_Table[1][3] = _42; m_Table[2][3] = _43; m_Table[3][3] = _44;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T>::QR_Matrix16(const QR_Matrix16& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T>::~QR_Matrix16()
{}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T>& QR_Matrix16<T>::operator =(const QR_Matrix16& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Matrix16<T>::operator ==(const QR_Matrix16& other)
{
    return IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Matrix16<T>::operator !=(const QR_Matrix16& other)
{
    return !IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
void QR_Matrix16<T>::Copy(const QR_Matrix16& other)
{
    // copy matrix table from other
    std::memcpy(m_Table, other.m_Table, sizeof(m_Table));
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Matrix16<T>::IsEqual(const QR_Matrix16& other) const
{
    // compare each matrix element with other matrix. NOTE don't use std::memcmp here because the
    // comparison will fail in the following cases:
    // 1. if 0.0f == -0.0f, the equality operator will return true whereas the memory cmp will fail
    // 2. if NaN, the memory compare will always fail
    for (QR_UInt8 i = 0; i < 4; ++i)
        for (QR_UInt8 j = 0; j < 4; ++j)
            if (m_Table[i][j] != other.m_Table[i][j])
                return false;

    return true;
}
//---------------------------------------------------------------------------
template <class T>
bool QR_Matrix16<T>::IsIdentity() const
{
    return IsEqual(Identity());
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Identity()
{
    return QR_Matrix16<T>(T(1.0), T(0.0), T(0.0), T(0.0),
                          T(0.0), T(1.0), T(0.0), T(0.0),
                          T(0.0), T(0.0), T(1.0), T(0.0),
                          T(0.0), T(0.0), T(0.0), T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Inverse(float& determinant)
{
    float t[3];
    float v[16];

    t[0] = m_Table[2][2] * m_Table[3][3] - m_Table[2][3] * m_Table[3][2];
    t[1] = m_Table[1][2] * m_Table[3][3] - m_Table[1][3] * m_Table[3][2];
    t[2] = m_Table[1][2] * m_Table[2][3] - m_Table[1][3] * m_Table[2][2];

    v[0] =  m_Table[1][1] * t[0] - m_Table[2][1] * t[1] + m_Table[3][1] * t[2];
    v[4] = -m_Table[1][0] * t[0] + m_Table[2][0] * t[1] - m_Table[3][0] * t[2];

    t[0] =  m_Table[1][0] * m_Table[2][1] - m_Table[2][0] * m_Table[1][1];
    t[1] =  m_Table[1][0] * m_Table[3][1] - m_Table[3][0] * m_Table[1][1];
    t[2] =  m_Table[2][0] * m_Table[3][1] - m_Table[3][0] * m_Table[2][1];

    v[8]  =  m_Table[3][3] * t[0] - m_Table[2][3] * t[1] + m_Table[1][3] * t[2];
    v[12] = -m_Table[3][2] * t[0] + m_Table[2][2] * t[1] - m_Table[1][2] * t[2];

    determinant = m_Table[0][0] * v[0] +
                  m_Table[0][1] * v[4] +
                  m_Table[0][2] * v[8] +
                  m_Table[0][3] * v[12];

    if (determinant == 0.0)
        return QR_Matrix16<T>();

    t[0] = m_Table[2][2] * m_Table[3][3] - m_Table[2][3] * m_Table[3][2];
    t[1] = m_Table[0][2] * m_Table[3][3] - m_Table[0][3] * m_Table[3][2];
    t[2] = m_Table[0][2] * m_Table[2][3] - m_Table[0][3] * m_Table[2][2];

    v[1] = -m_Table[0][1] * t[0] + m_Table[2][1] * t[1] - m_Table[3][1] * t[2];
    v[5] =  m_Table[0][0] * t[0] - m_Table[2][0] * t[1] + m_Table[3][0] * t[2];

    t[0] = m_Table[0][0] * m_Table[2][1] - m_Table[2][0] * m_Table[0][1];
    t[1] = m_Table[3][0] * m_Table[0][1] - m_Table[0][0] * m_Table[3][1];
    t[2] = m_Table[2][0] * m_Table[3][1] - m_Table[3][0] * m_Table[2][1];

    v[9]  = -m_Table[3][3] * t[0] - m_Table[2][3] * t[1] - m_Table[0][3] * t[2];
    v[13] =  m_Table[3][2] * t[0] + m_Table[2][2] * t[1] + m_Table[0][2] * t[2];

    t[0] = m_Table[1][2] * m_Table[3][3] - m_Table[1][3] * m_Table[3][2];
    t[1] = m_Table[0][2] * m_Table[3][3] - m_Table[0][3] * m_Table[3][2];
    t[2] = m_Table[0][2] * m_Table[1][3] - m_Table[0][3] * m_Table[1][2];

    v[2] =  m_Table[0][1] * t[0] - m_Table[1][1] * t[1] + m_Table[3][1] * t[2];
    v[6] = -m_Table[0][0] * t[0] + m_Table[1][0] * t[1] - m_Table[3][0] * t[2];

    t[0] = m_Table[0][0] * m_Table[1][1] - m_Table[1][0] * m_Table[0][1];
    t[1] = m_Table[3][0] * m_Table[0][1] - m_Table[0][0] * m_Table[3][1];
    t[2] = m_Table[1][0] * m_Table[3][1] - m_Table[3][0] * m_Table[1][1];

    v[10] =  m_Table[3][3] * t[0] + m_Table[1][3] * t[1] + m_Table[0][3] * t[2];
    v[14] = -m_Table[3][2] * t[0] - m_Table[1][2] * t[1] - m_Table[0][2] * t[2];

    t[0] = m_Table[1][2] * m_Table[2][3] - m_Table[1][3] * m_Table[2][2];
    t[1] = m_Table[0][2] * m_Table[2][3] - m_Table[0][3] * m_Table[2][2];
    t[2] = m_Table[0][2] * m_Table[1][3] - m_Table[0][3] * m_Table[1][2];

    v[3] = -m_Table[0][1] * t[0] + m_Table[1][1] * t[1] - m_Table[2][1] * t[2];
    v[7] =  m_Table[0][0] * t[0] - m_Table[1][0] * t[1] + m_Table[2][0] * t[2];

    v[11] = -m_Table[0][0] * (m_Table[1][1] * m_Table[2][3] - m_Table[1][3] * m_Table[2][1]) +
             m_Table[1][0] * (m_Table[0][1] * m_Table[2][3] - m_Table[0][3] * m_Table[2][1]) -
             m_Table[2][0] * (m_Table[0][1] * m_Table[1][3] - m_Table[0][3] * m_Table[1][1]);

    v[15] = m_Table[0][0] * (m_Table[1][1] * m_Table[2][2] - m_Table[1][2] * m_Table[2][1]) -
            m_Table[1][0] * (m_Table[0][1] * m_Table[2][2] - m_Table[0][2] * m_Table[2][1]) +
            m_Table[2][0] * (m_Table[0][1] * m_Table[1][2] - m_Table[0][2] * m_Table[1][1]);

    const T invDet = 1.0 / determinant;

    QR_Matrix16<T> result;

    for (QR_UInt8 i = 0; i < 4; ++i)
        for (QR_UInt8 j = 0; j < 4; ++j)
            result. m_Table[i][j] = v[4 * i + j] * invDet;

    return result;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Multiply(const QR_Matrix16<T>& other) const
{
    QR_Matrix16<T> matrix;

    for (QR_UInt8 i = 0; i < 4; ++i)
        for (QR_UInt8 j = 0; j < 4; ++j)
            matrix.m_Table[i][j] = m_Table[i][0] * other.m_Table[0][j] +
                                   m_Table[i][1] * other.m_Table[1][j] +
                                   m_Table[i][2] * other.m_Table[2][j] +
                                   m_Table[i][3] * other.m_Table[3][j];

    return matrix;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Translate(const QR_Vector3D<T>& t)
{
    m_Table[3][0] += m_Table[0][0] * t.m_X + m_Table[1][0] * t.m_Y + m_Table[2][0] * t.m_Z;
    m_Table[3][1] += m_Table[0][1] * t.m_X + m_Table[1][1] * t.m_Y + m_Table[2][1] * t.m_Z;
    m_Table[3][2] += m_Table[0][2] * t.m_X + m_Table[1][2] * t.m_Y + m_Table[2][2] * t.m_Z;
    m_Table[3][3] += m_Table[0][3] * t.m_X + m_Table[1][3] * t.m_Y + m_Table[2][3] * t.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Rotate(T              angle,
                                      const QR_Vector3D<T>& r)
{
    // calculate sinus, cosinus and inverted cosinus values
    const T c  = std::cos(angle);
    const T s  = std::sin(angle);
    const T ic = (T(1.0) - c);

    // create rotation matrix
    QR_Matrix16<T> matrix = Identity();
    matrix.m_Table[0][0] = (ic * r.m_X * r.m_X) + c;
    matrix.m_Table[1][0] = (ic * r.m_X * r.m_Y) - (s * r.m_Z);
    matrix.m_Table[2][0] = (ic * r.m_X * r.m_Z) + (s * r.m_Y);
    matrix.m_Table[0][1] = (ic * r.m_Y * r.m_X) + (s * r.m_Z);
    matrix.m_Table[1][1] = (ic * r.m_Y * r.m_Y) + c;
    matrix.m_Table[2][1] = (ic * r.m_Y * r.m_Z) - (s * r.m_X);
    matrix.m_Table[0][2] = (ic * r.m_Z * r.m_X) - (s * r.m_Y);
    matrix.m_Table[1][2] = (ic * r.m_Z * r.m_Y) + (s * r.m_X);
    matrix.m_Table[2][2] = (ic * r.m_Z * r.m_Z) + c;

    // combine current matrix with rotation matrix
    *this = matrix.Multiply(*this);

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Matrix16<T>::Scale(const QR_Vector3D<T>& s)
{
    m_Table[0][0] *= s.m_X; m_Table[1][0] *= s.m_Y; m_Table[2][0] *= s.m_Z;
    m_Table[0][1] *= s.m_X; m_Table[1][1] *= s.m_Y; m_Table[2][1] *= s.m_Z;
    m_Table[0][2] *= s.m_X; m_Table[1][2] *= s.m_Y; m_Table[2][2] *= s.m_Z;
    m_Table[0][3] *= s.m_X; m_Table[1][3] *= s.m_Y; m_Table[2][3] *= s.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
inline QR_Matrix16<T> QR_Matrix16<T>::Swap() const
{
    return QR_Matrix16<T>(m_Table[0][0], m_Table[1][0], m_Table[2][0], m_Table[3][0],
                          m_Table[0][1], m_Table[1][1], m_Table[2][1], m_Table[3][1],
                          m_Table[0][2], m_Table[1][2], m_Table[2][2], m_Table[3][2],
                          m_Table[0][3], m_Table[1][3], m_Table[2][3], m_Table[3][3]);
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Matrix16<T>::Transform(const QR_Vector3D<T>& vector) const
{
    // calculates x, y and z coordinates (don't use w component), and returns transformed vector
    return QR_Vector3D<T>
            ((vector.m_X * m_Table[0][0] + vector.m_Y * m_Table[1][0] + vector.m_Z * m_Table[2][0] + m_Table[3][0]),
             (vector.m_X * m_Table[0][1] + vector.m_Y * m_Table[1][1] + vector.m_Z * m_Table[2][1] + m_Table[3][1]),
             (vector.m_X * m_Table[0][2] + vector.m_Y * m_Table[1][2] + vector.m_Z * m_Table[2][2] + m_Table[3][2]));
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Matrix16<T>::TransformNormal(const QR_Vector3D<T>& normal) const
{
    return QR_Vector3D<T>
            ((normal.m_X * m_Table[0][0]) + normal.m_Y * m_Table[1][0] + (normal.m_Z * m_Table[2][0]),
             (normal.m_X * m_Table[0][1]) + normal.m_Y * m_Table[1][1] + (normal.m_Z * m_Table[2][1]),
             (normal.m_X * m_Table[0][2]) + normal.m_Y * m_Table[1][2] + (normal.m_Z * m_Table[2][2]));
}
//---------------------------------------------------------------------------
template <class T>
const T* QR_Matrix16<T>::GetPtr() const
{
    return &m_Table[0][0];
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
