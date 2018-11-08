/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente l'infrastructure de l'application.  *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#include "e_camera.h"

#include <d3dx9.h>
#include <dinput.h>
#include <windows.h>

#include "e_md2mesh.h"
#include "animationlist.h"

// On définit ici une procédure pour convertir le format du buffer clavier.
#define KEYDOWN( Name, Key ) ( Name[Key] & 0x80 )

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

    static Application*  p_Application;

  private:

    HWND                 hWnd;
    WNDCLASSEX           WinCl;
    MSG                  Messages;

    LPDIRECT3D9          p_D3D;
    LPDIRECT3DDEVICE9    p_D3DDevice;

    LPDIRECTINPUT8       p_DI;
    LPDIRECTINPUTDEVICE8 p_Keyboard;

    LPSTR                p_ClassName;
    LPSTR                p_WindowName;

    int                  p_WinWidth;
    int                  p_WinHeight;

  private:

    D3DLIGHT9            p_D3DLight;
    E_MD2Light           p_PreCalcLight;
    E_MD2LightningMode   p_LightningMode;

    int                  p_CurAnim;

    float                p_CurLightDir;

    E_Camera*            p_Camera;
    E_MD2Mesh*           p_AnimMesh;
    E_MD2Mesh*           p_Weapon;

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
    bool InitDInput();

    bool ReadKeyboard();

    VOID ShowError( LPSTR ErrorMessage );
    VOID OnRender();
};

#endif // APPLICATION_H
