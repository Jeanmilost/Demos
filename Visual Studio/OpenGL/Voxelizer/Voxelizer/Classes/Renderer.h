/****************************************************************************
 * ==> Renderer ------------------------------------------------------------*
 ****************************************************************************
 * Description: Generic renderer                                            *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License                                                              *
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

#pragma once

// std
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>

// classes
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Color.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

// windows
#ifdef _MSC_VER
    #include <Windows.h>
#endif

/**
* Generic renderer for 3D engine
*@author Jean-Milost Reymond
*/
class Renderer
{
    public:
        /**
        * Scene flags
        */
        enum class IESceneFlags
        {
            IE_SF_None       = 0x00,
            IE_SF_ClearColor = 0x01,
            IE_SF_ClearDepth = 0x02,
        };

        /**
        * Constructor
        */
        Renderer();

        /**
        * Destructor
        */
        virtual ~Renderer();

        /**
        * Sets pixel format to use
        *@param hDC - device context used to draw the scene
        *@return true on success, otherwise false
        */
        #ifdef _MSC_VER
            virtual bool SetPixelFormat(HDC hDC) const;
        #endif

        /**
        * Creates OpenGL viewport
        *@param width - viewport width
        *@param height - viewport height
        *@param zNear - viewport near clipping
        *@param zFar - viewport far clipping
        *@param[out] matrix - the projection matrix matching with viewport
        */
        virtual void CreateViewport(      float       w,
                                          float       h,
                                          float       zNear,
                                          float       zFar,
                                    const Shader*     pShader,
                                          Matrix4x4F& matrix) const = 0;

        /**
        * Begins a scene
        *@param color - scene background color
        *@param flags - scene flags
        */
        virtual void BeginScene(const ColorF& color, IESceneFlags flags) const = 0;

        /**
        * Ends a scene
        */
        virtual void EndScene() const = 0;

        /**
        * Generates a texture
        *@return texture
        */
        virtual Texture* GenerateTexture() const = 0;

        /**
        * Generates a shader
        *@return texture
        */
        virtual Shader* GenerateShader() const = 0;

        /**
        * Connects the projection matrix to the shader
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        virtual void ConnectProjectionMatrixToShader(const Shader*     pShader,
                                                     const Matrix4x4F& projectionMatrix) const = 0;

        /**
        * Connects the view matrix to the shader
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        virtual void ConnectViewMatrixToShader(const Shader*     pShader,
                                               const Matrix4x4F& viewMatrix) const = 0;

        /**
        * Sets the projection matrix
        *@param pShader - shader to connect to
        *@param projectionMatrix - projection matrix
        */
        virtual void SetProjectionMatrix(const Shader*     pShader,
                                         const Matrix4x4F& projectionMatrix) const = 0;

        /**
        * Sets the view matrix
        *@param pShader - shader to connect to
        *@param viewMatrix - view matrix
        */
        virtual void SetViewMatrix(const Shader*     pShader,
                                   const Matrix4x4F& viewMatrix) const = 0;

        /**
        * Draws a mesh using shader
        *@param mesh - mesh to draw
        *@param modelMatrix - model matrix to apply to mesh
        *@param pShader - shader that will be used to draw the model
        *@return true on success, otherwise false
        */
        virtual bool Draw(const Mesh&       mesh,
                          const Matrix4x4F& modelMatrix,
                          const Shader*     pShader) const = 0;

        /**
        * Draws a line
        *@param start - start line position
        *@param end - end line position
        *@param startColor - line start color
        *@param endColor - line start color
        *@param modelMatrix - model matrix that will be used to draw the line
        *@param pShader - shader that will be used to draw the line
        *@param width - line width
        *@param smooth - if true, smoothing mode will be enabled for this line
        *@return true on success, otherwise false
        */
        virtual void DrawLine(const Vector3F&   start,
                              const Vector3F&   end,
                              const ColorF&     startColor,
                              const ColorF&     endColor,
                              const Matrix4x4F& modelMatrix,
                              const Shader*     pShader,
                                    float       width  = 1.0f,
                                    bool        smooth = true) const = 0;

