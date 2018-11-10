/*****************************************************************************
 * ==> Main -----------------------------------------------------------------*
 *****************************************************************************
 * Description : Application main entry point                                *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

// windows
#include <windows.h>

// openGL
#include <gl/gl.h>

// qr engine
#include "QR_Random.h"
#include "QR_Camera.h"

// classes
#include "Classes/QR_Constants.h"
#include "Classes/QR_Starfield.h"

//------------------------------------------------------------------------------
// Global defines
//------------------------------------------------------------------------------
#define GLInt int
//------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
//------------------------------------------------------------------------------
// WinMain
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
        int iCmdShow)
{
    WNDCLASS wc;
    HWND     hWnd;
    HDC      hDC;
    HGLRC    hRC;
    MSG      msg;
    BOOL     bQuit = FALSE;

    // register window class
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = ::LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "StarFieldForm";
    RegisterClass(&wc);

    GLint backingWidth  = (GLInt)(QR_Constants::m_OrthoScreen_X * 2.0f);
    GLint backingHeight = (GLInt)(QR_Constants::m_OrthoScreen_Y * 2.0f);

    // create main window
    hWnd = CreateWindow("StarFieldForm",
                        "Star field",
                        WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
                        0,
                        0,
                        (unsigned)backingWidth,
                        (unsigned)backingHeight,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    // enable OpenGL for the window
    EnableOpenGL(hWnd, &hDC, &hRC);

    // initialize star field objects
    QR_Random::Initialize();
    QR_Starfield starfield;

    QR_Color background(0, 0, 0);
    QR_Color foreground(255, 255, 255);
    float    starVelocity   = 10.0f;
    float    rotateVelocity = 0.0001f;
    unsigned starsQuantity  = 500;

    starfield.InitializeStars(starsQuantity,
                              QR_Screen((unsigned)(backingHeight + 150.0f),
                                        (unsigned)(backingHeight + 150.0f)));
    starfield.SetBackgroundColor(background);
    starfield.SetStarsColor(foreground);
    starfield.SetSpeedVelocity(starVelocity);
    starfield.SetRotateVelocity(rotateVelocity);

    QR_Camera camera;
    camera.Initialize(backingWidth, backingHeight, M_StarDepth);
    camera.SetCamera();

    std::clock_t last = std::clock();

    // program main loop
    while (!bQuit)
    {
        // check for messages
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            // handle or dispatch messages
            if (msg.message == WM_QUIT)
                bQuit = TRUE;
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            std::clock_t now = std::clock();

            if (now - last > 1)
            {
                last = now;

                float r = (float)background.GetRed()   / 255.0f;
                float g = (float)background.GetGreen() / 255.0f;
                float b = (float)background.GetBlue()  / 255.0f;

                glClearColor(r, g, b, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glMatrixMode(GL_MODELVIEW);
                starfield.Render();

                SwapBuffers(hDC);
            }
        }
    }

    // shutdown OpenGL
    DisableOpenGL(hWnd, hDC, hRC);

    // destroy the window explicitly
    DestroyWindow(hWnd);

    return msg.wParam;
}
//------------------------------------------------------------------------------
// Window procedure
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            return 0;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    return 0;
            }

            return 0;

        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
}
//------------------------------------------------------------------------------
// Enable OpenGL
//------------------------------------------------------------------------------
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int                   iFormat;

    //get the device context (DC)
    *hDC = GetDC(hWnd);

    // set the pixel format for the DC
    ::ZeroMemory(&pfd, sizeof (pfd));
    pfd.nSize      = sizeof (pfd);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat        = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    // create and enable the render context (RC)
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}
//------------------------------------------------------------------------------
// Disable OpenGL
//------------------------------------------------------------------------------
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
//------------------------------------------------------------------------------
