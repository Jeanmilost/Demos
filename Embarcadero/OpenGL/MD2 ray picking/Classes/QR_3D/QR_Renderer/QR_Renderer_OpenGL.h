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

#ifndef QR_Renderer_OpenGLH
#define QR_Renderer_OpenGLH

// vcl
#if defined(CP_EMBARCADERO)
    #include <Vcl.Graphics.hpp>
#endif

// qr engine
#include "QR_Types.h"
#include "QR_Rect.h"
#include "QR_Renderer.h"

// windows
#if defined (OS_WIN)
    #include <Windows.h>
#endif

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
* Generic renderer for 3D engine
*@note This class is cross-platform
*@note USE_OPENGL_DIRECT_MODE must be globally defined to use the OpenGL direct
*      mode instead of shader. BE CAREFUL, OpenGL direct mode is deprecated
*@author Jean-Milost Reymond
*/
class QR_Renderer_OpenGL : public QR_Renderer
{
    public:
        /**
        * Constructor
        */
        QR_Renderer_OpenGL();

        /**
        * Destructor
        */
        virtual ~QR_Renderer_OpenGL();

        #if defined (OS_WIN)
            /**
            * Enables OpenGL
            *@param hWnd - control or form hanle on which OpenGL scene will be drawn
            *@return true on success, otherwise false
            */
            virtual bool EnableOpenGL(HWND hWnd);
        #endif

        #if defined (OS_WIN)
            /**
            * Disables OpenGL and clears memory
            *@param hWnd - control or form hanle on which OpenGL scene was drawn
            */
            virtual void DisableOpenGL(HWND hWnd);
        #endif

        /**
        * Creates OpenGL viewport
        *@param width - viewport width
        *@param height - viewport height
        */
        virtual void CreateViewport(int width, int height) const;

        /**
        * Begins a scene
        *@param color - scene background color
        *@param flags - scene flags
        */
        virtual void BeginScene(const QR_Color& color, IESceneFlags flags) const;

        /**
        * Ends a scene
        */
        virtual void EndScene() const;

        /**
        * Copies the current OpenGL rendering into a bitmap
        *@param pBitmap - bitmap to copy to
        */
        #ifdef CP_EMBARCADERO
            virtual void GetBitmapFromOpenGL(TBitmap* pBitmap) const;
        #endif

        /**
        * Converts bitmap in pixels byte array
        *@param pBitmap - bitmap to convert
        *@param[out] pPixels - pixels array when function returns, user is responsible to delete it
        *@param flipY - if true, image will be mirrored on the Y axis
        *@param bgr - if true, image will be converted from RGB to BGR (or RGBA to ABGR) format
        *@return true on success, otherwise false
        */
        #ifdef CP_EMBARCADERO
            virtual bool BytesFromBitmap(TBitmap* pBitmap, BYTE*& pPixels, bool flipY, bool bgr) const;
        #endif

        /**
        * Generates a texture
        *@return texture
        */
        virtual QR_Texture* GenerateTexture() const;

        /**
        * Generates a shader
        *@return texture
        */
        #ifdef USE_SHADER
            virtual QR_Shader* GenerateShader() const;
        #endif

        /**
        * Connects the projection matrix to the shader
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        #ifdef USE_SHADER
            virtual void ConnectProjectionMatrixToShader(const QR_Shader*    pShader,
                                                         const QR_Matrix16P& projectionMatrix) const;
        #endif

        /**
        * Connects the view matrix to the shader
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        #ifdef USE_SHADER
            virtual void ConnectViewMatrixToShader(const QR_Shader*    pShader,
                                                   const QR_Matrix16P& viewMatrix) const;
        #endif

        /**
        * Sets the projection matrix
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        #ifdef USE_SHADER
            virtual void SetProjectionMatrix(const QR_Shader*    pShader,
                                             const QR_Matrix16P& projectionMatrix) const;
        #else
            virtual void SetProjectionMatrix(const QR_Matrix16P& projectionMatrix) const;
        #endif

        /**
        * Sets the view matrix
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        #ifdef USE_SHADER
            virtual void SetViewMatrix(const QR_Shader*    pShader,
                                       const QR_Matrix16P& viewMatrix) const;
        #else
            virtual void SetViewMatrix(const QR_Matrix16P& viewMatrix) const;
        #endif

        /**
        * Draws a mesh using OpenGL
        *@param mesh - mesh to draw
        *@param translation - translation to apply to mesh
        *@param rotationX - rotation on x axis to apply to mesh
        *@param rotationY - rotation on y axis to apply to mesh
        *@param rotationZ - rotation on z axis to apply to mesh
        *@param scale - scaling to apply to mesh
        *@param textures - model textures
        */
        virtual void Draw(const QR_Mesh&          mesh,
                          const QR_Vector3DP&     translation,
                                float             rotationX,
                                float             rotationY,
                                float             rotationZ,
                          const QR_Vector3DP&     scale,
                          const QR_ModelTextures& textures) const;

