/****************************************************************************
 * ==> QR_ModelParser ------------------------------------------------------*
 ****************************************************************************
 * Description : Basic model parser interface                               *
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

#include "QR_ModelParser.h"

// qr engine
#include "QR_StdFileBuffer.h"

//---------------------------------------------------------------------------
// QR_ModelParser
//---------------------------------------------------------------------------
QR_ModelParser::QR_ModelParser() :
    m_HashCode(0)
{}
//---------------------------------------------------------------------------
QR_ModelParser::~QR_ModelParser()
{}
//------------------------------------------------------------------------------
void QR_ModelParser::Clear()
{
    m_HashCode = 0;
}
//------------------------------------------------------------------------------
void QR_ModelParser::Copy(const QR_ModelParser& other)
{
    m_HashCode = other.m_HashCode;
}
//------------------------------------------------------------------------------
bool QR_ModelParser::Load(const std::string& fileName)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    // read model content
    return Load(buffer, buffer.GetSize());
}
//------------------------------------------------------------------------------
bool QR_ModelParser::Load(const std::wstring& fileName)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    // read model content
    return Load(buffer, buffer.GetSize());
}
//---------------------------------------------------------------------------
