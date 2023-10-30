/****************************************************************************
 * ==> GJK 3D --------------------------------------------------------------*
 ****************************************************************************
 * Description : Test project for the GJK algorithm in 3D                   *
 * Developer   : Jean-Milost Reymond                                        *
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

 // classes
#include "DWF_TextureHelper.h"
#include "DWF_Texture_OpenGL.h"
#include "DWF_Shader_OpenGL.h"
#include "DWF_Renderer_OpenGL.h"
#include "DWF_Model.h"
#include "DWF_ModelFactory.h"
#include "DWF_IQM.h"
#include "DWF_CapsuleCollider.h"
#include "DWF_GJK.h"
#include "DWF_Sound_OpenAL.h"
#include "DWF_Sound_MiniAudio.h"

#include "DWF_StdFileBuffer.h"

// libraries
#include <windows.h>
#include <windowsx.h>

// openGL
#include <gl/gl.h>
#define GLEW_STATIC
#include <GL/glew.h>

// resources
#include "Resource.h"

//------------------------------------------------------------------------------
/**
* Matrix combination type
*/
enum class EMatCombType
{
    E_CT_Scale_Rotate_Translate,
    E_CT_Scale_Translate_Rotate,
    E_CT_Rotate_Translate_Scale,
    E_CT_Rotate_Scale_Translate,
    E_CT_Translate_Rotate_Scale,
    E_CT_Translate_Scale_Rotate
};
//------------------------------------------------------------------------------
/**
* Camera
*/
struct Camera
{
    EMatCombType       m_MatCombType = EMatCombType::E_CT_Scale_Translate_Rotate;
    float              m_xAngle      = 0.0f;
    float              m_yAngle      = 0.0f;
    float              m_zAngle      = 0.0f;
    DWF_Math::Vector3F m_Position;
    DWF_Math::Vector3F m_Factor;
};
//------------------------------------------------------------------------------
/**
* Arcball
*/
struct ArcBall
{
    DWF_Math::Vector3F m_Position;
    float              m_AngleX = 0.0f;
    float              m_AngleY = 0.0f;
    float              m_Radius = 0.0f;
};
//------------------------------------------------------------------------------
typedef std::map<std::string, DWF_Model::Texture*> Textures;

Textures g_TextureItems;
double   g_AnimLastTime  = 0.0;
double   g_AnimTime      = 0.0;
float    g_xPos          = 0.0f;
float    g_zPos          = 0.0f;
float    g_Velocity      = 0.025f;
int      g_LastMouseXPos = 0;
int      g_LastMouseYPos = 0;
int      g_XDelta        = 0;
int      g_YDelta        = 0;
bool     g_Walking       = false;
bool     g_Jumping       = false;
bool     g_WasWalking    = false;
bool     g_ShowSkeleton  = false;
bool     g_ShowPlayerCap = false;
//------------------------------------------------------------------------------
const char texVertShader[] = "#version 120\n"
                             "precision mediump float;\n"
                             "attribute    vec3 aVertices;"
                             "attribute    vec4 aColor;"
                             "attribute    vec2 aTexCoord;"
                             "uniform      mat4 uProjection;"
                             "uniform      mat4 uView;"
                             "uniform      mat4 uModel;"
                             "varying lowp vec4 vColor;"
                             "varying      vec2 vTexCoord;"
                             "void main(void)"
                             "{"
                             "    vColor      = aColor;"
                             "    vTexCoord   = aTexCoord;"
                             "    gl_Position = uProjection * uView * uModel * vec4(aVertices, 1.0);"
                             "}";
//------------------------------------------------------------------------------
const char colVertShader[] = "#version 120\n"
                             "precision mediump float;\n"
                             "attribute    vec3 aVertices;"
                             "attribute    vec4 aColor;"
                             "uniform      mat4 uProjection;"
                             "uniform      mat4 uView;"
                             "uniform      mat4 uModel;"
                             "varying lowp vec4 vColor;"
                             "void main(void)"
                             "{"
                             "    vColor      = aColor;"
                             "    gl_Position = uProjection * uView * uModel * vec4(aVertices, 1.0);"
                             "}";
//------------------------------------------------------------------------------
const char texFragShader[] = "#version 120\n"
                             "precision mediump float;\n"
                             "uniform      sampler2D sTexture;"
                             "varying lowp vec4      vColor;"
                             "varying      vec2      vTexCoord;"
                             "void main(void)"
                             "{"
                             "    gl_FragColor = vColor * texture2D(sTexture, vTexCoord);"
                             "}";
//------------------------------------------------------------------------------
const char colFragShader[] = "#version 120\n"
                             "precision mediump float;\n"
                             "varying lowp vec4 vColor;"
                             "void main(void)"
                             "{"
                             "    gl_FragColor = vColor;"
                             "}";
//------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            ::PostQuitMessage(0);
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
            switch (wParam)
            {
                case '1':
                    g_ShowSkeleton = !g_ShowSkeleton;
                    break;

                case '2':
                    g_ShowPlayerCap = !g_ShowPlayerCap;
                    break;

                case VK_ESCAPE:
                    ::PostQuitMessage(0);
                    break;
            }

            break;

        default:
            return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
