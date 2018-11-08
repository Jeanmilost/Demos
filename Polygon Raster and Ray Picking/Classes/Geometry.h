/****************************************************************************
 * ==> Geometry ------------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides geometry classes and functions        *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#ifndef GeometryH
#define GeometryH

/**
* Vector
*@author Jean-Milost Reymond
*/
template <class T>
struct Vector3D
{
    T m_X; // x coordinate for the 3D vector
    T m_Y; // y coordinate for the 3D vector
    T m_Z; // z coordinate for the 3D vector

    /**
    * Constructor
    */
    inline Vector3D();

    /**
    * Constructor
    *@param x - vector x value
    *@param y - vector y value
    *@param z - vector z value
    */
    inline Vector3D(T x, T y, T z);

    /**
    * Copy constructor
    *@param other - other vector to copy from
    */
    inline Vector3D(const Vector3D& other);

    /**
    * Destructor
    */
    inline ~Vector3D();

    /**
    * Copy operator
    *@param other - other vector to copy from
    *@return this vector
    */
    inline Vector3D& operator =(const Vector3D& other);

    /**
    * Operator + for single value
    *@param value - value to add
    *@return resulting added vector
    */
    inline Vector3D operator + (T value) const;

    /**
    * Operator +
    *@param other - other vector to add
    *@return resulting added vector
    */
    inline Vector3D operator + (const Vector3D& other) const;

    /**
    * Operator -
    *@return inverted vector
    */
    inline Vector3D operator - () const;

    /**
    * Operator - for single value
    *@param value - value to subtract
    *@return resulting subtracted vector
    */
    inline Vector3D operator - (T value) const;

    /**
    * Operator -
    *@param other - other vector to subtract
    *@return resulting subtracted vector
    */
    inline Vector3D operator - (const Vector3D& other) const;

    /**
    * Operator * for single value
    *@param value - value to multiply
    *@return resulting multiplied vector
    */
    inline Vector3D operator * (T value) const;

    /**
    * Operator *
    *@param other - other vector to multiply
    *@return resulting multiplied vector
    */
    inline Vector3D operator * (const Vector3D& other) const;

    /**
    * Operator / for single value
    *@param value - value to divide
    *@return resulting divided vector
    */
    inline Vector3D operator / (T value) const;

    /**
    * Operator /
    *@param other - other vector to divide
    *@return resulting divided vector
    */
    inline Vector3D operator / (const Vector3D& other) const;

    /**
    * Operator += for single value
    *@param value - value to add
    *@return resulting added vector
    */
    inline const Vector3D& operator += (T value);

    /**
    * Operator +=
    *@param other - other vector to add
    *@return resulting added vector
    */
    inline const Vector3D& operator += (const Vector3D& other);

    /**
    * Operator -= for single value
    *@param value - value to subtract
    *@return resulting subtracted vector
    */
    inline const Vector3D& operator -= (T value);

    /**
    * Operator -=
    *@param other - other vector to subtract
    *@return resulting subtracted vector
    */
    inline const Vector3D& operator -= (const Vector3D& other);

    /**
    * Operator *= for single value
    *@param value - value to multiply
    *@return resulting multiplied vector
    */
    inline const Vector3D& operator *= (T value);

    /**
    * Operator *=
    *@param other - other vector to multiply
    *@return resulting multiplied vector
    */
    inline const Vector3D& operator *= (const Vector3D& other);

    /**
    * Operator /= for single value
    *@param value - value to divide
    *@return resulting divided vector
    */
    inline const Vector3D& operator /= (T value);

    /**
    * Operator /=
    *@param other - other vector to divide
    *@return resulting divided vector
    */
    inline const Vector3D& operator /= (const Vector3D& other);

    /**
    * Operator ==
    *@param other - other vector to compare
    *@return true if vectors are identical, otherwise false
    */
    inline bool operator == (const Vector3D& other) const;

    /**
    * Operator !=
    *@param other - other vector to compare
    *@return true if vectors are not identical, otherwise false
    */
    inline bool operator != (const Vector3D& other) const;

