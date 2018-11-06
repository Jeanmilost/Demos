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

#include "QR_MSAA_OpenGL_Effect.h"

// qr engine
#include "QR_Exception.h"

//---------------------------------------------------------------------------
// QR_MSAA_OpenGL_Effect
//---------------------------------------------------------------------------
QR_MSAA_OpenGL_Effect::QR_MSAA_OpenGL_Effect() :
    QR_PostProcessingEffect(),
    m_pRenderer(NULL),
    m_Width(0),
    m_Height(0),
    m_FrameBuffer(0),
    m_RenderBufferObject(0),
    m_IntermediateFBO(0),
    m_Texture(0)
{}
//---------------------------------------------------------------------------
QR_MSAA_OpenGL_Effect::QR_MSAA_OpenGL_Effect(QR_Renderer_OpenGL* pRenderer,
                                             QR_UInt32           width,
                                             QR_UInt32           height,
                                             const std::string&  vertexShaderFileName,
                                             const std::string&  fragmentShaderFileName) :
    QR_PostProcessingEffect(),
    m_pRenderer(pRenderer),
    m_Width(width),
    m_Height(height),
    m_FrameBuffer(0),
    m_RenderBufferObject(0),
    m_IntermediateFBO(0),
    m_Texture(0)
{
    // quad vertex that fills the entire screen in normalized device coordinate
    M_Precision vertices[] =
    {
        // x,   y,    tu,   tv
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
    };

    std::memcpy(m_Vertices, vertices, 16 * sizeof(M_Precision));

    // create effect shader program
    m_Shader.CreateProgram();

    // do use default vertex shader?
    if (vertexShaderFileName.empty())
        m_Shader.Attach("#version 330 core\r\n\
                         in vec2 qr_vPosition;\r\n\
                         in vec2 qr_vTexCoord;\r\n\
                         out vec2 qr_fTexCoord;\r\n\
                         void main()\r\n\
                         {\r\n\
                             gl_Position  = vec4(qr_vPosition.x, qr_vPosition.y, 0.0f, 1.0f);\r\n\
                             qr_fTexCoord = qr_vTexCoord;\r\n\
                         }",
                        QR_Shader::IE_ST_Vertex);
    else
        m_Shader.AttachFile(vertexShaderFileName, QR_Shader::IE_ST_Vertex);

    // do use default fragment shader?
    if (fragmentShaderFileName.empty())
        m_Shader.Attach("#version 150\r\n\
                         uniform sampler2D qr_sColorMap;\r\n\
                         in vec2 qr_fTexCoord;\r\n\
                         void main()\r\n\
                         {\r\n\
                             gl_FragColor = texture(qr_sColorMap, qr_fTexCoord);\r\n\
                         }",
                        QR_Shader::IE_ST_Fragment);
    else
        m_Shader.AttachFile(fragmentShaderFileName, QR_Shader::IE_ST_Fragment);

    // try to link screen shader
    if (!m_Shader.Link(false))
        M_THROW_EXCEPTION("Could not link screen shader program");

    GenerateFrameBuffer();
}
//---------------------------------------------------------------------------
QR_MSAA_OpenGL_Effect::~QR_MSAA_OpenGL_Effect()
{
    DeleteFrameBuffer();
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::Configure()
{
    // enable multi sampling
    glEnable(GL_MULTISAMPLE);
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::Begin()
{
    // bind frame buffer on which the scene should be drawn
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    // configure the depth test for the rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::End()
{
    const bool isCullFaceEnabled = glIsEnabled(GL_CULL_FACE);

    int cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    // disable the culling to render the final antialiased surface
    glDisable(GL_CULL_FACE);
    glCullFace(GL_NONE);

    // bind screen shader program
    m_Shader.Use(true);

    // blit multi sampled buffer(s) to intermediate FBO normal color buffer.
    // Image is stored in screenTexture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFBO);
    glBlitFramebuffer(0,
                      0,
                      m_Width,
                      m_Height,
                      0,
                      0,
                      m_Width,
                      m_Height,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);

    // render quad with scene's visuals as its texture image
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // get shader position attribute
    #ifdef USE_SHADER
        GLint posAttrib = m_pRenderer->GetAttribute(&m_Shader, QR_Shader::IE_SA_Position);
    #else
        // just to allow to compile without USE_SHADER. But of course cannot work this way
        GLint posAttrib = -1;
    #endif

    // found it?
    if (posAttrib == -1)
        M_THROW_EXCEPTION("Could not find the position attribute in shader");

    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          4 * sizeof(M_Precision),
                          &m_Vertices[0]);

    // get shader texture coordinates attribute
    #ifdef USE_SHADER
        GLint uvAttrib = m_pRenderer->GetAttribute(&m_Shader, QR_Shader::IE_SA_Texture);
    #else
        // just to allow to compile without USE_SHADER. But of course cannot work this way
        GLint uvAttrib = -1;
    #endif

    // found it?
    if (uvAttrib == -1)
        M_THROW_EXCEPTION("Could not find the texture coordinates attribute in shader");

    glEnableVertexAttribArray(uvAttrib);
    glVertexAttribPointer(uvAttrib,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          4 * sizeof(M_Precision),
                          &m_Vertices[2]);

    // draw screen quad
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // unbind screen shader program
    m_Shader.Use(false);

    // restore the culling
    if (isCullFaceEnabled)
        glEnable(GL_CULL_FACE);

    glCullFace(cullFaceMode);
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::SetSize(QR_UInt32 width, QR_UInt32 height)
{
    m_Width  = width;
    m_Height = height;

    // recreate the frame buffer
    DeleteFrameBuffer();
    GenerateFrameBuffer();
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::GenerateFrameBuffer()
{
    const std::size_t sampleFactor = 4;

    // create frame buffer
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    // create a multi sampled color attachment texture
    GLuint textureColorBufferMultiSampled = GenerateMultiSampleTexture(sampleFactor);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           textureColorBufferMultiSampled,
                           0);

    GLuint m_RenderBufferObject;

    // create a render buffer object for depth and stencil attachments
    glGenRenderbuffers(1, &m_RenderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferObject);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                     sampleFactor,
                                     GL_DEPTH24_STENCIL8,
                                     m_Width,
                                     m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_RenderBufferObject);

    // try to build render buffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        M_THROW_EXCEPTION("Frame buffer was not build correctly");

    // bind render buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // create an intermediate frame buffer for multi sampling
    m_Texture = GenerateAttachmentTexture(false, false);
    glGenFramebuffers(1, &m_IntermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           m_Texture,
                           0);

    // try to build intermediate buffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        M_THROW_EXCEPTION("Intermediate frame buffer was not build correctly");

    // unbind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//---------------------------------------------------------------------------
void QR_MSAA_OpenGL_Effect::DeleteFrameBuffer()
{
    if (m_IntermediateFBO)
        glDeleteFramebuffers(1, &m_IntermediateFBO);

    if (m_RenderBufferObject)
        glDeleteRenderbuffers(1, &m_RenderBufferObject);

    if (m_FrameBuffer)
        glDeleteFramebuffers(1, &m_FrameBuffer);
}
//---------------------------------------------------------------------------
GLuint QR_MSAA_OpenGL_Effect::GenerateMultiSampleTexture(GLuint samples)
{
    // generate new texture
    GLuint texture;
    glGenTextures(1, &texture);

    // bind texture to multi sampler
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                            samples,
                            GL_RGB,
                            m_Width,
                            m_Height,
                            GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    return texture;
}
//---------------------------------------------------------------------------
GLuint QR_MSAA_OpenGL_Effect::GenerateAttachmentTexture(GLboolean depth,
                                                        GLboolean stencil)
{
    GLenum attachmentType;

    // select attachment type to use
    if (!depth && !stencil)
        attachmentType = GL_RGB;
    else
    if (depth && !stencil)
        attachmentType = GL_DEPTH_COMPONENT;
    else
    if (!depth && stencil)
        attachmentType = GL_STENCIL_INDEX;
    else
        M_THROW_EXCEPTION("Unknown attachment type");

    GLuint textureID;

    // generate texture ID and load texture data
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // both stencil and depth test are used?
    if (!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     attachmentType,
                     m_Width,
                     m_Height,
                     0,
                     attachmentType,
                     GL_UNSIGNED_BYTE,
                     NULL);
    else
        // using both stencil and depth test, needs special format arguments
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH24_STENCIL8,
                     m_Width,
                     m_Height,
                     0,
                     GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8,
                     NULL);

    // configure texture filters to use and bind texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
//---------------------------------------------------------------------------
