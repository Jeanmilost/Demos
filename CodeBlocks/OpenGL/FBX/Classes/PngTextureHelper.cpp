/****************************************************************************
 * ==> PngTextureHelper ----------------------------------------------------*
 ****************************************************************************
 * Description : PNG texture helper                                         *
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

#include "PngTextureHelper.h"

// std
#include <memory>
#include <string.h>

//------------------------------------------------------------------------------
bool PngTextureHelper::OpenImage(const std::string& fileName,
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

    png_image image;

    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, fileName.c_str()) != 0)
    {
        // set a simple sRGB format with a non-associated alpha channel as png file format to read
        image.format = is32bit ? PNG_FORMAT_RGBA : PNG_FORMAT_RGB;

        // allocate enough memory to hold the image in this format
        png_bytep pBuffer = (png_bytep)std::malloc(PNG_IMAGE_SIZE(image));

        if (pBuffer && png_image_finish_read(&image, nullptr, pBuffer, 0, nullptr) != 0)
            return LoadImage(image, pBuffer, is32bit, width, height, format, length, pPixels);
        else
        if (!pBuffer)
            png_image_free(&image);
        else
            std::free(pBuffer);
    }

    return false;
}
//------------------------------------------------------------------------------
bool PngTextureHelper::LoadImage(const png_image&   image,
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
    length  = PNG_IMAGE_SIZE(image);
    pPixels = nullptr;

    unsigned char* pPixelArray = new unsigned char[length];

    try
    {
        // search for bitmap format
        switch (format)
        {
            case 24:
            {
                for (unsigned y = 0; y < height; ++y)
                    memcpy(&pPixelArray[y * (width * 3)], &pBuffer[((height - 1) - y) * (width * 3)], width * 3);

                pPixels = pPixelArray;
                break;
            }

            case 32:
            {
                for (unsigned y = 0; y < height; ++y)
                    memcpy(&pPixelArray[y * (width * 4)], &pBuffer[((height - 1) - y) * (width * 4)], width * 4);

                pPixels = pPixelArray;
                break;
            }

            default: throw new std::invalid_argument("Unknown format");
        }
    }
    catch (...)
    {
        // clear memory
        delete[](unsigned char*)pPixels;
        throw;
    }

    return true;
}
//---------------------------------------------------------------------------
