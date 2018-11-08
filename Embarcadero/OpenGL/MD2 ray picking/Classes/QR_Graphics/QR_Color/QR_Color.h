/****************************************************************************
 * ==> QR_Color ------------------------------------------------------------*
 ****************************************************************************
 * Description : Universal color class                                      *
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

#ifndef QR_ColorH
#define QR_ColorH

// qr engine
#include "QR_Types.h"

/**
* Universal color class
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Color
{
    public:
        /**
        * Constructor
        */
        inline QR_Color();

        /**
        * Constructor
        *@param r - color red component
        *@param g - color green component
        *@param b - color blue component
        *@param a - color alpha component
        */
        inline QR_Color(QR_UInt8 r, QR_UInt8 g, QR_UInt8 b, QR_UInt8 a = 255);

        /**
        * Destructor
        */
        virtual inline ~QR_Color();

        /**
        * Equality operator
        *@param other - other color to compare with
        *@return true if colors are equals, otherwise false
        */
        virtual inline bool operator == (const QR_Color& other) const;

        /**
        * Not equality operator
        *@param other - other color to compare with
        *@return true if colors are equals, otherwise false
        */
        virtual inline bool operator != (const QR_Color& other) const;

        /**
        * Sets color
        *@param r - color red component
        *@param g - color green component
        *@param b - color blue component
        *@param a - color alpha component
        */
        virtual inline void SetColor(QR_UInt8 r, QR_UInt8 g, QR_UInt8 b, QR_UInt8 a = 255);

        /**
        * Gets color as 32 bit value
        *@return color as 32 bit value
        */
        virtual inline QR_UInt32 GetRGB() const;
        virtual inline QR_UInt32 GetBGR() const;
        virtual inline QR_UInt32 GetARGB() const;
        virtual inline QR_UInt32 GetRGBA() const;
        virtual inline QR_UInt32 GetABGR() const;
        virtual inline QR_UInt32 GetBGRA() const;

        /**
        * Sets color from 32 bit value
        *@return color as 32 bit value
        */
        virtual inline void SetRGB(QR_UInt32 value);
        virtual inline void SetBGR(QR_UInt32 value);
        virtual inline void SetARGB(QR_UInt32 value);
        virtual inline void SetRGBA(QR_UInt32 value);
        virtual inline void SetABGR(QR_UInt32 value);
        virtual inline void SetBGRA(QR_UInt32 value);

        /**
        * Gets red component
        *@return red component
        */
        virtual inline QR_UInt8 GetRed() const;

        /**
        * Gets green component
        *@return green component
        */
        virtual inline QR_UInt8 GetGreen() const;

        /**
        * Gets blue component
        *@return blue component
        */
        virtual inline QR_UInt8 GetBlue() const;

        /**
        * Gets alpha component
        *@return alpha component
        */
        virtual inline QR_UInt8 GetAlpha() const;

        /**
        * Sets red component
        *@param value - component value
        */
        virtual inline void SetRed(QR_UInt8 value);

        /**
        * Sets green component
        *@param value - component value
        */
        virtual inline void SetGreen(QR_UInt8 value);

        /**
        * Sets blue component
        *@param value - component value
        */
        virtual inline void SetBlue(QR_UInt8 value);

        /**
        * Sets alpha component
        *@param value - component value
        */
        virtual inline void SetAlpha(QR_UInt8 value);

        /**
        * Gets red component as float (between 0.0f and 1.0f)
        *@return red component
        */
        virtual inline QR_Float GetRedF() const;

        /**
        * Gets green component as float (between 0.0f and 1.0f)
        *@return green component
        */
        virtual inline QR_Float GetGreenF() const;

        /**
        * Gets blue component as float (between 0.0f and 1.0f)
        *@return blue component
        */
        virtual inline QR_Float GetBlueF() const;

        /**
        * Gets alpha component as float (between 0.0f and 1.0f)
        *@return alpha component
        */
        virtual inline QR_Float GetAlphaF() const;

        /**
        * Blend color with another color
        *@param other - other color to blend with
        *@param offset - blend offset (from 0.0f to 1.0f)
        *@return blended color
        */
        virtual inline QR_Color Blend(const QR_Color& other, const QR_Float& offset) const;

    private:
        QR_UInt8 m_R;
        QR_UInt8 m_G;
        QR_UInt8 m_B;
        QR_UInt8 m_A;
};

