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

#ifndef QR_TextureH
#define QR_TextureH

// std
#include <vector>
#include <map>

// qr engine
#include "QR_Buffer.h"
#include "QR_Shader.h"

/**
* Helper for textures
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_TextureHelper
{
    public:
        /**
        * Loads bitmap from file and convert to pixel array
        *@param fileName - bitmap file name to load
        *@param[out] width - bitmap width, in pixels
        *@param[out] height - bitmap height, in pixels
        *@param[out] format - bitmap format
        *@param[out] length - pixels length in bytes
        *@param[out] pPixels - bitmap pixel array
        *@returns true on success, otherwise false
        *@note Pixel array must be deleted when useless
        */
        static bool LoadFromBitmap(const std::string& fileName,
                                         QR_SizeT&    width,
                                         QR_SizeT&    height,
                                         QR_SizeT&    format,
                                         QR_SizeT&    length,
                                         void*&       pPixels);

        /**
        * Loads bitmap from file and convert to pixel array
        *@param fileName - bitmap file name to load
        *@param[out] width - bitmap width, in pixels
        *@param[out] height - bitmap height, in pixels
        *@param[out] format - bitmap format
        *@param[out] length - pixels length in bytes
        *@param[out] pPixels - bitmap pixel array
        *@returns true on success, otherwise false
        *@note Pixel array must be deleted when useless
        */
        static bool LoadFromBitmap(const std::wstring& fileName,
                                         QR_SizeT&     width,
                                         QR_SizeT&     height,
                                         QR_SizeT&     format,
                                         QR_SizeT&     length,
                                         void*&        pPixels);

        /**
        * Loads bitmap from buffer and convert to pixel array
        *@param buffer - buffer containing bitmap data to load
        *@param[out] width - bitmap width, in pixels
        *@param[out] height - bitmap height, in pixels
        *@param[out] format - bitmap format
        *@param[out] length - pixels length in bytes
        *@param[out] pPixels - bitmap pixel array
        *@returns true on success, otherwise false
        *@note Pixel array must be deleted when useless
        */
        static bool LoadFromBitmap(QR_Buffer& buffer,
                                   QR_SizeT&  width,
                                   QR_SizeT&  height,
                                   QR_SizeT&  format,
                                   QR_SizeT&  length,
                                   void*&     pPixels);
};

/**
* Texture
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Texture
{
    public:
        /**
        * Texture format
        */
        enum IEFormat
        {
            IE_FT_24bit,
            IE_FT_32bit,
        };

        /**
        * Texture target
        */
        enum IETarget
        {
            IE_TT_Texture_2D,
        };

        /**
        * Texture minifying filters
        */
        enum IEMinFilter
        {
            IE_MI_Nearest,
            IE_MI_Linear,
            IE_MI_Nearest_Mipmap_Nearest,
            IE_MI_Linear_Mipmap_Nearest,
            IE_MI_Nearest_Mipmap_Linear,
            IE_MI_Linear_Mipmap_Linear,
        };

        /**
        * Texture magnification filters
        */
        enum IEMagFilter
        {
            IE_MA_Nearest,
            IE_MA_Linear,
        };

        /**
        * Texture wrap modes
        */
        enum IEWrapMode
        {
            IE_WM_Clamp,
            IE_WM_Clamp_To_Edge,
            IE_WM_Clamp_To_Border,
            IE_WM_Mirrored_Repeat,
            IE_WM_Repeat,
        };

        IEFormat    m_Format;
        IETarget    m_Target;
        IEWrapMode  m_WrapMode;
        IEMinFilter m_MinFilter;
        IEMagFilter m_MagFilter;
        QR_UInt32   m_HashCode;
        int         m_Width;
        int         m_Height;

        QR_Texture();
        virtual ~QR_Texture();

        /**
        * Clears the texture
        */
        virtual void Clear();

        /**
        * Creates the texture
        *@param pPixels - texture pixels array
        *@return true on success, otherwise false
        */
        virtual bool Create(void* pPixels) = 0;

        /**
        * Deletes the texture
        */
        virtual void Delete() = 0;

        /**
        * Selects the texture
        *@param pShader - shader
        */
        virtual void Select(const QR_Shader* pShader) const = 0;

        /**
        * Gets the texture identifier
        */
        virtual inline QR_SizeT GetID() const = 0;
};

/**
* Texture list
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
typedef std::vector<QR_Texture*> QR_Textures;

/**
* Texture for 3D models
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_ModelTexture
{
    public:
        std::wstring m_Name;
        QR_Texture*  m_pTexture;
        bool         m_Enabled;

        QR_ModelTexture();
        virtual ~QR_ModelTexture();

        /**
        * Clears the texture
        */
        virtual void Clear();

        /**
        * Selects the texture
        *@param pShader - shader
        */
        virtual void Select(const QR_Shader* pShader) const;
};

/**
* Model texture list
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
typedef std::vector<QR_ModelTexture*> QR_ModelTextures;

#endif
