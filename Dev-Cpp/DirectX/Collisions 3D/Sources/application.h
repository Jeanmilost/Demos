/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente l'infrastructure de l'application.  *
 * Version     : 1.1                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#include "resources.h"

#include "e_camera.h"
#include "e_polygon.h"
#include "e_polygonlist.h"
#include "e_sprite.h"
#include "e_texture.h"
#include "e_collisions.h"

#include "map.h"

#include <d3dx9.h>
#include <dinput.h>
#include <windows.h>

// On définit ici une procédure pour convertir le format du buffer clavier.
#define KEYDOWN( Name, Key ) ( Name[Key] & 0x80 )

// Prototype de la fonction d'appel Windows.
LRESULT CALLBACK WindowProcedure( HWND   hWnd,
                                  UINT   Message,
                                  WPARAM wParam,
                                  LPARAM lParam );

/*
 * Enumération des types de collisions.
 */
enum CollisionType
{
    NO_COLLISION          = 0,
    COLLISION_ONLY        = 1,
    COLLISION_AND_SLIDING = 2
};

/*
 * Cette classe permet la gestion de l'infrastructure de l'application.
 */
class Application
{
  private:

    static Application*  p_Application;

  private:

    HWND                  hWnd;
    WNDCLASSEX            WinCl;
    MSG                   Messages;

    LPDIRECT3D9           p_D3D;
    LPDIRECT3DDEVICE9     p_D3DDevice;
    D3DPRESENT_PARAMETERS D3Dpp;

    LPDIRECTINPUT8        p_DI;
    LPDIRECTINPUTDEVICE8  p_Keyboard;

    LPSTR                 p_ClassName;
    LPSTR                 p_WindowName;

    int                   p_WinWidth;
    int                   p_WinHeight;

  private:

    E_Camera*             p_Camera;

    E_Sprite*             p_Wall[NB_WALLS];

    E_Texture*            p_Texture[NB_TEXTURES];

    LPSTR                 p_TextureName[NB_TEXTURES];

    D3DXVECTOR3           p_CurPos;

    float                 p_PosVelocity;
    float                 p_DirVelocity;
    float                 p_CurDir;
    float                 p_AvatarHeight;

    CollisionType         p_CollisionType;

  private:

    Application();
    ~Application();

  public:

    bool Initialize( HINSTANCE hInstance, int nFunsterStil );
    bool Restore();

    int Run();

    void OnMove();
    void Render();

    void UpdateMenu( int MenuID, bool Checked );

    void SetCollisionType( CollisionType Type );

    static Application* GetInstance();
    static void ReleaseInstance();

  private:

    bool OnInitialize();
    bool OnPostInitialize();
    bool CreateFrame( HINSTANCE hInstance );
    bool InitD3D();
    bool InitDInput();

    bool ReadKeyboard();

    D3DXVECTOR3 CheckNextPosition( D3DXVECTOR3 CurrentPosition,
                                   D3DXVECTOR3 NextPosition );

    void ShowError( LPSTR ErrorMessage );
    void OnRender();
};

#endif // APPLICATION_H
