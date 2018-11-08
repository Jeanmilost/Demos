/****************************************************************************
 * ==> QR_Renderer ---------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a generic renderer on which a renderer using a    *
 *               graphic library (e.g. DirectX, OpenGL, ...) may be built   *
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

#ifndef QR_RendererH
#define QR_RendererH

// std
#include <string>
#include <vector>

// qr engine
#include "QR_Types.h"
#include "QR_Exception.h"
#include "QR_Color.h"
#include "QR_Point.h"
#include "QR_Vector3D.h"
#include "QR_Matrix16.h"
#include "QR_Rect.h"
#include "QR_Vertex.h"
#include "QR_Texture.h"
#include "QR_PostProcessingEffect.h"

#ifdef USE_SHADER
    #include "QR_Shader.h"
#endif

// windows
#if defined (OS_WIN)
    #include <Windows.h>
#endif

/**
* Generic renderer for 3D engine
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Renderer
{
    public:
        /**
        * Scene flags
        */
        enum IESceneFlags
        {
            IE_SF_None       = 0x00,
            IE_SF_ClearColor = 0x01,
            IE_SF_ClearDepth = 0x02,
        };

        /**
        * Constructor
        */
        QR_Renderer();

        /**
        * Destructor
        */
        virtual ~QR_Renderer();

        #if defined (OS_WIN)
            /**
            * Sets pixel format to use with OpenGL
            *@param hDC - device context used to draw OpenGL scene
            *@return true on success, otherwise false
            */
            virtual bool SetPixelFormat(HDC hDC) const;
        #endif

        /**
        * Gets the post-processing effect
        *@return Post-processing effect (e.g. anti aliasing) currently defined, NULL if none
        */
        virtual QR_PostProcessingEffect* GetPostProcessingEffect() const;

        /**
        * Sets the post-processing effect
        *@param pEffect - post-processing effect (e.g. anti aliasing) to apply
        */
        virtual void SetPostProcessingEffect(QR_PostProcessingEffect* pEffect);

        /**
        * Creates OpenGL viewport
        *@param width - viewport width
        *@param height - viewport height
        */
        virtual void CreateViewport(int width, int height) const = 0;

        /**
        * Begins a scene
        *@param color - scene background color
        *@param flags - scene flags
        */
        virtual void BeginScene(const QR_Color& color, IESceneFlags flags) const = 0;

        /**
        * Ends a scene
        */
        virtual void EndScene() const = 0;

        /**
        * Generates a texture
        *@return texture
        */
        virtual QR_Texture* GenerateTexture() const = 0;

        /**
        * Generates a shader
        *@return texture
        */
        #ifdef USE_SHADER
            virtual QR_Shader* GenerateShader() const = 0;
        #endif

        /**
        * Connects the projection matrix to the shader
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        #ifdef USE_SHADER
            virtual void ConnectProjectionMatrixToShader(const QR_Shader*    pShader,
                                                         const QR_Matrix16P& projectionMatrix) const = 0;
        #endif

        /**
        * Connects the view matrix to the shader
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        #ifdef USE_SHADER
            virtual void ConnectViewMatrixToShader(const QR_Shader*    pShader,
                                                   const QR_Matrix16P& viewMatrix) const = 0;
        #endif

        /**
        * Sets the projection matrix
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        #ifdef USE_SHADER
            virtual void SetProjectionMatrix(const QR_Shader*    pShader,
                                             const QR_Matrix16P& projectionMatrix) const = 0;
        #else
            virtual void SetProjectionMatrix(const QR_Matrix16P& projectionMatrix) const = 0;
        #endif

        /**
        * Sets the view matrix
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        #ifdef USE_SHADER
            virtual void SetViewMatrix(const QR_Shader*    pShader,
                                       const QR_Matrix16P& viewMatrix) const = 0;
        #else
            virtual void SetViewMatrix(const QR_Matrix16P& viewMatrix) const = 0;
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
                          const QR_ModelTextures& textures) const = 0;

        /**
        * Draws a mesh using OpenGL
        *@param mesh - mesh to draw
        *@param modelMatrix - model matrix to apply to mesh
        *@param textures - model textures
        */
        virtual void Draw(const QR_Mesh&          mesh,
                          const QR_Matrix16P&     modelMatrix,
                          const QR_ModelTextures& textures) const = 0;

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
                              const QR_Shader*        pShader) const = 0;
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
                              const QR_Shader*        pShader) const = 0;
        #endif

        /**
        * Selects texture to draw
        *@param textures - model texture list
        *@param modelName - model name to draw (should match with a texture name in the list)
        */
        virtual void SelectTexture(const QR_ModelTextures& textures,
                                   const std::wstring&     modelName) const = 0;

        /**
        * Selects texture to draw
        *@param pShader - shader that will draw the texture
        *@param textures - model texture list
        *@param modelName - model name to draw (should match with a texture name in the list)
        */
        #ifdef USE_SHADER
            virtual void SelectTexture(const QR_Shader*        pShader,
                                       const QR_ModelTextures& textures,
                                       const std::wstring&     modelName) const = 0;
        #endif

        /**
        * Gets orthogonal projection matrix (glOrtho() OpenGL equivalent)
        *@param left - viewport left edge
        *@param right - viewport right edge
        *@param bottom - viewport bottom edge
        *@param top - viewport top edge
        *@param zNear - near clipping plane
        *@param zFar - far clipping plane
        *@return matrix
        */
        template <class T>
        static inline QR_Matrix16<T> GetOrtho(T left, T right, T bottom, T top, T zNear, T zFar);

        /**
        * Gets frustum projection matrix (glFrustum() OpenGL equivalent)
        *@param left - viewport left edge
        *@param right - viewport right edge
        *@param bottom - viewport bottom edge
        *@param top - viewport top edge
        *@param zNear - near clipping plane
        *@param zFar - far clipping plane
        *@return matrix
        */
        template <class T>
        static inline QR_Matrix16<T> GetFrustum(T left, T right, T bottom, T top, T zNear, T zFar);

        /**
        * Gets perspective matrix
        *@param fov - field of view, in degrees
        *@param aspectRatio - aspect ratio, generally width divided by height
        *@param zNear - near plane clipping
        *@param zFar - far plane clipping
        *@param ortho - if true, an orthogonal matrix will be used instead of frustum
        *@return matrix
        *@note This function returns the exactly same matrix as gluPerspective
        */
        template <class T>
        static inline QR_Matrix16<T> GetPerspective(T    fov,
                                                    T    aspectRatio,
                                                    T    zNear,
                                                    T    zFar,
                                                    bool ortho = false);

        /**
        * Creates combined look at matrix (left hand system)
        *@param position - eye (or camera) target position
        *@param direction - eye (or camera) direction vector
        *@param up - up vector direction
        *@return look at matrix
        */
        template <class T>
        static inline QR_Matrix16<T> LookAtLH(const QR_Vector3D<T>& position,
                                              const QR_Vector3D<T>& direction,
                                              const QR_Vector3D<T>& up);

        /**
        * Creates combined look at matrix (right hand system)
        *@param position - eye (or camera) target position
        *@param direction - eye (or camera) direction vector
        *@param up - up vector direction
        *@return look at matrix
        */
        template <class T>
        static inline QR_Matrix16<T> LookAtRH(const QR_Vector3D<T>& position,
                                              const QR_Vector3D<T>& direction,
                                              const QR_Vector3D<T>& up);

        /**
        * Unprojects a ray (i.e. transforms it in viewport coordinates)
        *@param projectionMatrix - projection matrix
        *@param viewMatrix - view matrix
        *@param[in, out] rayPos - ray position, unprojected ray position on function ends
        *@param[in, out] rayDir - ray direction, unprojected ray direction on function ends
        */
        template<class T>
        static inline void Unproject(const QR_Matrix16<T>& projectionMatrix,
                                     const QR_Matrix16<T>& viewMatrix,
                                           QR_Vector3D<T>& rayPos,
                                           QR_Vector3D<T>& rayDir);

        /**
        * Converts mouse position to viewport position
        *@param point - mouse position, in pixels
        *@param viewRect - viewport rectangle
        *@return converted point in the OpenGL space
        */
        template <class T>
        static inline QR_Vector3D<T> MousePosToViewportPos(const QR_PointI&  mousePos,
                                                           const QR_RectI&   clientRect,
                                                           const QR_Rect<T>& viewRect);

    protected:
        QR_PostProcessingEffect* m_pPostProcessingEffect;
};

