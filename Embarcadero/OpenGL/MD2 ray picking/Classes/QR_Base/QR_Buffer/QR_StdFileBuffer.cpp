/****************************************************************************
 * ==> QR_StdFileBuffer ----------------------------------------------------*
 ****************************************************************************
 * Description : File buffer based on std library                           *
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

#include "QR_StdFileBuffer.h"

// std
#include <cstring>

// qr engine
#include "QR_StringTools.h"

//---------------------------------------------------------------------------
// QR_StdFileBuffer
//---------------------------------------------------------------------------
QR_StdFileBuffer::QR_StdFileBuffer() :
    QR_FileBuffer(),
    m_FileBuffer(NULL)
{}
//---------------------------------------------------------------------------
QR_StdFileBuffer::~QR_StdFileBuffer()
{
    Close();
}
//---------------------------------------------------------------------------
bool QR_StdFileBuffer::Open(const std::wstring& fileName, IEMode mode)
{
    return (Open(QR_StringTools::UTF16ToUTF8(fileName), mode));
}
//---------------------------------------------------------------------------
bool QR_StdFileBuffer::Open(const std::string& fileName, IEMode mode)
{
    // call base function to execute common stuffs
    if (!QR_FileBuffer::Open(fileName, mode))
        return false;

    // open file stream
    switch (m_Mode)
    {
        case IE_M_Read:
            m_FileBuffer = std::fopen(fileName.c_str(), "r+b");
            break;

        case IE_M_Write:
            m_FileBuffer = std::fopen(fileName.c_str(), "w+b");
            break;

        case IE_M_RW:
            m_FileBuffer = std::fopen(fileName.c_str(), "rw+b");
            break;

        case IE_M_Unknown:
        default:
            return false;
    }

    return m_FileBuffer;
}
//---------------------------------------------------------------------------
void QR_StdFileBuffer::Clear()
{
    Close();
}
//---------------------------------------------------------------------------
bool QR_StdFileBuffer::Empty()
{
    return (!m_FileBuffer || !GetSize());
}
//---------------------------------------------------------------------------
QR_Buffer::ISizeType QR_StdFileBuffer::GetOffset() const
{
    // is file opened?
    if (!m_FileBuffer)
        return 0L;

    // get current offset
    return std::ftell(m_FileBuffer);
}
//---------------------------------------------------------------------------
QR_Buffer::ISizeType QR_StdFileBuffer::GetSize() const
{
    // is file opened?
    if (!m_FileBuffer)
        return 0L;

    // keep current position
    ISizeType curPos = std::ftell(m_FileBuffer);

    // get file size
    std::fseek(m_FileBuffer, 0, SEEK_END);
    ISizeType fileSize = std::ftell(m_FileBuffer);
    std::fseek(m_FileBuffer, curPos, SEEK_SET);

    return fileSize;
}
//---------------------------------------------------------------------------
QR_Buffer::ISizeType QR_StdFileBuffer::Seek(const ISizeType& start, const ISizeType& delta)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0L;

    // from file start?
    if (!start)
    {
        // no delta to seek to?
        if (!delta)
        {
            // seek to file beginning
            std::fseek(m_FileBuffer, 0, SEEK_SET);
            return 0L;
        }

        // seek to final position
        std::fseek(m_FileBuffer, delta, SEEK_SET);
        return delta;
    }

    // get current offset
    ISizeType offset = GetOffset();

    QR_BufferOffsetType startDelta;

    // do seek to start position?
    if (offset != start)
    {
        // calculate seek to start delta
        startDelta = (QR_BufferOffsetType)start - (QR_BufferOffsetType)offset;

        std::fseek(m_FileBuffer, startDelta, SEEK_CUR);
    }
    else
        startDelta = 0L;

    // seek to final position
    std::fseek(m_FileBuffer, delta, SEEK_CUR);

    return offset + startDelta + delta;
}
//---------------------------------------------------------------------------
QR_Buffer::ISizeType QR_StdFileBuffer::Read(void* pBuffer, const ISizeType& length)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0L;

    // buffer is opened in an incompatible mode?
    if (m_Mode != IE_M_Read && m_Mode != IE_M_RW)
        return 0L;

    // no source buffer?
    if (!pBuffer)
        return 0L;

    // no length to copy?
    if (!length)
        return 0L;

    // create temporary data buffer
    IDataType* pData = new QR_Char[length];

    try
    {
        // iterate through char to copy
        for (ISizeType i = 0; i < length; ++i)
        {
            // read next char
            QR_Int32 c = std::getc(m_FileBuffer);

            // end of file reached?
            if (c == EOF)
            {
                if (!i)
                    return 0L;

                // copy to final buffer
                std::memcpy(pBuffer, pData, i * sizeof(IDataType));

                return i;
            }

            // read next char and copy to buffer
            pData[i] = c;
        }

        // copy to final buffer
        std::memcpy(pBuffer, pData, length * sizeof(IDataType));
    }
    catch (...)
    {
        if (pData)
            delete[] pData;

        return 0L;
    }

    if (pData)
        delete[] pData;

    return length;
}
//---------------------------------------------------------------------------
QR_Buffer::ISizeType QR_StdFileBuffer::Write(void* pBuffer, const ISizeType& length)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0L;

    // buffer is opened in an incompatible mode?
    if (m_Mode != IE_M_Write && m_Mode != IE_M_RW)
        return 0L;

    // write buffer and return successfully written bytes
    return std::fwrite(pBuffer, length, 1, m_FileBuffer);
}
//---------------------------------------------------------------------------
std::string QR_StdFileBuffer::ToStr()
{
    // is file open?
    if (!m_FileBuffer)
        return "";

    // buffer is opened in an incompatible mode?
    if (m_Mode != IE_M_Read && m_Mode != IE_M_RW)
        return "";

    // get buffer size
    ISizeType size = GetSize();

    // seek to start
    Seek(0, 0);

    std::string str;
    str.resize(size);

    Read(&str[0], size);

    return str;
}
//---------------------------------------------------------------------------
void QR_StdFileBuffer::Close()
{
    // is file open?
    if (!m_FileBuffer)
        return;

    // close file
    std::fclose(m_FileBuffer);
}
//---------------------------------------------------------------------------
