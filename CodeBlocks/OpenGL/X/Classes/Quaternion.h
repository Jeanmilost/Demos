/****************************************************************************
 * ==> Quaternion ----------------------------------------------------------*
 ****************************************************************************
 * Description : Quaternion                                                 *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - x model reader                                             *
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

#pragma once

// std
#include <algorithm>

// retrograde engine
#include "Vector3.h"
#include "Matrix4x4.h"

/**
* Quaternion
*@author Jean-Milost Reymond
*/
template <class T>
class Quaternion
{
    public:
        T m_X; // quaternion x coordinate
        T m_Y; // quaternion y coordinate
        T m_Z; // quaternion z coordinate
        T m_W; // quaternion w coordinate

        inline Quaternion();

        /**
        * Constructor
        *@param x - x coordinate
        *@param y - y coordinate
        *@param z - z coordinate
        *@param w - w coordinate
        */
        inline Quaternion(T x, T y, T z, T w);

        /**
        * Copy constructor
        *@param other - other quaternion to copy from
        */
        inline Quaternion(const Quaternion& other);

        virtual ~Quaternion();

        /**
        * Copy operator
        *@param other - other vector to copy from
        *@return this vector
        */
        virtual inline Quaternion& operator = (const Quaternion& other);

        /**
        * Negation operator
        *@return inverted quaternion
        *@note BE CAREFUL This is a mathematical inversion, to invert the quaternion,
        *      use the Inverse() function insetad
        */
        virtual inline Quaternion operator - () const;

        /**
        * Equality operator
        *@param value - value to compare
        *@return true if values are identical, otherwise false
        */
        virtual inline bool operator == (const Quaternion& value) const;

        /**
        * Not equality operator
        *@param value - value to compare
        *@return true if values are not identical, otherwise false
        */
        virtual inline bool operator != (const Quaternion& value) const;

        /**
        * Initializes the quaternion to its identity value
        */
        virtual inline Quaternion Identity() const;

        /**
        * Builds the quaternion from an angle and a vector representing a rotation axis
        *@param angle - rotation angle
        *@param pAxis - rotation axis
        */
        virtual inline void FromAxis(T angle, const Vector3<T>& axis);

        /**
        * Builds the quaternion from Euler angles
        *@param angleX - rotation angle on x axis
        *@param angleY - rotation angle on y axis
        *@param angleZ - rotation angle on z axis
        */
        virtual inline void FromEuler(T angleX, T angleY, T angleZ);

        /**
        * Builds the quaternion from a pitch, yaw and roll angles
        *@param pitch - the pitch angle in radians, pitch is comparable to a head up/down movement
        *@param yaw - the yaw angle in radians, yaw is comparable to a head left/right movement
        *@param roll - the roll angle in radians, roll is comparable to an aircraft rolling movement
        */
        virtual inline void FromPitchYawRoll(T pitch, T yaw, T roll);

        /**
        * Gets the quaternion squared length
        *@return the quaternion squared length
        */
        virtual inline T LengthSquared() const;

        /**
        * Gets the quaternion length
        *@return the quaternion length
        */
        virtual inline T Length() const;

        /**
        * Normalizes the quaternion
        */
        virtual inline Quaternion Normalize() const;

        /**
        * Calculates the dot product between 2 quaternions
        *@param other - other quaternion to dot with
        *@return the resulting angle
        */
        virtual inline T Dot(const Quaternion& other) const;

        /**
        * Scales a quaternion
        *@param s - scale factor
        *@return scaled quaternion
        */
        virtual inline Quaternion Scale(T s) const;

        /**
        * Conjugates the quaternion
        *@return conjugated quaternion
        */
        virtual inline Quaternion Conjugate() const;

        /**
        * Multiplies a quaternion by another
        *@param other - other quaternion to multiply with
        *@param[out] pR - multiplied quaternion
        */
        virtual inline Quaternion Multiply(const Quaternion& other) const;

        /**
        * Inverses the quaternion
        *@return inverted quaternion
        */
        virtual inline Quaternion Inverse() const;

        /**
        * Rotates a vector by the quaternion
        *@param vector - vector to rotate
        *@return rotated vector
        */
        virtual inline Vector3<T> Rotate(const Vector3<T>& vector) const;