//---------------------------------------------------------------------------
// QR_Renderer
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Renderer::GetOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    // OpenGL specifications                                    can be rewritten as
    // |   2/(r-l)       0             0            0  |        |  2/(r-l)      0            0            0  |
    // |   0             2/(t-b)       0            0  |   =>   |  0            2/(t-b)      0            0  |
    // |   0             0            -2/(f-n)      0  |        |  0            0            2/(n-f)      0  |
    // |  -(r+l)/(r-l)  -(t+b)/(t-b)  -(f+n)/(f-n)  1  |        |  (r+l)/(l-r)  (t+b)/(b-t)  (f+n)/(n-f)  1  |

    // are input values out of bounds?
    if ((left == right) || (bottom == top) || (zNear == zFar))
        M_THROW_EXCEPTION("Incorrect input values - cannot create orthogonal matrix");

    // calculate matrix component values
    const T prl = right  + left;
    const T mrl = right  - left;
    const T mlr = left   - right;
    const T ptb = top    + bottom;
    const T mtb = top    - bottom;
    const T mbt = bottom - top;
    const T pfn = zFar   + zNear;
    const T mnf = zNear  - zFar;

    // build matrix
    return QR_Matrix16<T>(T(2.0) / mrl, T(0.0),       T(0.0),       T(0.0),
                          T(0.0),       T(2.0) / mtb, T(0.0),       T(0.0),
                          T(0.0),       T(0.0),       T(2.0) / mnf, T(0.0),
                          prl / mlr,    ptb / mbt,    pfn / mnf,    T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Renderer::GetFrustum(T left, T right, T bottom, T top, T zNear, T zFar)
{
    // OpenGL specifications                                   can be rewritten as
    // |  2n/(r-l)     0             0             0  |        |  2n/(r-l)     0            0             0  |
    // |  0            2n/(t-b)      0             0  |   =>   |  0            2n/(t-b)     0             0  |
    // |  (r+l)/(r-l)  (t+b)/(t-b)  -(f+n)/(f-n)  -1  |        |  (r+l)/(r-l)  (t+b)/(t-b)  (f+n)/(n-f)  -1  |
    // |  0            0            -2fn/(f-n)     0  |        |  0            0            2fn/(n-f)     0  |

    // are input values out of bounds?
    if ((zNear <= T(0.0)) || (zFar <= T(0.0)) || (left == right) || (bottom == top) || (zNear == zFar))
        M_THROW_EXCEPTION("Incorrect input values - cannot create frustum matrix");

    // calculate matrix component values
    const T x2n  = T(2.0) * zNear;
    const T x2nf = T(2.0) * zNear * zFar;
    const T pfn  = zFar   + zNear;
    const T mnf  = zNear  - zFar;
    const T prl  = right  + left;
    const T mrl  = right  - left;
    const T ptb  = top    + bottom;
    const T mtb  = top    - bottom;

    // build matrix
    return QR_Matrix16<T>(x2n / mrl, T(0.0),    T( 0.0),   T(0.0),
                          T(0.0),    x2n / mtb, T( 0.0),   T(0.0),
                          prl / mrl, ptb / mtb, pfn / mnf, x2nf / mnf,
                          T(0.0),    T(0.0),    T(-1.0),   T(0.0));
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Renderer::GetPerspective(T    fov,
                                           T    aspectRatio,
                                           T    zNear,
                                           T    zFar,
                                           bool ortho)
{
    const T maxY = zNear * std::tanf(fov * M_PI / T(360.0));
    const T maxX = maxY  * aspectRatio;

    // do use orthogonal perspective?
    if (ortho)
        return GetOrtho(-maxX, maxX, -maxY, maxY, zNear, zFar);
    else
        return GetFrustum(-maxX, maxX, -maxY, maxY, zNear, zFar);
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Renderer::LookAtLH(const QR_Vector3D<T>& position,
                                     const QR_Vector3D<T>& direction,
                                     const QR_Vector3D<T>& up)
{
    // compute per axis transformations
    const QR_Vector3D<T> zAxis = (direction - position).Normalize();
    const QR_Vector3D<T> xAxis = up.Cross(zAxis).Normalize();
    const QR_Vector3D<T> yAxis = zAxis.Cross(xAxis);

    // create look at matrix, translate eye position
    return QR_Matrix16<T>( xAxis.m_X,            yAxis.m_X,            zAxis.m_X,           T(0.0),
                           xAxis.m_Y,            yAxis.m_Y,            zAxis.m_Y,           T(0.0),
                           xAxis.m_Z,            yAxis.m_Z,            zAxis.m_Z,           T(0.0),
                          -xAxis.Dot(position), -yAxis.Dot(position), -zAxis.Dot(position), T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
QR_Matrix16<T> QR_Renderer::LookAtRH(const QR_Vector3D<T>& position,
                                     const QR_Vector3D<T>& direction,
                                     const QR_Vector3D<T>& up)
{
    // compute per axis transformations
    const QR_Vector3D<T> zAxis = (direction - position).Normalize();
    const QR_Vector3D<T> xAxis = up.Cross(zAxis).Normalize();
    const QR_Vector3D<T> yAxis = zAxis.Cross(xAxis);

    // create look at matrix, translate eye position
    return QR_Matrix16P(xAxis.m_X,           yAxis.m_X,           zAxis.m_X,           T(0.0),
                        xAxis.m_Y,           yAxis.m_Y,           zAxis.m_Y,           T(0.0),
                        xAxis.m_Z,           yAxis.m_Z,           zAxis.m_Z,           T(0.0),
                        xAxis.Dot(position), yAxis.Dot(position), zAxis.Dot(position), T(1.0));
}
//---------------------------------------------------------------------------
template<class T>
void QR_Renderer::Unproject(const QR_Matrix16<T>& projectionMatrix,
                            const QR_Matrix16<T>& viewMatrix,
                                  QR_Vector3D<T>& rayPos,
                                  QR_Vector3D<T>& rayDir)
{
    float determinant;

    // unproject the ray to make it in the viewport coordinates
    const QR_Matrix16P invertProj   = const_cast<QR_Matrix16P&>(projectionMatrix).Inverse(determinant);
    const QR_Matrix16P invertView   = const_cast<QR_Matrix16P&>(viewMatrix).Inverse(determinant);
    const QR_Matrix16P unprojectMat = invertProj.Multiply(invertView);
    rayPos                          = unprojectMat.Transform(rayPos);
    rayDir                          = unprojectMat.Transform(rayDir);
    rayDir                          = rayDir.Normalize();
}
//---------------------------------------------------------------------------
template <class T>
QR_Vector3D<T> QR_Renderer::MousePosToViewportPos(const QR_PointI&  mousePos,
                                                  const QR_RectI&   clientRect,
                                                  const QR_Rect<T>& viewRect)
{
    // calculate client width and height
    const T clientWidth  = clientRect.Width();
    const T clientHeight = clientRect.Height();

    // invalid client width or height?
    if (!clientWidth || !clientHeight)
        return QR_Vector3DP();

    // convert mouse position to OpenGL position
    return QR_Vector3DP(viewRect.m_Left + ((T(mousePos.m_X) * viewRect.Width())  / clientWidth),
                        viewRect.m_Top  - ((T(mousePos.m_Y) * viewRect.Height()) / clientHeight),
                        T(0.0));
}
//---------------------------------------------------------------------------

#endif
