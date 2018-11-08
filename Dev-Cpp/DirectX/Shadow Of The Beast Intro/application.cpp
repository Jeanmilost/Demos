/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente l'infrastructure de l'application.  *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "application.h"

// Pointeur vers la classe application.
Application* Application::p_Application = NULL;

// Procédure Windows, nécessaire à toutes applications Windows.
LRESULT CALLBACK WindowProcedure( HWND   hWnd,
                                  UINT   Message,
                                  WPARAM wParam,
                                  LPARAM lParam )
{
    switch( Message )
    {
        case WM_PAINT:
        {
            Application::GetInstance()->Render();
            ValidateRect( hWnd, NULL );
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            break;
        }

        default:
        {
            return DefWindowProc( hWnd, Message, wParam, lParam );
        }
    }

    return 0;
}

// Constructeur.
Application::Application()
{
    p_WindowName = "Shadow of the Beast Démo";
    p_ClassName  = "Application";

    p_WinWidth   = 640;
    p_WinHeight  = 480;

    p_D3D        = NULL;
    p_D3DDevice  = NULL;

    p_Camera     = NULL;

    p_Audio      = NULL;

    p_Landscape  = NULL;
}

// Destructeur.
Application::~Application()
{
    // Détruit l'objet caméra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // Détruit l'objet Landscape.
    if ( p_Landscape != NULL )
    {
        delete p_Landscape;
        p_Landscape = NULL;
    }

    if ( p_Audio != NULL )
    {
        p_Audio->Stop( p_MusicPerformance );
        delete p_Audio;
        p_Audio = NULL;
    }

    if ( p_MusicLoader != NULL )
    {
        p_MusicLoader->Release();
        p_MusicLoader = NULL;
    }

	if ( p_MusicPerformance != NULL )
	{
		p_MusicPerformance->CloseDown();
		p_MusicPerformance->Release();
		p_MusicPerformance = NULL;
	}

	CoUninitialize();

    // Détruit le device Direct3D utilisé par cette application.
    if( p_D3DDevice != NULL )
    {
        p_D3DDevice->Release();
        p_D3DDevice = NULL;
    }

    // Détruit l'objet Direct3D utilisé par cette application.
    if( p_D3D != NULL )
    {
        p_D3D->Release();
        p_D3D = NULL;
    }

    // Désalloue les ressources utilisées par la fenêtre.
    UnregisterClass( p_ClassName, WinCl.hInstance );
    DestroyWindow( hWnd );
}

// Cette fonction permet d'afficher un message d'erreur.
VOID Application::ShowError( LPSTR ErrorMessage )
{
    MessageBox( NULL,
                ErrorMessage,
                p_WindowName,
                MB_ICONERROR | MB_OK );
}

// Cette fonction permet de créer et d'initialiser la fenêtre.
bool Application::CreateFrame( HINSTANCE hInstance )
{
    // On crée ici les propriétés de la fenêtre.
    WinCl.hInstance     = hInstance;
    WinCl.lpszClassName = p_ClassName;
    WinCl.lpfnWndProc   = WindowProcedure;
    WinCl.style         = CS_DBLCLKS;
    WinCl.cbSize        = sizeof( WNDCLASSEX );
    WinCl.hIcon         = LoadIcon( hInstance, "A" );
    WinCl.hIconSm       = LoadIcon( hInstance, "A" );
    WinCl.hCursor       = LoadCursor( NULL, IDC_ARROW );
    WinCl.lpszMenuName  = NULL;
    WinCl.cbClsExtra    = 0;
    WinCl.cbWndExtra    = 0;
    WinCl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if ( !RegisterClassEx( &WinCl ) )
    {
        return false;
    }

    // Puis on initialise l'instance de la classe Window.
    hWnd = CreateWindowEx( 0,
                           p_ClassName,
                           p_WindowName,
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           p_WinWidth,
                           p_WinHeight,
                           HWND_DESKTOP,
                           NULL,
                           hInstance,
                           NULL );

    return true;
}

