/****************************************************************************
 * ==> Texture helper ------------------------------------------------------*
 ****************************************************************************
 * Description : Texture helper                                             *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - fbx model reader                                           *
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

#pragma once

// std
#include <string>

class TextureHelper
{
    public:
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
};
