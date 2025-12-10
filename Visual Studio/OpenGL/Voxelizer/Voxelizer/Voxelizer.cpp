/****************************************************************************
 * ==> Voxelizer -----------------------------------------------------------*
 ****************************************************************************
 * Description: Converts a bitmap to a simple Voxel model                   *
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
#include "PixelBuffer.h"
#include "VoxelAsset.h"
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
const char vertexShader[] = "#version 130\n"
                            "precision mediump float;\n"
                            "attribute    vec3 aVertices;\n"
                            "attribute    vec4 aColor;\n"
                            "uniform      mat4 uProjection;\n"
                            "uniform      mat4 uView;\n"
                            "uniform      mat4 uModel;\n"
                            "varying lowp vec4 vColor;\n"
                            "void main(void)\n"
                            "{\n"
                            "    vColor      = aColor;\n"
                            "    gl_Position = uProjection * uView * uModel * vec4(aVertices, 1.0);\n"
                            "}";
//------------------------------------------------------------------------------
const char fragmentShader[] = "#version 130\n"
                              "precision mediump float;\n"
                              "varying lowp vec4 vColor;\n"
                              "void main(void)\n"
                              "{\n"
                              "    gl_FragColor = vColor;\n"
                              "}";
//------------------------------------------------------------------------------
bool g_Rotate = true;
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
                case VK_SPACE:
                    g_Rotate = !g_Rotate;
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
int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    WNDCLASSEX wcex  = {};
    HWND       hWnd  = 0;
    MSG        msg   = {};
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
    wcex.lpszClassName = L"voxelizer_class";

    if (!RegisterClassEx(&wcex))
        return 0;

    // create main window
    hWnd = ::CreateWindowEx(0,
                            L"voxelizer_class",
                            L"Voxelizer",
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
    shader.Use(true);

    Matrix4x4F projMatrix;

    // create the viewport
    renderer.CreateViewport(float(clientRect.right  - clientRect.left),
                            float(clientRect.bottom - clientRect.top),
                            0.1f,
                            1000.0f,
                            &shader,
                            projMatrix);

    PixelBuffer bitmap;
    bitmap.FromBitmap(L"Assets\\bombjack_sprite.bmp");

    VoxelAsset voxelAsset(&shader, &renderer);
    voxelAsset.Load(bitmap);

    shader.Use(true);

    // connect the projection matrix to the shader
    renderer.ConnectProjectionMatrixToShader(&shader, projMatrix);

    // connect the view matrix to the shader
    Matrix4x4F viewMatrix = Matrix4x4F::Identity();
    renderer.ConnectViewMatrixToShader(&shader, viewMatrix);

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
            // calculate the elapsed time
            double elapsedTime = (double)::GetTickCount64() - lastTime;
            lastTime           = (double)::GetTickCount64();

            // draw the scene
            renderer.BeginScene(bgColor, (Renderer::IESceneFlags)((std::uint32_t)Renderer::IESceneFlags::IE_SF_ClearColor |
                                                                  (std::uint32_t)Renderer::IESceneFlags::IE_SF_ClearDepth));

            // create the model matrix
            Matrix4x4F matrix = Matrix4x4F::Identity();
            matrix.m_Table[3][2] = -25.0f;

            Vector3F axis;
            axis.m_X = 0.0f;
            axis.m_Y = 1.0f;
            axis.m_Z = 0.0f;

            // rotate and draw the model
            voxelAsset.Draw(matrix.Rotate(angle, axis));

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
