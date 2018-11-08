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
#include "e_text.h"
#include "e_polygon.h"
#include "e_polygonlist.h"
#include "e_boundingbox.h"
#include "e_boundingsphere.h"
#include "e_aabbtree.h"
#include "e_collisions.h"
#include "e_mesh.h"

#include <d3dx9.h>
#include <dinput.h>
#include <mmsystem.h>
#include <windows.h>

#include <iostream>

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

    E_Text*               p_Text;

    E_Mesh*               p_Mesh;

    E_BoundingBox         p_Box;
    E_BoundingSphere      p_Sphere;
    E_BoundingSphere      p_Player;

    E_AABBTree*           p_Tree;

    D3DXVECTOR3           p_CurPos;

    int                   p_PolygonsTested;

    float                 p_PosVelocity;
    float                 p_DirVelocity;
    float                 p_CurDir;

    bool                  p_ShowObjectSphere;
    bool                  p_ShowObjectBox;
    bool                  p_ShowPlayerSphere;
    bool                  p_ShowDiags;
    bool                  p_ShowHelp;
    bool                  p_SphereSphereCollision;
    bool                  p_SphereBoxCollision;
    bool                  p_SphereObjCollision;

    unsigned char         p_TextColorA;
    unsigned char         p_TextColorR;
    unsigned char         p_TextColorG;
    unsigned char         p_TextColorB;

  private:

    Application();
    ~Application();

  public:

    bool Initialize( HINSTANCE hInstance, int nFunsterStil );
    bool OnOpenFile();
    bool Restore();

    int Run();

    void ShowError( LPSTR ErrorMessage );
    void OnShowBBObject();
    void OnShowBSObject();
    void OnShowBSPlayer();
    void OnShowDiags();
    void OnShowHelp();
    void OnMove();
    void Render();

    void UpdateMenu( int MenuID, bool Checked );

    static Application* GetInstance();
    static void ReleaseInstance();

  private:

    bool LoadMesh( LPSTR Filename );
    bool OnInitialize();
    bool OnPostInitialize();
    bool CreateFrame( HINSTANCE hInstance );
    bool InitD3D();
    bool InitDInput();

    bool ReadKeyboard();

    bool CheckPosition( D3DXVECTOR3 Position );

    void OnRender();
    void OnRenderDiags();
    void OnRenderHelp();

    LPSTR ConvertBoolToText( bool Value );
};

#endif // APPLICATION_H