        /**
        * Gets the spherical linear interpolated quaternion between 2 quaternions
        *@param other - quaternion to interpolate with
        *@param p - interpolation position, in percent (between 0.0f and 1.0f)
        *@param[out] error - if true, an error happened while the interpolation was calculated
        *@return the resulting spherical linear interpolated quaternion
        */
        virtual inline Quaternion Slerp(const Quaternion& other, T p, bool& error) const;

        /**
        * Gets a quaternion from a matrix
        *@param matrix - the matrix
        *@param[out] error - if true, an error happened while the matrix was calculated
        *@return the quaternion
        */
        virtual inline Quaternion FromMatrix(const Matrix4x4<T>& matrix, bool& error) const;

        /**
        * Gets a rotation matrix from the quaternion
        *@return the rotation matrix
        */
        virtual inline Matrix4x4<T> ToMatrix() const;
};

typedef Quaternion<float>  QuaternionF;
typedef Quaternion<double> QuaternionD;

//---------------------------------------------------------------------------
// Quaternion functions
//---------------------------------------------------------------------------
template <class T>
Quaternion<T>::Quaternion() :
    m_X(T(0.0)),
    m_Y(T(0.0)),
    m_Z(T(0.0)),
    m_W(T(1.0))
{}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T>::Quaternion(T x, T y, T z, T w) :
    m_X(x),
    m_Y(y),
    m_Z(z),
    m_W(w)
{}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T>::Quaternion(const Quaternion& other) :
    m_X(other.m_X),
    m_Y(other.m_Y),
    m_Z(other.m_Z),
    m_W(other.m_W)
{}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T>::~Quaternion()
{}
//---------------------------------------------------------------------------
template<class T>
Quaternion<T>& Quaternion<T>::operator = (const Quaternion& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;
    m_Z = other.m_Z;
    m_W = other.m_W;

    return *this;
}
//---------------------------------------------------------------------------
template<class T>
Quaternion<T> Quaternion<T>::operator - () const
{
    return Quaternion<T>(-m_X, -m_Y, -m_Z, -m_W);
}
//---------------------------------------------------------------------------
template<class T>
bool Quaternion<T>::operator == (const Quaternion& value) const
{
    return (m_X == value.m_X && m_Y == value.m_Y && m_Z == value.m_Z && m_W == value.m_W);
}
//---------------------------------------------------------------------------
template<class T>
bool Quaternion<T>::operator !=(const Quaternion& value) const
{
    return (m_X != value.m_X || m_Y != value.m_Y || m_Z != value.m_Z || m_W != value.m_W);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Identity() const
{
    return Quaternion();
}
//---------------------------------------------------------------------------
template <class T>
void Quaternion<T>::FromAxis(T angle, const Vector3<T>& axis)
{
    const T s = std::sin(angle * T(0.5));

    m_X = (axis.m_X * s);
    m_Y = (axis.m_Y * s);
    m_Z = (axis.m_Z * s);
    m_W = std::cos(angle * T(0.5));
}
//---------------------------------------------------------------------------
template <class T>
void Quaternion<T>::FromEuler(T angleX, T angleY, T angleZ)
{
    const T cos_x_2 = std::cos(T(0.5) * angleX);
    const T cos_y_2 = std::cos(T(0.5) * angleY);
    const T cos_z_2 = std::cos(T(0.5) * angleZ);

    const T sin_x_2 = std::sin(T(0.5) * angleX);
    const T sin_y_2 = std::sin(T(0.5) * angleY);
    const T sin_z_2 = std::sin(T(0.5) * angleZ);

    // create the quaternion
    m_X = (cos_z_2 * cos_y_2 * sin_x_2) - (sin_z_2 * sin_y_2 * cos_x_2);
    m_Y = (sin_z_2 * cos_y_2 * sin_x_2) + (cos_z_2 * sin_y_2 * cos_x_2);
    m_Z = (sin_z_2 * cos_y_2 * cos_x_2) - (cos_z_2 * sin_y_2 * sin_x_2);
    m_W = (cos_z_2 * cos_y_2 * cos_x_2) + (sin_z_2 * sin_y_2 * sin_x_2);
}
//---------------------------------------------------------------------------
template <class T>
void Quaternion<T>::FromPitchYawRoll(T pitch, T yaw, T roll)
{
    // calculate the sinus and cosinus of each angles
    const T c1 = std::cos(T(0.5) * yaw);
    const T c2 = std::cos(T(0.5) * pitch);
    const T c3 = std::cos(T(0.5) * roll);
    const T s1 = std::sin(T(0.5) * yaw);
    const T s2 = std::sin(T(0.5) * pitch);
    const T s3 = std::sin(T(0.5) * roll);

    // calculate the quaternion values
    m_X = (s1 * s2 * c3) + (c1 * c2 * s3);
    m_Y = (s1 * c2 * c3) + (c1 * s2 * s3);
    m_Z = (c1 * s2 * c3) - (s1 * c2 * s3);
    m_W = (c1 * c2 * c3) - (s1 * s2 * s3);
}
//---------------------------------------------------------------------------
template <class T>
T Quaternion<T>::LengthSquared() const
{
    return ((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z) + (m_W * m_W));
}
//---------------------------------------------------------------------------
template <class T>
T Quaternion<T>::Length() const
{
    const T norm = LengthSquared();

    return sqrt(norm);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Normalize() const
{
    const T len = Length();

    if (!len)
        return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));

    return Quaternion((m_X / len), (m_Y / len), (m_Z / len), (m_W / len));
}
//---------------------------------------------------------------------------
template <class T>
T Quaternion<T>::Dot(const Quaternion& other) const
{
    return ((m_X * other.m_X) + (m_Y * other.m_Y) + (m_Z * other.m_Z) + (m_W * other.m_W));
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Scale(T s) const
{
    return Quaternion(m_X * s, m_Y * s, m_Z * s, m_W * s);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Conjugate() const
{
    return Quaternion(-m_X, -m_Y, -m_Z, m_W);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Multiply(const Quaternion& other) const
{
    return Quaternion(m_W * other.m_X + m_X * other.m_W + m_Y * other.m_Z - m_Z * other.m_Y,
                      m_W * other.m_Y - m_X * other.m_Z + m_Y * other.m_W + m_Z * other.m_X,
                      m_W * other.m_Z + m_X * other.m_Y - m_Y * other.m_X + m_Z * other.m_W,
                      m_W * other.m_W - m_X * other.m_X - m_Y * other.m_Y - m_Z * other.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Inverse() const
{
    // calculate the squared length of the quaternion
    const T lengthSq = LengthSquared();

    // empty quaternion?
    if (!lengthSq)
        return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));

    return Conjugate().Scale(T(1.0) / lengthSq);
}
//---------------------------------------------------------------------------
template <class T>
Vector3<T> Quaternion<T>::Rotate(const Vector3<T>& vector) const
{
    const Quaternion qv(vector.m_X, vector.m_Y, vector.m_Z, T(0.0));
    const Quaternion qi = Inverse();

    // rotate vector
    const Quaternion qm(m_X * qv.m_X * qi.m_X,
                        m_Y * qv.m_Y * qi.m_Y,
                        m_Z * qv.m_Z * qi.m_Z,
                        m_W * qv.m_W * qi.m_W);

    return Vector3<T>(qm.m_X, qm.m_Y, qm.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::Slerp(const Quaternion& other, T p, bool& error) const
{
    error = false;

    // are quaternions identical?
    if (m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z && m_W == other.m_W)
        return *this;

    // calculate dot product between q1 and q2
    T result = Dot(other);

    Quaternion interpolateWith;

    // check if angle is higher than 90° (this happen if dot product is less than 0)
    if (result < T(0.0))
    {
        // negate the second quaternion and the dot product result
        interpolateWith = -other;
        result          = -result;
    }
    else
        interpolateWith = other;

    // calculate the first and second scaling factor to apply to the interpolation
    T scale0 = T(1.0) - p;
    T scale1 =          p;

    // is angle large enough to apply the calculation?
    if ((T(1.0) - result) > T(0.1))
    {
        // calculate the angle between the 2 quaternions and get the sinus of that angle
        const T theta    = std::acos(result);
        const T sinTheta = std::sin(theta);

        // is resulting sinus equal to 0? (just to verify, should not happen)
        if (!sinTheta)
        {
            error = true;
            return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));
        }

        // calculate the scaling for q1 and q2, according to the angle and it's sine value
        scale0 = std::sin((T(1.0) - p) * theta) / sinTheta;
        scale1 = std::sin(          p  * theta) / sinTheta;
    }

    // calculate the resulting quaternion by using a special form of linear interpolation
    return Quaternion((scale0 * m_X) + (scale1 * interpolateWith.m_X),
                      (scale0 * m_Y) + (scale1 * interpolateWith.m_Y),
                      (scale0 * m_Z) + (scale1 * interpolateWith.m_Z),
                      (scale0 * m_W) + (scale1 * interpolateWith.m_W));
}
//---------------------------------------------------------------------------
template <class T>
Quaternion<T> Quaternion<T>::FromMatrix(const Matrix4x4<T>& matrix, bool& error) const
{
    error = false;

    // calculate the matrix diagonal by adding up it's diagonal indices (also known as "trace")
    const T diagonal = matrix.m_Table[0][0] + matrix.m_Table[1][1] + matrix.m_Table[2][2] + matrix.m_Table[3][3];

    // is diagonal greater than zero?
    if (diagonal > T(0.00000001))
    {
        // calculate the diagonal scale
        const T scale = sqrt(diagonal) * T(2.0);

        // calculate the quaternion values using the respective equation
        return Quaternion((matrix.m_Table[1][2] - matrix.m_Table[2][1]) / scale,
                          (matrix.m_Table[2][0] - matrix.m_Table[0][2]) / scale,
                          (matrix.m_Table[0][1] - matrix.m_Table[1][0]) / scale,
                          T(0.25) * scale);
    }

    // search for highest value in the matrix diagonal
    if (matrix.m_Table[0][0] > matrix.m_Table[1][1] && matrix.m_Table[0][0] > matrix.m_Table[2][2])
    {
        // calculate scale using the first diagonal element and double that value
        const T scale = sqrt(1.0f + matrix.m_Table[0][0] - matrix.m_Table[1][1] - matrix.m_Table[2][2]) * 2.0f;

        // should not happen, but better to verify
        if (!scale)
        {
            error = true;
            return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));
        }

        // calculate the quaternion values using the respective equation
        return Quaternion(T(0.25) * scale,
                          (matrix.m_Table[0][1] + matrix.m_Table[1][0]) / scale,
                          (matrix.m_Table[2][0] + matrix.m_Table[0][2]) / scale,
                          (matrix.m_Table[1][2] - matrix.m_Table[2][1]) / scale);
    }

    if (matrix.m_Table[1][1] > matrix.m_Table[2][2])
    {
        // calculate scale using the second diagonal element and double that value
        const T scale = sqrt(1.0f + matrix.m_Table[1][1] - matrix.m_Table[0][0] - matrix.m_Table[2][2]) * 2.0f;

        // should not happen, but better to verify
        if (!scale)
        {
            error = true;
            return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));
        }

        // calculate the quaternion values using the respective equation
        return Quaternion((matrix.m_Table[0][1] + matrix.m_Table[1][0]) / scale,
                          0.25f * scale,
                          (matrix.m_Table[1][2] + matrix.m_Table[2][1]) / scale,
                          (matrix.m_Table[2][0] - matrix.m_Table[0][2]) / scale);
    }

    // calculate scale using the third diagonal element and double that value
    const T scale = sqrt(1.0f + matrix.m_Table[2][2] - matrix.m_Table[0][0] - matrix.m_Table[1][1]) * 2.0f;

    // should not happen, but better to verify
    if (!scale)
    {
        error = true;
        return Quaternion(T(0.0), T(0.0), T(0.0), T(0.0));
    }

    // calculate the quaternion values using the respective equation
    return Quaternion((matrix.m_Table[2][0] + matrix.m_Table[0][2]) / scale,
                      (matrix.m_Table[1][2] + matrix.m_Table[2][1]) / scale,
                      0.25f * scale,
                      (matrix.m_Table[0][1] - matrix.m_Table[1][0]) / scale);
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Quaternion<T>::ToMatrix() const
{
    return Matrix4x4<T>(T(1.0) - T(2.0) * (m_Y * m_Y + m_Z * m_Z),          T(2.0) * (m_X * m_Y + m_Z * m_W),          T(2.0) * (m_X * m_Z - m_Y * m_W), T(0.0),
                                 T(2.0) * (m_X * m_Y - m_Z * m_W), T(1.0) - T(2.0) * (m_X * m_X + m_Z * m_Z),          T(2.0) * (m_Y * m_Z + m_X * m_W), T(0.0),
                                 T(2.0) * (m_X * m_Z + m_Y * m_W),          T(2.0) * (m_Y * m_Z - m_X * m_W), T(1.0) - T(2.0) * (m_X * m_X + m_Y * m_Y), T(0.0),
                        T(0.0),                                    T(0.0),                                    T(0.0),                                    T(1.0));
}
//---------------------------------------------------------------------------
