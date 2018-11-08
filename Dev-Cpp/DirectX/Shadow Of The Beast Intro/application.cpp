/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe repr�sente l'infrastructure de l'application.  *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "application.h"

// Pointeur vers la classe application.
Application* Application::p_Application = NULL;

// Proc�dure Windows, n�cessaire � toutes applications Windows.
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
    p_WindowName = "Shadow of the Beast D�mo";
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
    // D�truit l'objet cam�ra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // D�truit l'objet Landscape.
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

    // D�truit le device Direct3D utilis� par cette application.
    if( p_D3DDevice != NULL )
    {
        p_D3DDevice->Release();
        p_D3DDevice = NULL;
    }

    // D�truit l'objet Direct3D utilis� par cette application.
    if( p_D3D != NULL )
    {
        p_D3D->Release();
        p_D3D = NULL;
    }

    // D�salloue les ressources utilis�es par la fen�tre.
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

// Cette fonction permet de cr�er et d'initialiser la fen�tre.
bool Application::CreateFrame( HINSTANCE hInstance )
{
    // On cr�e ici les propri�t�s de la fen�tre.
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
    // On cr�e ici une instance de la classe Direct3D9.
    p_D3D = Direct3DCreate9( D3D_SDK_VERSION );

    if( p_D3D == NULL )
    {
        return false;
    }

    // On initialise ensuite les param�tres du device Direct3D.
    D3DPRESENT_PARAMETERS D3Dpp;
    ZeroMemory( &D3Dpp, sizeof( D3Dpp ) );

    D3Dpp.Windowed               = TRUE;
    D3Dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
    D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
    D3Dpp.EnableAutoDepthStencil = TRUE;
    D3Dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    D3Dpp.hDeviceWindow          = hWnd;

    // Puis, on tente la cr�ation de l'objet Device.
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

// Cette fonction est appel�e lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    RECT theRect;
    GetClientRect( hWnd, &theRect );

    // On cr�e ici l'objet Cam�ra. Cet objet est le port de vue.
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

// Cette fonction est appel�e � la fin de l'initialisation de l'application.
bool Application::OnPostInitialize( int nFunsterStil )
{
    D3DCAPS9 D3DCaps;

    // On obtient ici les capacit�s support�es par la carte graphique.
    p_D3DDevice->GetDeviceCaps( &D3DCaps );

    // Puis, teste si le device Direct3D supporte le format FOGVERTEX, ...
    if( ( D3DCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX ) == 0 )
    {
        return false;
    }

    D3DDISPLAYMODE D3DAdapterMode;

    // ... et on obtient les donn�es du mode d'affichage support� par la
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

    // On initialise les op�rations � appliquer sur les textures.
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

    // Pour finir, on affiche la fen�tre.
    ShowWindow( hWnd, nFunsterStil );

    p_Audio->Play( p_MusicPerformance );

    return true;
}

// Cette fonction initialise l'application.
bool Application::Initialize( HINSTANCE hInstance, int nFunsterStil )
{
    // On cr�e d'abord la fen�tre de l'application.
    if ( CreateFrame( hInstance ) == false )
    {
        ShowError( "Erreur lors de la cr�ation des ressources." );
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
        ShowError( "Erreur lors de l'initialisation de la fen�tre." );
        return false;
    }

    return true;
}

// Cette fonction est appel�e lorsque la sc�ne doit �tre dessin�e.
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

    // Ici, on demande � Direct3D le droit de construire la sc�ne.
    if( SUCCEEDED( p_D3DDevice->BeginScene() ) )
    {
        // Si l'op�ration a r�ussi, on appelle la fonction de rendu.
        OnRender();

        // Puis, on indique � Direct3D que la sc�ne est construite.
        p_D3DDevice->EndScene();
    }

    // Enfin, on affiche la sc�ne, en �changeant la surface de construction
    // et la surface de rendu.
    p_D3DDevice->Present( NULL, NULL, NULL, NULL );
}

// Cette fonction est appel�e lorsque la sc�ne est en cours de dessin.
VOID Application::OnRender()
{
    // On commence par placer le port de vue dans le monde 3D.
    p_Camera->SetCamera( p_D3DDevice );

    // Puis, on commande l'affichage du paysage.
    p_Landscape->Render( p_D3DDevice );
}

// Cette fonction est appel�e lorsqu'une nouvelle action est effectu�e.
void Application::OnMove()
{
    // On g�re les mouvements du paysage.
    p_Landscape->Scroll();

    // Enfin, on commande un rendu de la sc�ne.
    Render();
}

// Cette fonction est la fonction principale de l'application.
int Application::Run()
{
    // On appelle ici le moteur de messages de Windows.
    while( Messages.message != WM_QUIT )
    {
        // Si un message est arriv�, on le traite.
        PeekMessage( &Messages, NULL, 0U, 0U, PM_REMOVE );
        TranslateMessage( &Messages );
        DispatchMessage( &Messages );

        // D�s que le processeur a du temps de libre, on ex�cute le moteur
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
        // Si non, on en cr�e une.
        p_Application = new Application();
    }

    // Retourne l'instance de l'application.
    return p_Application;
}

// Cette fonction permet de d�truire l'instance de l'application.
void Application::ReleaseInstance()
{
    if ( p_Application != NULL )
    {
        delete p_Application;
        p_Application = NULL;
    }
}
