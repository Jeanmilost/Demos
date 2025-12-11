/****************************************************************************
 * ==> StdFileBuffer -------------------------------------------------------*
 ****************************************************************************
 * Description: File buffer based on the std library                        *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
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

#include "StdFileBuffer.h"

// std
#include <cstring>
#include <codecvt>

//---------------------------------------------------------------------------
// StdFileBuffer
//---------------------------------------------------------------------------
StdFileBuffer::StdFileBuffer() :
    FileBuffer()
{}
//---------------------------------------------------------------------------
StdFileBuffer::~StdFileBuffer()
{
    Close();
}
//---------------------------------------------------------------------------
bool StdFileBuffer::Open(const std::wstring& fileName, IEMode mode)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return (Open(converter.to_bytes(fileName.c_str()), mode));
}
//---------------------------------------------------------------------------
bool StdFileBuffer::Open(const std::string& fileName, IEMode mode)
{
    // call base function to execute common stuffs
    if (!FileBuffer::Open(fileName, mode))
        return false;

    // open file stream
    #ifdef _MSC_VER
        switch (m_Mode)
        {
            case IEMode::IE_M_Read:  return !fopen_s(&m_FileBuffer, fileName.c_str(), "r+b");
            case IEMode::IE_M_Write: return !fopen_s(&m_FileBuffer, fileName.c_str(), "w+b");
            case IEMode::IE_M_RW:    return !fopen_s(&m_FileBuffer, fileName.c_str(), "rw+b");
            default:                 return false;
        }
    #else
        switch (m_Mode)
        {
            case IEMode::IE_M_Read:  m_FileBuffer = std::fopen(fileName.c_str(), "r+b");  break;
            case IEMode::IE_M_Write: m_FileBuffer = std::fopen(fileName.c_str(), "w+b");  break;
            case IEMode::IE_M_RW:    m_FileBuffer = std::fopen(fileName.c_str(), "rw+b"); break;
            default:                 return false;
        }

        return m_FileBuffer;
    #endif
}
//---------------------------------------------------------------------------
void StdFileBuffer::Clear()
{
    Close();
}
//---------------------------------------------------------------------------
bool StdFileBuffer::Empty()
{
    return (!m_FileBuffer || !GetSize());
}
//---------------------------------------------------------------------------
std::size_t StdFileBuffer::GetOffset() const
{
    // is file opened?
    if (!m_FileBuffer)
        return 0;

    // get current offset
    return std::ftell(m_FileBuffer);
}
//---------------------------------------------------------------------------
std::size_t StdFileBuffer::GetSize() const
{
    // is file opened?
    if (!m_FileBuffer)
        return 0;

    // keep current position
    const std::size_t curPos = std::ftell(m_FileBuffer);

    // get file size
    #ifdef _MSC_VER
        _fseeki64(m_FileBuffer, 0, SEEK_END);
    #else
        std::fseek(m_FileBuffer, 0, SEEK_END);
    #endif
    const std::size_t fileSize = std::ftell(m_FileBuffer);
    #ifdef _MSC_VER
        _fseeki64(m_FileBuffer, curPos, SEEK_SET);
    #else
        std::fseek(m_FileBuffer, curPos, SEEK_SET);
    #endif

    return fileSize;
}
//---------------------------------------------------------------------------
std::size_t StdFileBuffer::Seek(std::size_t start, std::size_t delta)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0;

    // from file start?
    if (!start)
    {
        // no delta to seek to?
        if (!delta)
        {
            // seek to file beginning
            #ifdef _MSC_VER
                _fseeki64(m_FileBuffer, 0, SEEK_SET);
            #else
                std::fseek(m_FileBuffer, 0, SEEK_SET);
            #endif
            return 0L;
        }

        // seek to final position
        #ifdef _MSC_VER
            _fseeki64(m_FileBuffer, delta, SEEK_SET);
        #else
            std::fseek(m_FileBuffer, delta, SEEK_SET);
        #endif
        return delta;
    }

    // get current offset
    const std::size_t offset = GetOffset();

    std::size_t startDelta;

    // do seek to start position?
    if (offset != start)
    {
        // calculate seek to start delta
        startDelta = start - offset;

        #ifdef _MSC_VER
            _fseeki64(m_FileBuffer, startDelta, SEEK_CUR);
        #else
            std::fseek(m_FileBuffer, startDelta, SEEK_CUR);
        #endif
    }
    else
        startDelta = 0;

    // seek to final position
    #ifdef _MSC_VER
        _fseeki64(m_FileBuffer, delta, SEEK_CUR);
    #else
        std::fseek(m_FileBuffer, delta, SEEK_CUR);
    #endif

    return offset + startDelta + delta;
}
//---------------------------------------------------------------------------
std::size_t StdFileBuffer::Read(void* pBuffer, std::size_t length)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0;

    // buffer is opened in an incompatible mode?
    if (m_Mode != IEMode::IE_M_Read && m_Mode != IEMode::IE_M_RW)
        return 0;

    // no source buffer?
    if (!pBuffer)
        return 0;

    // no length to copy?
    if (!length)
        return 0;

    // create temporary data buffer
    std::uint8_t* pData = new std::uint8_t[length];

    try
    {
        // iterate through char to copy
        for (std::size_t i = 0; i < length; ++i)
        {
            // read next char
            std::int32_t c = std::getc(m_FileBuffer);

            // end of file reached?
            if (c == EOF)
            {
                if (!i)
                    return 0;

                // copy to final buffer
                std::memcpy(pBuffer, pData, i);

                return i;
            }

            // read next char and copy to buffer
            pData[i] = c;
        }

        // copy to final buffer
        std::memcpy(pBuffer, pData, length);
    }
    catch (...)
    {
        if (pData)
            delete[] pData;

        return 0;
    }

    if (pData)
        delete[] pData;

    return length;
}
//---------------------------------------------------------------------------
std::size_t StdFileBuffer::Write(const void* pBuffer, std::size_t length)
{
    // no opened file buffer
    if (!m_FileBuffer)
        return 0;

    // buffer is opened in an incompatible mode?
    if (m_Mode != IEMode::IE_M_Write && m_Mode != IEMode::IE_M_RW)
        return 0;

    // write buffer and return successfully written bytes
    return std::fwrite(pBuffer, length, 1, m_FileBuffer);
}
//---------------------------------------------------------------------------
std::string StdFileBuffer::ToStr()
{
    // is file open?
    if (!m_FileBuffer)
        return "";

    // buffer is opened in an incompatible mode?
    if (m_Mode != IEMode::IE_M_Read && m_Mode != IEMode::IE_M_RW)
        return "";

    // get buffer size
    const std::size_t size = GetSize();

    // seek to start
    Seek(0, 0);

    std::string str;
    str.resize(size);

    Read(&str[0], size);

    return str;
}
//---------------------------------------------------------------------------
void StdFileBuffer::Close()
{
    // is file open?
    if (!m_FileBuffer)
        return;

    // close file
    std::fclose(m_FileBuffer);
}
//---------------------------------------------------------------------------
