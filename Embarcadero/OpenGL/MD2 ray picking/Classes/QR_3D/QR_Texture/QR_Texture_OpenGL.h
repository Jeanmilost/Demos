/****************************************************************************
 * ==> QR_Texture_OpenGL ---------------------------------------------------*
 ****************************************************************************
 * Description : Provides a texture class based on OpenGL                   *
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

#ifndef QR_Texture_OpenGLH
#define QR_Texture_OpenGLH

// qr engine
#include "QR_Texture.h"

// OpenGL
#if defined (OS_WIN)
    #define GLEW_STATIC
    #include <GL/glew.h>
    #include <gl/gl.h>
#elif defined (OS_OSX)
    #ifdef USE_OPENGL_DIRECT_MODE
        #include <ES1/gl.h>
    #else
        #include <ES2/gl.h>
        #include <ES2/glext.h>
    #endif
#endif

/**
* Texture for 3D models ready to use in OpenGL
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Texture_OpenGL : public QR_Texture
{
    public:
        QR_Texture_OpenGL();
        virtual ~QR_Texture_OpenGL();

        /**
        * Clears the texture
        */
        virtual inline void Clear();

        /**
        * Creates the texture
        *@param pPixels - texture pixels array
        *@return true on success, otherwise false
        */
        virtual bool Create(void* pPixels);

        /**
        * Deletes the texture
        */
        virtual void Delete();

        /**
        * Selects the texture
        *@param pShader - shader
        */
        virtual void Select(const QR_Shader* pShader) const;

    protected:
        /**
        * Gets texture target
        *@return target
        */
        virtual GLuint GetTarget() const;

        /**
        * Gets texture format
        *@return format
        */
        virtual GLuint GetFormat() const;

        /**
        * Gets texture wrap mode
        *@return wrap mode
        */
        virtual GLuint GetWrapMode() const;

        /**
        * Gets texture mignifier filter
        *@return mignifier filter
        */
        virtual GLuint GetMinFilter() const;

        /**
        * Gets texture magnification filter
        *@return magnification filter
        */
        virtual GLuint GetMagFilter() const;

        /**
        * Gets the texture identifier
        */
        virtual inline QR_SizeT GetID() const;

    private:
        GLuint m_Index;
};

//---------------------------------------------------------------------------
// QR_Texture
//---------------------------------------------------------------------------
void QR_Texture_OpenGL::Clear()
{
    QR_Texture::Clear();

    Delete();
}
//---------------------------------------------------------------------------
QR_SizeT QR_Texture_OpenGL::GetID() const
{
    return m_Index;
}
//---------------------------------------------------------------------------

#endif
