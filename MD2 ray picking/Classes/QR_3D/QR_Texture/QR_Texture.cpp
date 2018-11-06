/****************************************************************************
 * ==> QR_Texture ----------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a texture class and his associated manager and    *
 *               helper                                                     *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - QR Engine                                                  *
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

#include "QR_Texture.h"

// qr engine
#include "QR_Exception.h"
#include "QR_STDTools.h"
#include "QR_StdFileBuffer.h"

//------------------------------------------------------------------------------
// QR_TextureHelper
//------------------------------------------------------------------------------
bool QR_TextureHelper::LoadFromBitmap(const std::string& fileName, QR_SizeT& width, QR_SizeT& height,
        QR_SizeT& format, QR_SizeT& length, void*& pPixels)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    return LoadFromBitmap(buffer, width, height, format, length, pPixels);
}
//------------------------------------------------------------------------------
bool QR_TextureHelper::LoadFromBitmap(const std::wstring& fileName, QR_SizeT& width, QR_SizeT& height,
        QR_SizeT& format, QR_SizeT& length, void*& pPixels)
{
    // create a file buffer and open it for read
    QR_StdFileBuffer buffer;
    buffer.Open(fileName, QR_FileBuffer::IE_M_Read);
    buffer.Seek(0, 0);

    return LoadFromBitmap(buffer, width, height, format, length, pPixels);
}
//------------------------------------------------------------------------------
bool QR_TextureHelper::LoadFromBitmap(QR_Buffer& buffer, QR_SizeT& width, QR_SizeT& height,
        QR_SizeT& format, QR_SizeT& length, void*& pPixels)
{
    QR_Char intBuffer[4];
    QR_Char shortBuffer[2];

    // read bitmap signature
    buffer.Read(shortBuffer, 2);

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return false;

    // skip 8 next bytes
    buffer.Seek(buffer.GetOffset(), 8);

    // read data offset
    buffer.Read(intBuffer, 4);
    QR_UInt32 dataOffset = (QR_UInt32)(((QR_UInt8)intBuffer[3] << 24) |
                                       ((QR_UInt8)intBuffer[2] << 16) |
                                       ((QR_UInt8)intBuffer[1] << 8)  |
                                        (QR_UInt8)intBuffer[0]);

    // read header size
    buffer.Read(intBuffer, 4);
    QR_UInt32 headerSize = (QR_UInt32)(((QR_UInt8)intBuffer[3] << 24) |
                                       ((QR_UInt8)intBuffer[2] << 16) |
                                       ((QR_UInt8)intBuffer[1] << 8)  |
                                        (QR_UInt8)intBuffer[0]);

    // search for bitmap type
    switch (headerSize)
    {
        // V3
        case 40:
        {
            // read bitmap width
            buffer.Read(intBuffer, 4);
            width = (QR_UInt32)(((QR_UInt8)intBuffer[3] << 24) |
                                ((QR_UInt8)intBuffer[2] << 16) |
                                ((QR_UInt8)intBuffer[1] << 8)  |
                                 (QR_UInt8)intBuffer[0]);

            // read bitmap height
            buffer.Read(intBuffer, 4);
            height = (QR_UInt32)(((QR_UInt8)intBuffer[3] << 24) |
                                 ((QR_UInt8)intBuffer[2] << 16) |
                                 ((QR_UInt8)intBuffer[1] << 8)  |
                                  (QR_UInt8)intBuffer[0]);

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap bit per pixels
            buffer.Read(shortBuffer, 2);
            QR_UInt16 bpp = (QR_UInt16)(((QR_UInt8)shortBuffer[1] << 8) |
                                         (QR_UInt8)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            // read bitmap compressed flag
            buffer.Read(shortBuffer, 2);
            QR_UInt16 compressed = (QR_UInt16)(((QR_UInt8)shortBuffer[1] << 8) |
                                                (QR_UInt8)shortBuffer[0]);

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
            buffer.Read(shortBuffer, 2);
            width = (QR_UInt32)(((QR_UInt8)shortBuffer[1] << 8) |
                                 (QR_UInt8)shortBuffer[0]);

            // read bitmap height
            buffer.Read(shortBuffer, 2);
            height = (QR_UInt32)(((QR_UInt8)shortBuffer[1] << 8) |
                                  (QR_UInt8)shortBuffer[0]);

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap bit per pixels
            buffer.Read(shortBuffer, 2);
            QR_UInt16 bpp = (QR_UInt16)(((QR_UInt8)shortBuffer[1] << 8) |
                                         (QR_UInt8)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // OS/2 V2
        case 64:
            M_THROW_EXCEPTION("Unsupported bitmap format");

        // Windows V4
        case 108:
        {
            // read bitmap width
            buffer.Read(shortBuffer, 2);
            width = (QR_UInt32)(((QR_UInt8)shortBuffer[1] << 8) |
                                 (QR_UInt8)shortBuffer[0]);

            // skip next 2 bytes
            buffer.Seek(buffer.GetOffset(), 2);

            // read bitmap height
            buffer.Read(shortBuffer, 2);
            height = (QR_UInt32)(((QR_UInt8)shortBuffer[1] << 8) |
                                  (QR_UInt8)shortBuffer[0]);

            // skip next 4 bytes
            buffer.Seek(buffer.GetOffset(), 4);

            // read bitmap bit per pixels
            buffer.Read(shortBuffer, 2);
            QR_UInt16 bpp = (QR_UInt16)(((QR_UInt8)shortBuffer[1] << 8) |
                                         (QR_UInt8)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return false;

            format = 24;
            break;
        }

        // Windows V5
        case 124:
            M_THROW_EXCEPTION("Unsupported bitmap format");

        default:
            M_THROW_EXCEPTION("Unsupported bitmap format");
    }

    QR_Char* pBitmapData = NULL;

    try
    {
        const QR_UInt32 bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
        const QR_UInt32 bitmapSize  = bytesPerRow * height;

        pBitmapData = new QR_Char[bitmapSize];

        // read bitmap data
        buffer.Seek(0, dataOffset);
        buffer.Read(pBitmapData, bitmapSize);

        // search for bitmap format
        switch (format)
        {
            case 24:
            {
                // calculate pixels array length
                length = (width * height * 3);

                unsigned char* pPixelArray = new QR_Char[length];

                // get bitmap data in right format
                for (QR_UInt32 y = 0; y < height; ++y)
                    for (QR_UInt32 x = 0; x < width; ++x)
                        for (QR_UInt8 c = 0; c < 3; ++c)
                            pPixelArray[3 * (width * y + x) + c] =
                                    pBitmapData[(bytesPerRow * ((height - 1) - y)) + (3 * x) + (2 - c)];

                pPixels = pPixelArray;
                break;
            }

            default: M_THROW_EXCEPTION("Unknown format");
        }
    }
    catch (...)
    {
        // clear memory
        delete[] pPixels;
        delete[] pBitmapData;

        throw;
    }

    // clear memory
    delete[] pBitmapData;

    return true;
}
//---------------------------------------------------------------------------
// QR_Texture
//---------------------------------------------------------------------------
QR_Texture::QR_Texture() :
    m_Format(IE_FT_24bit),
    m_Target(IE_TT_Texture_2D),
    m_WrapMode(IE_WM_Repeat),
    m_MinFilter(IE_MI_Nearest),
    m_MagFilter(IE_MA_Nearest),
    m_Width(0),
    m_Height(0)
{}
//---------------------------------------------------------------------------
QR_Texture::~QR_Texture()
{}
//---------------------------------------------------------------------------
void QR_Texture::Clear()
{
    m_Format    = IE_FT_24bit;
    m_Target    = IE_TT_Texture_2D;
    m_WrapMode  = IE_WM_Repeat;
    m_MinFilter = IE_MI_Nearest;
    m_MagFilter = IE_MA_Nearest;
    m_Width     = 0;
    m_Height    = 0;
}
//---------------------------------------------------------------------------
// QR_ModelTexture
//---------------------------------------------------------------------------
QR_ModelTexture::QR_ModelTexture() :
    m_pTexture(NULL),
    m_Enabled(false)
{}
//---------------------------------------------------------------------------
QR_ModelTexture::~QR_ModelTexture()
{}
//---------------------------------------------------------------------------
void QR_ModelTexture::Clear()
{
    m_Name.clear();

    if (m_pTexture)
        m_pTexture->Clear();

    m_Enabled = false;
}
//---------------------------------------------------------------------------
void QR_ModelTexture::Select(const QR_Shader* pShader) const
{
    // disabled?
    if (!m_Enabled)
        return;

    if (m_pTexture)
        m_pTexture->Select(pShader);
}
//---------------------------------------------------------------------------