//---------------------------------------------------------------------------
// QR_Color
//---------------------------------------------------------------------------
QR_Color::QR_Color() :
    m_R(0),
    m_G(0),
    m_B(0),
    m_A(255)
{}
//---------------------------------------------------------------------------
QR_Color::QR_Color(QR_UInt8 r, QR_UInt8 g, QR_UInt8 b, QR_UInt8 a) :
    m_R(r),
    m_G(g),
    m_B(b),
    m_A(a)
{}
//---------------------------------------------------------------------------
QR_Color::~QR_Color()
{}
//---------------------------------------------------------------------------
bool QR_Color::operator == (const QR_Color& other) const
{
    return (m_R == other.m_R && m_G == other.m_G && m_B == other.m_B && m_A == other.m_A);
}
//---------------------------------------------------------------------------
bool QR_Color::operator != (const QR_Color& other) const
{
    return (m_R != other.m_R || m_G != other.m_G || m_B != other.m_B || m_A != other.m_A);
}
//---------------------------------------------------------------------------
void QR_Color::SetColor(QR_UInt8 r, QR_UInt8 g, QR_UInt8 b, QR_UInt8 a)
{
    m_R = r;
    m_G = g;
    m_B = b;
    m_A = a;
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetRGB() const
{
    return (QR_UInt32)((m_R << 16) + (m_G << 8) + m_B);
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetBGR() const
{
    return (QR_UInt32)((m_B << 16) + (m_G << 8) + m_R);
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetARGB() const
{
    return (QR_UInt32)((m_A << 24) + (m_R << 16) + (m_G << 8) + m_B);
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetRGBA() const
{
    return (QR_UInt32)((m_R << 24) + (m_G << 16) + (m_B << 8) + m_A);
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetABGR() const
{
    return (QR_UInt32)((m_A << 24) + (m_B << 16) + (m_G << 8) + m_R);
}
//---------------------------------------------------------------------------
inline QR_UInt32 QR_Color::GetBGRA() const
{
    return (QR_UInt32)((m_B << 24) + (m_G << 16) + (m_R << 8) + m_A);
}
//---------------------------------------------------------------------------
inline void QR_Color::SetRGB(QR_UInt32 value)
{
    m_R = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_B =   value        & 0xFF;
    m_A = 255;
}
//---------------------------------------------------------------------------
inline void QR_Color::SetBGR(QR_UInt32 value)
{
    m_B = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_R =   value        & 0xFF;
    m_A = 255;
}
//---------------------------------------------------------------------------
inline void QR_Color::SetARGB(QR_UInt32 value)
{
    m_A = ((value >> 24) & 0xFF);
    m_R = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_B =   value        & 0xFF;
}
//---------------------------------------------------------------------------
inline void QR_Color::SetRGBA(QR_UInt32 value)
{
    m_R = ((value >> 24) & 0xFF);
    m_G = ((value >> 16) & 0xFF);
    m_B = ((value >> 8)  & 0xFF);
    m_A =   value        & 0xFF;
}
//---------------------------------------------------------------------------
inline void QR_Color::SetABGR(QR_UInt32 value)
{
    m_A = ((value >> 24) & 0xFF);
    m_B = ((value >> 16) & 0xFF);
    m_G = ((value >> 8)  & 0xFF);
    m_R =   value        & 0xFF;
}
//---------------------------------------------------------------------------
inline void QR_Color::SetBGRA(QR_UInt32 value)
{
    m_B = ((value >> 24) & 0xFF);
    m_G = ((value >> 16) & 0xFF);
    m_R = ((value >> 8)  & 0xFF);
    m_A =   value        & 0xFF;
}
//---------------------------------------------------------------------------
QR_UInt8 QR_Color::GetRed() const
{
    return m_R;
}
//---------------------------------------------------------------------------
QR_UInt8 QR_Color::GetGreen() const
{
    return m_G;
}
//---------------------------------------------------------------------------
QR_UInt8 QR_Color::GetBlue() const
{
    return m_B;
}
//---------------------------------------------------------------------------
QR_UInt8 QR_Color::GetAlpha() const
{
    return m_A;
}
//---------------------------------------------------------------------------
void QR_Color::SetRed(QR_UInt8 value)
{
    m_R = value;
}
//---------------------------------------------------------------------------
void QR_Color::SetGreen(QR_UInt8 value)
{
    m_G = value;
}
//---------------------------------------------------------------------------
void QR_Color::SetBlue(QR_UInt8 value)
{
    m_B = value;
}
//---------------------------------------------------------------------------
void QR_Color::SetAlpha(QR_UInt8 value)
{
    m_A = value;
}
//---------------------------------------------------------------------------
QR_Float QR_Color::GetRedF() const
{
    return ((QR_Float)m_R / 255.0f);
}
//---------------------------------------------------------------------------
QR_Float QR_Color::GetGreenF() const
{
    return ((QR_Float)m_G / 255.0f);
}
//---------------------------------------------------------------------------
QR_Float QR_Color::GetBlueF() const
{
    return ((QR_Float)m_B / 255.0f);
}
//---------------------------------------------------------------------------
QR_Float QR_Color::GetAlphaF() const
{
    return ((QR_Float)m_A / 255.0f);
}
//---------------------------------------------------------------------------
QR_Color QR_Color::Blend(const QR_Color& other, const QR_Float& offset) const
{
    // do return internal color?
    if (offset <= 0.0f)
        return *this;

    // do return other color?
    if (offset >= 1.0f)
        return other;

    // calculate blended color components
    QR_UInt8 colorR = m_R + offset * (other.m_R - m_R);
    QR_UInt8 colorG = m_G + offset * (other.m_G - m_G);
    QR_UInt8 colorB = m_B + offset * (other.m_B - m_B);
    QR_UInt8 colorA = m_A + offset * (other.m_A - m_A);

    // return blended color
    return QR_Color(colorR, colorG, colorB, colorA);
}
//---------------------------------------------------------------------------

#endif
