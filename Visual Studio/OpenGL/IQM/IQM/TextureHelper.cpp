/****************************************************************************
 * ==> Texture helper ------------------------------------------------------*
 ****************************************************************************
 * Description : Texture helper                                             *
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

#include "TextureHelper.h"

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
        #ifdef _WINDOWS
            const errno_t error = fopen_s(&pStream, fileName.c_str(), "rb");

            // error occurred?
            if (error != 0)
                return false;
        #else
            pStream = std::fopen(fileName.c_str(), "rb");
        #endif

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

    // close file stream
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

    char intBuffer[4]   = {0};
    char shortBuffer[2] = {0};

    std::size_t offset = 0;

    // read bitmap signature
    std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
    offset += 2;

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return false;

    // skip 8 next bytes
    offset += 8;

    // read data offset
    std::memcpy(&intBuffer[0], &pBuffer[offset], 4);
    offset += 4;
    const unsigned dataOffset = (unsigned)(((unsigned char)intBuffer[3] << 24) |
                                           ((unsigned char)intBuffer[2] << 16) |
                                           ((unsigned char)intBuffer[1] << 8)  |
                                            (unsigned char)intBuffer[0]);

    // read header size
    std::memcpy(&intBuffer[0], &pBuffer[offset], 4);
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
            std::memcpy(&intBuffer[0], &pBuffer[offset], 4);
            offset += 4;
            width   = (std::size_t)(((unsigned char)intBuffer[3] << 24) |
                                    ((unsigned char)intBuffer[2] << 16) |
                                    ((unsigned char)intBuffer[1] << 8)  |
                                     (unsigned char)intBuffer[0]);

            // read bitmap height
            std::memcpy(&intBuffer[0], &pBuffer[offset], 4);
            offset += 4;
            height  = (std::size_t)(((unsigned char)intBuffer[3] << 24) |
                                    ((unsigned char)intBuffer[2] << 16) |
                                    ((unsigned char)intBuffer[1] << 8)  |
                                     (unsigned char)intBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap bit per pixels
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            unsigned short bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                                   (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            // read bitmap compressed flag
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
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
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            width = (std::size_t)(((unsigned char)shortBuffer[1] << 8) |
                                   (unsigned char)shortBuffer[0]);

            // read bitmap height
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            height = (std::size_t)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap bit per pixels
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
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
            throw new std::exception("Unsupported bitmap format");

        // Windows V4
        case 108:
        {
            // read bitmap width
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            width   = (std::size_t)(((unsigned char)shortBuffer[1] << 8) |
                                     (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            offset += 2;

            // read bitmap height
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
            offset += 2;
            height  = (std::size_t)(((unsigned char)shortBuffer[1] << 8) |
                                     (unsigned char)shortBuffer[0]);

            // skip next 4 bytes
            offset += 4;

            // read bitmap bit per pixels
            std::memcpy(&shortBuffer[0], &pBuffer[offset], 2);
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
            throw new std::exception("Unsupported bitmap format");

        default:
            throw new std::exception("Unsupported bitmap format");
    }

    unsigned char* pBitmapData = nullptr;

    try
    {
        const unsigned bytesPerRow = (unsigned)(((width * 3 + 3) / 4) * 4 - (width * 3 % 4));
        const unsigned bitmapSize  = bytesPerRow * (unsigned)height;

        pBitmapData = new unsigned char[bitmapSize];

        // read bitmap data
        std::memcpy(pBitmapData, &pBuffer[dataOffset], bitmapSize);

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
                                    pBitmapData[bytesPerRow * ((unsigned)((height - 1) - y) + (3 * x) + (2 - c))];

                pPixels = pPixelArray;
                break;
            }

            default: throw new std::exception("Unknown format");
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
bool TextureHelper::OpenTgaData(const std::string& fileName,
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
        #ifdef _WINDOWS
            const errno_t error = fopen_s(&pStream, fileName.c_str(), "rb");

            // error occurred?
            if (error != 0)
                return false;
        #else
            pStream = std::fopen(fileName.c_str(), "rb");
        #endif

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

    // close file stream
    if (pStream)
        std::fclose(pStream);

    if (!LoadTgaData(pBuffer, width, height, format, length, pPixels))
        success = false;

    // delete buffer, if needed
    if (pBuffer)
        delete[] pBuffer;

    return success;
}
//---------------------------------------------------------------------------
bool TextureHelper::LoadTgaData(const unsigned char* pBuffer,
                                      std::size_t&   width,
                                      std::size_t&   height,
                                      std::size_t&   format,
                                      std::size_t&   length,
                                      void*&         pPixels)
{
    // validate the input
    if (!pBuffer)
        return false;

    ITGAHeader  header;
    std::size_t offset = 0;

    // read the header
    std::memcpy(&header, &pBuffer[offset], sizeof(ITGAHeader));

    // is image file valid?
    if (header.m_PixelSize != 8 && header.m_PixelSize != 24 && header.m_PixelSize != 32)
        return false;

    offset += sizeof(ITGAHeader) + header.m_IdentSize;

    // get the image width and height
    width  = header.m_Width[0]  + ((std::size_t)header.m_Width[1]  << 8);
    height = header.m_Height[0] + ((std::size_t)header.m_Height[1] << 8);

    unsigned char buffer[128] = {0};

    switch (header.m_ImageType)
    {
        case 1:
        {
            // validate the color map entry size
            if (header.m_CMapEntrySize != 8 && header.m_CMapEntrySize != 24 && header.m_CMapEntrySize != 32)
                return false;

            // get the color map size
            unsigned cMapSize = header.m_CMapSize[0] + (header.m_CMapSize[1] << 8);

            // get the pixel format
            format = header.m_PixelSize;

            // calculate the byte per pixels, and allocate memory for color map
            const std::size_t    bytePerPixel = header.m_CMapEntrySize / 8;
            const std::size_t    cMapLength   = (std::size_t)bytePerPixel * (std::size_t)cMapSize;
                  unsigned char* pCMap        = new unsigned char[cMapLength];

            // read the color map
            std::memcpy(pCMap, &pBuffer[offset], cMapLength);

            offset += cMapLength;

            // do convert the BGR pixel order to RGB?
            if (bytePerPixel >= 3)
                BGRToRGB(pCMap, (int)bytePerPixel * cMapSize, (int)bytePerPixel);

            // allocate memory for pixels and get indexes pointer
                                 length      = bytePerPixel * width * height;
                                 pPixels     = new unsigned char[length];
                  unsigned char* pIndexes    = &((unsigned char*)pPixels)[(bytePerPixel - 1) * width * height];
            const std::size_t    indexLength = width * height;

            // read the index table
            std::memcpy(pIndexes, &pBuffer[offset], indexLength);

            offset += indexLength;

            unsigned char* pSrc = pIndexes;
            unsigned char* pDst = &((unsigned char*)pPixels)[length];

            // copy the pixels
            for (std::size_t i = 0; i < height; ++i)
            {
                unsigned char* pRow;

                pDst -= bytePerPixel * width;
                pRow  = pDst;

                for (std::size_t j = 0; j < width; ++j)
                {
                    std::memcpy(pRow, &pCMap[*pSrc++ * bytePerPixel], bytePerPixel);
                    pRow += bytePerPixel;
                }
            }

            delete[] pCMap;

            return true;
        }

        case 2:
        {
            // get the pixel format
            format = header.m_PixelSize;

            // calculate the byte per pixels
            const std::size_t bytePerPixel = header.m_PixelSize / 8;

            // calculate the data length
            length = bytePerPixel * width * height;

            // create destination pixel buffer
                                 pPixels = new unsigned char[length];
                  unsigned char* pDst    = &((unsigned char*)pPixels)[length];
            const std::size_t    size    = bytePerPixel * width;

            // read the pixels
            for (std::size_t i = 0; i < height; ++i)
            {
                // move to the next row to read
                pDst -= bytePerPixel * width;

                // read the row
                std::memcpy(pDst, &pBuffer[offset], size);

                // go to next offset
                offset += size;
            }

            // do convert the BGR pixel order to RGB?
            if (bytePerPixel >= 3)
                BGRToRGB((unsigned char*)pPixels, (int)length, (int)bytePerPixel);

            return true;
        }

        case 9:
        {
            // validate the color map entry size
            if (header.m_CMapEntrySize != 8 && header.m_CMapEntrySize != 24 && header.m_CMapEntrySize != 32)
                return false;

            // get the color map size
            const unsigned cMapSize = header.m_CMapSize[0] + (header.m_CMapSize[1] << 8);

            // get the pixel format
            format = header.m_PixelSize;

            // calculate the byte per pixels, and allocate memory for color map
            const std::size_t    bytePerPixel = header.m_CMapEntrySize / 8;
            const std::size_t    cMapLength   = bytePerPixel * cMapSize;
                  unsigned char* pCMap        = new unsigned char[cMapLength];

            // read the color map
            std::memcpy(pCMap, &pBuffer[offset], cMapLength);

            offset += cMapLength;

            // do convert the BGR pixel order to RGB?
            if (bytePerPixel >= 3)
                BGRToRGB(pCMap, (int)bytePerPixel * cMapSize, (int)bytePerPixel);

            // allocate memory for pixels
            length  = bytePerPixel * width * height;
            pPixels = new unsigned char[length];

            unsigned char* pEnd = &((unsigned char*)pPixels)[length];
            unsigned char* pDst = pEnd - (bytePerPixel * width);

            // iterate though pixels to read
            for (; pDst >= (unsigned char*)pPixels; )
            {
                int c;

                // read next char
                std::memcpy(&c, &pBuffer[offset], 1);
                ++offset;

                // compressed pixel?
                if (c & 0x80)
                {
                    int index;

                    // read next char
                    std::memcpy(&index, &pBuffer[offset], 1);
                    ++offset;

                    unsigned char* pCol = &pCMap[index * bytePerPixel];

                    c -= 0x7F;
                    c *= (int)bytePerPixel;

                    while (c > 0 && pDst >= (unsigned char*)pPixels)
                    {
                        const float nF = std::min((float)c, (float)(pEnd - pDst));
                        const int   n  = (int)nF;

                        for (unsigned char* pRun = pDst + n; pDst < pRun; pDst += bytePerPixel)
                            std::memcpy(pDst, pCol, bytePerPixel);

                        c -= n;

                        if (pDst >= pEnd)
                        {
                            pEnd -= bytePerPixel * width;
                            pDst  = pEnd - bytePerPixel * width;
                        }
                    }
                }
                else
                {
                    c += 1;

                    while (c > 0 && pDst >= (unsigned char*)pPixels)
                    {
                        const float nF = std::min((float)c, (float)(pEnd - pDst) / bytePerPixel);
                        const int   n  = (int)nF;

                        // read the row
                        std::memcpy(&buffer, &pBuffer[offset], n);
                        offset += n;

                        for (unsigned char* pSrc = buffer; pSrc < &buffer[n]; pDst += bytePerPixel)
                            std::memcpy(pDst, &pCMap[*pSrc++ * bytePerPixel], bytePerPixel);

                        c -= n;

                        if (pDst >= pEnd)
                        {
                            pEnd -= bytePerPixel * width;
                            pDst  = pEnd - bytePerPixel * width;
                        }
                    }
                }
            }

            delete[] pCMap;

            return true;
        }

        case 10:
        {
            // get the pixel format
            format = header.m_PixelSize;

            // calculate the byte per pixels
            const std::size_t bytePerPixel = header.m_PixelSize / 8;

            // allocate memory for pixels
            length  = bytePerPixel * width * height;
            pPixels = new unsigned char[length];

            unsigned char* pEnd = &((unsigned char*)pPixels)[length];
            unsigned char* pDst = pEnd - bytePerPixel * width;

            // iterate though pixels to read
            for (; pDst >= (unsigned char*)pPixels; )
            {
                int c;

                // read next char
                std::memcpy(&c, &pBuffer[offset], 1);
                ++offset;

                // compressed pixel?
                if (c & 0x80)
                {
                    // read pixel
                    std::memcpy(buffer, &pBuffer[offset], bytePerPixel);

                    offset += bytePerPixel;

                    c -= 0x7F;

                    if (bytePerPixel >= 3)
                    {
                        buffer[0] = buffer[0] ^ buffer[2];
                        buffer[2] = buffer[0] ^ buffer[2];
                        buffer[0] = buffer[2] ^ buffer[0];
                    }

                    c *= (int)bytePerPixel;

                    while (c > 0)
                    {
                        const float nF = std::min((float)c, (float)(pEnd - pDst));
                        const int   n  = (int)nF;

                        for (unsigned char* pRun = pDst + n; pDst < pRun; pDst += bytePerPixel)
                            std::memcpy(pDst, buffer, bytePerPixel);

                        c -= n;

                        if (pDst >= pEnd)
                        {
                            pEnd -= bytePerPixel * width;
                            pDst  = pEnd - bytePerPixel * width;

                            if (pDst < (unsigned char*)pPixels)
                                break;
                        }
                    }
                }
                else
                {
                    c += 1;
                    c *= (int)bytePerPixel;

                    while (c > 0)
                    {
                        const float nF = std::min((float)c, (float)(pEnd - pDst));
                        const int   n = (int)nF;

                        // read pixel
                        std::memcpy(pDst, &pBuffer[offset], n);

                        if (bytePerPixel >= 3)
                            BGRToRGB(pDst, n, (int)bytePerPixel);

                        pDst += n;
                        c    -= n;

                        if (pDst >= pEnd)
                        {
                            pEnd -= bytePerPixel * width;
                            pDst  = pEnd - bytePerPixel * width;

                            if (pDst < (unsigned char*)pPixels)
                                break;
                        }
                    }
                }
            }

            return true;
        }

        default:
            return false;
    }
}
//---------------------------------------------------------------------------
