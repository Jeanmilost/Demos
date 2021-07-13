/****************************************************************************
 * ==> Texture helper ------------------------------------------------------*
 ****************************************************************************
 * Description : Texture helper                                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - x model reader                                             *
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

#include "TextureHelper.h"

// std
#include <cstdio>
#include <string.h>
#include <stdexcept>

//------------------------------------------------------------------------------
bool TextureHelper::OpenBitmapData(const std::string& fileName,
                                         std::size_t& width,
                                         std::size_t& height,
                                         std::size_t& format,
                                         std::size_t& length,
                                         void*&       pPixels)
{
    // no file name?
    if (fileName.empty())
        return false;

    unsigned char* pBuffer    = nullptr;
    std::FILE*     pStream    = nullptr;
    std::size_t    fileSize   = 0;
    std::size_t    bufferSize = 0;
    bool           success    = true;

    try
    {
        // open file for read
        pStream = std::fopen(fileName.c_str(), "rb");

        // is file stream opened?
        if (!pStream)
            return false;

        // get file size
        std::fseek(pStream, 0, SEEK_END);
        fileSize = std::ftell(pStream);
        std::fseek(pStream, 0, SEEK_SET);

        // copy file content to buffer
        pBuffer    = new unsigned char[fileSize + 1];
        bufferSize = std::fread(pBuffer, 1, fileSize, pStream);
    }
    catch (...)
    {
        success = false;
    }

    // close cached xml file
    if (pStream)
        std::fclose(pStream);

    if (!LoadBitmapData(pBuffer, width, height, format, length, pPixels))
        success = false;

    // delete buffer, if needed
    if (pBuffer)
        delete[] pBuffer;

    return success;
}
//------------------------------------------------------------------------------
bool TextureHelper::LoadBitmapData(const unsigned char* pBuffer,
                                         std::size_t&   width,
                                         std::size_t&   height,
                                         std::size_t&   format,
                                         std::size_t&   length,
                                         void*&         pPixels)
{
    pPixels = nullptr;

    char intBuffer[4];
    char shortBuffer[2];

    std::size_t offset = 0;

    // read bitmap signature
    memcpy(&shortBuffer[0], &pBuffer[offset], 2);
    offset += 2;

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return false;

    // skip 8 next bytes
    offset += 8;

    // read data offset
    memcpy(&intBuffer[0], &pBuffer[offset], 4);
    offset += 4;
    const unsigned dataOffset = (unsigned)(((unsigned char)intBuffer[3] << 24) |
                                           ((unsigned char)intBuffer[2] << 16) |
                                           ((unsigned char)intBuffer[1] << 8)  |
                                            (unsigned char)intBuffer[0]);

    // read header size
    memcpy(&intBuffer[0], &pBuffer[offset], 4);
    offset += 4;
    const unsigned headerSize = (unsigned)(((unsigned char)intBuffer[3] << 24) |
                                           ((unsigned char)intBuffer[2] << 16) |
                                           ((unsigned char)intBuffer[1] << 8)  |
                                            (unsigned char)intBuffer[0]);

    // search for bitmap type
    switch (headerSize)
    {
        // V3
        case 40:
        {
            // read bitmap width
            memcpy(&intBuffer[0], &pBuffer[offset], 4);
            offset += 4;
            width = (unsigned)(((unsigned char)intBuffer[3] << 24) |
                               ((unsigned char)intBuffer[2] << 16) |
                               ((unsigned char)intBuffer[1] << 8)  |
                                (unsigned char)intBuffer[0]);

            // read bitmap height
            memcpy(&intBuffer[0], &pBuffer[offset], 4);
            offset += 4;
            height = (unsigned)(((unsigned char)intBuffer[3] << 24) |
                                ((unsigned char)intBuffer[2] << 16) |
                                ((unsigned char)intBuffer[1] << 8)  |
                                 (unsigned char)intBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap bit per pixels
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            unsigned short bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                                   (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            // read bitmap compressed flag
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            unsigned short compressed = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                                          (unsigned char)shortBuffer[0]);

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
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            width = (unsigned)(((unsigned char)shortBuffer[1] << 8) |
                                (unsigned char)shortBuffer[0]);

            // read bitmap height
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            height = (unsigned)(((unsigned char)shortBuffer[1] << 8) |
                                 (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap bit per pixels
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            const unsigned short bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                                         (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // OS/2 V2
        case 64:
            throw new std::invalid_argument("Unsupported bitmap format");

        // Windows V4
        case 108:
        {
            // read bitmap width
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            width = (unsigned)(((unsigned char)shortBuffer[1] << 8) |
                                (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap height
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            height = (unsigned)(((unsigned char)shortBuffer[1] << 8) |
                                 (unsigned char)shortBuffer[0]);

            // skip next 4 bytes
            offset += 4;

            // read bitmap bit per pixels
            memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            const unsigned short bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                                         (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // Windows V5
        case 124:
            throw new std::invalid_argument("Unsupported bitmap format");

        default:
            throw new std::invalid_argument("Unsupported bitmap format");
    }

    unsigned char* pBitmapData = nullptr;

    try
    {
        const unsigned bytesPerRow = (unsigned)((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
        const unsigned bitmapSize  = bytesPerRow * (unsigned)height;

        pBitmapData = new unsigned char[bitmapSize];

        // read bitmap data
        memcpy(pBitmapData, &pBuffer[dataOffset], bitmapSize);

        // search for bitmap format
        switch (format)
        {
            case 24:
            {
                // calculate pixels array length
                length = (width * height * 3);

                unsigned char* pPixelArray = new unsigned char[length];

                // get bitmap data in right format
                for (unsigned y = 0; y < height; ++y)
                    for (unsigned x = 0; x < width; ++x)
                        for (unsigned char c = 0; c < 3; ++c)
                            pPixelArray[3 * (width * y + x) + c] =
                                    pBitmapData[(bytesPerRow * ((height - 1) - y)) + (3 * (std::size_t)x) + (2 - c)];

                pPixels = pPixelArray;
                break;
            }

            default: throw new std::invalid_argument("Unknown format");
        }
    }
    catch (...)
    {
        // clear memory
        delete[] (unsigned char*)pPixels;
        delete[] pBitmapData;

        throw;
    }

    // clear memory
    delete[] pBitmapData;

    return true;
}
//---------------------------------------------------------------------------
