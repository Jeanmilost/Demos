/******************************************************************************
 * ==> QR_Vector2D -----------------------------------------------------------*
 ******************************************************************************
 * Description : Euclidean vector 2D                                          *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#ifndef QR_Vector2DH
#define QR_Vector2DH

// std
#include <math.h>

// do use without others QR Engine classes?
#ifndef USE_QR_GEOMETRY_OLNY
    // qr engine
    #include "QR_Types.h"
    #include "QR_Exception.h"
#else
    #define M_THROW_EXCEPTION(message) throw message
#endif

/**
* Vector 2D
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
template <class T>
class QR_Vector2D
{
    public:
        T m_X; // x coordinate for the 2D vector
        T m_Y; // y coordinate for the 2D vector

        /**
        * Constructor
        */
        inline QR_Vector2D();

        /**
        * Constructor
        *@param x - vector x value
        *@param y - vector y value
        */
        inline QR_Vector2D(const T& x, const T& y);

        /**
        * Copy constructor
        *@param other - other vector to copy from
        */
        inline QR_Vector2D(const QR_Vector2D& other);

        /**
        * Destructor
        */
        virtual inline ~QR_Vector2D();

        /**
        * Copy operator
        *@param other - other vector to copy from
        *@return this vector
        */
        virtual inline QR_Vector2D& operator =(const QR_Vector2D& other);

        /**
        * Operator + for single value
        *@param value - value to add
        *@return resulting added vector
        */
        virtual inline QR_Vector2D operator + (T value) const;

        /**
        * Operator +
        *@param other - other vector to add
        *@return resulting added vector
        */
        virtual inline QR_Vector2D operator + (const QR_Vector2D& other) const;

        /**
        * Operator -
        *@return inverted vector
        */
        virtual inline QR_Vector2D operator - () const;

        /**
        * Operator - for single value
        *@param value - value to subtract
        *@return resulting subtracted vector
        */
        virtual inline QR_Vector2D operator - (T value) const;

        /**
        * Operator -
        *@param other - other vector to subtract
        *@return resulting subtracted vector
        */
        virtual inline QR_Vector2D operator - (const QR_Vector2D& other) const;

        /**
        * Operator * for single value
        *@param value - value to multiply
        *@return resulting multiplied vector
        */
        virtual inline QR_Vector2D operator * (T value) const;

        /**
        * Operator *
        *@param other - other vector to multiply
        *@return resulting multiplied vector
        */
        virtual inline QR_Vector2D operator * (const QR_Vector2D& other) const;

        /**
        * Operator / for single value
        *@param value - value to divide
        *@return resulting divided vector
        */
        virtual inline QR_Vector2D operator / (T value) const;

        /**
        * Operator /
        *@param other - other vector to divide
        *@return resulting divided vector
        */
        virtual inline QR_Vector2D operator / (const QR_Vector2D& other) const;

        /**
        * Operator +=
        *@param value - value to add
        *@return resulting added vector
        */
        virtual inline const QR_Vector2D& operator += (T value);

        /**
        * Operator +=
        *@param other - other vector to add
        *@return resulting added vector
        */
        virtual inline const QR_Vector2D& operator += (const QR_Vector2D& other);

        /**
        * Operator -=
        *@param value - value to subtract
        *@return resulting subtracted vector
        */
        virtual inline const QR_Vector2D& operator -= (T value);

        /**
        * Operator -=
        *@param other - other vector to substract
        *@return resulting substracted vector
        */
        virtual inline const QR_Vector2D& operator -= (const QR_Vector2D& other);

        /**
        * Operator *=
        *@param value - value to multiply
        *@return resulting multiplied vector
        */
        virtual inline const QR_Vector2D& operator *= (T value);

        /**
        * Operator *=
        *@param other - other vector to multiply
        *@return resulting multiplicated vector
        */
        virtual inline const QR_Vector2D& operator *= (const QR_Vector2D& other);

        /**
        * Operator /=
        *@param value - value to divide
        *@return resulting divided vector
        */
        virtual inline const QR_Vector2D& operator /= (T value);

        /**
        * Operator -=
        *@param other - other vector to divide
        *@return resulting divided vector
        */
        virtual inline const QR_Vector2D& operator /= (const QR_Vector2D& other);

        /**
        * Operator ==
        *@param other - other vector to compare
        *@return true if vectors are identical, otherwise false
        */
        virtual inline bool operator == (const QR_Vector2D& other) const;

        /**
        * Operator !=
        *@param other - other vector to compare
        *@return true if vectors are not identical, otherwise false
        */
        virtual inline bool operator != (const QR_Vector2D& other) const;

        /**
        * Copies vector
        *@param other - other vector to copy from
        */
        virtual inline void Copy(const QR_Vector2D& other);

        /**
        * Gets the vector length
        *@return the vector length
        */
        virtual inline T Length() const;

        /**
        * Normalizes the vector
        *@return normalized vector
        */
        virtual inline QR_Vector2D Normalize() const;

        /**
        * Calculates the cross product between 2 vectors
        *@param vector - vector with which cross product is calculated
        *@return cross product
        */
        virtual inline QR_Vector2D Cross(const QR_Vector2D& vector) const;

        /**
        * Calculates the dot product between 2 vectors
        *@param vector - vector with which dot product is calculated
        *@return dot product
        */
        virtual inline T Dot(const QR_Vector2D& vector) const;

        /**
        * Calculates interpolation vector between 2 vectors
        *@param other - other vector to interpolate with
        *@param position - interpolation position, in percent (between 0.0f and 1.0f)
        *@return interpolation vector
        */
        virtual inline QR_Vector2D Interpolate(const QR_Vector2D& other,
                                               const QR_Float&    position) const;
};