//------------------------------------------------------------------------------
DWF_Model::Texture* OnLoadTexture(const std::string& textureName, bool is32bit)
{
    Textures::iterator it = g_TextureItems.find(textureName);

    if (it == g_TextureItems.end())
    {
        std::size_t width   = 0;
        std::size_t height  = 0;
        std::size_t format  = 0;
        std::size_t length  = 0;
        void*       pPixels = nullptr;

        if (!DWF_Model::TextureHelper::OpenTgaData("Resources\\" + textureName, width, height, format, length, pPixels))
            return nullptr;

        if (!pPixels)
            return nullptr;

        std::unique_ptr<DWF_Model::Texture_OpenGL> pTexture(new DWF_Model::Texture_OpenGL());
        pTexture->m_Width     = (int)width;
        pTexture->m_Height    = (int)height;
        pTexture->m_Format    = format == 24 ? DWF_Model::Texture::IEFormat::IE_FT_24bit : DWF_Model::Texture::IEFormat::IE_FT_32bit;
        pTexture->m_WrapMode  = DWF_Model::Texture::IEWrapMode::IE_WM_Clamp;
        pTexture->m_MinFilter = DWF_Model::Texture::IEMinFilter::IE_MI_Linear;
        pTexture->m_MagFilter = DWF_Model::Texture::IEMagFilter::IE_MA_Linear;
        pTexture->Create(pPixels);

        g_TextureItems[textureName] = pTexture.get();

        return pTexture.release();
    }

    std::unique_ptr<DWF_Model::Texture_OpenGL> pTexture(static_cast<DWF_Model::Texture_OpenGL*>(it->second->Clone()));

    return pTexture.release();
}
//---------------------------------------------------------------------------
int DrawIQM(const DWF_Model::IQM&                iqmModel,
            const DWF_Math::Matrix4x4F&          modelMatrix,
            const DWF_Renderer::Shader_OpenGL*   pShader,
            const DWF_Renderer::Renderer_OpenGL* pRenderer,
                  int                            animSetIndex,
                  int&                           frameIndex,
                  int                            frameCount,
                  bool                           loop)
{
    // no renderer?
    if (!pRenderer)
        return 0;

    // no shader?
    if (!pShader)
        return 0;

    // calculate the elapsed time
    double elapsedTime  = (g_AnimLastTime == 0.0) ? 0.0 : (double)::GetTickCount64() - g_AnimLastTime;
    g_AnimLastTime      = (double)::GetTickCount64();
    g_AnimTime         += (elapsedTime / 1500.0);

    while (g_AnimTime > 0.025)
    {
        ++frameIndex;
        g_AnimTime -= 0.025;
    }

    if (loop)
        frameIndex = (frameIndex % frameCount);
    else
    if (frameIndex >= frameCount)
        frameIndex = frameCount - 1;

    DWF_Model::Model* pModel = iqmModel.GetModel(animSetIndex, frameCount, frameIndex);

    // no model to draw?
    if (!pModel)
        return 0;

    const std::size_t meshCount = pModel->m_Mesh.size();

    // iterate through the meshes to draw
    for (std::size_t i = 0; i < meshCount; ++i)
        // draw the model mesh
        pRenderer->Draw(*pModel->m_Mesh[i], modelMatrix, pShader);

    return frameIndex;
}
//---------------------------------------------------------------------------
void DrawBone(const DWF_Model::IQM&                iqmModel,
              const DWF_Model::Model*              pModel,
              const DWF_Model::Model::IBone*       pBone,
              const DWF_Math::Matrix4x4F&          modelMatrix,
              const DWF_Renderer::Shader_OpenGL*   pShader,
              const DWF_Renderer::Renderer_OpenGL* pRenderer,
                    int                            animSetIndex,
                    int                            frameCount,
                    int                            frameIndex)
{
    if (!pModel)
        return;

    if (!pBone)
        return;

    if (!pRenderer)
        return;

    if (!pShader)
        return;

    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
    {
        DWF_Model::Model::IBone* pChild = pBone->m_Children[i];
        DWF_Math::Matrix4x4F     topMatrix;

        if (pModel->m_PoseOnly)
            pModel->GetBoneMatrix(pBone, DWF_Math::Matrix4x4F::Identity(), topMatrix);
        else
        if (pModel->m_AnimationSet.size())
            iqmModel.GetBoneAnimMatrix(pBone,
                                       pModel->m_AnimationSet[animSetIndex],
                                       frameIndex,
                                       DWF_Math::Matrix4x4F::Identity(),
                                       topMatrix);

        DWF_Math::Matrix4x4F bottomMatrix;

        if (pModel->m_PoseOnly)
            pModel->GetBoneMatrix(pChild, DWF_Math::Matrix4x4F::Identity(), bottomMatrix);
        else
        if (pModel->m_AnimationSet.size())
            iqmModel.GetBoneAnimMatrix(pChild,
                                       pModel->m_AnimationSet[animSetIndex],
                                       frameIndex,
                                       DWF_Math::Matrix4x4F::Identity(),
                                       bottomMatrix);

        glDisable(GL_DEPTH_TEST);
        pRenderer->DrawLine(DWF_Math::Vector3F(topMatrix.m_Table[3][0],    topMatrix.m_Table[3][1],    topMatrix.m_Table[3][2]),
                            DWF_Math::Vector3F(bottomMatrix.m_Table[3][0], bottomMatrix.m_Table[3][1], bottomMatrix.m_Table[3][2]),
                            DWF_Model::ColorF(0.25f, 0.12f, 0.1f, 1.0f),
                            DWF_Model::ColorF(0.95f, 0.06f, 0.15f, 1.0f),
                            modelMatrix,
                            pShader);
        glEnable(GL_DEPTH_TEST);

        DrawBone(iqmModel, pModel, pChild, modelMatrix, pShader, pRenderer, animSetIndex, frameCount, frameIndex);
    }
}
//---------------------------------------------------------------------------
void DrawSkeleton(const DWF_Model::IQM&                iqmModel,
                  const DWF_Math::Matrix4x4F&          modelMatrix,
                  const DWF_Renderer::Shader_OpenGL*   pShader,
                  const DWF_Renderer::Renderer_OpenGL* pRenderer,
                        int                            animSetIndex,
                        int                            frameCount,
                        int                            frameIndex)
{
    if (!pRenderer)
        return;

    if (!pShader)
        return;

    DWF_Model::Model* pModel = iqmModel.GetModel(animSetIndex, frameCount, frameIndex);

    DrawBone(iqmModel, pModel, pModel->m_pSkeleton, modelMatrix, pShader, pRenderer, animSetIndex, frameCount, frameIndex);
}
//---------------------------------------------------------------------------
void BuildMatrix(const DWF_Math::Vector3F& pos, float roll, float pitch, float yaw, float scale, DWF_Math::Matrix4x4F& matrix)
{
    DWF_Math::Matrix4x4F defMatrix = DWF_Math::Matrix4x4F::Identity();
    DWF_Math::Vector3F   axis;

    // set rotation axis
    axis.m_X = 1.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 0.0f;

    // create the rotation matrix
    DWF_Math::Matrix4x4F rotateXMatrix = defMatrix.Rotate(roll, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // set rotation axis
    axis.m_X = 0.0f;
    axis.m_Y = 1.0f;
    axis.m_Z = 0.0f;

    // create the rotation matrix
    DWF_Math::Matrix4x4F rotateYMatrix = defMatrix.Rotate(pitch, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // set rotation axis
    axis.m_X = 0.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 1.0f;

    // create the rotation matrix
    DWF_Math::Matrix4x4F rotateZMatrix = defMatrix.Rotate(yaw, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    DWF_Math::Vector3F factor;

    // set scale factor
    factor.m_X = scale;
    factor.m_Y = scale;
    factor.m_Z = scale;

    // create the scale matrix
    DWF_Math::Matrix4x4F scaleMatrix = defMatrix.Scale(factor);

    // build the model matrix
    DWF_Math::Matrix4x4F intermediateMatrix = scaleMatrix.Multiply(rotateXMatrix);
    intermediateMatrix                      = intermediateMatrix.Multiply(rotateYMatrix);
    matrix                                  = intermediateMatrix.Multiply(rotateZMatrix);

    // place it in the world
    matrix.m_Table[3][0] = pos.m_X;
    matrix.m_Table[3][1] = pos.m_Y;
    matrix.m_Table[3][2] = pos.m_Z;
}
//---------------------------------------------------------------------------
void BuildCameraMatrix(const DWF_Math::Vector3F& pos, float roll, float pitch, float yaw, float scale, DWF_Math::Matrix4x4F& matrix)
{
    const float max = 2.0f * (float)M_PI;

    BuildMatrix(pos, std::fmodf(roll, max), std::fmodf(pitch, max), std::fmodf(yaw, max), scale, matrix);
}
//---------------------------------------------------------------------------
void BuildModelMatrix(const DWF_Math::Vector3F& pos, float roll, float pitch, float yaw, float scale, DWF_Math::Matrix4x4F& matrix)
{
    const float max = 2.0f * (float)M_PI;

    BuildMatrix(pos,
                std::fmodf(-(float)((M_PI / 2.0) + roll),  max),
                std::fmodf( (float)( M_PI        + pitch), max),
                std::fmodf(                       yaw,     max),
                scale,
                matrix);
}
//---------------------------------------------------------------------------
DWF_Math::Matrix4x4F CameraToMatrix(const Camera& camera)
{
    DWF_Math::Matrix4x4F defMatrix = DWF_Math::Matrix4x4F::Identity();

    // build a scale matrix
    const DWF_Math::Matrix4x4F scaleMatrix = defMatrix.Scale(camera.m_Factor);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    DWF_Math::Vector3F axis;

    // build a rotation matrix on the x axis
    axis.m_X = 1.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 0.0f;
    const DWF_Math::Matrix4x4F rotateXMatrix = defMatrix.Rotate(camera.m_xAngle, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // build a rotation matrix on the y axis
    axis.m_X = 0.0f;
    axis.m_Y = 1.0f;
    axis.m_Z = 0.0f;
    const DWF_Math::Matrix4x4F rotateYMatrix = defMatrix.Rotate(camera.m_yAngle, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // build a rotation matrix on the z axis
    axis.m_X = 0.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 1.0f;
    const DWF_Math::Matrix4x4F rotateZMatrix = defMatrix.Rotate(camera.m_zAngle, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // build a translation matrix
    const DWF_Math::Matrix4x4F translateMatrix = defMatrix.Translate(camera.m_Position);

    // build model matrix
    switch (camera.m_MatCombType)
    {
        case EMatCombType::E_CT_Scale_Rotate_Translate:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = scaleMatrix.Multiply(rotateXMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateYMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(rotateZMatrix);
            return buildMatrix3.Multiply(translateMatrix);
        }

        case EMatCombType::E_CT_Scale_Translate_Rotate:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = scaleMatrix.Multiply(translateMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateXMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(rotateYMatrix);
            return buildMatrix3.Multiply(rotateZMatrix);
        }

        case EMatCombType::E_CT_Rotate_Translate_Scale:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = rotateXMatrix.Multiply(rotateYMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateZMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(translateMatrix);
            return buildMatrix3.Multiply(scaleMatrix);
        }

        case EMatCombType::E_CT_Rotate_Scale_Translate:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = rotateXMatrix.Multiply(rotateYMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateZMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(scaleMatrix);
            return buildMatrix3.Multiply(translateMatrix);
        }

        case EMatCombType::E_CT_Translate_Rotate_Scale:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = translateMatrix.Multiply(rotateXMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateYMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(rotateZMatrix);
            return buildMatrix3.Multiply(scaleMatrix);
        }

        case EMatCombType::E_CT_Translate_Scale_Rotate:
        {
            const DWF_Math::Matrix4x4F buildMatrix1 = translateMatrix.Multiply(scaleMatrix);
            const DWF_Math::Matrix4x4F buildMatrix2 = buildMatrix1.Multiply(rotateXMatrix);
            const DWF_Math::Matrix4x4F buildMatrix3 = buildMatrix2.Multiply(rotateYMatrix);
            return buildMatrix3.Multiply(rotateZMatrix);
        }
    }

    return DWF_Math::Matrix4x4F();
}
//------------------------------------------------------------------------------
DWF_Math::Matrix4x4F ArcBallToMatrix(const ArcBall& arcball)
{
    // are angles out of bounds?
#ifdef __CODEGEARC__
    const float angleX = fmod(arcball.m_AngleX, (float)(M_PI * 2.0));
    const float angleY = fmod(arcball.m_AngleY, (float)(M_PI * 2.0));
#else
    const float angleX = std::fmodf(arcball.m_AngleX, (float)(M_PI * 2.0));
    const float angleY = std::fmodf(arcball.m_AngleY, (float)(M_PI * 2.0));
#endif

    DWF_Math::Matrix4x4F defMatrix = DWF_Math::Matrix4x4F::Identity();
    DWF_Math::Vector3F   axis;

    // create a matrix for the rotation on the X axis
    axis.m_X = 1.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 0.0f;
    const DWF_Math::Matrix4x4F cameraMatrixX = defMatrix.Rotate(angleX, axis);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    // create a matrix for the rotation on the Y axis
    axis.m_X = 0.0f;
    axis.m_Y = 1.0f;
    axis.m_Z = 0.0f;
    const DWF_Math::Matrix4x4F cameraMatrixY = defMatrix.Rotate(angleY, axis);

    // combine the rotation matrices
    const DWF_Math::Matrix4x4F cameraMatrixXY = cameraMatrixY.Multiply(cameraMatrixX);

    defMatrix = DWF_Math::Matrix4x4F::Identity();

    const DWF_Math::Matrix4x4F cameraPos = defMatrix.Translate(arcball.m_Position);

    Camera camera;

    // configure the camera
    camera.m_Position.m_X =  0.0f;
    camera.m_Position.m_Y =  0.0f;
    camera.m_Position.m_Z = -arcball.m_Radius;
    camera.m_xAngle       =  0.0f;
    camera.m_yAngle       =  0.0f;
    camera.m_zAngle       =  0.0f;
    camera.m_Factor.m_X   =  1.0f;
    camera.m_Factor.m_Y   =  1.0f;
    camera.m_Factor.m_Z   =  1.0f;
    camera.m_MatCombType  =  EMatCombType::E_CT_Scale_Rotate_Translate;

    // build the camera
    const DWF_Math::Matrix4x4F cameraMatrix = CameraToMatrix(camera);
    return cameraPos.Multiply(cameraMatrixXY.Multiply(cameraMatrix));
}
//------------------------------------------------------------------------------
DWF_Math::Matrix4x4F MoveAndDrawPlayer(ArcBall&                       arcball,
                                       DWF_Math::Matrix4x4F&          viewMatrix,
                                 const DWF_Model::IQM&                iqm_idle,
                                       int&                           idleIndex,
                                 const DWF_Model::IQM&                iqm_walk,
                                       int&                           walkIndex,
                                 const DWF_Model::IQM&                iqm_jump,
                                       int&                           jumpIndex,
                                 const DWF_Renderer::Shader_OpenGL&   texShader,
                                 const DWF_Renderer::Shader_OpenGL&   colShader,
                                 const DWF_Renderer::Renderer_OpenGL& renderer,
                                       double                         elapsedTime)
{
    POINT p;
    
    // get current mouse position
    ::GetCursorPos(&p);

    // calculate delta on x and y axis
    g_XDelta = g_LastMouseXPos - p.x;
    g_YDelta = g_LastMouseYPos - p.y;

    // update the last known position
    g_LastMouseXPos = p.x;
    g_LastMouseYPos = p.y;

    // calculate the new direction from last mouse move
    arcball.m_AngleY -= std::fmodf((float)g_XDelta * 0.01f, (float)M_PI * 2.0f);

    // reset the deltas (otherwise the player will turn forever)
    g_XDelta = 0;
    g_YDelta = 0;

    // calculate the next position
    arcball.m_Position = DWF_Math::Vector3F(g_xPos, -0.5f, 2.0f + g_zPos);

    // calculate the resulting view matrix and connect it to the shaders
    viewMatrix = ArcBallToMatrix(arcball);
    renderer.ConnectViewMatrixToShader(&texShader, viewMatrix);
    renderer.ConnectViewMatrixToShader(&colShader, viewMatrix);

    DWF_Math::Matrix4x4F modelMatrix;

    // calculate the model matrix
    BuildModelMatrix(DWF_Math::Vector3F(-g_xPos, 0.0f, -2.0f - g_zPos), 0.0f, -arcball.m_AngleY, 0.0f, 0.005f, modelMatrix);

    // get the pressed key, if any, and convert it to the matching player state
    if (::GetKeyState(VK_SPACE) & 0x8000)
        g_Jumping = true;
    else
    if ((::GetKeyState(VK_UP) & 0x8000) || (::GetKeyState(87) & 0x8000) || (::GetKeyState(119) & 0x8000))
        g_Walking = true;
    else
        g_Walking = false;

    // dispatch the player state
    if (g_Jumping)
    {
        /*REM
        // was previously walking before jumping?
        if (g_WasWalking)
        {
            // continue to move the player forward
            g_xPos += g_Velocity * std::cosf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
            g_zPos += g_Velocity * std::sinf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
        }
        */

        // draw the jump model
        const int jumpFrameIndex = DrawIQM(iqm_jump, modelMatrix, &texShader, &renderer, 0, jumpIndex, 24, false);

        // draw the jump skeleton
        if (g_ShowSkeleton)
            DrawSkeleton(iqm_jump, modelMatrix, &colShader, &renderer, 0, 24, jumpFrameIndex);

        // calculate the model height (done after model rendering because animation already includes it)
        modelMatrix.m_Table[3][1] = std::sinf((jumpFrameIndex * (float)M_PI) / 23.0f) * 0.5f;

        // jump animation end reached?
        if (jumpFrameIndex >= 23)
        {
            g_Jumping = false;
            jumpIndex = 0;
        }
    }
    else
    if (g_Walking)
    {
        /*REM
        // move player forward
        g_xPos += g_Velocity * std::cosf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
        g_zPos += g_Velocity * std::sinf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
        */

        // draw the walk model
        const int walkFrameIndex = DrawIQM(iqm_walk, modelMatrix, &texShader, &renderer, 0, walkIndex, 30, true);

        // draw the walk skeleton
        if (g_ShowSkeleton)
            DrawSkeleton(iqm_walk, modelMatrix, &colShader, &renderer, 0, 30, walkFrameIndex);

        g_WasWalking = true;
    }
    else
    {
        // draw the idle model
        const int idleFrameIndex = DrawIQM(iqm_idle, modelMatrix, &texShader, &renderer, 0, idleIndex, 180, true);

        // draw the idle skeleton
        if (g_ShowSkeleton)
            DrawSkeleton(iqm_idle, modelMatrix, &colShader, &renderer, 0, 180, idleFrameIndex);

        g_WasWalking = false;
    }

    return modelMatrix;
}
//------------------------------------------------------------------------------
void DrawPlayerCapsule(DWF_Model::Model*              pCapsule,
                 const DWF_Math::Matrix4x4F&          playerMatrix,
                 const DWF_Renderer::Shader_OpenGL&   shader,
                 const DWF_Renderer::Renderer_OpenGL& renderer)
{
    if (!g_ShowPlayerCap)
        return;

    // place the capsule
    DWF_Math::Matrix4x4F matrix = DWF_Math::Matrix4x4F::Identity();
    matrix.m_Table[3][0]        = playerMatrix.m_Table[3][0];
    matrix.m_Table[3][1]        = playerMatrix.m_Table[3][1];
    matrix.m_Table[3][2]        = playerMatrix.m_Table[3][2];

    shader.Use(true);

    // draw the capsule
    for (std::size_t i = 0; i < pCapsule->m_Mesh.size(); ++i)
        renderer.Draw(*pCapsule->m_Mesh[i], matrix, &shader);
}
//------------------------------------------------------------------------------
void DrawBackground(DWF_Model::Model*              pBackground,
              const DWF_Renderer::Shader_OpenGL&   shader,
              const DWF_Renderer::Renderer_OpenGL& renderer)
{
    // place the background
    DWF_Math::Matrix4x4F matrix = DWF_Math::Matrix4x4F::Identity();
    BuildMatrix(DWF_Math::Vector3F(0.0f, 0.0f, -2.0f), (float)M_PI / 2.0f, 0.0f, 0.0f, 1.0f, matrix);
    shader.Use(true);

    // draw it
    for (std::size_t i = 0; i < pBackground->m_Mesh.size(); ++i)
        renderer.Draw(*pBackground->m_Mesh[i], matrix, &shader);
}
//------------------------------------------------------------------------------
DWF_Collider::Capsule_Collider DrawCapsule(DWF_Model::Model* pCapsule,
                                     const DWF_Renderer::Shader_OpenGL&   shader,
                                     const DWF_Renderer::Renderer_OpenGL& renderer)
{
    DWF_Collider::Capsule_Collider collider;
    collider.m_yBase  = 0.0f;
    collider.m_yCap   = 0.85f;
    collider.m_Radius = 0.17f;
    collider.m_Pos    = DWF_Math::Vector3F(5.0f, 0.0f, -2.0f);

    // place the capsule
    DWF_Math::Matrix4x4F matrix = DWF_Math::Matrix4x4F::Identity();
    matrix.m_Table[3][0]        = collider.m_Pos.m_X;
    matrix.m_Table[3][1]        = collider.m_Pos.m_Y;
    matrix.m_Table[3][2]        = collider.m_Pos.m_Z;

    shader.Use(true);

    // draw the capsule
    for (std::size_t i = 0; i < pCapsule->m_Mesh.size(); ++i)
        renderer.Draw(*pCapsule->m_Mesh[i], matrix, &shader);

    return collider;
}
//------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    /*
    DWF_Audio::OpenAL* pOpenALInstance = DWF_Audio::OpenAL::GetInstance();

    DWF_Audio::Sound_OpenAL sound;
    sound.OpenWav(L"Resources\\Angels.wav");
    sound.Play();
    sound.Loop(true);
    */

    /*
    DWF_Audio::Sound_MiniAudio sound;
    //sound.Open(L"Resources\\iLaby3D.mp3");
    sound.Open(L"Resources\\Angels.wav");
    sound.Play();
    sound.Loop(true);
    */

    /*
    DWF_Buffer::StdFileBuffer fb;
    fb.Open(L"Resources\\To be loved.mp3", DWF_Buffer::FileBuffer::IEMode::IE_M_Read);

    DWF_Audio::Sound_MiniAudio sound2;
    sound2.Open(fb);
    sound2.Play();
    sound2.Loop(true);
    */

    WNDCLASSEX wcex = { 0 };
    MSG        msg;
    BOOL       bQuit = FALSE;

    // register window class
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_OWNDC;
    wcex.lpfnWndProc   = WindowProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = ::LoadIcon(::GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON));
    wcex.hIconSm       = ::LoadIcon(::GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON_SMALL));
    wcex.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = L"GJK3D";

    if (!RegisterClassEx(&wcex))
        return 0;

    // create main window
    HWND hWnd = ::CreateWindowEx(0,
                                 L"GJK3D",
                                 L"GJK 3D algorithm",
                                 WS_DLGFRAME | WS_CAPTION | WS_SYSMENU,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 800,
                                 650,
                                 nullptr,
                                 nullptr,
                                 hInstance,
                                 nullptr);

    ::ShowWindow(hWnd, nCmdShow);

    // get the window client rect
    RECT clientRect;
    ::GetClientRect(hWnd, &clientRect);

    // get the window device context
    HDC hDC = ::GetDC(hWnd);

    // please wait text background
    HBRUSH hBrush = ::CreateSolidBrush(RGB(20, 30, 43));
    ::FillRect(hDC, &clientRect, hBrush);
    ::DeleteObject(hBrush);

    // please wait text
    ::SetBkMode(hDC, TRANSPARENT);
    ::SetBkColor(hDC, 0x000000);
    ::SetTextColor(hDC, 0xffffff);
    ::DrawText(hDC, L"Please wait...", 14, &clientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ::ReleaseDC(hWnd, hDC);

    DWF_Renderer::Renderer_OpenGL renderer;

    // enable OpenGL for the window
    renderer.EnableOpenGL(hWnd);

    // stop GLEW crashing on OSX :-/
    glewExperimental = GL_TRUE;

    // initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        // shutdown OpenGL
        renderer.DisableOpenGL(hWnd);

        // destroy the window explicitly
        ::DestroyWindow(hWnd);

        return 0;
    }

    DWF_Renderer::Shader_OpenGL texShader;
    texShader.CreateProgram();
    texShader.Attach(texVertShader, DWF_Renderer::Shader::IEType::IE_ST_Vertex);
    texShader.Attach(texFragShader, DWF_Renderer::Shader::IEType::IE_ST_Fragment);
    texShader.Link(true);

    DWF_Renderer::Shader_OpenGL colShader;
    colShader.CreateProgram();
    colShader.Attach(colVertShader, DWF_Renderer::Shader::IEType::IE_ST_Vertex);
    colShader.Attach(colFragShader, DWF_Renderer::Shader::IEType::IE_ST_Fragment);
    colShader.Link(true);

    DWF_Model::IQM iqm_idle;
    iqm_idle.Set_OnLoadTexture(OnLoadTexture);
    iqm_idle.Open("Resources\\Deborah_Idle.iqm");

    DWF_Model::IQM iqm_walk;
    iqm_walk.Set_OnLoadTexture(OnLoadTexture);
    iqm_walk.Open("Resources\\Deborah_Walk.iqm");

    DWF_Model::IQM iqm_jump;
    iqm_jump.Set_OnLoadTexture(OnLoadTexture);
    iqm_jump.Open("Resources\\Deborah_Jump.iqm");

    DWF_Collider::Capsule_Collider playerCollider;
    playerCollider.m_yBase  = 0.0f;
    playerCollider.m_yCap   = 0.85f;
    playerCollider.m_Radius = 0.17f;

    DWF_Model::VertexFormat vf;
    vf.m_Type   = DWF_Model::VertexFormat::IEType::IE_VT_Triangles;
    vf.m_Format = DWF_Model::VertexFormat::IEFormat::IE_VF_Colors;

    DWF_Model::VertexCulling vc;

    DWF_Model::Material mat;
    mat.m_Color.m_B = 1.0f;
    mat.m_Color.m_G = 0.0f;
    mat.m_Color.m_R = 0.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pPlayerCapsule(DWF_Model::Factory::GetCapsule(0.85f, 0.17f, 16.0f, vf, vc, mat));

    vf.m_Type   = DWF_Model::VertexFormat::IEType::IE_VT_Triangles;
    vf.m_Format = (DWF_Model::VertexFormat::IEFormat)((int)DWF_Model::VertexFormat::IEFormat::IE_VF_Colors |
                                                      (int)DWF_Model::VertexFormat::IEFormat::IE_VF_TexCoords);

    //vc.m_Face = DWF_Model::VertexCulling::IECullingFace::IE_CF_CCW;
    //vc.m_Type = DWF_Model::VertexCulling::IECullingType::IE_CT_Back;

    mat.m_Color.m_B = 1.0f;
    mat.m_Color.m_G = 1.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pBackground(DWF_Model::Factory::GetSurface(20.0f, 20.0f, vf, vc, mat));
    pBackground->m_Mesh[0]->m_VB[0]->m_Material.m_pTexture = OnLoadTexture("background.tga", false);

    vf.m_Format = DWF_Model::VertexFormat::IEFormat::IE_VF_Colors;

    mat.m_Color.m_B = 0.0f;
    mat.m_Color.m_G = 0.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pCapsule(DWF_Model::Factory::GetCapsule(0.85f, 0.17f, 16.0f, vf, vc, mat));

    /*
    DWF_Model::VertexFormat vf;
    vf.m_Type = DWF_Model::VertexFormat::IEType::IE_VT_Triangles;
    vf.m_Format = DWF_Model::VertexFormat::IEFormat::IE_VF_Colors;

    DWF_Model::VertexCulling vc;
    //vc.m_Face = DWF_Model::VertexCulling::IECullingFace::IE_CF_CCW;
    //vc.m_Type = DWF_Model::VertexCulling::IECullingType::IE_CT_Back;

    DWF_Model::Material mat;
    mat.m_Color.m_B = 0.0f;
    mat.m_Color.m_G = 0.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pCapsule(DWF_Model::Factory::GetCapsule(0.2f, 0.05f, 16.0f, vf, vc, mat));

    mat.m_Color.m_B = 0.0f;
    mat.m_Color.m_G = 1.0f;
    mat.m_Color.m_R = 0.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pSpiral(DWF_Model::Factory::GetSpiral(0.0f,
        0.0f,
        1.0f,
        2.2f,
        0.0f,
        0.0f,
        5.0f / 36.0f, //m_Velocity / 36.0f,
        25,
        36,
        vf,
        vc,
        mat));

    mat.m_Color.m_B = 1.0f;
    mat.m_Color.m_G = 1.0f;
    mat.m_Color.m_R = 0.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pSphere(DWF_Model::Factory::GetSphere(0.1f, 20, 20, vf, vc, mat));

    mat.m_Color.m_B = 1.0f;
    mat.m_Color.m_G = 0.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pSurface(DWF_Model::Factory::GetSurface(0.1f, 0.1f, vf, vc, mat));

    mat.m_Color.m_B = 0.0f;
    mat.m_Color.m_G = 1.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pBox(DWF_Model::Factory::GetBox(0.1f, 0.1f, 0.1f, false, vf, vc, mat));

    mat.m_Color.m_B = 1.0f;
    mat.m_Color.m_G = 1.0f;
    mat.m_Color.m_R = 1.0f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pCylinder(DWF_Model::Factory::GetCylinder(0.1f, 0.05f, 0.1f, 50, vf, vc, mat));

    mat.m_Color.m_B = 0.15f;
    mat.m_Color.m_G = 0.3f;
    mat.m_Color.m_R = 0.45f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pDisk(DWF_Model::Factory::GetDisk(0.0f, 0.0f, 0.1f, 50, vf, vc, mat));

    mat.m_Color.m_B = 0.77f;
    mat.m_Color.m_G = 0.12f;
    mat.m_Color.m_R = 0.34f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pRing(DWF_Model::Factory::GetRing(0.0f, 0.0f, 0.05f, 0.1f, 50, vf, vc, mat));

    mat.m_Color.m_B = 0.1f;
    mat.m_Color.m_G = 0.5f;
    mat.m_Color.m_R = 0.75f;
    mat.m_Color.m_A = 1.0f;

    std::unique_ptr<DWF_Model::Model> pTorus(DWF_Model::Factory::GetTorus(0.0f,
        0.0f,
        DWF_Math::Vector2F(0.05f, 0.05f),
        DWF_Math::Vector2F(0.1f, 0.1f),
        50,
        50,
        vf,
        vc,
        mat));
    */

    DWF_Math::Matrix4x4F projMatrix;

    // create the viewport
    renderer.CreateViewport(float(clientRect.right  - clientRect.left),
                            float(clientRect.bottom - clientRect.top),
                            0.1f,
                            100.0f,
                            &texShader,
                            projMatrix);

    // connect the projection matrix to the line shader
    renderer.ConnectProjectionMatrixToShader(&colShader, projMatrix);

    // connect the view matrix to the both model and line shaders
    DWF_Math::Matrix4x4F viewMatrix = DWF_Math::Matrix4x4F::Identity();
    renderer.ConnectViewMatrixToShader(&texShader, viewMatrix);
    renderer.ConnectViewMatrixToShader(&colShader, viewMatrix);

    ArcBall arcball;
    arcball.m_AngleX = 0.25f;
    arcball.m_Radius = 2.0f;

    DWF_Model::ColorF bgColor;
    bgColor.m_R = 0.08f;
    bgColor.m_G = 0.12f;
    bgColor.m_B = 0.17f;
    bgColor.m_A = 1.0f;

    double lastTime  = 0.0f;
    int    idleIndex = 0;
    int    walkIndex = 0;
    int    jumpIndex = 0;

    // program main loop
    while (!bQuit)
        // check for messages
        if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // handle or dispatch messages
            if (msg.message == WM_QUIT)
                bQuit = TRUE;
            else
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            // calculate the elapsed time
            double elapsedTime = (double)::GetTickCount64() - lastTime;
            lastTime           = (double)::GetTickCount64();

            // draw the scene
            renderer.BeginScene(bgColor,
                                (DWF_Renderer::Renderer::IESceneFlags)((unsigned)DWF_Renderer::Renderer::IESceneFlags::IE_SF_ClearColor |
                                                                       (unsigned)DWF_Renderer::Renderer::IESceneFlags::IE_SF_ClearDepth));

            // move and draw the player
            DWF_Math::Matrix4x4F playerMatrix = MoveAndDrawPlayer(arcball,
                                                                  viewMatrix,
                                                                  iqm_idle,
                                                                  idleIndex,
                                                                  iqm_walk,
                                                                  walkIndex,
                                                                  iqm_jump,
                                                                  jumpIndex,
                                                                  texShader,
                                                                  colShader,
                                                                  renderer,
                                                                  elapsedTime);

            // update player collider position
            playerCollider.m_Pos = DWF_Math::Vector3F(playerMatrix.m_Table[3][0],
                                                      playerMatrix.m_Table[3][1],
                                                      playerMatrix.m_Table[3][2]);

            // draw the capsule around the player
            DrawPlayerCapsule(pPlayerCapsule.get(), playerMatrix, colShader, renderer);

            // draw the background
            DrawBackground(pBackground.get(), texShader, renderer);

            // draw the background
            DWF_Collider::Capsule_Collider capsuleCollider = DrawCapsule(pCapsule.get(), colShader, renderer);

            // dispatch the player state
            if (g_Jumping)
            {
                // was previously walking before jumping?
                if (g_WasWalking)
                {
                    // continue to move the player forward
                    g_xPos += g_Velocity * std::cosf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
                    g_zPos += g_Velocity * std::sinf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
                }
            }
            else
            if (g_Walking)
            {
                // move player forward
                g_xPos += g_Velocity * std::cosf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
                g_zPos += g_Velocity * std::sinf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
            }

            DWF_Math::Vector3F mtv;

            while (DWF_Collider::GJK::Resolve(playerCollider, capsuleCollider, mtv))
            {
                g_xPos -= g_Velocity * std::cosf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);
                g_zPos -= g_Velocity * std::sinf(arcball.m_AngleY + (float)(M_PI * 0.5)) * (float)(elapsedTime * 0.025);

                playerCollider.m_Pos = DWF_Math::Vector3F(-g_xPos, 0.0f, -2.0f - g_zPos);
            }

            /*REM
            DWF_Math::Matrix4x4F capsuleMatrix = DWF_Math::Matrix4x4F::Identity();
            capsuleMatrix.m_Table[3][0] = 0.25f;
            capsuleMatrix.m_Table[3][1] = 0.0f;
            capsuleMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pCapsule->m_Mesh.size(); ++i)
                renderer.Draw(*pCapsule->m_Mesh[i], capsuleMatrix, &lineShader);

            DWF_Math::Matrix4x4F spiralMatrix = DWF_Math::Matrix4x4F::Identity();
            spiralMatrix.m_Table[3][0] = 0.0f;
            spiralMatrix.m_Table[3][1] = 0.0f;
            spiralMatrix.m_Table[3][2] = 0.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pSpiral->m_Mesh.size(); ++i)
                renderer.Draw(*pSpiral->m_Mesh[i], spiralMatrix, &lineShader);

            DWF_Math::Matrix4x4F sphereMatrix = DWF_Math::Matrix4x4F::Identity();
            sphereMatrix.m_Table[3][0] = -0.25f;
            sphereMatrix.m_Table[3][1] = 0.0f;
            sphereMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pSphere->m_Mesh.size(); ++i)
                renderer.Draw(*pSphere->m_Mesh[i], sphereMatrix, &lineShader);

            DWF_Math::Matrix4x4F surfaceMatrix = DWF_Math::Matrix4x4F::Identity();
            surfaceMatrix.m_Table[3][0] = 0.0f;
            surfaceMatrix.m_Table[3][1] = -0.25f;
            surfaceMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pSurface->m_Mesh.size(); ++i)
                renderer.Draw(*pSurface->m_Mesh[i], surfaceMatrix, &lineShader);

            DWF_Math::Matrix4x4F boxMatrix = DWF_Math::Matrix4x4F::Identity();
            boxMatrix.m_Table[3][0] = -0.25f;
            boxMatrix.m_Table[3][1] = -0.25f;
            boxMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pBox->m_Mesh.size(); ++i)
                renderer.Draw(*pBox->m_Mesh[i], boxMatrix, &lineShader);

            DWF_Math::Matrix4x4F cylinderMatrix = DWF_Math::Matrix4x4F::Identity();
            cylinderMatrix.m_Table[3][0] = 0.25f;
            cylinderMatrix.m_Table[3][1] = 0.35f;
            cylinderMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pCylinder->m_Mesh.size(); ++i)
                renderer.Draw(*pCylinder->m_Mesh[i], cylinderMatrix, &lineShader);

            DWF_Math::Matrix4x4F diskMatrix = DWF_Math::Matrix4x4F::Identity();
            diskMatrix.m_Table[3][0] = -0.25f;
            diskMatrix.m_Table[3][1] = 0.35f;
            diskMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pDisk->m_Mesh.size(); ++i)
                renderer.Draw(*pDisk->m_Mesh[i], diskMatrix, &lineShader);

            DWF_Math::Matrix4x4F ringMatrix = DWF_Math::Matrix4x4F::Identity();
            ringMatrix.m_Table[3][0] = 0.25f;
            ringMatrix.m_Table[3][1] = -0.35f;
            ringMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pRing->m_Mesh.size(); ++i)
                renderer.Draw(*pRing->m_Mesh[i], ringMatrix, &lineShader);

            DWF_Math::Matrix4x4F torusMatrix = DWF_Math::Matrix4x4F::Identity();
            torusMatrix.m_Table[3][0] = 0.35f;
            torusMatrix.m_Table[3][1] = -0.25f;
            torusMatrix.m_Table[3][2] = -1.0f;
            lineShader.Use(true);
            for (std::size_t i = 0; i < pTorus->m_Mesh.size(); ++i)
                renderer.Draw(*pTorus->m_Mesh[i], torusMatrix, &lineShader);
            */

            renderer.EndScene();
        }

    // shutdown OpenGL
    renderer.DisableOpenGL(hWnd);

    // destroy the window explicitly
    ::DestroyWindow(hWnd);

    return (int)msg.wParam;
}
//------------------------------------------------------------------------------
