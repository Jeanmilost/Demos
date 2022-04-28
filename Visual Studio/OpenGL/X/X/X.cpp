/****************************************************************************
 * ==> X -------------------------------------------------------------------*
 ****************************************************************************
 * Description : DirectX .x model reader demo                               *
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

 // classes
#include "XModel.h"
#include "TextureHelper.h"
#include "Texture_OpenGL.h"
#include "Shader_OpenGL.h"
#include "Renderer_OpenGL.h"
#include "Model.h"

// libraries
#include <windows.h>

// openGL
#include <gl/gl.h>
#define GLEW_STATIC
#include <GL/glew.h>

//------------------------------------------------------------------------------
bool g_ShowSkeleton   = false;
bool g_PauseAnim      = false;
int  g_LastKnownFrame = 0;
//------------------------------------------------------------------------------
const char vertexShader[] = "precision mediump float;"
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
const char lineVertShader[] = "precision mediump float;"
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
const char fragmentShader[] = "precision mediump float;"
                              "uniform      sampler2D sTexture;"
                              "varying lowp vec4      vColor;"
                              "varying      vec2      vTexCoord;"
                              "void main(void)"
                              "{"
                              "    gl_FragColor = vColor * texture2D(sTexture, vTexCoord);"
                              "}";
//------------------------------------------------------------------------------
const char lineFragShader[] = "precision mediump float;"
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

                case VK_SPACE:
                    g_PauseAnim = !g_PauseAnim;
                    break;

                case VK_ESCAPE:
                    ::PostQuitMessage(0);
                    break;
            }

            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
//------------------------------------------------------------------------------
Texture* OnLoadTexture(const std::string& textureName, bool is32bit)
{
    std::size_t width   = 0;
    std::size_t height  = 0;
    std::size_t format  = 0;
    std::size_t length  = 0;
    void*       pPixels = nullptr;

    if (!TextureHelper::OpenBitmapData("Resources\\Models\\Tiny\\" + textureName, width, height, format, length, pPixels))
        return nullptr;

    if (!pPixels)
        return nullptr;

    std::unique_ptr<Texture_OpenGL> pTexture(new Texture_OpenGL());
    pTexture->m_Width     = (int)width;
    pTexture->m_Height    = (int)height;
    pTexture->m_Format    = format == 24 ? Texture::IEFormat::IE_FT_24bit : Texture::IEFormat::IE_FT_32bit;
    pTexture->m_WrapMode  = Texture::IEWrapMode::IE_WM_Clamp;
    pTexture->m_MinFilter = Texture::IEMinFilter::IE_MI_Linear;
    pTexture->m_MagFilter = Texture::IEMagFilter::IE_MA_Linear;
    pTexture->Create(pPixels);

    return pTexture.release();
}
//---------------------------------------------------------------------------
int DrawX(const XModel&          xModel,
          const Matrix4x4F&      modelMatrix,
          const Shader_OpenGL*   pShader,
          const Renderer_OpenGL* pRenderer,
                int              animSetIndex,
                int              frameCount)
{
    // no renderer?
    if (!pRenderer)
        return 0;

    // no shader?
    if (!pShader)
        return 0;

    const int frameIndex       = g_PauseAnim ? g_LastKnownFrame : (::GetTickCount64() * 5) % frameCount;
              g_LastKnownFrame = frameIndex;

    Model* pModel = xModel.GetModel(animSetIndex, frameCount, frameIndex);

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
void DrawBone(const XModel&          xModel,
              const Model*           pModel,
              const Model::IBone*    pBone,
              const Matrix4x4F&      modelMatrix,
              const Shader_OpenGL*   pShader,
              const Renderer_OpenGL* pRenderer,
              int                    animSetIndex,
              int                    frameCount,
              int                    frameIndex)
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
        Model::IBone* pChild = pBone->m_Children[i];

        Matrix4x4F topMatrix;

        if (pModel->m_PoseOnly)
            pModel->GetBoneMatrix(pBone, Matrix4x4F::Identity(), topMatrix);
        else
        if (pModel->m_AnimationSet.size())
            xModel.GetBoneAnimMatrix(pBone,
                                     pModel->m_AnimationSet[animSetIndex],
                                     frameIndex,
                                     Matrix4x4F::Identity(),
                                     topMatrix);

        Matrix4x4F bottomMatrix;

        if (pModel->m_PoseOnly)
            pModel->GetBoneMatrix(pChild, Matrix4x4F::Identity(), bottomMatrix);
        else
        if (pModel->m_AnimationSet.size())
            xModel.GetBoneAnimMatrix(pChild,
                                     pModel->m_AnimationSet[animSetIndex],
                                     frameIndex,
                                     Matrix4x4F::Identity(),
                                     bottomMatrix);

        glDisable(GL_DEPTH_TEST);
        pRenderer->DrawLine(Vector3F(topMatrix.m_Table[3][0],    topMatrix.m_Table[3][1],    topMatrix.m_Table[3][2]),
                            Vector3F(bottomMatrix.m_Table[3][0], bottomMatrix.m_Table[3][1], bottomMatrix.m_Table[3][2]),
                            ColorF(0.25f, 0.12f, 0.1f, 1.0f),
                            ColorF(0.95f, 0.06f, 0.15f, 1.0f),
                            modelMatrix,
                            pShader);
        glEnable(GL_DEPTH_TEST);

        DrawBone(xModel, pModel, pChild, modelMatrix, pShader, pRenderer, animSetIndex, frameCount, frameIndex);
    }
}
//---------------------------------------------------------------------------
void DrawSkeleton(const XModel&          xModel,
                  const Matrix4x4F&      modelMatrix,
                  const Shader_OpenGL*   pShader,
                  const Renderer_OpenGL* pRenderer,
                  int                    animSetIndex,
                  int                    frameCount,
                  int                    frameIndex)
{
    if (!pRenderer)
        return;

    if (!pShader)
        return;

    Model* pModel = xModel.GetModel(animSetIndex, frameCount, frameIndex);

    DrawBone(xModel, pModel, pModel->m_pSkeleton, modelMatrix, pShader, pRenderer, animSetIndex, frameCount, frameIndex);
}
//------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    WNDCLASSEX wcex = {0};
    HWND       hWnd;
    MSG        msg;
    BOOL       bQuit = FALSE;

    // register window class
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_OWNDC;
    wcex.lpfnWndProc   = WindowProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = ::LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = L"xReader";
    wcex.hIconSm       = ::LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    // create main window
    hWnd = ::CreateWindowEx(0,
                            L"xReader",
                            L".x reader",
                            WS_DLGFRAME | WS_CAPTION | WS_SYSMENU,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            800,
                            600,
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

    Renderer_OpenGL renderer;

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

    Shader_OpenGL shader;
    shader.CreateProgram();
    shader.Attach(vertexShader,   Shader::IEType::IE_ST_Vertex);
    shader.Attach(fragmentShader, Shader::IEType::IE_ST_Fragment);
    shader.Link(true);

    Shader_OpenGL lineShader;
    lineShader.CreateProgram();
    lineShader.Attach(lineVertShader, Shader::IEType::IE_ST_Vertex);
    lineShader.Attach(lineFragShader, Shader::IEType::IE_ST_Fragment);
    lineShader.Link(true);

    XModel x;
    x.Set_OnLoadTexture(OnLoadTexture);
    x.Open("Resources\\Models\\Tiny\\tiny_4anim.x");

    Matrix4x4F projMatrix;

    // create the viewport
    renderer.CreateViewport(float(clientRect.right  - clientRect.left),
                            float(clientRect.bottom - clientRect.top),
                            0.1f,
                            1000.0f,
                            &shader,
                            projMatrix);

    // connect the projection matrix to the line shader
    renderer.ConnectProjectionMatrixToShader(&lineShader, projMatrix);

    // connect the view matrix to the both model and line shaders
    Matrix4x4F viewMatrix = Matrix4x4F::Identity();
    renderer.ConnectViewMatrixToShader(&shader,     viewMatrix);
    renderer.ConnectViewMatrixToShader(&lineShader, viewMatrix);

    ColorF bgColor;
    bgColor.m_R = 0.08f;
    bgColor.m_G = 0.12f;
    bgColor.m_B = 0.17f;
    bgColor.m_A = 1.0f;

    float  angle    = 0.0f;
    double lastTime = 0.0f;

    // program main loop
    while (!bQuit)
    {
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
            Matrix4x4F matrix = Matrix4x4F::Identity();
            Vector3F   axis;

            // create the X rotation matrix
            Matrix4x4F rotMatX;
            axis.m_X = 1.0f;
            axis.m_Y = 0.0f;
            axis.m_Z = 0.0f;
            rotMatX  = matrix.Rotate(float(-M_PI) / 2.0f, axis);

            // create the Y rotation matrix
            Matrix4x4F rotMatZ;
            axis.m_X = 0.0f;
            axis.m_Y = 0.0f;
            axis.m_Z = 1.0f;
            rotMatZ  = matrix.Rotate(angle, axis);

            // create the scale matrix
            Matrix4x4F scaleMat    = Matrix4x4F::Identity();
            scaleMat.m_Table[0][0] = 0.075f;
            scaleMat.m_Table[1][1] = 0.075f;
            scaleMat.m_Table[2][2] = 0.075f;

            // combine the rotation matrices
            rotMatZ.Multiply(rotMatX);

            // place the model in the 3d world (update the matrix directly)
            Matrix4x4F modelMatrix    =  rotMatZ.Multiply(scaleMat);
            modelMatrix.m_Table[3][1] = -18.0f;
            modelMatrix.m_Table[3][2] = -50.0f;

            // draw the scene
            renderer.BeginScene(bgColor, (Renderer::IESceneFlags)((unsigned)Renderer::IESceneFlags::IE_SF_ClearColor |
                                                                  (unsigned)Renderer::IESceneFlags::IE_SF_ClearDepth));

            // draw the model
            const int frameIndex = DrawX(x, modelMatrix, &shader, &renderer, 1, 4800);

            // draw the skeleton
            if (g_ShowSkeleton)
                DrawSkeleton(x, modelMatrix, &lineShader, &renderer, 1, 4800, frameIndex);

            renderer.EndScene();

            // calculate the elapsed time
            double elapsedTime = (double)::GetTickCount64() - lastTime;
            lastTime           = (double)::GetTickCount64();
            angle              = std::fmodf(angle + ((float)elapsedTime * 0.001f), 2.0f * (float)M_PI);

            Sleep(1);
        }
    }

    // shutdown OpenGL
    renderer.DisableOpenGL(hWnd);

    // destroy the window explicitly
    ::DestroyWindow(hWnd);

    return (int)msg.wParam;
}
//------------------------------------------------------------------------------
