/*****************************************************************************
 * ==> QR_Color -------------------------------------------------------------*
 *****************************************************************************
 * Description : Universal color class                                       *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Color.h"
#include <memory>

#ifdef __WIN32__
    // needed for std::memset, otherwise compiler will not found it
    #include <cstring>
#endif

//------------------------------------------------------------------------------
// QR_Color
//------------------------------------------------------------------------------
QR_Color::QR_Color()
{
    std::memset(this, 0, sizeof(*this));
}
//------------------------------------------------------------------------------
QR_Color::QR_Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
    m_R(r),
    m_G(g),
    m_B(b),
    m_A(a)
{}
//------------------------------------------------------------------------------
void QR_Color::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_R = r;
    m_G = g;
    m_B = b;
    m_A = a;
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetRGB() const
{
    return (unsigned)((m_R << 16) + (m_G << 8) + m_B);
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetBGR() const
{
    return (unsigned)((m_B << 16) + (m_G << 8) + m_R);
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetARGB() const
{
    return (unsigned)((m_A << 24) + (m_R << 16) + (m_G << 8) + m_B);
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetRGBA() const
{
    return (unsigned)((m_R << 24) + (m_G << 16) + (m_B << 8) + m_A);
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetABGR() const
{
    return (unsigned)((m_A << 24) + (m_B << 16) + (m_G << 8) + m_R);
}
//------------------------------------------------------------------------------
inline unsigned QR_Color::GetBGRA() const
{
    return (unsigned)((m_B << 24) + (m_G << 16) + (m_R << 8) + m_A);
}
//------------------------------------------------------------------------------
inline void QR_Color::SetRGB(unsigned value)
{
    m_R = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_B =   value        & 0xFF;
    m_A = 255;
}
//------------------------------------------------------------------------------
inline void QR_Color::SetBGR(unsigned value)
{
    m_B = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_R =   value        & 0xFF;
    m_A = 255;
}
//------------------------------------------------------------------------------
inline void QR_Color::SetARGB(unsigned value)
{
    m_A = ((value >> 24) & 0xFF);
    m_R = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_B =   value        & 0xFF;
}
//------------------------------------------------------------------------------
inline void QR_Color::SetRGBA(unsigned value)
{
    m_R = ((value >> 24) & 0xFF);
    m_G = ((value >> 16) & 0xFF);
    m_B = ((value >> 8)  & 0xFF);
    m_A =   value        & 0xFF;
}
//------------------------------------------------------------------------------
inline void QR_Color::SetABGR(unsigned value)
{
    m_A = ((value >> 24) & 0xFF);
    m_B = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_R =   value        & 0xFF;
}
//------------------------------------------------------------------------------
inline void QR_Color::SetBGRA(unsigned value)
{
    m_B = ((value >> 24) & 0xFF);
    m_G = ((value >> 16) & 0xFF);
    m_R = ((value >> 8)  & 0xFF);
    m_A =   value        & 0xFF;
}
//------------------------------------------------------------------------------
unsigned char QR_Color::GetRed() const
{
    return m_R;
}
//------------------------------------------------------------------------------
unsigned char QR_Color::GetGreen() const
{
    return m_G;
}
//------------------------------------------------------------------------------
unsigned char QR_Color::GetBlue() const
{
    return m_B;
}
//------------------------------------------------------------------------------
unsigned char QR_Color::GetAlpha() const
{
    return m_A;
}
//------------------------------------------------------------------------------
void QR_Color::SetRed(unsigned char value)
{
    m_R = value;
}
//------------------------------------------------------------------------------
void QR_Color::SetGreen(unsigned char value)
{
    m_G = value;
}
//------------------------------------------------------------------------------
void QR_Color::SetBlue(unsigned char value)
{
    m_B = value;
}
//------------------------------------------------------------------------------
void QR_Color::SetAlpha(unsigned char value)
{
    m_A = value;
}
//------------------------------------------------------------------------------
QR_Color QR_Color::Blend(const QR_Color& other, float offset) const
{
    // do return internal color?
    if (offset <= 0.0f)
        return *this;

    // do return other color?
    if (offset >= 1.0f)
        return other;

    // calculate blended color components
    unsigned char colorR = m_R + offset * (other.m_R - m_R);
    unsigned char colorG = m_G + offset * (other.m_G - m_G);
    unsigned char colorB = m_B + offset * (other.m_B - m_B);
    unsigned char colorA = m_A + offset * (other.m_A - m_A);

    // return blended color
    return QR_Color(colorR, colorG, colorB, colorA);
}
//------------------------------------------------------------------------------