        /**
        * Selects texture to draw
        *@param pShader - shader that will draw the texture
        *@param pTexture - the texture to select
        */
        virtual void SelectTexture(const Shader* pShader, const Texture* pTexture) const = 0;

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
        static inline Matrix4x4<T> GetOrtho(T left, T right, T bottom, T top, T zNear, T zFar);

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
        static inline Matrix4x4<T> GetFrustum(T left, T right, T bottom, T top, T zNear, T zFar);

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
        static inline Matrix4x4<T> GetPerspective(T    fov,
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
        static inline Matrix4x4<T> LookAtLH(const Vector3<T>& position,
                                            const Vector3<T>& direction,
                                            const Vector3<T>& up);

        /**
        * Creates combined look at matrix (right hand system)
        *@param position - eye (or camera) target position
        *@param direction - eye (or camera) direction vector
        *@param up - up vector direction
        *@return look at matrix
        */
        template <class T>
        static inline Matrix4x4<T> LookAtRH(const Vector3<T>& position,
                                            const Vector3<T>& direction,
                                            const Vector3<T>& up);

        /**
        * Unprojects a ray (i.e. transforms it in viewport coordinates)
        *@param projectionMatrix - projection matrix
        *@param viewMatrix - view matrix
        *@param[in, out] rayPos - ray position, unprojected ray position on function ends
        *@param[in, out] rayDir - ray direction, unprojected ray direction on function ends
        */
        template<class T>
        static inline void Unproject(const Matrix4x4<T>& projectionMatrix,
                                     const Matrix4x4<T>& viewMatrix,
                                           Vector3<T>&   rayPos,
                                           Vector3<T>&   rayDir);
};

//---------------------------------------------------------------------------
// Renderer
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Renderer::GetOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    // OpenGL specifications                                 can be rewritten as
    // |  2/(r-l)  0         0        -(r+l)/(r-l)  |        |  2/(r-l)  0        0        (r+l)/(l-r)  |
    // |  0        2/(t-b)   0        -(t+b)/(t-b)  |   =>   |  0        2/(t-b)  0        (t+b)/(b-t)  |
    // |  0        0        -2/(f-n)  -(f+n)/(f-n)  |        |  0        0        2/(n-f)  (f+n)/(n-f)  |
    // |  0        0         0         1            |        |  0        0        0        1            |

