/****************************************************************************
 * ==> DWF_TextureHelper ---------------------------------------------------*
 ****************************************************************************
 * Description : Texture helper                                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
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

#pragma once

// std
#include <string>

// libraries
#ifdef USE_PNG
    #include <png.h>
#endif

namespace DWF_Model
{
    /**
    * Texture helper
    *@author Jean-Milost Reymond
    */
    class TextureHelper
    {
        public:
            /**
            * Targa (.tga) header
            */
            struct ITGAHeader
            {
                unsigned char m_IdentSize     =  0;
                unsigned char m_CMapType      =  0;
                unsigned char m_ImageType     =  0;
                unsigned char m_CMapOrigin[2] = {0};
                unsigned char m_CMapSize[2]   = {0};
                unsigned char m_CMapEntrySize =  0;
                unsigned char m_XOrigin[2]    = {0};
                unsigned char m_YOrigin[2]    = {0};
                unsigned char m_Width[2]      = {0};
                unsigned char m_Height[2]     = {0};
                unsigned char m_PixelSize     =  0;
                unsigned char m_DescByte      =  0;
            };

            /**
            * Loads bitmap from file and convert to pixel array
            *@param fileName - bitmap file name to load
            *@param[out] width - bitmap width, in pixels
            *@param[out] height - bitmap height, in pixels
            *@param[out] format - bitmap format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            static bool OpenBitmapData(const std::string& fileName,
                                             std::size_t& width,
                                             std::size_t& height,
                                             std::size_t& format,
                                             std::size_t& length,
                                             void*&       pPixels);

            /**
            * Loads bitmap from buffer and convert to pixel array
            *@param pBuffer - buffer containing bitmap data to load
            *@param[out] width - bitmap width, in pixels
            *@param[out] height - bitmap height, in pixels
            *@param[out] format - bitmap format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            static bool LoadBitmapData(const unsigned char* pBuffer,
                                             std::size_t&   width,
                                             std::size_t&   height,
                                             std::size_t&   format,
                                             std::size_t&   length,
                                             void*&         pPixels);

            /**
            * Loads Targa from file and convert to pixel array
            *@param fileName - Targa file name to load
            *@param[out] width - image width, in pixels
            *@param[out] height - image height, in pixels
            *@param[out] format - image format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            static bool OpenTgaData(const std::string& fileName,
                                          std::size_t& width,
                                          std::size_t& height,
                                          std::size_t& format,
                                          std::size_t& length,
                                          void*&       pPixels);

            /**
            * Loads Targa from buffer and convert to pixel array
            *@param pBuffer - buffer containing Targa data to load
            *@param[out] width - image width, in pixels
            *@param[out] height - image height, in pixels
            *@param[out] format - image format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            static bool LoadTgaData(const unsigned char* pBuffer,
                                          std::size_t&   width,
                                          std::size_t&   height,
                                          std::size_t&   format,
                                          std::size_t&   length,
                                          void*&         pPixels);

            /**
            * Opens png from file and convert to pixel array
            *@param fileName - bitmap file name to load
            *@param is32bit - if true, the image should be opened in 32 bit BGRA format
            *@param[out] width - bitmap width, in pixels
            *@param[out] height - bitmap height, in pixels
            *@param[out] format - bitmap format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            #ifdef USE_PNG
                static bool OpenPngData(const std::string& fileName,
                                              bool         is32bit,
                                              std::size_t& width,
                                              std::size_t& height,
                                              std::size_t& format,
                                              std::size_t& length,
                                              void*&       pPixels);
            #endif

            /**
            * Loads png from buffer and convert to pixel array
            *@param image - source png image
            *@param pBuffer - buffer containing bitmap data to load
            *@param is32bit - if true, the image should be opened in 32 bit BGRA format
            *@param[out] width - bitmap width, in pixels
            *@param[out] height - bitmap height, in pixels
            *@param[out] format - bitmap format (i.e. byte per pixels)
            *@param[out] length - pixels length in bytes
            *@param[out] pPixels - bitmap pixel array
            *@return true on success, otherwise false
            *@note Pixel array must be deleted when useless
            */
            #ifdef USE_PNG
                static bool LoadPngData(const png_image&   image,
                                        const png_bytep    pBuffer,
                                              bool         is32bit,
                                              std::size_t& width,
                                              std::size_t& height,
                                              std::size_t& format,
                                              std::size_t& length,
                                              void*&       pPixels);
            #endif

            /**
            * Converts RGB to BGR
            *@param pData - data to convert, converted data on function ends
            *@param len - data length
            *@param bpp - byte per pixels
            */
            static inline void BGRToRGB(unsigned char* pData, int len, int bpp);
    };

    //---------------------------------------------------------------------------
    // TextureHelper
    //---------------------------------------------------------------------------
    void TextureHelper::BGRToRGB(unsigned char* pData, int len, int bpp)
    {
        unsigned char* pEnd;

        for (pEnd = &pData[len]; pData < pEnd; pData += bpp)
        {
            pData[0] = pData[0] ^ pData[2];
            pData[2] = pData[0] ^ pData[2];
            pData[0] = pData[2] ^ pData[0];
        }
    }
    //---------------------------------------------------------------------------
}
