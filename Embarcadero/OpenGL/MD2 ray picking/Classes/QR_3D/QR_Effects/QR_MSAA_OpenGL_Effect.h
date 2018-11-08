/****************************************************************************
 * ==> QR_MSAA_OpenGL_Effect -----------------------------------------------*
 ****************************************************************************
 * Description : Post-processing multi sampled anti aliasing effect for     *
 *               OpenGL renderer                                            *
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

#ifndef QR_MSAA_OPENGL_EFFECTH
#define QR_MSAA_OPENGL_EFFECTH

// qr engine
#include "QR_Types.h"
#include "QR_PostProcessingEffect.h"
#include "QR_Shader_OpenGL.h"
#include "QR_Renderer_OpenGL.h"

// openGL
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
* Post-processing multi sampled anti aliasing effect for OpenGL renderer
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MSAA_OpenGL_Effect : public QR_PostProcessingEffect
{
    public:
        /**
        * Constructor
        *@param pRenderer - renderer to use to draw the scene
        *@param width - screen width, in pixels
        *@param height - screen height, in pixels
        *@param vertexShaderFileName - vertex shader file name to use
        *@param fragmentShaderFileName - fragment shader file name to use
        */
        QR_MSAA_OpenGL_Effect(QR_Renderer_OpenGL* pRenderer,
                              QR_UInt32           width,
                              QR_UInt32           height,
                              const std::string&  vertexShaderFileName   = "",
                              const std::string&  fragmentShaderFileName = "");

        /**
        * Destructor
        */
        virtual ~QR_MSAA_OpenGL_Effect();

        /**
        * Configures effect
        */
        virtual void Configure();

        /**
        * Begins effect
        */
        virtual void Begin();

        /**
        * Ends effect
        */
        virtual void End();

        /**
        * Sets the effect size
        *@param width - new width to set
        *@param height - new height to set
        */
        virtual void SetSize(QR_UInt32 width, QR_UInt32 height);

    private:
        QR_Renderer_OpenGL* m_pRenderer;
        QR_UInt32           m_Width;
        QR_UInt32           m_Height;
        GLuint              m_FrameBuffer;
        GLuint              m_RenderBufferObject;
        GLuint              m_IntermediateFBO;
        GLuint              m_Texture;
        QR_Shader_OpenGL    m_Shader;
        M_Precision         m_Vertices[16];

        /**
        * Constructor
        *@note Cannot construct object of this type without a size
        */
        QR_MSAA_OpenGL_Effect();

        /**
        * Generates a new frame buffer for the effect
        */
        void GenerateFrameBuffer();

        /**
        * Deletes the current frame buffer
        */
        void DeleteFrameBuffer();

        /**
        * Generates a texture compatible with multi sampling operations
        *@param samples - samples
        *@return texture identifier
        */
        GLuint GenerateMultiSampleTexture(GLuint samples);

        /**
        * Generates a texture for attachment to final render surface
        *@param depth - color depth
        *@param stencil - if true, stencil buffer will be used
        *@return texture identifier
        */
        GLuint GenerateAttachmentTexture(GLboolean depth, GLboolean stencil);
};

#endif