    /**
    * Copies vector
    *@param other - other vector to copy from
    */
    inline void Copy(const Vector3D& other);

    /**
    * Checks if vector is set to [0, 0, 0] coordinate
    *@return true if vector is set to [0, 0, 0] coordinate, otherwise false
    */
    inline bool IsZero() const;

    /**
    * Gets the vector length
    *@return the vector length
    */
    inline T Length() const;

    /**
    * Normalizes the vector
    *@return normalized vector
    */
    inline Vector3D Normalize() const;

    /**
    * Calculates the cross product between 2 vectors
    *@param vector - vector with which cross product is calculated
    *@return cross product
    */
    inline Vector3D Cross(const Vector3D& vector) const;

    /**
    * Calculates the dot product between 2 vectors
    *@param vector - vector with which dot product is calculated
    *@return dot product
    */
    inline T Dot(const Vector3D& vector) const;

    /**
    * Calculates interpolation vector between 2 vectors
    *@param other - other vector to interpolate with
    *@param position - interpolation position, in percent (between 0.0f and 1.0f)
    *@return interpolation vector
    */
    inline Vector3D Interpolate(const Vector3D& other, T position) const;
};

typedef Vector3D<float>  Vector3DF;
typedef Vector3D<double> Vector3DD;

//---------------------------------------------------------------------------
// Vector3D
//---------------------------------------------------------------------------
template <class T>
Vector3D<T>::Vector3D() :
    m_X(T(0.0)),
    m_Y(T(0.0)),
    m_Z(T(0.0))
{}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T>::Vector3D(T x, T y, T z) :
    m_X(x),
    m_Y(y),
    m_Z(z)
{}
//---------------------------------------------------------------------------
template<class T>
Vector3D<T>::Vector3D(const Vector3D& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T>::~Vector3D()
{}
//---------------------------------------------------------------------------
template<class T>
Vector3D<T>& Vector3D<T>::operator =(const Vector3D& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator + (T value) const
{
    return Vector3D<T>(m_X + value, m_Y + value, m_Z + value);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator + (const Vector3D<T>& other) const
{
    return Vector3D<T>(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator - () const
{
    return Vector3D<T>(-m_X, -m_Y, -m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator - (T value) const
{
    return Vector3D<T>(m_X - value, m_Y - value, m_Z - value);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator - (const Vector3D<T>& other) const
{
    return Vector3D<T>(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator * (T value) const
{
    return Vector3D<T>(m_X * value, m_Y * value, m_Z * value);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator * (const Vector3D<T>& other) const
{
    return Vector3D<T>(m_X * other.m_X, m_Y * other.m_Y, m_Z * other.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator / (T value) const
{
    if (!value)
        M_THROW_EXCEPTION("Division by 0 is prohibited");

    return Vector3D<T>(m_X / value, m_Y / value, m_Z / value);
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::operator / (const Vector3D<T>& other) const
{
    if (!other.m_X)
        M_THROW_EXCEPTION("Vector x axis - division by 0 is prohibited");

    if (!other.m_Y)
        M_THROW_EXCEPTION("Vector y axis - division by 0 is prohibited");

    if (!other.m_Z)
        M_THROW_EXCEPTION("Vector z axis - division by 0 is prohibited");

    return Vector3D<T>(m_X / other.m_X, m_Y / other.m_Y, m_Z / other.m_Z);
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator += (T value)
{
    m_X += value;
    m_Y += value;
    m_Z += value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator += (const Vector3D<T>& other)
{
    m_X += other.m_X;
    m_Y += other.m_Y;
    m_Z += other.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator -= (T value)
{
    m_X -= value;
    m_Y -= value;
    m_Z -= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator -= (const Vector3D<T>& other)
{
    m_X -= other.m_X;
    m_Y -= other.m_Y;
    m_Z -= other.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator *= (T value)
{
    m_X *= value;
    m_Y *= value;
    m_Z *= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator *= (const Vector3D<T>& other)
{
    m_X *= other.m_X;
    m_Y *= other.m_Y;
    m_Z *= other.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator /= (T value)
{
    if (!value)
        M_THROW_EXCEPTION("Division by 0 is prohibited");

    m_X /= value;
    m_Y /= value;
    m_Z /= value;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
const Vector3D<T>& Vector3D<T>::operator /= (const Vector3D<T>& other)
{
    if (!other.m_X)
        M_THROW_EXCEPTION("Vector x axis - division by 0 is prohibited");

    if (!other.m_Y)
        M_THROW_EXCEPTION("Vector y axis - division by 0 is prohibited");

    if (!other.m_Z)
        M_THROW_EXCEPTION("Vector z axis - division by 0 is prohibited");

    m_X /= other.m_X;
    m_Y /= other.m_Y;
    m_Z /= other.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool Vector3D<T>::operator == (const Vector3D<T>& other) const
{
    return ((m_X == other.m_X) && (m_Y == other.m_Y) && (m_Z == other.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
bool Vector3D<T>::operator != (const Vector3D<T>& other) const
{
    return ((m_X != other.m_X) || (m_Y != other.m_Y) || (m_Z != other.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
void Vector3D<T>::Copy(const Vector3D& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;
    m_Z = other.m_Z;
}
//---------------------------------------------------------------------------
template <class T>
bool Vector3D<T>::IsZero() const
{
    return !m_X && !m_Y && !m_Z;
}
//---------------------------------------------------------------------------
template <class T>
T Vector3D<T>::Length() const
{
    return sqrt((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z));
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::Normalize() const
{
    const T len = Length();

    if (len == T(0.0))
        return Vector3D<T>();

    return Vector3D<T>((m_X / len), (m_Y / len), (m_Z / len));
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::Cross(const Vector3D<T>& vector) const
{
    return Vector3D<T>((m_Y * vector.m_Z) - (vector.m_Y * m_Z),
                       (m_Z * vector.m_X) - (vector.m_Z * m_X),
                       (m_X * vector.m_Y) - (vector.m_X * m_Y));
}
//---------------------------------------------------------------------------
template <class T>
T Vector3D<T>::Dot(const Vector3D<T>& vector) const
{
    return ((m_X * vector.m_X) + (m_Y * vector.m_Y) + (m_Z * vector.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Vector3D<T>::Interpolate(const Vector3D<T>& other, T position) const
{
    // is position out of bounds? Limit to min or max values in this case
    if (position < T(0.0))
        return *this;
    else
    if (position > T(1.0))
        return other;

    Vector3D<T> interpolation;

    // calculate interpolation
    interpolation.m_X = m_X + position * (other.m_X - m_X);
    interpolation.m_Y = m_Y + position * (other.m_Y - m_Y);
    interpolation.m_Z = m_Z + position * (other.m_Z - m_Z);

    return interpolation;
}
//---------------------------------------------------------------------------

/**
* 4x4 matrix
*@note The matrix table can be read in both mathematical or in-memory order. From a mathematical
*      point of view, the m_Table content should be read as m_Table[x][y], whereas it should be
*      read as m_Table[y][x] from the in-memory point of view. The both representation are
*      equivalent and interchangeable, and may be used anytime if the same logical is used
*      constantly, it's e.g. the reason why the GetPtr() function may be used to connect to
*      OpenGL functions, although the matrix was initialized following a mathematical
*      representation. However, this may be highly confusing when you have to manipulate the
*      source code, because the table items may appear to be crossed. For that reason, be aware
*      of what you are doing if you have to modify the source code
*@author Jean-Milost Reymond
*/
template <class T>
struct Matrix4x4
{
    T m_Table[4][4]; // 4x4 matrix array

    /**
    * Constructor
    */
    inline Matrix4x4();

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
    inline Matrix4x4(T _11, T _12, T _13, T _14,
                     T _21, T _22, T _23, T _24,
                     T _31, T _32, T _33, T _34,
                     T _41, T _42, T _43, T _44);

    /**
    * Copy constructor
    *@param other - other matrix to copy from
    */
    inline Matrix4x4(const Matrix4x4& other);

    /**
    * Destructor
    */
    inline ~Matrix4x4();

    /**
    * Assignation operator
    *@param other - other matrix to copy from
    */
    inline Matrix4x4& operator =(const Matrix4x4& other);

    /**
    * Equality operator
    *@param other - other matrix to compare
    *@return true if both matrix are equals, otherwise false
    */
    inline bool operator ==(const Matrix4x4& other);

    /**
    * Not equality operator
    *@param other - other matrix to compare
    *@return true if both matrix are not equals, otherwise false
    */
    inline bool operator !=(const Matrix4x4& other);

    /**
    * Copies matrix from another
    *@param other - other matrix to copy from
    */
    inline void Copy(const Matrix4x4& other);

    /**
    * Checks if matrix and other matrix are equals
    *@param other - other matrix to compare
    *@return true if matrix are equals, otherwise false
    */
    bool IsEqual(const Matrix4x4& other) const;

    /**
    * Checks if matrix is an identity matrix
    *@return true if matrix is an identity matrix, otherwise false
    */
    inline bool IsIdentity() const;

    /**
    * Gets an identity matrix
    *@return identity matrix
    */
    static inline Matrix4x4 Identity();

    /**
    * Inverses a matrix
    *@param[out] determinant - matrix determinant
    */
    Matrix4x4 Inverse(T& determinant);

    /**
    * Multiplies matrix by another matrix
    *@param other - other matrix to multiply with
    *@return multiplied resulting matrix
    */
    Matrix4x4 Multiply(const Matrix4x4<T>& other) const;

    /**
    * Translates matrix
    *@param t - translation vector
    *@return copy of translated matrix
    */
    inline Matrix4x4 Translate(const Vector3D<T>& t);

    /**
    * Rotates matrix
    *@param angle - rotation angle in radians
    *@param r - rotation direction (e.g. {0.0f, 0.0f, 1.0f} for a z-axis rotation)
    *@return copy of rotated matrix
    *@note rotation direction vector should be normalized before calling
    *      this function
    */
    inline Matrix4x4 Rotate(T angle, const Vector3D<T>& r);

    /**
    * Scales matrix
    *@param s - scale vector
    *@return copy of scaled matrix
    */
    inline Matrix4x4 Scale(const Vector3D<T>& s);

    /**
    * Applies a transformation matrix to a vector
    *@param vector - vector to transform
    *@return transformed vector
    */
    inline Vector3D<T> Transform(const Vector3D<T>& vector) const;

    /**
    * Applies a transformation matrix to a normal
    *@param normal - normal to transform
    *@return transformed normal
    */
    inline Vector3D<T> TransformNormal(const Vector3D<T>& normal) const;

    /**
    * Gets table pointer
    *@return pointer
    */
    inline const T* GetPtr() const;
};

typedef Matrix4x4<float>  Matrix4x4F;
typedef Matrix4x4<double> Matrix4x4D;

//---------------------------------------------------------------------------
// Matrix4x4
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T>::Matrix4x4()
{
    // initialize matrix table
    m_Table[0][0] = 1.0f; m_Table[1][0] = 0.0f; m_Table[2][0] = 0.0f; m_Table[3][0] = 0.0f;
    m_Table[0][1] = 0.0f; m_Table[1][1] = 1.0f; m_Table[2][1] = 0.0f; m_Table[3][1] = 0.0f;
    m_Table[0][2] = 0.0f; m_Table[1][2] = 0.0f; m_Table[2][2] = 1.0f; m_Table[3][2] = 0.0f;
    m_Table[0][3] = 0.0f; m_Table[1][3] = 0.0f; m_Table[2][3] = 0.0f; m_Table[3][3] = 1.0f;
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T>::Matrix4x4(T _11, T _12, T _13, T _14,
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
Matrix4x4<T>::Matrix4x4(const Matrix4x4& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T>::~Matrix4x4()
{}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T>& Matrix4x4<T>::operator =(const Matrix4x4& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
template <class T>
bool Matrix4x4<T>::operator ==(const Matrix4x4& other)
{
    return IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
bool Matrix4x4<T>::operator !=(const Matrix4x4& other)
{
    return !IsEqual(other);
}
//---------------------------------------------------------------------------
template <class T>
void Matrix4x4<T>::Copy(const Matrix4x4& other)
{
    // copy matrix table from other
    std::memcpy(m_Table, other.m_Table, sizeof(m_Table));
}
//---------------------------------------------------------------------------
template <class T>
bool Matrix4x4<T>::IsEqual(const Matrix4x4& other) const
{
    // compare each matrix element with other matrix. NOTE don't use std::memcmp here because the
    // comparison will fail in the following cases:
    // 1. if 0.0f == -0.0f, the equality operator will return true whereas the memory cmp will fail
    // 2. if NaN, the memory compare will always fail
    for (UInt8 i = 0; i < 4; ++i)
        for (UInt8 j = 0; j < 4; ++j)
            if (m_Table[i][j] != other.m_Table[i][j])
                return false;

    return true;
}
//---------------------------------------------------------------------------
template <class T>
bool Matrix4x4<T>::IsIdentity() const
{
    return IsEqual(Identity());
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Matrix4x4<T>::Identity()
{
    return Matrix4x4<T>(T(1.0), T(0.0), T(0.0), T(0.0),
                        T(0.0), T(1.0), T(0.0), T(0.0),
                        T(0.0), T(0.0), T(1.0), T(0.0),
                        T(0.0), T(0.0), T(0.0), T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Matrix4x4<T>::Inverse(T& determinant)
{
    T t[3];
    T v[16];

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

    if (determinant == T(0.0))
        return Matrix4x4<T>();

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

    const T invDet = T(1.0) / determinant;

    Matrix4x4<T> result;

    for (UInt8 i = 0; i < 4; ++i)
        for (UInt8 j = 0; j < 4; ++j)
            result.m_Table[i][j] = v[4 * i + j] * invDet;

    return result;
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Matrix4x4<T>::Multiply(const Matrix4x4<T>& other) const
{
    Matrix4x4<T> matrix;

    for (UInt8 i = 0; i < 4; ++i)
        for (UInt8 j = 0; j < 4; ++j)
            matrix.m_Table[i][j] = m_Table[i][0] * other.m_Table[0][j] +
                                   m_Table[i][1] * other.m_Table[1][j] +
                                   m_Table[i][2] * other.m_Table[2][j] +
                                   m_Table[i][3] * other.m_Table[3][j];

    return matrix;
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Matrix4x4<T>::Translate(const Vector3D<T>& t)
{
    m_Table[3][0] += m_Table[0][0] * t.m_X + m_Table[1][0] * t.m_Y + m_Table[2][0] * t.m_Z;
    m_Table[3][1] += m_Table[0][1] * t.m_X + m_Table[1][1] * t.m_Y + m_Table[2][1] * t.m_Z;
    m_Table[3][2] += m_Table[0][2] * t.m_X + m_Table[1][2] * t.m_Y + m_Table[2][2] * t.m_Z;
    m_Table[3][3] += m_Table[0][3] * t.m_X + m_Table[1][3] * t.m_Y + m_Table[2][3] * t.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Matrix4x4<T>::Rotate(T angle, const Vector3D<T>& r)
{
    // calculate sinus, cosinus and inverted cosinus values
    const T c  = std::cos(angle);
    const T s  = std::sin(angle);
    const T ic = (T(1.0) - c);

    // create rotation matrix
    Matrix4x4<T> matrix = Identity();
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
Matrix4x4<T> Matrix4x4<T>::Scale(const Vector3D<T>& s)
{
    m_Table[0][0] *= s.m_X; m_Table[1][0] *= s.m_Y; m_Table[2][0] *= s.m_Z;
    m_Table[0][1] *= s.m_X; m_Table[1][1] *= s.m_Y; m_Table[2][1] *= s.m_Z;
    m_Table[0][2] *= s.m_X; m_Table[1][2] *= s.m_Y; m_Table[2][2] *= s.m_Z;
    m_Table[0][3] *= s.m_X; m_Table[1][3] *= s.m_Y; m_Table[2][3] *= s.m_Z;

    return *this;
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Matrix4x4<T>::Transform(const Vector3D<T>& vector) const
{
    // calculate x, y and z coordinates (don't use w component), and return transformed vector
    return Vector3D<T>
            ((vector.m_X * m_Table[0][0] + vector.m_Y * m_Table[1][0] + vector.m_Z * m_Table[2][0] + m_Table[3][0]),
             (vector.m_X * m_Table[0][1] + vector.m_Y * m_Table[1][1] + vector.m_Z * m_Table[2][1] + m_Table[3][1]),
             (vector.m_X * m_Table[0][2] + vector.m_Y * m_Table[1][2] + vector.m_Z * m_Table[2][2] + m_Table[3][2]));
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Matrix4x4<T>::TransformNormal(const Vector3D<T>& normal) const
{
    return Vector3D<T>
            ((normal.m_X * m_Table[0][0]) + normal.m_Y * m_Table[1][0] + (normal.m_Z * m_Table[2][0]),
             (normal.m_X * m_Table[0][1]) + normal.m_Y * m_Table[1][1] + (normal.m_Z * m_Table[2][1]),
             (normal.m_X * m_Table[0][2]) + normal.m_Y * m_Table[1][2] + (normal.m_Z * m_Table[2][2]));
}
//---------------------------------------------------------------------------
template <class T>
const T* Matrix4x4<T>::GetPtr() const
{
    return &m_Table[0][0];
}
//---------------------------------------------------------------------------

/**
* Ray
*@author Jean-Milost Reymond
*/
template <class T>
class Ray
{
    public:
        /**
        * Constructor
        */
        inline Ray();

        /**
        * Constructor
        *@param pos - ray position
        *@param dir - ray direction
        */
        inline Ray(const Vector3D<T>& pos, const Vector3D<T>& dir);

        /**
        * Destructor
        */
        virtual inline ~Ray();

        /**
        * Gets ray position
        *@return ray position
        */
        virtual inline Vector3D<T> GetPos() const;

        /**
        * Sets ray position
        *@param pos - ray position
        */
        virtual inline void SetPos(const Vector3D<T>& pos);

        /**
        * Gets ray direction
        *@return ray direction
        */
        virtual inline Vector3D<T> GetDir() const;

        /**
        * Sets ray direction
        *@param dir - ray direction
        */
        virtual inline void SetDir(const Vector3D<T>& dir);

        /**
        * Gets ray inverted direction
        *@return ray inverted direction
        */
        virtual inline Vector3D<T> GetInvDir() const;

    private:
        Vector3D<T> m_Pos;    // ray position
        Vector3D<T> m_Dir;    // ray direction
        Vector3D<T> m_InvDir; // inverted direction (i.e. 1 / dir)
};

typedef Ray<float>  RayF;
typedef Ray<double> RayD;

//---------------------------------------------------------------------------
// Ray
//---------------------------------------------------------------------------
template <class T>
Ray<T>::Ray() :
    m_InvDir(Vector3D<T>(std::numeric_limits<T>::infinity(),
                         std::numeric_limits<T>::infinity(),
                         std::numeric_limits<T>::infinity()))
{}
//---------------------------------------------------------------------------
template <class T>
Ray<T>::Ray(const Vector3D<T>& pos, const Vector3D<T>& dir) :
    m_Pos(pos),
    m_Dir(dir),
    m_InvDir(Vector3D<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                         (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                         (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z)))
{}
//---------------------------------------------------------------------------
template <class T>
Ray<T>::~Ray()
{}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Ray<T>::GetPos() const
{
    return m_Pos;
}
//---------------------------------------------------------------------------
template <class T>
void Ray<T>::SetPos(const Vector3D<T>& pos)
{
    m_Pos = pos;
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Ray<T>::GetDir() const
{
    return m_Dir;
}
//---------------------------------------------------------------------------
template <class T>
void Ray<T>::SetDir(const Vector3D<T>& dir)
{
    m_Dir = dir;

    // calculate the invert direction
    m_InvDir = Vector3D<T>((dir.m_X == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_X),
                           (dir.m_Y == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Y),
                           (dir.m_Z == T(0.0)) ? std::numeric_limits<T>::infinity() : (T(1.0) / dir.m_Z));
}
//---------------------------------------------------------------------------
template <class T>
Vector3D<T> Ray<T>::GetInvDir() const
{
    return m_InvDir;
}
//---------------------------------------------------------------------------

/**
* Plane
*@author Jean-Milost Reymond
*/
template <class T>
class Plane
{
    public:
        T m_A; // a coordinate for the plane
        T m_B; // b coordinate for the plane
        T m_C; // c coordinate for the plane
        T m_D; // d coordinate for the plane

        /**
        * Constructor
        */
        inline Plane();

        /**
        * Constructor
        *@param a - a coordinate
        *@param b - b coordinate
        *@param c - c coordinate
        *@param d - d coordinate
        */
        inline Plane(T a, T b, T c, T d);

        /**
        * Copy constructor
        *@param other - other matrix to copy
        */
        inline Plane(const Plane& other);

        /**
        * Destructor
        */
        virtual inline ~Plane();

        /**
        * Assignation operator
        *@param other - other matrix to copy from
        */
        virtual inline Plane& operator =(const Plane& other);

        /**
        * Operator -
        *@return inverted plane
        */
        virtual inline Plane operator - () const;

        /**
        * Operator ==
        *@param other - other plane to compare
        *@return true if planes are identical, otherwise false
        */
        virtual inline bool operator == (const Plane& other) const;

        /**
        * Operator !=
        *@param other - other plane to compare
        *@return true if planes are not identical, otherwise false
        */
        virtual inline bool operator != (const Plane& other) const;

        /**
        * Copies plane from another
        *@param other - other matrix to copy from
        */
        virtual inline void Copy(const Plane& other);

        /**
        * Calculates a plane using 3 vertex
        *@param v1 - value of the first vertex
        *@param v2 - value of the second vertex
        *@param v3 - value of the thrid vertex
        *@return the built plane
        */
        static Plane FromPoints(const Vector3D<T>& v1,
                                const Vector3D<T>& v2,
                                const Vector3D<T>& v3);

        /**
        * Calculates a plane using a point and a normal
        *@param point - a point belongs to the plane
        *@param normal - normal of the plane
        *@return the built plane
        */
        static Plane FromPointNormal(const Vector3D<T>& point, const Vector3D<T>& normal);

        /**
        * Calculates distance to plane
        *@param point - point from which the distance must be calculated
        *@return distance to plane
        */
        virtual inline T DistanceTo(const Vector3D<T>& point) const;

        /**
        * Checks if plane intersects ray and calculates intersection point
        *@param ray - ray
        *@param[out] p - calculated point on plane
        *@return true if plane intersects ray, otherwise false
        */
        virtual inline bool Intersect(const Ray<T>& ray, Vector3D<T>& p) const;

        /**
        * Compares plane with the given plane using the given tolerance
        *@param other - other plane to compare
        *@param tolerance - tolerance for comparison
        *@return true if planes are equals in the limits of the given tolerance,
        *         otherwise false
        */
        virtual inline bool Compare(const Plane& other, T tolerance) const;
};

typedef Plane<float>  PlaneF;
typedef Plane<double> PlaneD;

//---------------------------------------------------------------------------
// Plane
//---------------------------------------------------------------------------
template <class T>
Plane<T>::Plane() :
    m_A(T(0.0)),
    m_B(T(0.0)),
    m_C(T(0.0)),
    m_D(T(0.0))
{}
//---------------------------------------------------------------------------
template <class T>
Plane<T>::Plane(T a, T b, T c, T d) :
    m_A(a),
    m_B(b),
    m_C(c),
    m_D(d)
{}
//---------------------------------------------------------------------------
template <class T>
Plane<T>::Plane(const Plane& other)
{
    Copy(other);
}
//---------------------------------------------------------------------------
template <class T>
Plane<T>::~Plane()
{}
//---------------------------------------------------------------------------
template <class T>
Plane<T>& Plane<T>::operator =(const Plane& other)
{
    Copy(other);
    return *this;
}
//---------------------------------------------------------------------------
template <class T>
Plane<T> Plane<T>::operator - () const
{
    return Plane<T>(-m_A, -m_B, -m_C, -m_D);
}
//---------------------------------------------------------------------------
template <class T>
bool Plane<T>::operator == (const Plane<T>& other) const
{
    return ((m_A == other.m_A) && (m_B == other.m_B) && (m_C == other.m_C) && (m_D == other.m_D));
}
//---------------------------------------------------------------------------
template <class T>
bool Plane<T>::operator != (const Plane<T>& other) const
{
    return ((m_A != other.m_A) || (m_B != other.m_B) || (m_C != other.m_C) || (m_D != other.m_D));
}
//---------------------------------------------------------------------------
template <class T>
void Plane<T>::Copy(const Plane& other)
{
    m_A = other.m_A;
    m_B = other.m_B;
    m_C = other.m_C;
    m_D = other.m_D;
}
//---------------------------------------------------------------------------
template <class T>
Plane<T> Plane<T>::FromPoints(const Vector3D<T>& v1,
                              const Vector3D<T>& v2,
                              const Vector3D<T>& v3)
{
    // calculate edge vectors
    const Vector3D<T> e1 = v2 - v1;
    const Vector3D<T> e2 = v3 - v1;

    // calculate the normal of the plane
    const Vector3D<T> normal = e1.Cross(e2).Normalize();

    // calculate and return the plane
    return FromPointNormal(v1, normal);
}
//---------------------------------------------------------------------------
template <class T>
Plane<T> Plane<T>::FromPointNormal(const Vector3D<T>& point, const Vector3D<T>& normal)
{
    // the a, b, and c components are only the normal of the plane, and the D
    // component can be calculated using the aX + bY + cZ + d = 0 formula
    return Plane<T>(normal.m_X,
                    normal.m_Y,
                    normal.m_Z,
                  -(normal.Dot(point)));
}
//---------------------------------------------------------------------------
template <class T>
T Plane<T>::DistanceTo(const Vector3D<T>& point) const
{
    // get the normal of the plane
    const Vector3D<T> n(m_A, m_B, m_C);

    // calculate the distance between the plane and the point
    return n.Dot(point) + m_D;
}
//---------------------------------------------------------------------------
template <class T>
bool Plane<T>::Intersect(const Ray<T>& ray, Vector3D<T>& p) const
{
    const Vector3D<T> lp = ray.GetPos();
    const Vector3D<T> ld = ray.GetDir();

    // get the normal of the plane
    const Vector3D<T> normal(m_A, m_B, m_C);

    // calculate the angle between the line and the normal to the plane
    const T dot = normal.Dot(ld);

    // if normal to the plane is perpendicular to the line, then the line is
    // either parallel to the plane and there are no solutions or the line is
    // on the plane in which case there are an infinite number of solutions
    if (!dot)
        return false;

    const T temp = (m_D + normal.Dot(lp)) / dot;

    // calculate the intersection point
    p = Vector3D<T>(lp.m_X - (temp * ld.m_X),
                    lp.m_Y - (temp * ld.m_Y),
                    lp.m_Z - (temp * ld.m_Z));

    return true;
}
//---------------------------------------------------------------------------
template <class T>
bool Plane<T>::Compare(const Plane<T>& other, T tolerance) const
{
    return (((m_A >= (other.m_A - tolerance))  &&
             (m_A <= (other.m_A + tolerance))) &&
            ((m_B >= (other.m_B - tolerance))  &&
             (m_B <= (other.m_B + tolerance))) &&
            ((m_C >= (other.m_C - tolerance))  &&
             (m_C <= (other.m_C + tolerance))) &&
            ((m_D >= (other.m_D - tolerance))  &&
             (m_D <= (other.m_D + tolerance))));
}
//---------------------------------------------------------------------------

#endif
