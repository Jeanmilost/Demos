/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente l'infrastructure de l'application.  *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <dmusicc.h>
#include <dmusici.h>

#include <d3dx9.h>
#include <windows.h>

#include "e_audio.h"
#include "e_camera.h"

#include "sotb_landscape.h"

// Prototype de la fonction d'appel Windows.
LRESULT CALLBACK WindowProcedure( HWND   hWnd,
                                  UINT   Message,
                                  WPARAM wParam,
                                  LPARAM lParam );

/*
 * Cette classe permet la gestion de l'infrastructure de l'application.
 */
class Application
{
  private:

    static Application* p_Application;

  private:

    HWND                      hWnd;
    WNDCLASSEX                WinCl;
    MSG                       Messages;

    IDirectMusicLoader8*      p_MusicLoader;
    IDirectMusicPerformance8* p_MusicPerformance;

    LPDIRECT3D9               p_D3D;
    LPDIRECT3DDEVICE9         p_D3DDevice;

    LPSTR                     p_ClassName;
    LPSTR                     p_WindowName;

    int                       p_WinWidth;
    int                       p_WinHeight;

  private:

    E_Camera*           p_Camera;

    E_Audio*            p_Audio;

    SOTB_Landscape*     p_Landscape;

  private:

    Application();
    ~Application();

  public:

    bool Initialize( HINSTANCE hInstance, int nFunsterStil );

    int Run();

    void OnMove();
    VOID Render();

    static Application* GetInstance();
    static void ReleaseInstance();

  private:

    bool OnInitialize();
    bool OnPostInitialize( int nFunsterStil );
    bool CreateFrame( HINSTANCE hInstance );
    bool InitD3D();
    bool InitDMusic();

    VOID ShowError( LPSTR ErrorMessage );
    VOID OnRender();
};

#endif // APPLICATION_H
