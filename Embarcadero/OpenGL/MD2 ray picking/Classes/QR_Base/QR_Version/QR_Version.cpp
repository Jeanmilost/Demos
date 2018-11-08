/****************************************************************************
 * ==> QR_Version ----------------------------------------------------------*
 * **************************************************************************
 * Description : Version management class, used e.g. for files, ...         *
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

#include "QR_Version.h"

// std
#include <sstream>

//---------------------------------------------------------------------------
// QR_Version
//---------------------------------------------------------------------------
QR_Version::QR_Version() :
    m_Major(0),
    m_Minor(0),
    m_Release(0),
    m_Build(0)
{}
//---------------------------------------------------------------------------
QR_Version::QR_Version(QR_UInt32 major, QR_UInt32 minor, QR_UInt32 release, QR_UInt32 build)
{
    // set version
    SetMajor(major);
    SetMinor(minor);
    SetRelease(release);
    SetBuild(build);
}
//---------------------------------------------------------------------------
QR_Version::~QR_Version()
{}
//---------------------------------------------------------------------------
QR_UInt32 QR_Version::GetMajor() const
{
    return m_Major;
}
//---------------------------------------------------------------------------
void QR_Version::SetMajor(QR_UInt32 value)
{
    m_Major = value;
}
//---------------------------------------------------------------------------
QR_UInt32 QR_Version::GetMinor() const
{
    return m_Minor;
}
//---------------------------------------------------------------------------
void QR_Version::SetMinor(QR_UInt32 value)
{
    m_Minor = value;
}
//---------------------------------------------------------------------------
QR_UInt32 QR_Version::GetRelease() const
{
    return m_Release;
}
//---------------------------------------------------------------------------
void QR_Version::SetRelease(QR_UInt32 value)
{
    m_Release = value;
}
//---------------------------------------------------------------------------
QR_UInt32 QR_Version::GetBuild() const
{
    return m_Build;
}
//---------------------------------------------------------------------------
void QR_Version::SetBuild(QR_UInt32 value)
{
    m_Build = value;
}
//---------------------------------------------------------------------------
std::string QR_Version::ToStr()
{
    std::ostringstream sstr;

    // build version string
    sstr << m_Major << "." << m_Minor << "." << m_Release << "." << m_Build;

    return sstr.str();
}
//---------------------------------------------------------------------------
std::wstring QR_Version::ToWStr()
{
    std::wostringstream sstr;

    // build version string
    sstr << m_Major << L"." << m_Minor << L"." << m_Release << L"." << m_Build;

    return sstr.str();
}
//---------------------------------------------------------------------------
