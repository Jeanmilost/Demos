/****************************************************************************
 * ==> QR_Exception --------------------------------------------------------*
 * **************************************************************************
 * Description : Engine exception class                                     *
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

#include "QR_Exception.h"

// std
#include <sstream>

//---------------------------------------------------------------------------
// QR_ExceptionFormatter
//---------------------------------------------------------------------------
std::string QR_ExceptionFormatter::Format(const std::string& type,
                                          const std::string& message,
                                          const std::string& file,
                                          const std::string& function,
                                          QR_SizeT           line)
{
    std::ostringstream sstr;

    sstr << "[";

    if (type.empty())
        sstr << "Unknown";
    else
        sstr << type;

    sstr << " exception raised]" << std::endl << "[Message]  ";

    if (message.empty())
        sstr << "Unknown exception" << std::endl;
    else
        sstr << message << std::endl;

    sstr << "[File] "     << file     << std::endl
         << "[Function] " << function << std::endl
         << "[Line] "     << line     << std::endl;

    return sstr.str();
}
//---------------------------------------------------------------------------
std::string QR_ExceptionFormatter::Format_HTML(const std::string& type,
                                               const std::string& message,
                                               const std::string& file,
                                               const std::string& function,
                                               QR_SizeT           line)
{
    std::ostringstream sstr;

    sstr << "[";

    if (type.empty())
        sstr << "Unknown";
    else
        sstr << type;

    sstr << " exception raised]" << "<br>" << "[Message]  ";

    if (message.empty())
        sstr << "Unknown exception" << "<br>";
    else
        sstr << message << "<br>";

    sstr << "[File] "     << file     << "<br>"
         << "[Function] " << function << "<br>"
         << "[Line] "     << line     << "<br>";

    return sstr.str();
}
//---------------------------------------------------------------------------
std::string QR_ExceptionFormatter::Format_HTML_Private(const std::string& type,
                                                       const std::string& message,
                                                       const std::string& file,
                                                       const std::string& function,
                                                       QR_SizeT           line)
{
    std::ostringstream sstr;

    sstr << "[";

    if (type.empty())
        sstr << "Unknown";
    else
        sstr << type;

    sstr << " exception raised]" << "<br>" << "[Message]  ";

    if (message.empty())
        sstr << "Unknown exception" << "<br>";
    else
        sstr << message << "<br>";

    sstr << "[File] ######<br>"
         << "[Function] ######<br>"
         << "[Line] " << line << "<br>";

    return sstr.str();
}
//---------------------------------------------------------------------------
std::string QR_ExceptionFormatter::Message(const std::string& message,
                                           const std::string& function,
                                           QR_SizeT           line)
{
    std::ostringstream sstr;

    if (message.empty())
        sstr << "Unknown exception";
    else
        sstr << message;

    sstr << std::endl;

    sstr << "[Function] " << function << std::endl
         << "[Line] "     << line << std::endl;

    return sstr.str();
}
//---------------------------------------------------------------------------
std::string QR_ExceptionFormatter::Message_HTML(const std::string& message,
                                                const std::string& function,
                                                QR_SizeT           line)
{
    std::ostringstream sstr;

    if (message.empty())
        sstr << "Unknown exception";
    else
        sstr << message;

    sstr << "<br>";

    sstr << "[Function] " << function << "<br>"
         << "[Line] "     << line << "<br>";

    return sstr.str();
}
//---------------------------------------------------------------------------
// QR_Exception
//---------------------------------------------------------------------------
QR_Exception::QR_Exception() throw()
{}
//---------------------------------------------------------------------------
QR_Exception::QR_Exception(const std::string& message,
                           const std::string& file,
                           const std::string& function,
                           QR_SizeT           line) throw()
{
    m_Message  = message;
    m_File     = file;
    m_Function = function;
    m_Line     = line;
}
//---------------------------------------------------------------------------
QR_Exception::~QR_Exception() throw()
{}
//---------------------------------------------------------------------------
const QR_Char* QR_Exception::what() const throw()
{
    return Format();
}
//---------------------------------------------------------------------------
const QR_Char* QR_Exception::Format() const
{
    std::ostringstream sstr;

    sstr << "[QR_Exception raised]" << std::endl << "[Message]  ";

    if (m_Message.empty())
        sstr << "Unknown exception" << std::endl;
    else
        sstr << m_Message << std::endl;

    sstr << "[File] "     << m_File << std::endl
         << "[Function] " << m_Function << std::endl
         << "[Line] "     << m_Line << std::endl;

    return sstr.str().c_str();
}
//---------------------------------------------------------------------------
std::string QR_Exception::Format_HTML() const
{
    std::ostringstream sstr;

    sstr << "[QR_Exception raised]" << "<br>" << "[Message]  ";

    if (m_Message.empty())
        sstr << "Unknown exception" << "<br>";
    else
        sstr << m_Message << "<br>";

    sstr << "[File] "     << m_File << "<br>"
         << "[Function] " << m_Function << "<br>"
         << "[Line] "     << m_Line << "<br>";

    return sstr.str().c_str();
}
//---------------------------------------------------------------------------
std::string QR_Exception::Format_HTML_Private() const
{
    std::ostringstream sstr;

    sstr << "[QR_Exception raised]" << "<br>" << "[Message]  ";

    if (m_Message.empty())
        sstr << "Unknown exception" << "<br>";
    else
        sstr << m_Message << "<br>";

    sstr << "[File] ######<br>"
         << "[Function] ######<br>"
         << "[Line] " << m_Line << "<br>";

    return sstr.str().c_str();
}
//---------------------------------------------------------------------------
const std::string& QR_Exception::Message() const
{
    return m_Message;
}
//---------------------------------------------------------------------------
