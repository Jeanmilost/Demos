/******************************************************************************
 * ==> QR_Vector3D -----------------------------------------------------------*
 ******************************************************************************
 * Description : Euclidean vector 3D                                          *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#include "QR_Vector3D.h"
#include <memory>
#include <math.h>

#ifdef __WIN32__
    // needed for std::memset, otherwise compiler will not found it
    #include <cstring>
#endif

//------------------------------------------------------------------------------
// Class QR_Vector3D - c++ class
//------------------------------------------------------------------------------
QR_Vector3D::QR_Vector3D()
{
    // initialize memory
    std::memset(this, 0, sizeof(*this));
}
//------------------------------------------------------------------------------
QR_Vector3D::QR_Vector3D(const float& x, const float& y, const float& z)
{
    m_X = x;
    m_Y = y;
    m_Z = z;
}
//------------------------------------------------------------------------------
#ifdef USING_DIRECTX
    QR_Vector3D::QR_Vector3D(const D3DXVECTOR3& d3dxVector)
    {
        m_X = d3dxVector.x;
        m_Y = d3dxVector.y;
        m_Z = d3dxVector.z;
    }
#endif
//------------------------------------------------------------------------------
QR_Vector3D::~QR_Vector3D()
{}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::operator + (const QR_Vector3D& other) const
{
    return QR_Vector3D(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::operator - () const
{
    return QR_Vector3D(-m_X, -m_Y, -m_Z);
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::operator - (const QR_Vector3D& other) const
{
    return QR_Vector3D(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z);
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::operator * (const QR_Vector3D& other) const
{
    return QR_Vector3D(m_X * other.m_X, m_Y * other.m_Y, m_Z * other.m_Z);
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::operator / (const QR_Vector3D& other) const
{
    return QR_Vector3D(m_X / other.m_X, m_Y / other.m_Y, m_Z / other.m_Z);
}
//------------------------------------------------------------------------------
const QR_Vector3D& QR_Vector3D::operator += (const QR_Vector3D& other)
{
    m_X += other.m_X;
    m_Y += other.m_Y;
    m_Z += other.m_Z;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector3D& QR_Vector3D::operator -= (const QR_Vector3D& other)
{
    m_X -= other.m_X;
    m_Y -= other.m_Y;
    m_Z -= other.m_Z;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector3D& QR_Vector3D::operator *= (const QR_Vector3D& other)
{
    m_X *= other.m_X;
    m_Y *= other.m_Y;
    m_Z *= other.m_Z;

    return *this;
}
//------------------------------------------------------------------------------
const QR_Vector3D& QR_Vector3D::operator /= (const QR_Vector3D& other)
{
    m_X /= other.m_X;
    m_Y /= other.m_Y;
    m_Z /= other.m_Z;

    return *this;
}
//------------------------------------------------------------------------------
bool QR_Vector3D::operator == (const QR_Vector3D& other) const
{
    return ((m_X == other.m_X) && (m_Y == other.m_Y) && (m_Z == other.m_Z));
}
//------------------------------------------------------------------------------
bool QR_Vector3D::operator != (const QR_Vector3D& other) const
{
    return ((m_X != other.m_X) || (m_Y != other.m_Y) || (m_Z != other.m_Z));
}
//------------------------------------------------------------------------------
float QR_Vector3D::Length() const
{
    return sqrt((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z));
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::Normalize() const
{
    float len = Length();

    return QR_Vector3D((m_X / len), (m_Y / len), (m_Z / len));
}
//------------------------------------------------------------------------------
QR_Vector3D QR_Vector3D::Cross(const QR_Vector3D& vector) const
{
    return QR_Vector3D((m_Y * vector.m_Z) - (vector.m_Y * m_Z),
                       (m_Z * vector.m_X) - (vector.m_Z * m_X),
                       (m_X * vector.m_Y) - (vector.m_X * m_Y));
}
//------------------------------------------------------------------------------
float QR_Vector3D::Dot(const QR_Vector3D& vector) const
{
    return ((m_X * vector.m_X) + (m_Y * vector.m_Y) + (m_Z * vector.m_Z));
}
//------------------------------------------------------------------------------
#ifdef USING_DIRECTX
    D3DXVECTOR3 QR_Vector3D::ToD3DXVector3() const
    {
        return D3DXVECTOR3(m_X, m_Y, m_Z);
    }
#endif
//------------------------------------------------------------------------------

