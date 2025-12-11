/****************************************************************************
 * ==> PixelBuffer ---------------------------------------------------------*
 ****************************************************************************
 * Description: Pixel buffer                                                *
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

#pragma once

// std
#include <string>

// classes
#include "Buffer.h"

/**
* Pixel buffer
*@author Jean-Milost Reymond
*/
class PixelBuffer
{
    public:
        /**
        * Image type
        */
        enum class IEImageType
        {
            IE_IT_Raw,
            IE_IT_Bitmap
        };

        /**
        * Pixel type
        */
        enum class IEPixelType
        {
            IE_PT_RGB,
            IE_PT_BGR,
            IE_PT_RGBA,
            IE_PT_BGRA,
            IE_PT_ARGB,
            IE_PT_ABGR
        };

        IEImageType   m_ImageType    = PixelBuffer::IEImageType::IE_IT_Raw;
        IEPixelType   m_PixelType    = PixelBuffer::IEPixelType::IE_PT_RGBA;
        std::uint32_t m_Width        = 0;
        std::uint32_t m_Height       = 0;
        std::uint32_t m_Stride       = 0;
        std::uint32_t m_BytePerPixel = 0;
        std::size_t   m_DataLength   = 0;
        void*         m_pData        = nullptr;

        PixelBuffer();
        virtual ~PixelBuffer();

        /**
        * Read pixel buffer from a bitmap file
        *@param fileName - bitmap file name to read from
        */
        virtual bool FromBitmap(const std::string& fileName);

        /**
        * Read pixel buffer from a bitmap file
        *@param fileName - bitmap file name to read from
        */
        virtual bool FromBitmap(const std::wstring& fileName);

        /**
        * Read pixel buffer from a bitmap buffer
        *@param buffer - buffer containing bitmap to read
        */
        virtual bool FromBitmap(Buffer& buffer);
};
