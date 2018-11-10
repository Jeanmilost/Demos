/******************************************************************************
 * ==> QR_Vector2D -----------------------------------------------------------*
 ******************************************************************************
 * Description : Euclidean vector 2D                                          *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#include "QR_Vector2D.h"
#include <memory>
#include <math.h>

#ifdef __WIN32__
    // needed for std::memset, otherwise compiler will not found it
    #include <cstring>
#endif

//------------------------------------------------------------------------------
// Class QR_Vector2D - c++ class
//------------------------------------------------------------------------------
QR_Vector2D::QR_Vector2D()
{
    // initialize memory
    std::memset(this, 0, sizeof(*this));
}
//------------------------------------------------------------------------------
QR_Vector2D::QR_Vector2D(const float& x, const float& y)
{
    m_X = x;
    m_Y = y;
}
//------------------------------------------------------------------------------
#ifdef USING_DIRECTX
    QR_Vector2D::QR_Vector2D(const D3DXVECTOR2& d3dxVector)
    {
        m_X = d3dxVector.x;
        m_Y = d3dxVector.y;
    }
#endif
//------------------------------------------------------------------------------
QR_Vector2D::~QR_Vector2D()
{}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::operator + (const QR_Vector2D& other) const
{
    return QR_Vector2D(m_X + other.m_X, m_Y + other.m_Y);
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::operator - () const
{
    return QR_Vector2D(-m_X, -m_Y);
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::operator - (const QR_Vector2D& other) const
{
    return QR_Vector2D(m_X - other.m_X, m_Y - other.m_Y);
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::operator * (const QR_Vector2D& other) const
{
    return QR_Vector2D(m_X * other.m_X, m_Y * other.m_Y);
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::operator / (const QR_Vector2D& other) const
{
    return QR_Vector2D(m_X / other.m_X, m_Y / other.m_Y);
}
//------------------------------------------------------------------------------
const QR_Vector2D& QR_Vector2D::operator += (const QR_Vector2D& other)
{
    m_X += other.m_X;
    m_Y += other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector2D& QR_Vector2D::operator -= (const QR_Vector2D& other)
{
    m_X -= other.m_X;
    m_Y -= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector2D& QR_Vector2D::operator *= (const QR_Vector2D& other)
{
    m_X *= other.m_X;
    m_Y *= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector2D& QR_Vector2D::operator /= (const QR_Vector2D& other)
{
    m_X /= other.m_X;
    m_Y /= other.m_Y;

    return *this;
}
//------------------------------------------------------------------------------
bool QR_Vector2D::operator == (const QR_Vector2D& other) const
{
    return ((m_X == other.m_X) && (m_Y == other.m_Y));
}
//------------------------------------------------------------------------------
bool QR_Vector2D::operator != (const QR_Vector2D& other) const
{
    return ((m_X != other.m_X) || (m_Y != other.m_Y));
}
//------------------------------------------------------------------------------
float QR_Vector2D::Length() const
{
    return sqrt((m_X * m_X) + (m_Y * m_Y));
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::Normalize() const
{
    float len = Length();

    return QR_Vector2D((m_X / len), (m_Y / len));
}
//------------------------------------------------------------------------------
QR_Vector2D QR_Vector2D::Cross(const QR_Vector2D& vector) const
{
    return QR_Vector2D((m_Y * vector.m_X) - (vector.m_Y * m_X),
                       (m_X * vector.m_Y) - (vector.m_X * m_Y));
}
//------------------------------------------------------------------------------
float QR_Vector2D::Dot(const QR_Vector2D& vector) const
{
    return ((m_X * vector.m_X) + (m_Y * vector.m_Y));
}
//------------------------------------------------------------------------------
#ifdef USING_DIRECTX
    D3DXVECTOR2 QR_Vector2D::ToD3DXVector2() const
    {
        return D3DXVECTOR2(m_X, m_Y);
    }
#endif
//------------------------------------------------------------------------------

