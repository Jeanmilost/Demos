/****************************************************************************
 * ==> DWF_Renderer_OpenGL -------------------------------------------------*
 ****************************************************************************
 * Description: OpenGL renderer                                             *
 * Developer:   Jean-Milost Reymond                                         *
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

#include "DWF_Renderer_OpenGL.h"

// classes
#include "DWF_Texture_OpenGL.h"
#include "DWF_Shader_OpenGL.h"

using namespace DWF_Renderer;

//---------------------------------------------------------------------------
// Renderer_OpenGL
//---------------------------------------------------------------------------
Renderer_OpenGL::Renderer_OpenGL() :
    Renderer(),
    m_hDC(nullptr),
    m_hRC(nullptr)
{}
//---------------------------------------------------------------------------
Renderer_OpenGL::~Renderer_OpenGL()
{}
//---------------------------------------------------------------------------
bool Renderer_OpenGL::EnableOpenGL(HWND hWnd)
{
    // no window handle?
    if (!hWnd)
        return false;

    // get the device context (DC)
    m_hDC = ::GetDC(hWnd);

    // failed to get device context?
    if (!m_hDC)
        return false;

    // configure pixel format
    if (!SetPixelFormat(m_hDC))
        return false;

    // create OpenGL render context
    m_hRC = wglCreateContext(m_hDC);

    // succeeded?
    if (!m_hRC)
        return false;

    // enable OpenGL render context
    if (!SelectContext())
        return false;

    return true;
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::DisableOpenGL(HWND hWnd)
{
    // disable and delete OpenGL context
    if (m_hRC)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hRC);
        m_hRC = nullptr;
    }

    // delete device context
    if (hWnd && m_hDC)
    {
        ::ReleaseDC(hWnd, m_hDC);
        m_hDC = nullptr;
    }
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::CreateViewport(float                       w,
                                     float                       h,
                                     float                       zNear,
                                     float                       zFar,
                                     const Shader*               pShader,
                                           DWF_Math::Matrix4x4F& matrix) const
{
    if (!pShader)
        return;

    pShader->Use(true);

    // prevent the width to reach 0
    if (!w)
        w = 0.1f;

    // prevent the height to reach 0
    if (!h)
        h = 0.1f;

    // calculate matrix items
    const float fov    = 45.0f;
    const float aspect = w / h;

    // create the OpenGL viewport
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // create the projection matrix
    matrix = GetPerspective(fov, aspect, zNear, zFar);

    // connect projection matrix to shader
    SetProjectionMatrix(pShader, matrix);
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::BeginScene(const DWF_Model::ColorF& color, IESceneFlags flags) const
{
    // enable OpenGL render context
    if (!SelectContext())
        return;

    GLbitfield openGLSceneFlags = 0;

    // clear background color, if needed
    if ((unsigned)flags & (unsigned)IESceneFlags::IE_SF_ClearColor)
    {
        glClearColor((GLclampf)color.m_R, (GLclampf)color.m_G, (GLclampf)color.m_B, (GLclampf)color.m_A);

        openGLSceneFlags |= GL_COLOR_BUFFER_BIT;
    }

    // clear Z buffer, if needed
    if ((unsigned)flags & (unsigned)IESceneFlags::IE_SF_ClearDepth)
    {
        glClearDepth(1.0f);

        openGLSceneFlags |= GL_DEPTH_BUFFER_BIT;
    }

    // clear scene, fill with background color and set render flags
    glClear(openGLSceneFlags);
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::EndScene() const
{
    // enable OpenGL render context
    if (!SelectContext())
        return;

    // no device context?
    if (!m_hDC)
        return;

    // present back buffer
    ::SwapBuffers(m_hDC);
}
//---------------------------------------------------------------------------
bool Renderer_OpenGL::SelectContext() const
{
    return wglMakeCurrent(m_hDC, m_hRC);
}
//---------------------------------------------------------------------------
DWF_Model::Texture* Renderer_OpenGL::GenerateTexture() const
{
    return new DWF_Model::Texture_OpenGL();
}
//---------------------------------------------------------------------------
Shader* Renderer_OpenGL::GenerateShader() const
{
    return new Shader_OpenGL();
}
//--------------------------------------------------------------------------------------------------
void Renderer_OpenGL::ConnectProjectionMatrixToShader(const Shader*               pShader,
                                                      const DWF_Math::Matrix4x4F& projectionMatrix) const
{
    if (!pShader)
        return;

    // bind shader program
    pShader->Use(true);

    // get perspective (or projection) matrix slot from shader
    const GLint uniform = GetUniform(pShader, Shader::IEAttribute::IE_SA_ProjectionMatrix);

    // found it?
    if (uniform == -1)
        throw new std::exception("Program uniform not found - perspective");

    // connect perspective (or projection) matrix to shader
    glUniformMatrix4fv(uniform, 1, GL_FALSE, projectionMatrix.GetPtr());

    // unbind shader program
    pShader->Use(false);
}
//--------------------------------------------------------------------------------------------------
void Renderer_OpenGL::ConnectViewMatrixToShader(const Shader*               pShader,
                                                const DWF_Math::Matrix4x4F& viewMatrix) const
{
    if (!pShader)
        return;

    // bind shader program
    pShader->Use(true);

    // get view matrix slot from shader
    const GLint uniform = GetUniform(pShader, Shader::IEAttribute::IE_SA_ViewMatrix);

    // found it?
    if (uniform == -1)
        throw new std::exception("Program uniform not found - view");

    // connect view matrix to shader
    glUniformMatrix4fv(uniform, 1, GL_FALSE, viewMatrix.GetPtr());

    // unbind shader program
    pShader->Use(false);
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::SetProjectionMatrix(const Shader*               pShader,
                                          const DWF_Math::Matrix4x4F& projectionMatrix) const
{
    ConnectProjectionMatrixToShader(pShader, projectionMatrix);
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::SetViewMatrix(const Shader*               pShader,
                                    const DWF_Math::Matrix4x4F& viewMatrix) const
{
    ConnectViewMatrixToShader(pShader, viewMatrix);
}
//---------------------------------------------------------------------------
bool Renderer_OpenGL::Draw(const DWF_Model::Mesh&      mesh,
                           const DWF_Math::Matrix4x4F& modelMatrix,
                           const Shader*               pShader) const
{
    return Draw(mesh, modelMatrix, pShader, false);
}
//---------------------------------------------------------------------------
bool Renderer_OpenGL::Draw(const DWF_Model::Mesh&      mesh,
                           const DWF_Math::Matrix4x4F& modelMatrix,
                           const Shader*               pShader,
                                 bool                  disableDepthTestOnTransparency) const
{
    // get mesh count
    const std::size_t count = mesh.m_VB.size();

    // no mesh to draw?
    if (!count)
        return false;

    // no shader program?
    if (!pShader)
        return false;

    // certify that depth test is enabled
    glEnable(GL_DEPTH_TEST);

    try
    {
        // bind shader program
        pShader->Use(true);

        // get model matrix slot from shader
        GLint uniform = GetUniform(pShader, Shader::IEAttribute::IE_SA_ModelMatrix);

        // found it?
        if (uniform == -1)
            return false;

        // required to access the matrix pointer, even in read-only mode
        DWF_Math::Matrix4x4F* pModelMatrix = const_cast<DWF_Math::Matrix4x4F*>(&modelMatrix);

        // connect model matrix to shader
        glUniformMatrix4fv(uniform, 1, GL_FALSE, pModelMatrix->GetPtr());

        // iterate through OpenGL meshes
        for (std::size_t i = 0; i < count; ++i)
        {
            // configure the culling
            switch (mesh.m_VB[i]->m_Culling.m_Type)
            {
                case DWF_Model::VertexCulling::IECullingType::IE_CT_None:  glDisable(GL_CULL_FACE); glCullFace(GL_NONE);           break;
                case DWF_Model::VertexCulling::IECullingType::IE_CT_Front: glEnable (GL_CULL_FACE); glCullFace(GL_FRONT);          break;
                case DWF_Model::VertexCulling::IECullingType::IE_CT_Back:  glEnable (GL_CULL_FACE); glCullFace(GL_BACK);           break;
                case DWF_Model::VertexCulling::IECullingType::IE_CT_Both:  glEnable (GL_CULL_FACE); glCullFace(GL_FRONT_AND_BACK); break;
                default:                                                   glDisable(GL_CULL_FACE); glCullFace(GL_NONE);           break;
            }

            // configure the culling face
            switch (mesh.m_VB[i]->m_Culling.m_Face)
            {
                case DWF_Model::VertexCulling::IECullingFace::IE_CF_CW:  glFrontFace(GL_CW);  break;
                case DWF_Model::VertexCulling::IECullingFace::IE_CF_CCW: glFrontFace(GL_CCW); break;
            }

            // configure the alpha blending
            if (mesh.m_VB[i]->m_Material.m_Transparent)
            {
                if (disableDepthTestOnTransparency)
                    glDisable(GL_DEPTH_TEST);

                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else
                glDisable(GL_BLEND);

            // configure the wireframe mode
            if (mesh.m_VB[i]->m_Material.m_Wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // get shader position attribute
            GLint posAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Vertices);

            // found it?
            if (posAttrib == -1)
                return false;

            // calculate stride. NOTE the vertex contains always at least 3 values for the x, y and z coordinates
            std::size_t stride       =  3;
            GLint       normalAttrib = -1;

            // do use shader normal attribute?
            if ((unsigned)mesh.m_VB[i]->m_Format.m_Format & (unsigned)DWF_Model::VertexFormat::IEFormat::IE_VF_Normals)
            {
                // get shader normal attribute
                normalAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Normal);

                // found it?
                if (normalAttrib == -1)
                    return false;

                stride += 3;
            }

            GLint uvAttrib = -1;

            // do use shader UV attribute?
            if ((unsigned)mesh.m_VB[i]->m_Format.m_Format & (unsigned)DWF_Model::VertexFormat::IEFormat::IE_VF_TexCoords)
            {
                // get shader UV attribute
                uvAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Texture);

                // found it?
                if (uvAttrib == -1)
                    return false;

                // add texture coordinates to stride
                stride += 2;
            }

            GLint colorAttrib = -1;

            // do use shader color attribute?
            if ((unsigned)mesh.m_VB[i]->m_Format.m_Format & (unsigned)DWF_Model::VertexFormat::IEFormat::IE_VF_Colors)
            {
                // get shader color attribute
                colorAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Color);

                // found it?
                if (colorAttrib == -1)
                    return false;

                // add color to stride
                stride += 4;
            }

            // select the texture to apply
            SelectTexture(pShader, mesh.m_VB[i]->m_Material.m_pTexture);

            std::size_t offset = 0;

            // connect vertices to vertex shader position attribute
            glEnableVertexAttribArray(posAttrib);
            glVertexAttribPointer(posAttrib,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  (GLsizei)(stride * sizeof(float)),
                                  &mesh.m_VB[i]->m_Data[offset]);

            offset = 3;

            // vertex buffer contains normals?
            if (normalAttrib != -1)
            {
                // connect the vertices to the vertex shader normal attribute
                glEnableVertexAttribArray(normalAttrib);
                glVertexAttribPointer(normalAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      (GLsizei)(stride * sizeof(float)),
                                      &mesh.m_VB[i]->m_Data[offset]);

                offset += 3;
            }

            // vertex buffer contains texture coordinates?
            if (uvAttrib != -1)
            {
                // connect the color to the vertex shader vColor attribute and redirect to
                // the fragment shader
                glEnableVertexAttribArray(uvAttrib);
                glVertexAttribPointer(uvAttrib,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      (GLsizei)(stride * sizeof(float)),
                                      &mesh.m_VB[i]->m_Data[offset]);

                offset += 2;
            }

            // vertex buffer contains colors?
            if (colorAttrib != -1)
            {
                // connect the color to the vertex shader vColor attribute and redirect to
                // the fragment shader
                glEnableVertexAttribArray(colorAttrib);
                glVertexAttribPointer(colorAttrib,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      (GLsizei)(stride * sizeof(float)),
                                      &mesh.m_VB[i]->m_Data[offset]);
            }

            // draw mesh
            switch (mesh.m_VB[i]->m_Format.m_Type)
            {
                case DWF_Model::VertexFormat::IEType::IE_VT_Triangles:     glDrawArrays(GL_TRIANGLES,      0, (GLsizei)(mesh.m_VB[i]->m_Data.size() / stride)); break;
                case DWF_Model::VertexFormat::IEType::IE_VT_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)(mesh.m_VB[i]->m_Data.size() / stride)); break;
                case DWF_Model::VertexFormat::IEType::IE_VT_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, (GLsizei)(mesh.m_VB[i]->m_Data.size() / stride)); break;
                case DWF_Model::VertexFormat::IEType::IE_VT_Quads:         glDrawArrays(GL_QUADS,          0, (GLsizei)(mesh.m_VB[i]->m_Data.size() / stride)); break;
                case DWF_Model::VertexFormat::IEType::IE_VT_QuadStrip:     glDrawArrays(GL_QUAD_STRIP,     0, (GLsizei)(mesh.m_VB[i]->m_Data.size() / stride)); break;
                case DWF_Model::VertexFormat::IEType::IE_VT_Unknown:
                default:                                                   throw new std::exception("Unknown vertex type");
            }
        }
    }
    catch (...)
    {
        // unbind shader program
        pShader->Use(false);
        throw;
    }

    // unbind shader program
    pShader->Use(false);

    return true;
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::DrawLine(const DWF_Math::Vector3F&   start,
                               const DWF_Math::Vector3F&   end,
                               const DWF_Model::ColorF&    startColor,
                               const DWF_Model::ColorF&    endColor,
                               const DWF_Math::Matrix4x4F& modelMatrix,
                               const Shader*               pShader,
                                     float                 width,
                                     bool                  smooth) const
{
    // no shader program?
    if (!pShader)
        return;

    // set the line width to use
    glLineWidth(width);

    // do draw smooth lines?
    if (smooth)
    {
        // enabled the line smoothing mode
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }

    // bind shader program
    pShader->Use(true);

    // get model matrix slot from shader
    const GLint uniform = GetUniform(pShader, Shader::IEAttribute::IE_SA_ModelMatrix);

    // found it?
    if (uniform != -1)
        // connect model matrix to shader
        glUniformMatrix4fv(uniform, 1, GL_FALSE, modelMatrix.GetPtr());

    // generate the line vertex buffer
    const float lineVertex[] =
    {
        start.m_X, start.m_Y, start.m_Z, startColor.m_R, startColor.m_G, startColor.m_B, startColor.m_A,
        end.m_X,   end.m_Y,   end.m_Z,   endColor.m_R,   endColor.m_G,   endColor.m_B,   endColor.m_A
    };

    const std::size_t stride = 7;

    // get shader vertices attribute
    const GLint posAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Vertices);

    // found it?
    if (posAttrib == -1)
        return;

    // get shader color attribute
    const GLint colorAttrib = GetAttribute(pShader, Shader::IEAttribute::IE_SA_Color);

    // found it?
    if (colorAttrib == -1)
        return;

    // link the line buffer to the shader
    glVertexAttribPointer(posAttrib,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          stride * sizeof(float),
                          &lineVertex[0]);
    glVertexAttribPointer(colorAttrib,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          stride * sizeof(float),
                          &lineVertex[3]);

    // draw the line
    glDrawArrays(GL_LINES, 0, 2);

    // unbind shader program
    pShader->Use(false);
}
//---------------------------------------------------------------------------
void Renderer_OpenGL::SelectTexture(const Shader* pShader, const DWF_Model::Texture* pTexture) const
{
    // do draw textures?
    if (!pTexture)
    {
        glDisable(GL_TEXTURE_2D);
        return;
    }

    // select texture
    pTexture->Select(pShader);
}
//---------------------------------------------------------------------------
int Renderer_OpenGL::GetUniform(const Shader* pShader, Shader::IEAttribute attribute)
{
    // no shader?
    if (!pShader)
        return -1;

    // get uniform property name
    std::string propertyName = pShader->GetAttributeName(attribute);

    // found it?
    if (!propertyName.length())
        return -1;

    // get model matrix slot from shader
    return glGetUniformLocation((GLuint)pShader->GetProgramID(), propertyName.c_str());
}
//---------------------------------------------------------------------------
int Renderer_OpenGL::GetAttribute(const Shader* pShader, Shader::IEAttribute attribute)
{
    // no shader?
    if (!pShader)
        return -1;

    // get attribute property name
    std::string propertyName = pShader->GetAttributeName(attribute);

    // found it?
    if (!propertyName.length())
        return -1;

    // get shader interpolation position attribute
    return glGetAttribLocation((GLuint)pShader->GetProgramID(), propertyName.c_str());
}
//---------------------------------------------------------------------------