// Cette fonction initialise Direct3D.
bool Application::InitD3D()
{
    // On crée ici une instance de la classe Direct3D9.
    p_D3D = Direct3DCreate9( D3D_SDK_VERSION );

    if( p_D3D == NULL )
    {
        return false;
    }

    // On initialise ensuite les paramètres du device Direct3D.
    D3DPRESENT_PARAMETERS D3Dpp;
    ZeroMemory( &D3Dpp, sizeof( D3Dpp ) );

    D3Dpp.Windowed               = TRUE;
    D3Dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
    D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
    D3Dpp.EnableAutoDepthStencil = TRUE;
    D3Dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    D3Dpp.hDeviceWindow          = hWnd;

    // Puis, on tente la création de l'objet Device.
    if( FAILED( p_D3D->CreateDevice( D3DADAPTER_DEFAULT,
                                     D3DDEVTYPE_HAL,
                                     hWnd,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                     &D3Dpp,
                                     &p_D3DDevice ) ) )
    {
        return false;
    }

    return true;
}

bool Application::InitDMusic()
{
    CoInitialize( NULL );

    CoCreateInstance( CLSID_DirectMusicLoader,
                      NULL,
                      CLSCTX_INPROC,
                      IID_IDirectMusicLoader8,
                      (void**)&p_MusicLoader );

    if ( p_MusicLoader == NULL )
    {
        return false;
    }

    CoCreateInstance( CLSID_DirectMusicPerformance,
                      NULL,
                      CLSCTX_INPROC,
                      IID_IDirectMusicPerformance8,
                      (void**)&p_MusicPerformance );

    if ( p_MusicPerformance == NULL )
    {
        return false;
    }

    if ( FAILED ( p_MusicPerformance->InitAudio( NULL,
                                                 NULL,
                                                 hWnd,
                                                 DMUS_APATH_DYNAMIC_STEREO,
                                                 64,
                                                 DMUS_AUDIOF_ALL,
                                                 NULL ) ) )
    {
        return false;
    }

    return TRUE;
}

// Cette fonction est appelée lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    RECT theRect;
    GetClientRect( hWnd, &theRect );

    // On crée ici l'objet Caméra. Cet objet est le port de vue.
    p_Camera = new E_Camera( (float)( theRect.right  - theRect.left ) - 1.0f,
                             (float)( theRect.bottom - theRect.top ) - 1.0f );

    if ( p_Camera == NULL )
    {
        return false;
    }

    p_Camera->SetClipFar( 1000.0f );

    p_Landscape = new SOTB_Landscape();

    if ( p_Landscape == NULL )
    {
        return false;
    }

    if ( p_Landscape->Initialize( p_D3DDevice ) == false )
    {
        return false;
    }

    p_Audio = new E_Audio();

    if ( p_Audio == NULL )
    {
        return false;
    }

    if ( p_Audio->LoadFile( p_MusicLoader,
                            p_MusicPerformance,
                            "Sounds\\Intro.wav" ) == false )
    {
        return false;
    }

    return true;
}

// Cette fonction est appelée à la fin de l'initialisation de l'application.
bool Application::OnPostInitialize( int nFunsterStil )
{
    D3DCAPS9 D3DCaps;

    // On obtient ici les capacités supportées par la carte graphique.
    p_D3DDevice->GetDeviceCaps( &D3DCaps );

    // Puis, teste si le device Direct3D supporte le format FOGVERTEX, ...
    if( ( D3DCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX ) == 0 )
    {
        return false;
    }

    D3DDISPLAYMODE D3DAdapterMode;

    // ... et on obtient les données du mode d'affichage supporté par la
    // carte graphique, ...
    if ( FAILED ( p_D3D->GetAdapterDisplayMode( D3DCaps.AdapterOrdinal,
                                                &D3DAdapterMode ) ) )
    {
        return false;
    }

    D3DDISPLAYMODE D3DDisplayMode;

    // ... ainsi que celles du mode courant, ...
    if ( FAILED ( p_D3DDevice->GetDisplayMode( 0, &D3DDisplayMode ) ) )
    {
        return false;
    }

    // ... pour tester que le post-pixel processing soit pris en charge.
    if( FAILED( p_D3D->CheckDeviceFormat
                  ( D3DCaps.AdapterOrdinal,
                    D3DCaps.DeviceType,
                    D3DAdapterMode.Format,
                    D3DUSAGE_RENDERTARGET |
                        D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                    D3DRTYPE_SURFACE,
                    D3DDisplayMode.Format ) ) )
    {
        return false;
    }

    // On initialise les opérations à appliquer sur les textures.
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    // Pour le rendu 2D, on utilise aucun filtre sur les textures.
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );

    // Enfin, on initialise les options de rendu de Direct3D.
    p_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, FALSE );
    p_D3DDevice->SetRenderState( D3DRS_ZENABLE,      FALSE );
    p_D3DDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );

    // Pour finir, on affiche la fenêtre.
    ShowWindow( hWnd, nFunsterStil );

    p_Audio->Play( p_MusicPerformance );

    return true;
}

