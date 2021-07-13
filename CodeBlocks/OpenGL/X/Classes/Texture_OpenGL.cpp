/****************************************************************************
 * ==> Texture_OpenGL ------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a texture descriptor based on OpenGL              *
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

#include "Texture_OpenGL.h"

// std
#include <stdexcept>

// windows
#include <Windows.h>

//---------------------------------------------------------------------------
// Texture_OpenGL
//---------------------------------------------------------------------------
Texture_OpenGL::Texture_OpenGL() :
    Texture(),
    m_Index(0)
{}
//---------------------------------------------------------------------------
Texture_OpenGL::~Texture_OpenGL()
{
    // NOTE don't remove the Texture_OpenGL namespace, to ensure that the Delete() function
    // belonging to this class will be called
    Texture_OpenGL::Delete();
}
//---------------------------------------------------------------------------
bool Texture_OpenGL::Create(void* pPixels)
{
    const GLuint target    = GetTarget();
    const GLuint format    = GetFormat();
    const GLuint wrapMode  = GetWrapMode();
    const GLuint minFilter = GetMinFilter();
    const GLuint magFilter = GetMagFilter();

    // create and bind new OpenGL texture
    glGenTextures(1, &m_Index);
    glBindTexture(target, m_Index);

    // set texture environment parameters
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // set texture filtering
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

    // set texture wrapping mode
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);

    // generate texture from bitmap data
    glTexImage2D(target,
                 0,
                 format,
                 m_Width,
                 m_Height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 pPixels);

    return true;
}
//---------------------------------------------------------------------------
void Texture_OpenGL::Delete()
{
    if (m_Index)
    {
        glDeleteTextures(1, &m_Index);
        m_Index = 0;
    }
}
//---------------------------------------------------------------------------
void Texture_OpenGL::Select(const  Shader* pShader) const
{
    // no texture?
    if (!m_Index)
        return;

    const GLuint target = GetTarget();

    // select texture
    glEnable(target);
    glBindTexture(target, m_Index);

    if (pShader)
        glActiveTexture(GL_TEXTURE0);
}
//---------------------------------------------------------------------------
GLuint Texture_OpenGL::GetTarget() const
{
    // search for texture target
    switch (m_Target)
    {
        case IE_TT_Texture_2D: return GL_TEXTURE_2D;
        default:               throw new std::invalid_argument("Unknown target");
    }
}
//---------------------------------------------------------------------------
GLuint Texture_OpenGL::GetFormat() const
{
    // search for texture format to apply
    switch (m_Format)
    {
        case IE_FT_24bit: return GL_RGB;
        case IE_FT_32bit: return GL_RGBA;
        default:          throw new std::invalid_argument("Unknown format");
    }
}
//---------------------------------------------------------------------------
GLuint Texture_OpenGL::GetWrapMode() const
{
    // search for wrap mode to apply
    switch (m_WrapMode)
    {
        case IE_WM_Clamp:           return GL_CLAMP;
        case IE_WM_Clamp_To_Edge:   return GL_CLAMP_TO_EDGE;
        case IE_WM_Clamp_To_Border: return GL_CLAMP_TO_BORDER;
        case IE_WM_Mirrored_Repeat: return GL_MIRRORED_REPEAT;
        case IE_WM_Repeat:          return GL_REPEAT;
        default:                    throw new std::invalid_argument("Unknown wrap mode");
    }
}
//---------------------------------------------------------------------------
GLuint Texture_OpenGL::GetMinFilter() const
{
    // search for mignifier filter to apply
    switch (m_MinFilter)
    {
        case IE_MI_Nearest:                return GL_NEAREST;
        case IE_MI_Linear:                 return GL_LINEAR;
        case IE_MI_Nearest_Mipmap_Nearest: return GL_NEAREST_MIPMAP_NEAREST;
        case IE_MI_Linear_Mipmap_Nearest:  return GL_LINEAR_MIPMAP_NEAREST;
        case IE_MI_Nearest_Mipmap_Linear:  return GL_NEAREST_MIPMAP_LINEAR;
        case IE_MI_Linear_Mipmap_Linear:   return GL_LINEAR_MIPMAP_LINEAR;
        default:                           throw new std::invalid_argument("Unknown magnification filter");
    }
}
//---------------------------------------------------------------------------
GLuint Texture_OpenGL::GetMagFilter() const
{
    // search for magnification filter to apply
    switch (m_MagFilter)
    {
        case IE_MA_Nearest: return GL_NEAREST;
        case IE_MA_Linear:  return GL_LINEAR;
        default:            throw new std::invalid_argument("Unknown magnification filter");
    }
}
//---------------------------------------------------------------------------
