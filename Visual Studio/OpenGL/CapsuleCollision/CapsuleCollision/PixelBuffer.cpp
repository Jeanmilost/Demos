/****************************************************************************
 * ==> PixelBuffer ---------------------------------------------------------*
 ****************************************************************************
 * Description: Pixel buffer                                                *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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

#include "PixelBuffer.h"

// std
#include <memory>

// classes
#include "StdFileBuffer.h"

//---------------------------------------------------------------------------
// PixelBuffer
//---------------------------------------------------------------------------
PixelBuffer::PixelBuffer()
{}
//---------------------------------------------------------------------------
PixelBuffer::~PixelBuffer()
{
    if (m_pData)
        delete[] m_pData;
}
//---------------------------------------------------------------------------
bool PixelBuffer::FromBitmap(const std::string& fileName)
{
    // no file name?
    if (fileName.empty())
        return false;

    // no file name?
    if (fileName.empty())
        return false;

    // open the file
    std::unique_ptr<StdFileBuffer> pFileBuffer(new StdFileBuffer());
    pFileBuffer->Open(fileName, FileBuffer::IEMode::IE_M_Read);
    pFileBuffer->Seek(0, 0);

    return FromBitmap(*pFileBuffer);
}
//---------------------------------------------------------------------------
bool PixelBuffer::FromBitmap(const std::wstring& fileName)
{
    // no file name?
    if (fileName.empty())
        return false;

    // open the file
    std::unique_ptr<StdFileBuffer> pFileBuffer(new StdFileBuffer());
    pFileBuffer->Open(fileName, FileBuffer::IEMode::IE_M_Read);
    pFileBuffer->Seek(0, 0);

    return FromBitmap(*pFileBuffer);
}
//---------------------------------------------------------------------------
bool PixelBuffer::FromBitmap(Buffer& buffer)
{
    // validate the input
    if (!buffer.GetSize())
        return false;

    const std::size_t  startOffset = buffer.GetOffset();
          std::uint8_t signature[2];

    // read bitmap signature
    buffer.Read(&signature[0], 2);

    // is bitmap signature correct?
    if (signature[0] != 'B' || signature[1] != 'M')
        return 0;

    // skip 8 next bytes
    buffer.Seek(buffer.GetOffset(), 8);

    std::uint32_t dataOffset = 0;

    // read data offset
    buffer.Read(&dataOffset, sizeof(std::uint32_t));

    std::uint32_t headerSize = 0;

    // read header size
    buffer.Read(&headerSize, sizeof(std::uint32_t));

    std::uint16_t bpp = 0;

    // search for bitmap type
    switch (headerSize)
    {
        // V3
        case 40:
        {
            // read bitmap width
            buffer.Read(&m_Width, sizeof(std::uint32_t));

            // read bitmap height
            buffer.Read(&m_Height, sizeof(std::uint32_t));

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap bit per pixels
            buffer.Read(&bpp, sizeof(std::uint16_t));

            // is bpp supported?
            if (bpp != 24)
                return false;

            std::uint16_t compressed = 0;

            // read bitmap compressed flag
            buffer.Read(&compressed, sizeof(std::uint16_t));

            // is compressed?
            if (compressed)
                return false;

            break;
        }

        // OS/2 V1
        case 12:
        {
            std::uint16_t width  = 0;
            std::uint16_t height = 0;

            // read bitmap width
            buffer.Read(&width, sizeof(std::uint16_t));

            // read bitmap height
            buffer.Read(&height, sizeof(std::uint16_t));

            m_Width  = width;
            m_Height = height;

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap bit per pixels
            buffer.Read(&bpp, sizeof(std::uint16_t));

            // is bpp supported?
            if (bpp != 24)
                return false;

            break;
        }

        // Windows V4
        case 108:
        {
            std::uint16_t width  = 0;
            std::uint16_t height = 0;

            // read bitmap width
            buffer.Read(&width, sizeof(std::uint16_t));

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap height
            buffer.Read(&height, sizeof(std::uint16_t));

            m_Width  = width;
            m_Height = height;

            // skip next 4 bytes
            buffer.Seek(buffer.GetOffset(), 4);

            // read bitmap bit per pixels
            buffer.Read(&bpp, sizeof(std::uint16_t));

            // is bpp supported?
            if (bpp != 24)
                return false;

            break;
        }

        default:
            // unsupported bitmap format
            return false;
    }

    m_ImageType    = PixelBuffer::IEImageType::IE_IT_Bitmap;
    m_PixelType    = PixelBuffer::IEPixelType::IE_PT_BGR;
    m_BytePerPixel = bpp / 8;
    m_Stride       = (((m_Width) * 3 + 3) / 4) * 4 - ((m_Width) * 3 % 4);
    m_DataLength   = m_Stride * m_Height;
    m_pData        = new std::uint8_t[m_DataLength];

    buffer.Seek(startOffset, dataOffset);

    // read bitmap data
    buffer.Read(m_pData, m_DataLength);

    return true;
}
//---------------------------------------------------------------------------