// Cette fonction initialise l'application.
bool Application::Initialize( HINSTANCE hInstance, int nFunsterStil )
{
    // On crée d'abord la fenêtre de l'application.
    if ( CreateFrame( hInstance ) == false )
    {
        ShowError( "Erreur lors de la création des ressources." );
        return false;
    }

    // On initialise ensuite Direct3D.
    if ( InitD3D() == false )
    {
        ShowError( "Erreur lors de l'initialisation de Direct3D." );
        return false;
    }

    // Puis, on initialise DirectMusic.
    if ( InitDMusic() == false )
    {
        ShowError( "Erreur lors de l'initialisation de DirectMusic." );
        return false;
    }

    // On appelle ensuite la fonction d'initialisation de l'application.
    if ( OnInitialize() == false )
    {
        ShowError( "Erreur lors de l'initialisation de l'application." );
        return false;
    }

    // Puis, on appelle la fonction de post-initialisation.
    if ( OnPostInitialize( nFunsterStil ) == false )
    {
        ShowError( "Erreur lors de l'initialisation de la fenêtre." );
        return false;
    }

    return true;
}

// Cette fonction est appelée lorsque la scène doit être dessinée.
VOID Application::Render()
{
    // On teste d'abord si l'objet Direct3D exite.
    if( p_D3DDevice == NULL )
    {
        return;
    }

    // On effectue ici le nettoyage de la surface de construction,
    // ainsi que le nettoyage du Z-Buffer.
    p_D3DDevice->Clear( 0,
                        NULL,
                        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        D3DCOLOR_XRGB( 0, 0, 0 ),
                        1.0f,
                        0 );

    // Ici, on demande à Direct3D le droit de construire la scène.
    if( SUCCEEDED( p_D3DDevice->BeginScene() ) )
    {
        // Si l'opération a réussi, on appelle la fonction de rendu.
        OnRender();

        // Puis, on indique à Direct3D que la scène est construite.
        p_D3DDevice->EndScene();
    }

    // Enfin, on affiche la scène, en échangeant la surface de construction
    // et la surface de rendu.
    p_D3DDevice->Present( NULL, NULL, NULL, NULL );
}

// Cette fonction est appelée lorsque la scène est en cours de dessin.
VOID Application::OnRender()
{
    // On commence par placer le port de vue dans le monde 3D.
    p_Camera->SetCamera( p_D3DDevice );

    // Puis, on commande l'affichage du paysage.
    p_Landscape->Render( p_D3DDevice );
}

// Cette fonction est appelée lorsqu'une nouvelle action est effectuée.
void Application::OnMove()
{
    // On gère les mouvements du paysage.
    p_Landscape->Scroll();

    // Enfin, on commande un rendu de la scène.
    Render();
}

// Cette fonction est la fonction principale de l'application.
int Application::Run()
{
    // On appelle ici le moteur de messages de Windows.
    while( Messages.message != WM_QUIT )
    {
        // Si un message est arrivé, on le traite.
        PeekMessage( &Messages, NULL, 0U, 0U, PM_REMOVE );
        TranslateMessage( &Messages );
        DispatchMessage( &Messages );

        // Dès que le processeur a du temps de libre, on exécute le moteur
        // d'actions de l'application.
        OnMove();
    }

    return Messages.wParam;
}

// Cette fonction permet d'obtenir l'instance de l'application.
Application* Application::GetInstance()
{
    // Teste si l'instance de l'application existe.
    if ( p_Application == NULL )
    {
        // Si non, on en crée une.
        p_Application = new Application();
    }

    // Retourne l'instance de l'application.
    return p_Application;
}

// Cette fonction permet de détruire l'instance de l'application.
void Application::ReleaseInstance()
{
    if ( p_Application != NULL )
    {
        delete p_Application;
        p_Application = NULL;
    }
}
