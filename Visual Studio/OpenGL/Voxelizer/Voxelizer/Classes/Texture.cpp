/****************************************************************************
 * ==> Texture -------------------------------------------------------------*
 ****************************************************************************
 * Description: Texture descriptor                                          *
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

#include "Texture.h"

// std
#include <memory>
#include <string>

// classes
#include "MemoryBuffer.h"
#include "StdFileBuffer.h"

//---------------------------------------------------------------------------
// Texture
//---------------------------------------------------------------------------
Texture::Texture() :
    m_Format(IEFormat::IE_FT_24bit),
    m_Target(IETarget::IE_TT_Texture_2D),
    m_WrapMode(IEWrapMode::IE_WM_Repeat),
    m_MinFilter(IEMinFilter::IE_MI_Linear),
    m_MagFilter(IEMagFilter::IE_MA_Linear),
    m_Width(0),
    m_Height(0)
{}
//---------------------------------------------------------------------------
Texture::~Texture()
{}
//---------------------------------------------------------------------------
void Texture::Clear()
{
    m_Format    = IEFormat::IE_FT_24bit;
    m_Target    = IETarget::IE_TT_Texture_2D;
    m_WrapMode  = IEWrapMode::IE_WM_Repeat;
    m_MinFilter = IEMinFilter::IE_MI_Linear;
    m_MagFilter = IEMagFilter::IE_MA_Linear;
    m_Width     = 0;
    m_Height    = 0;
}
//------------------------------------------------------------------------------
bool Texture::GetPixelsFromBitmap(const std::string& fileName,
                                        std::size_t& width,
                                        std::size_t& height,
                                        std::size_t& format,
                                        std::size_t& length,
                                        void*&       pPixels)
{
    // no file name?
    if (fileName.empty())
        return false;

    // open file for read
    std::unique_ptr<StdFileBuffer> pFileBuffer(new StdFileBuffer());
    pFileBuffer->Open(fileName, FileBuffer::IEMode::IE_M_Read);
    pFileBuffer->Seek(0, 0);

    return GetPixelsFromBitmap(*pFileBuffer, width, height, format, length, pPixels);
}
//------------------------------------------------------------------------------
bool Texture::GetPixelsFromBitmap(Buffer&      buffer,
                                  std::size_t& width,
                                  std::size_t& height,
                                  std::size_t& format,
                                  std::size_t& length,
                                  void*&       pPixels)
{
    pPixels = nullptr;

    std::uint8_t shortBuffer[2] = {0};

    // read bitmap signature
    buffer.Read(&shortBuffer[0], 2);

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return false;

    // skip 8 next bytes
    buffer.Seek(buffer.GetOffset(), 8);

    std::uint32_t dataOffset = 0;
    std::uint32_t headerSize = 0;
    std::uint16_t shortValue = 0;
    std::uint32_t value      = 0;

    // read data offset and header size
    buffer.Read(&dataOffset, sizeof(std::uint32_t));
    buffer.Read(&headerSize, sizeof(std::uint32_t));

    // search for bitmap type
    switch (headerSize)
    {
        // V3
        case 40:
        {
            // read bitmap width
            buffer.Read(&value, sizeof(std::uint32_t));
            width = value;

            // read bitmap height
            buffer.Read(&value, sizeof(std::uint32_t));
            height = value;

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            std::uint16_t bpp = 0;

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

            format = 24;
            break;
        }

        // OS/2 V1
        case 12:
        {
            // read bitmap width
            buffer.Read(&shortValue, sizeof(std::uint16_t));
            width = (std::size_t)shortValue;

            // read bitmap height
            buffer.Read(&shortValue, sizeof(std::uint16_t));
            height = (std::size_t)shortValue;

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            std::uint16_t bpp = 0;

            // read bitmap bit per pixels
            buffer.Read(&bpp, sizeof(std::uint16_t));

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // OS/2 V2
        case 64:
            throw new std::exception("Unsupported bitmap format - OS/2 V2");

        // Windows V4
        case 108:
        {
            // read bitmap width
            buffer.Read(&shortValue, sizeof(std::uint16_t));
            width = (std::size_t)shortValue;

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap height
            buffer.Read(&shortValue, sizeof(std::uint16_t));
            height = (std::size_t)shortValue;

            // skip next 4 bytes
            buffer.Seek(buffer.GetOffset(), 4);

            std::uint16_t bpp = 0;

            // read bitmap bit per pixels
            buffer.Read(&bpp, sizeof(std::uint16_t));

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // Windows V5
        case 124:
            throw new std::exception("Unsupported bitmap format - Windows V5");

        default:
            throw new std::exception(("Unsupported bitmap format - unknown - " + std::to_string(headerSize)).c_str());
    }

    std::uint8_t* pBitmapData = nullptr;
    std::uint8_t* pPixelArray = nullptr;

    try
    {
        const std::uint32_t bytesPerRow = (std::uint32_t)((std::size_t)((width * 3 + 3) / 4) * 4 - (width * 3 % 4));
        const std::uint32_t bitmapSize  = bytesPerRow * (std::uint32_t)height;

        pBitmapData = new std::uint8_t[bitmapSize];

        // read bitmap data
        buffer.Seek(0, dataOffset);
        buffer.Read(pBitmapData, bitmapSize);

        // search for bitmap format
        switch (format)
        {
            case 24:
                // calculate pixels array length
                length = (width * height * 3);

                pPixelArray = new std::uint8_t[length];

                // get bitmap data in right format
                for (std::uint32_t y = 0; y < height; ++y)
                    for (std::uint32_t x = 0; x < width; ++x)
                        for (std::uint8_t c = 0; c < 3; ++c)
                            pPixelArray[3 * (width * y + x) + c] =
                                    pBitmapData[((std::size_t)bytesPerRow * (((std::size_t)height - 1) - (std::size_t)y)) +
                                                                       (3 *   (std::size_t)x)     + (2 - (std::size_t)c)];

                pPixels = pPixelArray;
                break;

            default:
                throw new std::exception(("Unsupported bitmap format - " + std::to_string(format)).c_str());
        }
    }
    catch (...)
    {
        // clear memory
        delete[] pPixelArray;
        delete[] pBitmapData;

        throw;
    }

    // clear memory
    delete[] pBitmapData;

    return true;
}
//------------------------------------------------------------------------------
bool Texture::GetPixelsFromPng(const std::string& fileName,
                                     bool         is32bit,
                                     std::size_t& width,
                                     std::size_t& height,
                                     std::size_t& format,
                                     std::size_t& length,
                                     void*&       pPixels)
{
    // no file name?
    if (fileName.empty())
        return false;

    png_image image = {};
    image.version   = PNG_IMAGE_VERSION;

    // open image
    if (png_image_begin_read_from_file(&image, fileName.c_str()) != 0)
    {
        // set a simple sRGB format with a non-associated alpha channel as png file format to read
        image.format = is32bit ? PNG_FORMAT_RGBA : PNG_FORMAT_RGB;

        // allocate enough memory to hold the image in this format
        png_bytep pBuffer = (png_bytep)std::malloc((std::size_t)PNG_IMAGE_SIZE(image));

        if (pBuffer && png_image_finish_read(&image, nullptr, pBuffer, 0, nullptr) != 0)
            return GetPixelsFromPng(image, pBuffer, is32bit, width, height, format, length, pPixels);
        else
        if (!pBuffer)
            png_image_free(&image);
        else
            std::free(pBuffer);
    }

    return false;
}
//------------------------------------------------------------------------------
bool Texture::GetPixelsFromPng(Buffer&      buffer,
                               bool         is32bit,
                               std::size_t& width,
                               std::size_t& height,
                               std::size_t& format,
                               std::size_t& length,
                               void*&       pPixels)
{
    std::uint8_t* pMemBuffer = nullptr;

    try
    {
        // copy buffer content in memory
        const std::size_t size = buffer.GetSize() - buffer.GetOffset();
        pMemBuffer             = new std::uint8_t[size];
        buffer.Read(&pMemBuffer[0], size);

        png_image image = {};
        image.version   = PNG_IMAGE_VERSION;

        // open image
        if (png_image_begin_read_from_memory(&image, &pMemBuffer[0], size) != 0)
        {
            // set a simple sRGB format with a non-associated alpha channel as png file format to read
            image.format = is32bit ? PNG_FORMAT_RGBA : PNG_FORMAT_RGB;

            // allocate enough memory to hold the image in this format
            png_bytep pBuffer = (png_bytep)std::malloc((std::size_t)PNG_IMAGE_SIZE(image));

            if (pBuffer && png_image_finish_read(&image, nullptr, pBuffer, 0, nullptr) != 0)
                return GetPixelsFromPng(image, pBuffer, is32bit, width, height, format, length, pPixels);
            else
            if (!pBuffer)
                png_image_free(&image);
            else
                std::free(pBuffer);
        }
    }
    catch (...)
    {
        // clear memory
        if (pMemBuffer)
            delete[] pMemBuffer;

        throw;
    }

    // clear memory
    if (pMemBuffer)
        delete[] pMemBuffer;

    return false;
}
//------------------------------------------------------------------------------
bool Texture::GetPixelsFromPng(const png_image&   image,
                               const png_bytep    pBuffer,
                                     bool         is32bit,
                                     std::size_t& width,
                                     std::size_t& height,
                                     std::size_t& format,
                                     std::size_t& length,
                                     void*&       pPixels)
{
    width   = image.width;
    height  = image.height;
    format  = is32bit ? 32 : 24;
    length  = (std::size_t)PNG_IMAGE_SIZE(image);
    pPixels = nullptr;

    std::uint8_t* pPixelArray = nullptr;

    try
    {
        pPixelArray = new std::uint8_t[length];

        // search for bitmap format
        switch (format)
        {
            case 24:
                for (std::uint32_t y = 0; y < height; ++y)
                    std::memcpy(&pPixelArray[y * (width * 3)], &pBuffer[((height - 1) - y) * (width * 3)], width * 3);

                pPixels = pPixelArray;
                break;

            case 32:
                for (std::uint32_t y = 0; y < height; ++y)
                    std::memcpy(&pPixelArray[y * (width * 4)], &pBuffer[((height - 1) - y) * (width * 4)], width * 4);

                pPixels = pPixelArray;
                break;

            default:
                throw new std::exception(("Unsupported png format - " + std::to_string(format)).c_str());
        }
    }
    catch (...)
    {
        // clear memory
        if (pPixelArray)
            delete[] pPixelArray;

        throw;
    }

    return true;
}
//---------------------------------------------------------------------------
