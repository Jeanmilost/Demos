/****************************************************************************
 * ==> Texture_OpenGL ------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a texture class based on OpenGL                   *
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

#pragma once

// classes
#include "Texture.h"

// OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/gl.h>

/**
* Texture for 3D models ready to use in OpenGL
*@author Jean-Milost Reymond
*/
class Texture_OpenGL : public Texture
{
    public:
        Texture_OpenGL();
        virtual ~Texture_OpenGL();

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
        virtual void Select(const Shader* pShader) const;

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
        * Gets texture minify filter
        *@return minify filter
        */
        virtual GLuint GetMinFilter() const;

        /**
        * Gets texture magnify filter
        *@return magnify filter
        */
        virtual GLuint GetMagFilter() const;

        /**
        * Gets the texture identifier
        *@return Texture identifier
        */
        virtual inline std::size_t GetID() const;

    private:
        GLuint m_Index;
};

//---------------------------------------------------------------------------
// Texture_OpenGL
//---------------------------------------------------------------------------
void Texture_OpenGL::Clear()
{
    Texture::Clear();

    Delete();
}
//---------------------------------------------------------------------------
std::size_t Texture_OpenGL::GetID() const
{
    return m_Index;
}
//---------------------------------------------------------------------------
