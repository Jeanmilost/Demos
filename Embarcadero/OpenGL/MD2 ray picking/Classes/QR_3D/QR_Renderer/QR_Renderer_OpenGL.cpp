/****************************************************************************
 * ==> QR_Renderer_OpenGL --------------------------------------------------*
 ****************************************************************************
 * Description : Renderer using OpenGL for drawing                          *
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

#include "QR_Renderer_OpenGL.h"

// qr engine
#include "QR_Exception.h"
#include "QR_MathTools.h"
#include "QR_Texture_OpenGL.h"

#ifdef USE_SHADER
    #include "QR_Shader_OpenGL.h"
#endif

//---------------------------------------------------------------------------
// QR_Renderer_OpenGL
//---------------------------------------------------------------------------
QR_Renderer_OpenGL::QR_Renderer_OpenGL() :
    QR_Renderer()
    #if defined(OS_WIN)
        ,
        m_hWnd(NULL),
        m_hDC(NULL),
        m_hRC(NULL)
    #endif
{}
//---------------------------------------------------------------------------
QR_Renderer_OpenGL::~QR_Renderer_OpenGL()
{}
//---------------------------------------------------------------------------
#if defined(OS_WIN)
    bool QR_Renderer_OpenGL::EnableOpenGL(HWND hWnd)
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
        if (!wglMakeCurrent(m_hDC, m_hRC))
            return false;

        return true;
    }
#endif
//---------------------------------------------------------------------------
#if defined(OS_WIN)
    void QR_Renderer_OpenGL::DisableOpenGL(HWND hWnd)
    {
        // disable and delete OpenGL context
        if (m_hRC)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(m_hRC);
        }

        // delete device context
        if (hWnd && m_hDC)
            ::ReleaseDC(hWnd, m_hDC);
    }
#endif
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::CreateViewport(int width, int height) const
{
    #if defined(OS_WIN)
        // enable OpenGL render context
        if (!wglMakeCurrent(m_hDC, m_hRC))
            return;
    #endif

    // invalid width?
    if (!width)
        width = 1;

    // invalid height?
    if (!height)
        height = 1;

    // set viewport
    glViewport(0, 0, width, height);
}
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::BeginScene(const QR_Color& color, IESceneFlags flags) const
{
    #if defined(OS_WIN)
        // enable OpenGL render context
        if (!wglMakeCurrent(m_hDC, m_hRC))
            return;
    #endif

    // begin post-processing effect
    if (m_pPostProcessingEffect)
        m_pPostProcessingEffect->Begin();

    GLbitfield openGLSceneFlags = 0;

    // clear background color, if needed
    if (flags & IE_SF_ClearColor)
    {
        glClearColor((GLclampf)color.GetRedF(), (GLclampf)color.GetGreenF(),
                (GLclampf)color.GetBlueF(), (GLclampf)color.GetAlphaF());

        openGLSceneFlags |= GL_COLOR_BUFFER_BIT;
    }

    // clear Z buffer, if needed
    if (flags & IE_SF_ClearDepth)
    {
        glClearDepth(1.0f);

        openGLSceneFlags |= GL_DEPTH_BUFFER_BIT;
    }

    // clear scene, fill with background color and set render flags
    glClear(openGLSceneFlags);
}
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::EndScene() const
{
    #if defined(OS_WIN)
        // enable OpenGL render context
        if (!wglMakeCurrent(m_hDC, m_hRC))
            return;
    #endif

    // end post-processing effect
    if (m_pPostProcessingEffect)
        m_pPostProcessingEffect->End();

    #if defined(OS_WIN)
        // no device context?
        if (!m_hDC)
            return;

        // present back buffer
        ::SwapBuffers(m_hDC);
    #elif defined(OS_OSX)
        // nothing to do, Apple OpenGLES implementation does everything for us
    #else
        #error "Do implement EndScene() for this platform"
    #endif
}
//---------------------------------------------------------------------------
#ifdef CP_EMBARCADERO
    void QR_Renderer_OpenGL::GetBitmapFromOpenGL(TBitmap* pBitmap) const
    {
        // no bitmap?
        if (!pBitmap)
            return;

        GLint dimensions[4];

        // get viewport dimensions
        glGetIntegerv(GL_VIEWPORT, dimensions);

        TRGBQuad* pPixels = NULL;

        try
        {
            // create bits to contain bitmap
            pPixels = new TRGBQuad[dimensions[2] * dimensions[3] * 4];

            // flush OpenGL
            glFinish();
            glPixelStorei(GL_PACK_ALIGNMENT,   4);
            glPixelStorei(GL_PACK_ROW_LENGTH,  0);
            glPixelStorei(GL_PACK_SKIP_ROWS,   0);
            glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

            // get pixels from last OpenGL rendering
            glReadPixels(0, 0, dimensions[2], dimensions[3], GL_RGBA, GL_UNSIGNED_BYTE, pPixels);

            // configure destination bitmap
            pBitmap->PixelFormat = pf32bit;
            pBitmap->SetSize(dimensions[2], dimensions[3]);

            // iterate through lines to copy
            for (GLint y = 0; y < dimensions[3]; ++y)
            {
                // get next line to copy and calculate y position (origin is on the left bottom on the
                // source, but on the left top on the destination)
                TRGBQuad*         pLine = static_cast<TRGBQuad*>(pBitmap->ScanLine[y]);
                const std::size_t yPos  = ((dimensions[3] - 1) - y) * dimensions[2];

                // iterate through pixels to copy
                for (GLint x = 0; x < dimensions[2]; ++x)
                {
                    // take the opportunity to swap the pixel RGB values
                    pLine[x].rgbRed      = pPixels[yPos + x].rgbBlue;
                    pLine[x].rgbGreen    = pPixels[yPos + x].rgbGreen;
                    pLine[x].rgbBlue     = pPixels[yPos + x].rgbRed;
                    pLine[x].rgbReserved = pPixels[yPos + x].rgbReserved;
                }
            }
        }
        __finally
        {
            if (pPixels)
                delete[] pPixels;
        }
    }
#endif
//---------------------------------------------------------------------------
#ifdef CP_EMBARCADERO
    bool QR_Renderer_OpenGL::BytesFromBitmap(TBitmap* pBitmap, BYTE*& pPixels, bool flipY, bool bgr) const
    {
        // no bitmap?
        if (!pBitmap)
            return false;

        // is bitmap empty?
        if ((pBitmap->Width <= 0) || (pBitmap->Height <= 0))
            return false;

        // get bitmap and pixel size
        const std::size_t width     = pBitmap->Width;
        const std::size_t height    = pBitmap->Height;
        const std::size_t pixelSize = (pBitmap->PixelFormat == pf32bit) ? sizeof(TRGBQuad) : sizeof(TRGBTriple);

        // calculate line size
        const std::size_t lineSize = width * pixelSize;

        // create pixels buffer
        pPixels = new BYTE[height * lineSize];

        // iterate through bitmap lines
        for (std::size_t y = 0; y < height; ++y)
        {
            // calculate next offset
            const std::size_t offset = flipY ? ((height - 1) - y) * lineSize : y * lineSize;

            // is 24 or 32 bit bitmap?
            if (pBitmap->PixelFormat == pf24bit)
            {
                // get pixels line from bitmap
                TRGBTriple* pLineRGB = reinterpret_cast<TRGBTriple*>(pBitmap->ScanLine[y]);

                // do swap pixels?
                if (bgr)
                    // memory copy 24 bit pixels line, as pixels are already in RGB format
                    std::memcpy(&pPixels[offset], pLineRGB, lineSize);
                else
                    // iterate through line pixels
                    for (std::size_t x = 0; x < width; ++x)
                    {
                        // calculate next pixel offset
                        const std::size_t offsetX = offset + (x * pixelSize);

                        // copy and swap pixel
                        pPixels[offsetX]     = pLineRGB[x].rgbtRed;
                        pPixels[offsetX + 1] = pLineRGB[x].rgbtGreen;
                        pPixels[offsetX + 2] = pLineRGB[x].rgbtBlue;
                    }
            }
            else
            {
                // get pixels line from bitmap
                TRGBQuad* pLineRGBA = reinterpret_cast<TRGBQuad*>(pBitmap->ScanLine[y]);

                // do swap pixels?
                if (bgr)
                    // memory copy 32 bit pixels line, as pixels are already in RGB format
                    std::memcpy(&pPixels[offset], pLineRGBA, lineSize);
                else
                    // iterate through line pixels
                    for (std::size_t x = 0; x < width; ++x)
                    {
                        // calculate next pixel offset
                        const std::size_t offsetX = offset + (x * pixelSize);

                        // copy and swap pixel
                        pPixels[offsetX]     = pLineRGBA[x].rgbRed;
                        pPixels[offsetX + 1] = pLineRGBA[x].rgbGreen;
                        pPixels[offsetX + 2] = pLineRGBA[x].rgbBlue;
                        pPixels[offsetX + 3] = pLineRGBA[x].rgbReserved;
                    }
            }
        }

        return true;
    }
#endif
//---------------------------------------------------------------------------
QR_Texture* QR_Renderer_OpenGL::GenerateTexture() const
{
    return new QR_Texture_OpenGL();
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    QR_Shader* QR_Renderer_OpenGL::GenerateShader() const
    {
        return new QR_Shader_OpenGL();
    }
#endif
//--------------------------------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_Renderer_OpenGL::ConnectProjectionMatrixToShader(const QR_Shader*    pShader,
                                                             const QR_Matrix16P& projectionMatrix) const
    {
        if (!pShader)
            return;

        // bind shader program
        pShader->Use(true);

        // get perspective (or projection) matrix slot from shader
        const GLint uniform = GetUniform(pShader, QR_Shader::IE_SA_PerspectiveMatrix);

        // found it?
        if (uniform == -1)
            M_THROW_EXCEPTION("Program uniform not found - perspective");

        // connect perspective (or projection) matrix to shader
        glUniformMatrix4fv(uniform, 1, GL_FALSE, projectionMatrix.GetPtr());

        // unbind shader program
        pShader->Use(false);
    }
#endif
//--------------------------------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_Renderer_OpenGL::ConnectViewMatrixToShader(const QR_Shader*    pShader,
                                                       const QR_Matrix16P& viewMatrix) const
    {
        if (!pShader)
            return;

        // bind shader program
        pShader->Use(true);

        // get view (or camera) matrix slot from shader
        const GLint uniform = GetUniform(pShader, QR_Shader::IE_SA_CameraMatrix);

        // found it?
        if (uniform == -1)
            M_THROW_EXCEPTION("Program uniform not found - camera");

        // connect view (or camera) matrix to shader
        glUniformMatrix4fv(uniform, 1, GL_FALSE, viewMatrix.GetPtr());

        // unbind shader program
        pShader->Use(false);
    }
#endif
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_Renderer_OpenGL::SetProjectionMatrix(const QR_Shader*    pShader,
                                                 const QR_Matrix16P& projectionMatrix) const
#else
    void QR_Renderer_OpenGL::SetProjectionMatrix(const QR_Matrix16P& projectionMatrix) const
#endif
{
    #ifdef USE_SHADER
        ConnectProjectionMatrixToShader(pShader, projectionMatrix);
    #else
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projectionMatrix.GetPtr());
    #endif
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_Renderer_OpenGL::SetViewMatrix(const QR_Shader*    pShader,
                                           const QR_Matrix16P& viewMatrix) const
#else
    void QR_Renderer_OpenGL::SetViewMatrix(const QR_Matrix16P& viewMatrix) const
#endif
{
    #ifdef USE_SHADER
        ConnectViewMatrixToShader(pShader, viewMatrix);
    #else
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(viewMatrix.GetPtr());
    #endif
}
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::Draw(const QR_Mesh&          mesh,
                              const QR_Vector3DP&     translation,
                                    float             rotationX,
                                    float             rotationY,
                                    float             rotationZ,
                              const QR_Vector3DP&     scale,
                              const QR_ModelTextures& textures) const
{
    // get mesh count
    const std::size_t count = mesh.size();

    // no mesh to draw?
    if (!count)
        return;

    std::size_t stride;

    // calculate stride. As all meshes share the same vertex properties, the first mesh can be used
    // to extract vertex format info
    if (mesh[0]->m_CoordType == QR_Vertex::IE_VC_XYZ)
        stride = 3;
    else
        stride = 2;

    // do use normals array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_Normals)
        stride += 3;

    // do use textures coordinates array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_TexCoords)
        stride += 2;

    // do use colors array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_Colors)
        stride += 4;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    // place model into 3D world
    glTranslatef(translation.m_X, translation.m_Y, translation.m_Z);
    glRotatef(QR_MathTools::RadToDeg(rotationX), 1.0, 0.0, 0.0);
    glRotatef(QR_MathTools::RadToDeg(rotationY), 0.0, 1.0, 0.0);
    glRotatef(QR_MathTools::RadToDeg(rotationZ), 0.0, 0.0, 1.0);
    glScalef(scale.m_X, scale.m_Y, scale.m_Z);

    // iterate through vertices to draw
    for (std::size_t i = 0; i < count; ++i)
    {
        SelectTexture(textures, mesh[i]->m_Name.c_str());

        // bind vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,
                        GL_FLOAT,
                        stride * sizeof(float),
                        &mesh[i]->m_Buffer[0]);

        std::size_t offset = 3;

        // bind normals array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Normals)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT,
                            stride * sizeof(float),
                            &mesh[i]->m_Buffer[offset]);

            offset += 3;
        }

        // bind texture coordinates array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_TexCoords)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2,
                              GL_FLOAT,
                              stride * sizeof(float),
                              &mesh[i]->m_Buffer[offset]);

            offset += 2;
        }

        // bind colors array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Colors)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4,
                           GL_FLOAT,
                           stride * sizeof(float),
                           &mesh[i]->m_Buffer[offset]);
        }

        // draw mesh
        switch (mesh[i]->m_Type)
        {
            case QR_Vertex::IE_VT_Triangles:     glDrawArrays(GL_TRIANGLES,      0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_Quads:         glDrawArrays(GL_QUADS,          0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_QuadStrip:     glDrawArrays(GL_QUAD_STRIP,     0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_Unknown:
            default:                             M_THROW_EXCEPTION("Unknown vertex type");
        }

        // unbind vertex array
        glDisableClientState(GL_VERTEX_ARRAY);

        // unbind normals array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Normals)
            glDisableClientState(GL_NORMAL_ARRAY);

        // unbind texture coordinates array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_TexCoords)
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        // unbind colors array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Colors)
            glDisableClientState(GL_COLOR_ARRAY);

        glFlush();
    }

    glPopMatrix();
}
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::Draw(const QR_Mesh&          mesh,
                              const QR_Matrix16P&     modelMatrix,
                              const QR_ModelTextures& textures) const
{
    // get mesh count
    const std::size_t count = mesh.size();

    // no mesh to draw?
    if (!count)
        return;

    std::size_t stride;

    // calculate stride. As all meshes share the same vertex properties, the first mesh can be used
    // to extract vertex format info
    if (mesh[0]->m_CoordType == QR_Vertex::IE_VC_XYZ)
        stride = 3;
    else
        stride = 2;

    // do use normals array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_Normals)
        stride += 3;

    // do use textures coordinates array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_TexCoords)
        stride += 2;

    // do use colors array?
    if (mesh[0]->m_Format & QR_Vertex::IE_VF_Colors)
        stride += 4;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    // unfortunately, because Delphi don't allow to declare a const function as in C++
    QR_Matrix16P* pModelMatrix = const_cast<QR_Matrix16P*>(&modelMatrix);

    // place model into 3D world
    glLoadMatrixf(pModelMatrix->GetPtr());

    // iterate through vertices to draw
    for (std::size_t i = 0; i < count; ++i)
    {
        SelectTexture(textures, mesh[i]->m_Name.c_str());

        // bind vertex array
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,
                        GL_FLOAT,
                        stride * sizeof(float),
                        &mesh[i]->m_Buffer[0]);

        std::size_t offset = 3;

        // bind normals array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Normals)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT,
                            stride * sizeof(float),
                            &mesh[i]->m_Buffer[offset]);

            offset += 3;
        }

        // bind texture coordinates array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_TexCoords)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2,
                              GL_FLOAT,
                              stride * sizeof(float),
                              &mesh[i]->m_Buffer[offset]);

            offset += 2;
        }

        // bind colors array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Colors)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4,
                           GL_FLOAT,
                           stride * sizeof(float),
                           &mesh[i]->m_Buffer[offset]);
        }

        // draw mesh
        switch (mesh[i]->m_Type)
        {
            case QR_Vertex::IE_VT_Triangles:     glDrawArrays(GL_TRIANGLES,      0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_Quads:         glDrawArrays(GL_QUADS,          0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_QuadStrip:     glDrawArrays(GL_QUAD_STRIP,     0, mesh[i]->m_Buffer.size() / stride); break;
            case QR_Vertex::IE_VT_Unknown:
            default:                             M_THROW_EXCEPTION("Unknown vertex type");
        }

        // unbind vertex array
        glDisableClientState(GL_VERTEX_ARRAY);

        // unbind normals array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Normals)
            glDisableClientState(GL_NORMAL_ARRAY);

        // unbind texture coordinates array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_TexCoords)
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        // unbind colors array
        if (mesh[i]->m_Format & QR_Vertex::IE_VF_Colors)
            glDisableClientState(GL_COLOR_ARRAY);

        glFlush();
    }

    glPopMatrix();
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    bool QR_Renderer_OpenGL::Draw(const QR_Mesh&          mesh,
                                  const QR_Matrix16P&     modelMatrix,
                                  const QR_ModelTextures& textures,
                                  const QR_Shader*        pShader) const
    {
        // get mesh count
        const std::size_t count = mesh.size();

        // no mesh to draw?
        if (!count)
            return false;

        // no shader program?
        if (!pShader)
            return false;

        try
        {
            // bind shader program
            pShader->Use(true);

            // get model matrix slot from shader
            GLint uniform = GetUniform(pShader, QR_Shader::IE_SA_ModelMatrix);

            // found it?
            if (uniform == -1)
                return false;

            // unfortunately, because Delphi don't allow to declare a const function as in C++
            QR_Matrix16P* pModelMatrix = const_cast<QR_Matrix16P*>(&modelMatrix);

            // connect model matrix to shader
            glUniformMatrix4fv(uniform, 1, GL_FALSE, pModelMatrix->GetPtr());

            // get shader position attribute
            GLint posAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Position);

            // found it?
            if (posAttrib == -1)
                return false;

            std::size_t stride;

            // calculate stride. As all meshes share the same vertex properties, the first mesh can
            // be used to extract vertex format info
            if (mesh[0]->m_CoordType == QR_Vertex::IE_VC_XYZ)
                stride = 3;
            else
                stride = 2;

            GLint normalAttrib = -1;

            // do use shader normal attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_Normals)
            {
                // get shader normal attribute
                normalAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Normal);

                // found it?
                if (normalAttrib == -1)
                    return false;

                stride += 3;
            }

            GLint uvAttrib = -1;

            // do use shader UV attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_TexCoords)
            {
                // get shader UV attribute
                uvAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Texture);

                // found it?
                if (uvAttrib == -1)
                    return false;

                // add texture coordinates to stride
                stride += 2;
            }

            GLint colorAttrib = -1;

            // do use shader color attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_Colors)
            {
                // get shader color attribute
                colorAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Color);

                // found it?
                if (colorAttrib == -1)
                    return false;

                // add color to stride
                stride += 4;
            }

            // iterate through OpenGL meshes
            for (std::size_t i = 0; i < count; ++i)
            {
                SelectTexture(pShader, textures, mesh[i]->m_Name);

                std::size_t offset = 0;

                // connect vertices to vertex shader position attribute
                glEnableVertexAttribArray(posAttrib);
                glVertexAttribPointer(posAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      stride * sizeof(float),
                                      &mesh[i]->m_Buffer[offset]);

                if (mesh[i]->m_CoordType == QR_Vertex::IE_VC_XYZ)
                    offset = 3;
                else
                    offset = 2;

                // vertex buffer contains normals?
                if (normalAttrib != -1)
                {
                    // connect the vertices to the vertex shader normal attribute
                    glEnableVertexAttribArray(normalAttrib);
                    glVertexAttribPointer(normalAttrib,
                                          3,
                                          GL_FLOAT,
                                          GL_FALSE,
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);

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
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);

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
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);
                }

                // draw mesh
                switch (mesh[i]->m_Type)
                {
                    case QR_Vertex::IE_VT_Triangles:     glDrawArrays(GL_TRIANGLES,      0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_Quads:         glDrawArrays(GL_QUADS,          0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_QuadStrip:     glDrawArrays(GL_QUAD_STRIP,     0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_Unknown:
                    default:                             M_THROW_EXCEPTION("Unknown vertex type");
                }
            }
        }
        __finally
        {
            // unbind shader program
            pShader->Use(false);
        }

        return true;
    }
#endif
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    bool QR_Renderer_OpenGL::Draw(const QR_Mesh&          mesh,
                                  const QR_Mesh&          nextMesh,
                                  const QR_Matrix16P&     modelMatrix,
                                        float             interpolationFactor,
                                  const QR_ModelTextures& textures,
                                  const QR_Shader*        pShader) const
    {
        // get mesh count
        const std::size_t count = mesh.size();

        // no mesh to draw?
        if (!count)
            return false;

        // no shader program?
        if (!pShader)
            return false;

        try
        {
            // bind shader program
            pShader->Use(true);

            // get model matrix slot from shader
            GLint uniform = GetUniform(pShader, QR_Shader::IE_SA_ModelMatrix);

            // found it?
            if (uniform == -1)
                return false;

            // unfortunately, because Delphi don't allow to declare a const function as in C++
            QR_Matrix16P* pModelMatrix = const_cast<QR_Matrix16P*>(&modelMatrix);

            // connect model matrix to shader
            glUniformMatrix4fv(uniform, 1, GL_FALSE, pModelMatrix->GetPtr());

            // get shader position attribute
            GLint interpolationAttrib = GetUniform(pShader, QR_Shader::IE_SA_Interpolation);

            // found interpolation attribute?
            if (interpolationAttrib == -1)
                return false;

            // send interpolation factor to shader program
            glUniform1f(interpolationAttrib, interpolationFactor);

            // get shader position attribute
            GLint posAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Position);

            // found it?
            if (posAttrib == -1)
                return false;

            // get shader interpolation position attribute
            GLint iPosAttrib = GetAttribute(pShader, QR_Shader::IE_SA_InterpolationPos);

            // found it?
            if (iPosAttrib == -1)
                return false;

            std::size_t stride;

            // calculate stride. As all meshes share the same vertex properties, the first mesh can
            // be used to extract vertex format info
            if (mesh[0]->m_CoordType == QR_Vertex::IE_VC_XYZ)
                stride = 3;
            else
                stride = 2;

            GLint normalAttrib  = -1;
            GLint iNormalAttrib = -1;

            // do use shader normal attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_Normals)
            {
                // get shader normal attribute
                normalAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Normal);

                // found it?
                if (normalAttrib == -1)
                    return false;

                // get shader normal attribute
                iNormalAttrib = GetAttribute(pShader, QR_Shader::IE_SA_InterpolationNormal);

                // found it?
                if (iNormalAttrib == -1)
                    return false;

                stride += 3;
            }

            GLint uvAttrib = -1;

            // do use shader UV attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_TexCoords)
            {
                // get shader UV attribute
                uvAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Texture);

                // found it?
                if (uvAttrib == -1)
                    return false;

                // add texture coordinates to stride
                stride += 2;
            }

            GLint colorAttrib = -1;

            // do use shader color attribute?
            if (mesh[0]->m_Format & QR_Vertex::IE_VF_Colors)
            {
                // get shader color attribute
                colorAttrib = GetAttribute(pShader, QR_Shader::IE_SA_Color);

                // found it?
                if (colorAttrib == -1)
                    return false;

                // add color to stride
                stride += 4;
            }

            // iterate through OpenGL meshes
            for (std::size_t i = 0; i < count; ++i)
            {
                SelectTexture(pShader, textures, mesh[i]->m_Name);

                std::size_t offset = 0;

                // connect vertices to vertex shader position attribute
                glEnableVertexAttribArray(posAttrib);
                glVertexAttribPointer(posAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      stride * sizeof(float),
                                      &mesh[i]->m_Buffer[offset]);

                // connect vertices to vertex shader position attribute
                glEnableVertexAttribArray(iPosAttrib);
                glVertexAttribPointer(iPosAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      stride * sizeof(float),
                                      &nextMesh[i]->m_Buffer[offset]);

                if (mesh[i]->m_CoordType == QR_Vertex::IE_VC_XYZ)
                    offset = 3;
                else
                    offset = 2;

                // vertex buffer contains normals?
                if (normalAttrib != -1)
                {
                    // connect the normals to the vertex shader normal attribute
                    glEnableVertexAttribArray(normalAttrib);
                    glVertexAttribPointer(normalAttrib,
                                          3,
                                          GL_FLOAT,
                                          GL_FALSE,
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);

                    // vertex buffer contains interpolated normals?
                    if (iNormalAttrib != -1)
                    {
                        // connect the interpolated normals to the vertex shader normal attribute
                        glEnableVertexAttribArray(iNormalAttrib);
                        glVertexAttribPointer(iNormalAttrib,
                                              3,
                                              GL_FLOAT,
                                              GL_FALSE,
                                              stride * sizeof(float),
                                              &nextMesh[i]->m_Buffer[offset]);
                    }

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
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);

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
                                          stride * sizeof(float),
                                          &mesh[i]->m_Buffer[offset]);
                }

                // draw mesh
                switch (mesh[i]->m_Type)
                {
                    case QR_Vertex::IE_VT_Triangles:     glDrawArrays(GL_TRIANGLES,      0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_Quads:         glDrawArrays(GL_QUADS,          0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_QuadStrip:     glDrawArrays(GL_QUAD_STRIP,     0, mesh[i]->m_Buffer.size() / stride); break;
                    case QR_Vertex::IE_VT_Unknown:
                    default:                   M_THROW_EXCEPTION("Unknown vertex type");
                }
            }
        }
        __finally
        {
            // unbind shader program
            pShader->Use(false);
        }

        return true;
    }
#endif
//---------------------------------------------------------------------------
void QR_Renderer_OpenGL::SelectTexture(const QR_ModelTextures& textures,
                                       const std::wstring&     modelName) const
{
    // do draw textures?
    if (!textures.size())
    {
        glDisable(GL_TEXTURE_2D);
        return;
    }

    int index = -1;

    const int textureCount = textures.size();

    // iterate through textures belonging to model
    for (int i = 0; i < textureCount; ++i)
        // found a texture to draw?
        if (textures[i] && textures[i]->m_Enabled && textures[i]->m_Name == modelName)
        {
            // get texture index
            index = i;
            break;
        }

    // found texture index to select?
    if (index >= 0)
    {
        // select texture
        textures[index]->Select(NULL);
        return;
    }

    glDisable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    void QR_Renderer_OpenGL::SelectTexture(const QR_Shader*        pShader,
                                           const QR_ModelTextures& textures,
                                           const std::wstring&     modelName) const
    {
        // get color map slot from shader
        GLint uniform = GetUniform(pShader, QR_Shader::IE_SA_ColorMap);

        // found it?
        if (uniform == -1)
            // nothing to do (some shader may have no texture to handle)
            return;

        // do draw textures?
        if (!textures.size())
        {
            glDisable(GL_TEXTURE_2D);
            return;
        }

        int index = -1;

        const int textureCount = textures.size();

        // iterate through textures belonging to model
        for (int i = 0; i < textureCount; ++i)
            // found a texture to draw?
            if (textures[i] && textures[i]->m_Enabled && textures[i]->m_Name == modelName)
            {
                // get texture index
                index = i;
                break;
            }

        // found texture index to select?
        if (index >= 0)
        {
            // select texture
            textures[index]->Select(pShader);
            return;
        }

        glDisable(GL_TEXTURE_2D);
    }
#endif
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    int QR_Renderer_OpenGL::GetUniform(const QR_Shader* pShader, QR_Shader::IEAttribute attribute)
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
        return glGetUniformLocation(pShader->GetProgramID(), propertyName.c_str());
    }
#endif
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    int QR_Renderer_OpenGL::GetAttribute(const QR_Shader* pShader, QR_Shader::IEAttribute attribute)
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
        return glGetAttribLocation(pShader->GetProgramID(), propertyName.c_str());
    }
#endif
//---------------------------------------------------------------------------
