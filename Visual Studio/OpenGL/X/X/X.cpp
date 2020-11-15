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
#include "Shader_OpenGL.h"
#include "Renderer_OpenGL.h"

// libraries
#include <windows.h>

// openGL
#include <gl/gl.h>
#define GLEW_STATIC
#include <GL/glew.h>

//------------------------------------------------------------------------------
const char vertexShader2[] = "precision mediump float;"
"attribute    vec3 aVertices;"
"uniform      mat4 uProjection;"
"uniform      mat4 uView;"
"uniform      mat4 uModel;"
"void main(void)"
"{"
"    gl_Position = uProjection * uView * uModel * vec4(aVertices, 1.0);"
"}";
//------------------------------------------------------------------------------
const char fragmentShader2[] = "precision mediump float;"
"void main(void)"
"{"
"    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
"}";
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
const char fragmentShader[] = "precision mediump float;"
"uniform      sampler2D sTexture;"
"varying lowp vec4      vColor;"
"varying      vec2      vTexCoord;"
"void main(void)"
"{"
"    gl_FragColor = vColor * texture2D(sTexture, vTexCoord);"
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
    Renderer_OpenGL renderer;

    XModel x;
    x.Open("Resources\\Models\\Tiny\\tiny_4anim.x");

    WNDCLASSEX wcex;
    HWND       hWnd;
    MSG        msg;
    BOOL       bQuit = FALSE;
    float      theta = 0.0f;

    // register window class
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_OWNDC;
    wcex.lpfnWndProc   = WindowProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = ::LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = L"xReader";
    wcex.hIconSm       = ::LoadIcon(NULL, IDI_APPLICATION);

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
                            NULL,
                            NULL,
                            hInstance,
                            NULL);

    ::ShowWindow(hWnd, nCmdShow);

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

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    Shader_OpenGL shader;
    shader.CreateProgram();
    shader.Attach(vertexShader2, Shader::IE_ST_Vertex);
    shader.Attach(fragmentShader2, Shader::IE_ST_Fragment);
    shader.Link(true);

    RECT clientRect;
    ::GetClientRect(hWnd, &clientRect);

    Matrix4x4F projMatrix;

    // create the viewport
    renderer.CreateViewport(clientRect.right - clientRect.left,
                            clientRect.bottom - clientRect.top,
                            0.1f,
                            1000.0f,
                            &shader,
                            projMatrix);

    Matrix4x4F viewMatrix = Matrix4x4F::Identity();
    renderer.ConnectViewMatrixToShader(&shader, viewMatrix);

    XModel::IModel* pModel = x.GetModel();

    ColorF bgColor;
    bgColor.m_R = 0.08f;
    bgColor.m_G = 0.12f;
    bgColor.m_B = 0.17f;
    bgColor.m_A = 1.0f;

    Matrix4x4F matrix = Matrix4x4F::Identity();

    // create the rotation matrix
    Matrix4x4F rotMat;
    Vector3F axis;
    axis.m_X = 0.0f;
    axis.m_Y = 0.0f;
    axis.m_Z = 1.0f;
    rotMat = matrix.Rotate(M_PI, axis);

    // create the scale matrix
    Matrix4x4F scaleMat = Matrix4x4F::Identity();
    scaleMat.m_Table[0][0] = 0.075f;
    scaleMat.m_Table[1][1] = 0.075f;
    scaleMat.m_Table[2][2] = 0.075f;

    // place the model in the 3d world (update the matrix directly)
    Matrix4x4F modelMatrix = scaleMat;//rotMat.Multiply(scaleMat);
    modelMatrix.m_Table[3][1] =  10.0f;
    modelMatrix.m_Table[3][2] = -50.0f;

    /*REM
    Mesh mesh;
    VertexBuffer vb;
    vb.m_Format.m_Type = VertexFormat::IE_VT_Triangles;

    vb.m_Data.push_back(0.0f);
    vb.m_Data.push_back(1.0f);
    vb.m_Data.push_back(-2.0f);

    vb.m_Data.push_back(0.87f);
    vb.m_Data.push_back(-0.5f);
    vb.m_Data.push_back(-2.0f);

    vb.m_Data.push_back(-0.87f);
    vb.m_Data.push_back(-0.5f);
    vb.m_Data.push_back(-2.0f);

    mesh.m_VB.push_back(&vb);
    */

    // program main loop
    while (!bQuit)
    {
        // check for messages
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
            // OpenGL animation code goes here
            renderer.BeginScene(bgColor, (Renderer::IESceneFlags)(Renderer::IE_SF_ClearColor | Renderer::IE_SF_ClearDepth));

            /*REM
            glPushMatrix();
            glRotatef(theta, 0.0f, 0.0f, 1.0f);

            glBegin(GL_TRIANGLES);

            glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
            glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
            glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

            glEnd();

            glPopMatrix();
            */

            //REM renderer.Draw(mesh, Matrix4x4F::Identity(), &shader);

            for (std::size_t i = 0; i < pModel->m_Mesh.size(); ++i)
                renderer.Draw(*pModel->m_Mesh[i], modelMatrix, &shader);

            renderer.EndScene();

            theta += 1.0f;
            Sleep(1);
        }
    }

    // shutdown OpenGL
    renderer.DisableOpenGL(hWnd);

    // destroy the window explicitly
    ::DestroyWindow(hWnd);

    return msg.wParam;
}
//------------------------------------------------------------------------------