        /**
        * Draws a mesh using OpenGL
        *@param mesh - mesh to draw
        *@param modelMatrix - model matrix to apply to mesh
        *@param textures - model textures
        */
        virtual void Draw(const QR_Mesh&          mesh,
                          const QR_Matrix16P&     modelMatrix,
                          const QR_ModelTextures& textures) const;

        /**
        * Draws a mesh using OpenGL and shader
        *@param mesh - mesh to draw
        *@param modelMatrix - model matrix to apply to mesh
        *@param textures - model textures
        *@param pShader - shader that will be used to draw the model
        *@return true on success, otherwise false
        */
        #ifdef USE_SHADER
            virtual bool Draw(const QR_Mesh&          mesh,
                              const QR_Matrix16P&     modelMatrix,
                              const QR_ModelTextures& textures,
                              const QR_Shader*        pShader) const;
        #endif

        /**
        * Draws a mesh using OpenGL and shader
        *@param mesh - mesh to draw
        *@param nextMesh - mesh to interpolate with
        *@param modelMatrix - model matrix to apply to mesh
        *@param interpolationFactor - interpolation factor
        *@param textures - model textures
        *@param pShader - shader that will be used to draw the model
        *@return true on success, otherwise false
        */
        #ifdef USE_SHADER
            virtual bool Draw(const QR_Mesh&          mesh,
                              const QR_Mesh&          nextMesh,
                              const QR_Matrix16P&     modelMatrix,
                                    float             interpolationFactor,
                              const QR_ModelTextures& textures,
                              const QR_Shader*        pShader) const;
        #endif

        /**
        * Selects texture to draw
        *@param textures - model texture list
        *@param modelName - model name to draw (should match with a texture name in the list)
        */
        virtual void SelectTexture(const QR_ModelTextures& textures,
                                   const std::wstring&     modelName) const;

        /**
        * Selects texture to draw
        *@param pShader - shader that will draw the texture
        *@param textures - model texture list
        *@param modelName - model name to draw (should match with a texture name in the list)
        */
        #ifdef USE_SHADER
            virtual void SelectTexture(const QR_Shader*        pShader,
                                       const QR_ModelTextures& textures,
                                       const std::wstring&     modelName) const;
        #endif

        #if defined (OS_WIN)
            /**
            * Converts mouse position to viewport position
            *@param hWnd - handle of the window or control on which mouse is hoving
            *@param viewRect - OpenGL view rectangle
            *@return converted point in the OpenGL space
            */
            template <class T>
            static QR_Vector3D<T> MousePosToViewportPos(HWND hWnd, const QR_Rect<T>& viewRect);
        #endif

        /**
        * Gets shader uniform hnadle
        *@param pShader - shader containing uniform to get
        *@param attribute - uniform attribute to get from shader
        *@return uniform handle, -1 if not found or on error
        */
        #ifdef USE_SHADER
            static GLint GetUniform(const QR_Shader* pShader, QR_Shader::IEAttribute attribute);
        #endif

        /**
        * Gets shader attribute
        *@param pShader - shader containing attribute to get
        *@param attribute - attribute to get from shader
        *@return uniform handle, -1 if not found or on error
        */
        #ifdef USE_SHADER
            static GLint GetAttribute(const QR_Shader* pShader, QR_Shader::IEAttribute attribute);
        #endif

    private:
        #if defined (OS_WIN)
            HWND  m_hWnd;
            HDC   m_hDC;
            HGLRC m_hRC;
        #endif
};

//---------------------------------------------------------------------------
// QR_Renderer_OpenGL
//---------------------------------------------------------------------------
#if defined(OS_WIN)
    template <class T>
    QR_Vector3D<T> QR_Renderer_OpenGL::MousePosToViewportPos(HWND hWnd, const QR_Rect<T>& viewRect)
    {
        // no window or control?
        if (!hWnd)
            return QR_Vector3DP();

        ::POINT p;

        // get mouse position
        ::GetCursorPos(&p);

        // convert to window or control client coordinates
        if (!::ScreenToClient(hWnd, &p))
            return QR_Vector3DP();

        ::RECT clientRect;

        // get window or control client rect
        ::GetClientRect(hWnd, &clientRect);

        return QR_Renderer::MousePosToViewportPos(QR_Point<int>(p.x, p.y),
                                                  QR_Rect<int>(clientRect.left,
                                                             clientRect.top,
                                                             clientRect.right,
                                                             clientRect.bottom),
                                                  viewRect);
    }
#endif
//---------------------------------------------------------------------------

#endif