#ifdef USE_QR_GEOMETRY_OLNY
    // formatted vector 2d using float or double
    typedef QR_Vector2D<QR_Float>  QR_Vector2DF;
    typedef QR_Vector2D<QR_Double> QR_Vector2DD;
#else
    // formatted vector 2d using global precision
    typedef QR_Vector2D<M_Precision> QR_Vector2DP;
#endif

//------------------------------------------------------------------------------
// QR_Vector2D implementation - c++ cross-platform
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T>::QR_Vector2D() :
    m_X(0),
    m_Y(0)
{}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T>::QR_Vector2D(const T& x, const T& y) :
    m_X(x),
    m_Y(y)
{}
//---------------------------------------------------------------------------
template<class T>
QR_Vector2D<T>::QR_Vector2D(const QR_Vector2D& other)
{
    Copy(other);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T>::~QR_Vector2D()
{}
//---------------------------------------------------------------------------
template<class T>
QR_Vector2D<T>& QR_Vector2D<T>::operator =(const QR_Vector2D& other)
{
    Copy(other);
    return *this;
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator + (T value) const
{
    return QR_Vector2D<T>(m_X + value, m_Y + value);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator + (const QR_Vector2D<T>& other) const
{
    return QR_Vector2D<T>(m_X + other.m_X, m_Y + other.m_Y);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator - () const
{
    return QR_Vector2D<T>(-m_X, -m_Y);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator - (T value) const
{
    return QR_Vector2D<T>(m_X - value, m_Y - value);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator - (const QR_Vector2D<T>& other) const
{
    return QR_Vector2D<T>(m_X - other.m_X, m_Y - other.m_Y);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator * (T value) const
{
    return QR_Vector2D<T>(m_X * value, m_Y * value);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator * (const QR_Vector2D<T>& other) const
{
    return QR_Vector2D<T>(m_X * other.m_X, m_Y * other.m_Y);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator / (T value) const
{
    if (value == 0.0f)
        M_THROW_EXCEPTION("Division by 0 is prohibited");

    return QR_Vector2D<T>(m_X / value, m_Y / value);
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::operator / (const QR_Vector2D<T>& other) const
{
    if (other.m_X == 0.0f)
        M_THROW_EXCEPTION("Vector x axis - division by 0 is prohibited");

    if (other.m_Y == 0.0f)
        M_THROW_EXCEPTION("Vector y axis - division by 0 is prohibited");

    return QR_Vector2D<T>(m_X / other.m_X, m_Y / other.m_Y);
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator += (T value)
{
    m_X += value;
    m_Y += value;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator += (const QR_Vector2D<T>& other)
{
    m_X += other.m_X;
    m_Y += other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator -= (T value)
{
    m_X -= value;
    m_Y -= value;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator -= (const QR_Vector2D<T>& other)
{
    m_X -= other.m_X;
    m_Y -= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator *= (T value)
{
    m_X *= value;
    m_Y *= value;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator *= (const QR_Vector2D<T>& other)
{
    m_X *= other.m_X;
    m_Y *= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator /= (T value)
{
    if (value == 0.0f)
        M_THROW_EXCEPTION("Division by 0 is prohibited");

    m_X /= value;
    m_Y /= value;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
const QR_Vector2D<T>& QR_Vector2D<T>::operator /= (const QR_Vector2D<T>& other)
{
    if (other.m_X == 0.0f)
        M_THROW_EXCEPTION("Vector x axis - division by 0 is prohibited");

    if (other.m_Y == 0.0f)
        M_THROW_EXCEPTION("Vector y axis - division by 0 is prohibited");

    m_X /= other.m_X;
    m_Y /= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
template <class T>
bool QR_Vector2D<T>::operator == (const QR_Vector2D<T>& other) const
{
    return ((m_X == other.m_X) && (m_Y == other.m_Y));
}
//------------------------------------------------------------------------------
template <class T>
bool QR_Vector2D<T>::operator != (const QR_Vector2D<T>& other) const
{
    return ((m_X != other.m_X) || (m_Y != other.m_Y));
}
//------------------------------------------------------------------------------
template <class T>
void QR_Vector2D<T>::Copy(const QR_Vector2D& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;
}
//------------------------------------------------------------------------------
template <class T>
T QR_Vector2D<T>::Length() const
{
    return sqrt((m_X * m_X) + (m_Y * m_Y));
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::Normalize() const
{
    T len = Length();

    if (len == 0.0f)
        return QR_Vector2D<T>();

    return QR_Vector2D<T>((m_X / len), (m_Y / len));
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::Cross(const QR_Vector2D<T>& vector) const
{
    return QR_Vector2D<T>((m_Y * vector.m_X) - (vector.m_Y * m_X),
                          (m_X * vector.m_Y) - (vector.m_X * m_Y));
}
//------------------------------------------------------------------------------
template <class T>
T QR_Vector2D<T>::Dot(const QR_Vector2D<T>& vector) const
{
    return ((m_X * vector.m_X) + (m_Y * vector.m_Y));
}
//------------------------------------------------------------------------------
template <class T>
QR_Vector2D<T> QR_Vector2D<T>::Interpolate(const QR_Vector2D<T>& other,
                                           const QR_Float&       position) const
{
    // is position out of bounds? Limit to min or max values in this case
    if (position < 0.0f)
        return *this;
    else
    if (position > 1.0f)
        return other;

    QR_Vector2D<T> interpolation;

    // calculate interpolation
    interpolation.m_X = m_X + position * (other.m_X - m_X);
    interpolation.m_Y = m_Y + position * (other.m_Y - m_Y);

    return interpolation;
}
//------------------------------------------------------------------------------

//---------------------------------------------------------------------------
// RAD studio
//---------------------------------------------------------------------------
#ifdef __CODEGEARC__
    // needed to avoid the W8058 error "Cannot create pre-compiled header: header incomplete" warning in BCC compilers
    ;
#endif
//---------------------------------------------------------------------------

#endif // QR_Vector2DH
