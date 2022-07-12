/****************************************************************************
 * ==> CollisionsTest ------------------------------------------------------*
 ****************************************************************************
 * Description: Capsule collisions test project                             *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - Capsule collision demo                                     *
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
#include "Capsule.h"
#include "MeshFactory.h"
#include "Texture_OpenGL.h"
#include "Shader_OpenGL.h"
#include "Renderer_OpenGL.h"

// openGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/gl.h>

// libraries
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// resources
#include "Resource.h"

//------------------------------------------------------------------------------
const Vector3F g_DefCapsule1Top   (  0.0f,  40.0f,    0.0f);
const Vector3F g_DefCapsule1Bottom(  0.0f,   0.0f,    0.0f);
const Vector3F g_DefCapsule2Top   (  0.0f,  40.0f,    0.0f);
const Vector3F g_DefCapsule2Bottom(  0.0f,   0.0f,    0.0f);
      Vector3F g_Capsule1Pos      (-15.0f, -20.0f, -100.0f);
      Vector3F g_Capsule2Pos      ( 15.0f, -20.0f, -100.0f);
      bool     g_Rotate = false;
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
const char colTestVertShader[] = "precision mediump float;"
                                 "attribute    vec3 aVertices;"
                                 "attribute    vec4 aColor;"
                                 "uniform      int  uIsColliding;"
                                 "uniform      mat4 uProjection;"
                                 "uniform      mat4 uView;"
                                 "uniform      mat4 uModel;"
                                 "varying lowp vec4 vColor;"
                                 "void main(void)"
                                 "{"
                                 "    if (uIsColliding == 1)"
                                 "        vColor = vec4(0.0, 1.0, 0.0, 1.0);"
                                 "    else"
                                 "        vColor = aColor;"
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
                              ""
                              "    if (gl_FragColor.a < 0.5)"
                              "        discard;"
                              "}";
//------------------------------------------------------------------------------
const char colTestFragShader[] = "precision mediump float;"
                                 "varying lowp vec4 vColor;"
                                 "void main(void)"
                                 "{"
                                 "    gl_FragColor = vColor;"
                                 "}";
//------------------------------------------------------------------------------
Capsule g_Capsule1;
Capsule g_Capsule2;
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
                    break;

                case '2':
                    break;

                case VK_SPACE:
                    g_Rotate = !g_Rotate;
                    break;

                case VK_ESCAPE:
                    ::PostQuitMessage(0);
                    break;

                case VK_LEFT:
                    g_Capsule1Pos.m_X += 0.5f;
                    g_Capsule2Pos.m_X -= 0.5f;
                    break;

                case VK_RIGHT:
                    g_Capsule1Pos.m_X -= 0.5f;
                    g_Capsule2Pos.m_X += 0.5f;
                    break;
            }

            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
//------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    WNDCLASSEX wcex = {};
    HWND       hWnd = 0;
    MSG        msg = {};
    BOOL       bQuit = FALSE;

    // register window class
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_OWNDC;
    wcex.lpfnWndProc   = WindowProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLLISIONSTEST));
    wcex.hIconSm       = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = L"CapsuleCollisionsTest";

    if (!RegisterClassEx(&wcex))
        return 0;

    // create main window
    hWnd = ::CreateWindowEx(0,
                            L"CapsuleCollisionsTest",
                            L"Capsule Collisions Test - L/R Arrows to move capsules, space bar to rotate",
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

    Shader_OpenGL colTestShader;
    colTestShader.CreateProgram();
    colTestShader.Attach(colTestVertShader, Shader::IEType::IE_ST_Vertex);
    colTestShader.Attach(colTestFragShader, Shader::IEType::IE_ST_Fragment);
    colTestShader.Link(true);

    g_Capsule1.m_Top    = g_DefCapsule1Top;
    g_Capsule1.m_Radius = 10.0f;

    g_Capsule2.m_Top    = g_DefCapsule2Top;
    g_Capsule2.m_Radius = 10.0f;

    Material material;
    material.m_Color = ColorF(1.0f, 0.1f, 0.05f, 1.0f);

    VertexBuffer::ICulling culling;

    VertexBuffer::IFormat format;
    format.m_Format = (VertexBuffer::IFormat::IEFormat)((std::int32_t)VertexBuffer::IFormat::IEFormat::IE_VF_Colors);
    format.m_Type   =  VertexBuffer::IFormat::IEType::IE_VT_Triangles;

    Mesh capsuleMesh1;
    MeshFactory::GetCapsule(40.0f, 10.0f, 16.0f, format, culling, material, capsuleMesh1);

    material.m_Color = ColorF(0.05f, 0.1f, 1.0f, 1.0f);

    Mesh capsuleMesh2;
    MeshFactory::GetCapsule(40.0f, 10.0f, 16.0f, format, culling, material, capsuleMesh2);

    Matrix4x4F projMatrix;

    // create the viewport
    renderer.CreateViewport(float(clientRect.right - clientRect.left),
                            float(clientRect.bottom - clientRect.top),
                            0.1f,
                            1000.0f,
                            &shader,
                            projMatrix);

    // connect the projection matrix to the line shader
    renderer.ConnectProjectionMatrixToShader(&colTestShader, projMatrix);

    // connect the view matrix to the both model and line shaders
    Matrix4x4F viewMatrix = Matrix4x4F::Identity();
    renderer.ConnectViewMatrixToShader(&colTestShader, viewMatrix);

    colTestShader.Use(true);

    const GLint isColliding = glGetUniformLocation((GLuint)colTestShader.GetProgramID(), "uIsColliding");

    // found it?
    if (isColliding != -1)
        glUniform1i(isColliding, 0);

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

            // create the X rotation matrix
            Matrix4x4F rotMatX;
            Vector3F   axis;
            axis.m_X = 1.0f;
            axis.m_Y = 0.0f;
            axis.m_Z = 0.0f;
            rotMatX  = matrix.Rotate(0.0f, axis);

            // create the Y rotation matrix
            Matrix4x4F rotMatY;
            axis.m_X = 0.0f;
            axis.m_Y = 1.0f;
            axis.m_Z = 0.0f;
            rotMatY  = matrix.Rotate(0.0f, axis);

            // create the Y rotation matrix
            Matrix4x4F rotMatZ;
            axis.m_X = 0.0f;
            axis.m_Y = 0.0f;
            axis.m_Z = 1.0f;
            rotMatZ  = matrix.Rotate(angle, axis);

            Matrix4x4F rotMat;

            // combine the rotation matrices
                     rotMatY.Multiply(rotMatX);
            rotMat = rotMatZ.Multiply(rotMatY);

            // place the models in the 3d world (update the matrix directly)
            Matrix4x4F modelMatrixC1    = rotMat;
            modelMatrixC1.m_Table[3][0] = g_Capsule1Pos.m_X;
            modelMatrixC1.m_Table[3][1] = g_Capsule1Pos.m_Y;
            modelMatrixC1.m_Table[3][2] = g_Capsule1Pos.m_Z;

            matrix = Matrix4x4F::Identity();
            rotMatZ = matrix.Rotate(-angle, axis);
            rotMat  = rotMatZ.Multiply(rotMatY);

            Matrix4x4F modelMatrixC2    = rotMat;
            modelMatrixC2.m_Table[3][0] = g_Capsule2Pos.m_X;
            modelMatrixC2.m_Table[3][1] = g_Capsule2Pos.m_Y;
            modelMatrixC2.m_Table[3][2] = g_Capsule2Pos.m_Z;

            // calculate the elapsed time
            double elapsedTime = (double)::GetTickCount64() - lastTime;
            lastTime = (double)::GetTickCount64();

            // draw the scene
            renderer.BeginScene(bgColor, (Renderer::IESceneFlags)((std::uint32_t)Renderer::IESceneFlags::IE_SF_ClearColor |
                                                                  (std::uint32_t)Renderer::IESceneFlags::IE_SF_ClearDepth));

            // draw the capsule
            //renderer.Draw(capsuleMesh, modelMatrix, &lineShader);
            //renderer.Draw(capsuleMesh1, modelMatrix, &shader);
            //renderer.Draw(capsuleMesh1, modelMatrix, &dirLightShader);
            renderer.Draw(capsuleMesh1, modelMatrixC1, &colTestShader);

            //renderer.Draw(capsuleMesh2, modelMatrix, &shader);
            //renderer.Draw(capsuleMesh2, modelMatrix, &dirLightShader);
            renderer.Draw(capsuleMesh2, modelMatrixC2, &colTestShader);

            colTestShader.Use(true);

            float depth;

            g_Capsule1.m_Top    = modelMatrixC1.Transform(g_DefCapsule1Top);
            g_Capsule1.m_Bottom = modelMatrixC1.Transform(g_DefCapsule1Bottom);
            g_Capsule2.m_Top    = modelMatrixC2.Transform(g_DefCapsule2Top);
            g_Capsule2.m_Bottom = modelMatrixC2.Transform(g_DefCapsule2Bottom);

            if (g_Capsule1.Intersect(g_Capsule2, depth))
            {
                if (isColliding != -1)
                    glUniform1i(isColliding, 1);
            }
            else
            if (isColliding != -1)
                glUniform1i(isColliding, 0);

            renderer.EndScene();

            // calculate the next angle
            if (g_Rotate)
                angle = std::fmodf(angle + ((float)elapsedTime * 0.001f), 2.0f * (float)M_PI);

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