    // are input values out of bounds?
    if ((left == right) || (bottom == top) || (zNear == zFar))
        throw new std::exception("Incorrect input values - cannot create orthogonal matrix");

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
    return Matrix4x4<T>(T(2.0) / mrl, T(0.0),       T(0.0),       prl / mlr,
                        T(0.0),       T(2.0) / mtb, T(0.0),       ptb / mbt,
                        T(0.0),       T(0.0),       T(2.0) / mnf, pfn / mnf,
                        T(0.0),       T(0.0),       T(0.0),       T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Renderer::GetFrustum(T left, T right, T bottom, T top, T zNear, T zFar)
{
    // OpenGL specifications                                           can be rewritten as
    // |  2n/(r-l)     0             0             0          |        |  2n/(r-l)     0             0            0          |
    // |  0            2n/(t-b)      0             0          |   =>   |  0            2n/(t-b)      0            0          |
    // |  (r+l)/(r-l)  (t+b)/(t-b)  -(f+n)/(f-n)  -2fn/(f-n)  |        |  (r+l)/(r-l)  (t+b)/(t-b)   (f+n)/(n-f)  2fn/(n-f)  |
    // |  0            0            -1             0          |        |  0            0            -1            0          |

    // are input values out of bounds?
    if ((zNear <= T(0.0)) || (zFar <= T(0.0)) || (left == right) || (bottom == top) || (zNear == zFar))
        throw new std::exception("Incorrect input values - cannot create frustum matrix");

    // calculate matrix component values
    const T x2n  = T(2.0) * zNear;
    const T x2nf = x2n    * zFar;
    const T pfn  = zFar   + zNear;
    const T mnf  = zNear  - zFar;
    const T prl  = right  + left;
    const T mrl  = right  - left;
    const T ptb  = top    + bottom;
    const T mtb  = top    - bottom;

    // build matrix
    return Matrix4x4<T>(x2n / mrl, T(0.0),    T(0.0),    T(0.0),
                        T(0.0),    x2n / mtb, T(0.0),    T(0.0),
                        prl / mrl, ptb / mtb, pfn / mnf, x2nf / mnf,
                        T(0.0),    T(0.0),    T(-1.0),   T(0.0));
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Renderer::GetPerspective(T    fov,
                                      T    aspectRatio,
                                      T    zNear,
                                      T    zFar,
                                      bool ortho)
{
    const T maxY = zNear * tanf(fov * T(M_PI) / T(360.0));
    const T maxX = maxY  * aspectRatio;

    // do use orthogonal perspective?
    if (ortho)
        return GetOrtho(-maxX, maxX, -maxY, maxY, zNear, zFar);

    return GetFrustum(-maxX, maxX, -maxY, maxY, zNear, zFar);
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Renderer::LookAtLH(const Vector3<T>& position,
                                const Vector3<T>& direction,
                                const Vector3<T>& up)
{
    // compute per axis transformations
    const Vector3<T> zAxis = (direction - position).Normalize();
    const Vector3<T> xAxis =  up.Cross(zAxis).Normalize();
    const Vector3<T> yAxis =  zAxis.Cross(xAxis);

    // create look at matrix, translate eye position
    return Matrix4x4<T>(xAxis.m_X, xAxis.m_Y, xAxis.m_Z, -xAxis.Dot(position),
                        yAxis.m_X, yAxis.m_Y, yAxis.m_Z, -yAxis.Dot(position),
                        zAxis.m_X, zAxis.m_Y, zAxis.m_Z, -zAxis.Dot(position),
                        T(0.0),    T(0.0),    T(0.0),     T(1.0));
}
//---------------------------------------------------------------------------
template <class T>
Matrix4x4<T> Renderer::LookAtRH(const Vector3<T>& position,
                                const Vector3<T>& direction,
                                const Vector3<T>& up)
{
    // compute per axis transformations
    const Vector3<T> zAxis = (direction - position).Normalize();
    const Vector3<T> xAxis =  up.Cross(zAxis).Normalize();
    const Vector3<T> yAxis =  zAxis.Cross(xAxis);

    // create look at matrix, translate eye position
    return Matrix4x4<T>(xAxis.m_X, xAxis.m_Y, xAxis.m_Z, xAxis.Dot(position),
                        yAxis.m_X, yAxis.m_Y, yAxis.m_Z, yAxis.Dot(position),
                        zAxis.m_X, zAxis.m_Y, zAxis.m_Z, zAxis.Dot(position),
                        T(0.0),    T(0.0),    T(0.0),    T(1.0));
}
//---------------------------------------------------------------------------
template<class T>
void Renderer::Unproject(const Matrix4x4<T>& projectionMatrix,
                         const Matrix4x4<T>& viewMatrix,
                               Vector3<T>&   rayPos,
                               Vector3<T>&   rayDir)
{
    T determinant = T(0.0);

    // unproject the ray to make it in the viewport coordinates
    const Matrix4x4<T> invertProj = const_cast<Matrix4x4<T>&>(projectionMatrix).Inverse(determinant);
                       rayPos     = invertProj.Transform(rayPos);
                       rayDir     = invertProj.Transform(rayDir);
                       rayDir     = rayDir.Normalize();

    const Matrix4x4<T> invertView = const_cast<Matrix4x4<T>&>(viewMatrix).Inverse(determinant);
                       rayPos     = invertView.Transform(rayPos);
                       rayDir     = invertView.TransformNormal(rayDir);
                       rayDir     = rayDir.Normalize();
}
//---------------------------------------------------------------------------
