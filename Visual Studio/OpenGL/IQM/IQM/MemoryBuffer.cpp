/****************************************************************************
 * ==> MemoryBuffer --------------------------------------------------------*
 ****************************************************************************
 * Description : Buffer keeping data in memory                              *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - iqm model reader                                           *
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

#include "MemoryBuffer.h"

// std
#include <cstring>

//---------------------------------------------------------------------------
// MemoryBuffer
//---------------------------------------------------------------------------
MemoryBuffer::MemoryBuffer() :
    Buffer()
{}
//---------------------------------------------------------------------------
MemoryBuffer::~MemoryBuffer()
{}
//---------------------------------------------------------------------------
void MemoryBuffer::Clear()
{
    // clear data buffer
    m_Data.clear();

    // reset values
    m_Offset = 0;
}
//---------------------------------------------------------------------------
bool MemoryBuffer::Empty()
{
    return m_Data.empty();
}
//---------------------------------------------------------------------------
const std::uint8_t* MemoryBuffer::GetPtr() const
{
    // are data empty?
    if (m_Data.empty())
        return nullptr;

    return &m_Data[0];
}
//---------------------------------------------------------------------------
std::size_t MemoryBuffer::GetOffset() const
{
    return m_Offset;
}
//---------------------------------------------------------------------------
std::size_t MemoryBuffer::GetSize() const
{
    return m_Data.size();
}
//---------------------------------------------------------------------------
std::size_t MemoryBuffer::Seek(std::size_t start, std::size_t delta)
{
    // calculate new offset position
    m_Offset = (start + delta);

    // get data size
    const std::size_t dataSize = m_Data.size();

    // is offset out of bounds?
    if (m_Offset >= dataSize)
        // extend data buffer to be able to contain data up to the offset
        m_Data.resize((m_Offset + 1) - dataSize);

    return m_Offset;
}
//---------------------------------------------------------------------------
std::size_t MemoryBuffer::Read(void* pBuffer, std::size_t length)
{
    // no destination buffer to copy to?
    if (!pBuffer)
        return 0;

    // no length to copy?
    if (!length)
        return 0;

    // get data size
    const std::size_t dataSize = m_Data.size();

    // no source data?
    if (!dataSize)
        return 0;

    // is offset out of buffer?
    if (m_Offset >= dataSize)
        return 0;

    // calculate end offset
    std::size_t endOffset = m_Offset + length;

    // is end offset out of bounds?
    if (endOffset > dataSize)
        endOffset = dataSize;

    // calculate length to copy
    const std::size_t lengthToCopy = endOffset - m_Offset;

    // copy buffer content to destination
    std::memcpy(pBuffer, &m_Data[m_Offset], lengthToCopy);

    // update offset position
    m_Offset = endOffset;

    return lengthToCopy;
}
//---------------------------------------------------------------------------
std::size_t MemoryBuffer::Write(const void* pBuffer, std::size_t length)
{
    // no source buffer to copy from?
    if (!pBuffer)
        return 0;

    // no length to copy?
    if (!length)
        return 0;

    // get data size
    const std::size_t dataSize = m_Data.size();

    // calculate end offset
    std::size_t endOffset = m_Offset + length;

    // is end offset out of bounds?
    if (endOffset > dataSize)
        // extend buffer to contain new data to write
        m_Data.resize(endOffset - dataSize);

    // calculate length to copy
    const std::size_t lengthToCopy = endOffset - m_Offset;

    // copy buffer content to destination
    std::memcpy(&m_Data[m_Offset], pBuffer, lengthToCopy);

    // update offset position
    m_Offset = endOffset;

    return lengthToCopy;
}
//---------------------------------------------------------------------------
std::string MemoryBuffer::ToStr()
{
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
