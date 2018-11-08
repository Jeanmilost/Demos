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
        // Le système envoie WM_PAINT lorsque la fenêtre doit être redessinée.
        case WM_PAINT:
        {
            // On exécute la fonction de rendu, ...
            Application::GetInstance()->Render();

            // ... puis, on valide la zone d'affichage de l'application.
            ValidateRect( hWnd, NULL );
            return 0;
        }

        // Le système envoie WM_DESTROY lorsque l'application doit être quittée.
        case WM_DESTROY:
        {
            // On envoie le message de fermeture à l'application.
            PostQuitMessage( 0 );
            break;
        }

        // Par défaut, on appelle la fonction DefWindowProc.
        default:
        {
            return DefWindowProc( hWnd, Message, wParam, lParam );
        }
    }

    return 0;
}

// Constructeur de la classe Application.
Application::Application()
{
    p_WindowName     = "Animation MD2";
    p_ClassName      = "Application";

    p_WinWidth       = 600;
    p_WinHeight      = 600;

    p_D3D            = NULL;
    p_D3DDevice      = NULL;
    p_DI             = NULL;
    p_Keyboard       = NULL;

    p_CurAnim        = 0;

    p_CurLightDir    = 0.0f;

    p_LightningMode  = E_MD2_LIGHTNING_NONE;

    p_Camera         = NULL;
    p_AnimMesh       = NULL;
    p_Weapon         = NULL;
}

// Destructeur de la classe Application.
Application::~Application()
{
    // Désalloue les ressources de l'objet caméra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // Désalloue les ressources du personnage.
    if ( p_AnimMesh != NULL )
    {
        delete p_AnimMesh;
        p_AnimMesh = NULL;
    }

    // Désalloue les ressources de l'arme du personnage.
    if ( p_Weapon != NULL )
    {
        delete p_Weapon;
        p_Weapon = NULL;
    }

    // Désalloue les ressources du clavier.
    if ( p_Keyboard != NULL )
    {
        p_Keyboard->Unacquire();
        p_Keyboard->Release();
        p_Keyboard = NULL;
    }

    // Désalloue l'objet DirectInput utilisé par cette application.
    if ( p_DI != NULL )
    {
         p_DI->Release();
         p_DI = NULL;
     }

    // Désalloue le device Direct3D utilisé par cette application.
    if( p_D3DDevice != NULL )
    {
        p_D3DDevice->Release();
        p_D3DDevice = NULL;
    }

    // Désalloue l'objet Direct3D utilisé par cette application.
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
    // La fonction MessageBox de l'API Windows permet d'afficher une boîte
    // de dialogue simple, avec un texte, pour faciliter l'affichage des
    // messages d'information de l'application.
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

    // On teste si l'objet Direct3D est bien crée en mémoire.
    if( p_D3D == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
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

    // Puis, on tente la création de l'objet Device.
    if( FAILED( p_D3D->CreateDevice( D3DADAPTER_DEFAULT,
                                     D3DDEVTYPE_HAL,
                                     hWnd,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                     &D3Dpp,
                                     &p_D3DDevice ) ) )
    {
        // Si la fonction échoue, on retourne une erreur.
        return false;
    }

    // Pour finir, on retourne un message de réussite.
    return true;
}

// Cette fonction initialise DirectInput.
bool Application::InitDInput()
{
    // On crée d'abord les paramètres d'acquision du clavier.
    DWORD CoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND;

    // On crée ici une instance de la classe DirectInput8.
    if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),
                                    DIRECTINPUT_VERSION, 
                                    IID_IDirectInput8,
                                    (VOID**)&p_DI,
                                    NULL ) ) )
    {
        // Si la fonction échoue, on retourne une erreur.
        return false;
    }

    // Puis, on tente la création du Device.
    if( FAILED( p_DI->CreateDevice( GUID_SysKeyboard, &p_Keyboard, NULL ) ) )
    {
        // Si la fonction échoue, on retourne une erreur.
        return false;
    }

    // Ensuite, on initialise le buffer de réception pour les données
    // du clavier.
    if( FAILED( p_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        // Si la fonction échoue, on retourne une erreur.
        return false;
    }

    // On détermine ici le niveau de coopération de DirectInput.
    if ( FAILED ( p_Keyboard->SetCooperativeLevel( hWnd, CoopFlags ) ) )
    {
        // Si la fonction échoue, on retourne une erreur.
        return false;
    }

    // Puis, on tente l'acquision du clavier.
    p_Keyboard->Acquire();

    // Pour finir, on retourne un message de réussite.
    return true;
}

// Cette fonction est appelée lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    // On crée ici l'objet Caméra. Cet objet représente aussi le port de vue.
    p_Camera = new E_Camera( p_D3DDevice );

    // On teste ensuite que l'objet caméra soit bien crée en mémoire.
    if ( p_Camera == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On paramètre les points de coupe proche et lointain de la caméra.
    p_Camera->SetClipNear( 1.0f );
    p_Camera->SetClipFar( 1000.0f );

    // Puis, on crée l'objet personnage.
    p_AnimMesh = new E_MD2Mesh();

    // On teste que l'objet personnage soit bien crée en mémoire.
    if ( p_AnimMesh == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On configure ensuite l'objet personnage.
    p_AnimMesh->SetModelFilename( ".\\Models\\Ogros.md2" );
    p_AnimMesh->SetTextureFilename( ".\\Models\\igdosh.jpg" );
    p_AnimMesh->SetRotationX( -( (float)D3DX_PI / 2.0f ) );
    p_AnimMesh->SetRotationZ( -( (float)D3DX_PI / 2.0f ) );
    p_AnimMesh->SetTranslationZ( 100.0f );

    // Puis, on tente d'initialiser l'objet personnage.
    if ( p_AnimMesh->Initialize( p_D3DDevice ) == false )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On paramètre ensuite l'animation courante du personnage.
    p_AnimMesh->SetAnimationParameters( AnimValues[0] );

    // Puis, on crée l'objet arme.
    p_Weapon = new E_MD2Mesh();

    // On teste que l'objet arme soit bien crée en mémoire.
    if ( p_AnimMesh == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On configure ensuite l'objet arme.
    p_Weapon->SetModelFilename( ".\\Models\\Weapon.md2" );
    p_Weapon->SetTextureFilename( ".\\Models\\Weapon.jpg" );
    p_Weapon->SetRotationX( -( (float)D3DX_PI / 2.0f ) );
    p_Weapon->SetRotationZ( -( (float)D3DX_PI / 2.0f ) );
    p_Weapon->SetTranslationZ( 100.0f );

    // Puis, on tente d'initialiser l'objet arme.
    if ( p_Weapon->Initialize( p_D3DDevice ) == false )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On paramètre ensuite l'animation courante de l'arme.
    p_Weapon->SetAnimationParameters( AnimValues[0] );

    // On crée les valeurs de l'éclairage pré-calculé.
    p_PreCalcLight.Light.r = 255;
    p_PreCalcLight.Light.g = 255;
    p_PreCalcLight.Light.b = 255;
    p_PreCalcLight.Ambient.r = 0;
    p_PreCalcLight.Ambient.g = 0;
    p_PreCalcLight.Ambient.b = 255;

    // Puis, on crée les valeurs de l'éclairage Direct3D.
    ZeroMemory( &p_D3DLight, sizeof(D3DLIGHT9) );
    p_D3DLight.Type      = D3DLIGHT_DIRECTIONAL;
    p_D3DLight.Diffuse.r = 1.0f;
    p_D3DLight.Diffuse.g = 1.0f;
    p_D3DLight.Diffuse.b = 1.0f;
    p_D3DLight.Range     = 1000.0f;

    // Pour finir, on retourne le message de réussite.
    return true;
}

// Cette fonction est appelée à la fin de l'initialisation de l'application.
bool Application::OnPostInitialize( int nFunsterStil )
{
    // On initialise les opérations à appliquer sur les textures.
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    // Puis,on initialise les filtres à appliquer sur les textures.
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

    // Ensuite, on initialise les options de rendu de Direct3D.
    p_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    p_D3DDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    p_D3DDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );

    // Pour finir, on affiche la fenêtre, ...
    ShowWindow( hWnd, nFunsterStil );

    // ... et on retourne un message de réussite.
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

    // Puis, on initialise Direct3D.
    if ( InitD3D() == false )
    {
        ShowError( "Erreur lors de l'initialisation de Direct3D." );
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

    // Enfin, on initialise DirectInput.
    if ( InitDInput() == false )
    {
        ShowError( "Erreur lors de l'initialisation de DirectInput." );
        return false;
    }

    // Si tout s'est bien passé, on retourne le message de réussite.
    return true;
}

// Cette fonction est appelée lorsque la scène doit être dessinée.
VOID Application::Render()
{
    // On teste d'abord si l'objet Direct3D exite.
    if( p_D3DDevice == NULL )
    {
        // Si ce n'est pas le cas, le rendu n'est pas possible.
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

    // Ensuite, on commande le rendu des objets animés.
    p_AnimMesh->Render( p_D3DDevice );
    p_Weapon->Render( p_D3DDevice );
}

// Effectue une action en fonction de la touche pressée sur le clavier.
bool Application::ReadKeyboard()
{
    BYTE Diks[256];

    TCHAR strNewText[256*5+1] = TEXT( "" );
    TCHAR strElement[10];

    // On teste d'abord si le Device clavier existe.
    if( p_Keyboard == NULL )
    {
        // Si ce n'est pas le cas, le clavier ne peut pas être lu.
        return false;
    }

    // Si oui, on prépare la mémoire de réception des touches.
    ZeroMemory( Diks, sizeof( Diks ) );

    // Puis, on tente une lecture du buffer clavier.
    HRESULT hr = p_Keyboard->GetDeviceState( sizeof( Diks ), Diks );

    // On teste si l'opération a réussi.
    if ( hr != DI_OK )
    {
        // Si non, l'application a peut-être perdu le focus. Dans ce cas,
        // on attend que le focus soit à nouveau attribué ...
        while( p_Keyboard->Acquire() == DIERR_INPUTLOST )
        {
        }

        // ... et on retourne un message pour indiquer que le clavier n'a pas
        // été lu cette fois-ci. A la prochaine lecture, le Device devrait être
        // disponible.
        return false;
    }

    // On teste si la touche O a été pressée.
    if( KEYDOWN( Diks, DIK_O ) )
    {
        // On passe au set d'animation suivant.
        p_CurAnim++;

        // On teste ensuite que la valeur du set ne soit pas hors des limites.
        if ( p_CurAnim >= 21 )
        {
            // Si c'est le cas, on corrige la valeur.
            p_CurAnim = 0;
        }

        // On configure ensuite les objets avec le nouveau set d'animation.
        p_AnimMesh->SetAnimationParameters( AnimValues[p_CurAnim] );
        p_Weapon->SetAnimationParameters  ( AnimValues[p_CurAnim] );
    }

    // On teste si la touche P a été pressée.
    if( KEYDOWN( Diks, DIK_P ) )
    {
        // On passe au set d'animation précédent.
        p_CurAnim--;

        // On teste ensuite que la valeur du set ne soit pas hors des limites.
        if ( p_CurAnim <= 0 )
        {
            // Si c'est le cas, on corrige la valeur.
            p_CurAnim = 20;
        }

        // On configure ensuite les objets avec le nouveau set d'animation.
        p_AnimMesh->SetAnimationParameters( AnimValues[p_CurAnim] );
        p_Weapon->SetAnimationParameters  ( AnimValues[p_CurAnim] );
    }

    // On teste si la touche 1 a été pressée.
    if( KEYDOWN( Diks, DIK_1 ) )
    {
        // Aucun éclairage ne doit être rendu.
        p_LightningMode = E_MD2_LIGHTNING_NONE;

        // On paramètre les objets à E_MD2_LIGHTNING_NONE.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // Puis, on désactive le moteur de lumières de Direct3D.
        p_D3DDevice->LightEnable( 0, FALSE );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    }


    // On teste si la touche 2 a été pressée.
    if( KEYDOWN( Diks, DIK_2 ) )
    {
        // On demande un éclairage pré-calculé.
        p_LightningMode = E_MD2_LIGHTNING_PRECALCULATED;

        // La lumière directionnelle a besoin d'une direction pour fonctionner.
        // On crée ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // Puis, on normalise ce vecteur, et on inscrit le résultat.
        D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction, &VecDir );

        // On paramètre les objets à E_MD2_LIGHTNING_PRECALCULATED.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // Puis, on indique aux objets les paramètres de la lumière.
        p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
        p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );

        // Enfin, on s'assure que le moteur de lumières de Direct3D soit bien
        // désactivé.
        p_D3DDevice->LightEnable( 0, FALSE );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    }

    // On teste si la touche 3 a été pressée.
    if( KEYDOWN( Diks, DIK_3 ) )
    {
        // On demande un éclairage géré par Direct3D.
        p_LightningMode = E_MD2_LIGHTNING_DIRECT3D;

        // La lumière directionnelle a besoin d'une direction pour fonctionner.
        // On crée ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // Puis, on normalise cette direction, et on inscrit la valeur.
        D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

        // On paramètre les objets à E_MD2_LIGHTNING_DIRECT3D.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // On indique ensuite les paramètres de la lumière à Direct3D.
        p_D3DDevice->SetLight   ( 0, &p_D3DLight );
        p_D3DDevice->LightEnable( 0, TRUE );

        // Pour finir, on configure la lumière d'ambiance avec une couleur
        // rouge, et on démarre le moteur de lumières de Direct3D.
        p_D3DDevice->SetRenderState( D3DRS_AMBIENT,  0xFF0000 );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    }

    // On teste si la touche fléchée haut a été pressée.
    if( KEYDOWN( Diks, DIK_UP ) )
    {
        // On incrémente la direction de la lumière.
        p_CurLightDir += 0.1f;

        // Puis, on teste si la nouvelle valeur est correcte.
        if ( p_CurLightDir >= ( D3DX_PI * 2 ) )
        {
            // Si ce n'est pas le cas, on corrige la valeur.
            p_CurLightDir = ( D3DX_PI * 2 );
        }

        // La lumière directionnelle a besoin d'une direction pour fonctionner.
        // On crée ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // On teste ensuite quel mode de lumière est actuellement en service.
        if ( p_LightningMode == E_MD2_LIGHTNING_PRECALCULATED )
        {
            // Si c'est la lumière pré-calculée, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction,
                               &VecDir );

            // On configure ensuite les objets avec la nouvelle valeur.
            p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
            p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );
        }
        else if ( p_LightningMode == E_MD2_LIGHTNING_DIRECT3D )
        {
            // Si c'est la lumière Direct3D, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

            // On configure ensuite Direct3D avec la nouvelle valeur.
            p_D3DDevice->SetLight   ( 0, &p_D3DLight );
            p_D3DDevice->LightEnable( 0, TRUE );
        }
    }

    // On teste si la touche fléchée bas a été pressée.
    if( KEYDOWN( Diks, DIK_DOWN ) )
    {
        // On décrémente la direction de la lumière.
        p_CurLightDir -= 0.1f;

        // Puis, on teste si la nouvelle valeur est correcte.
        if ( p_CurLightDir <= 0.0f )
        {
            // Si ce n'est pas le cas, on corrige la valeur.
            p_CurLightDir = 0.0f;
        }

        // La lumière directionnelle a besoin d'une direction pour fonctionner.
        // On crée ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // On teste ensuite quel mode de lumière est actuellement en service.
        if ( p_LightningMode == E_MD2_LIGHTNING_PRECALCULATED )
        {
            // Si c'est la lumière pré-calculée, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction,
                               &VecDir );

            // On configure ensuite les objets avec la nouvelle valeur.
            p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
            p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );
        }
        else if ( p_LightningMode == E_MD2_LIGHTNING_DIRECT3D )
        {
            // Si c'est la lumière Direct3D, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

            // On configure ensuite Direct3D avec la nouvelle valeur.
            p_D3DDevice->SetLight   ( 0, &p_D3DLight );
            p_D3DDevice->LightEnable( 0, TRUE );
        }
    }

    // On teste si la touche fléchée gauche a été pressée.
    if( KEYDOWN( Diks, DIK_LEFT ) )
    {
        // On configure la nouvelle position dans les objets.
        p_AnimMesh->SetRotationZ( p_AnimMesh->GetRotationZ() - 0.1f );
        p_Weapon->SetRotationZ( p_Weapon->GetRotationZ() - 0.1f );
    }

    // On teste si la touche fléchée droite a été pressée.
    if( KEYDOWN( Diks, DIK_RIGHT ) )
    {
        // On configure la nouvelle position dans les objets.
        p_AnimMesh->SetRotationZ( p_AnimMesh->GetRotationZ() + 0.1f );
        p_Weapon->SetRotationZ( p_Weapon->GetRotationZ() + 0.1f );
    }

    // On teste si la touche pressée est la touche Escape.
    if( KEYDOWN( Diks, DIK_ESCAPE ) )
    {
        // Si oui, on envoie un message de fermeture au système.
        SendMessage( hWnd, WM_CLOSE, 0, 0 );
    }

    return true;
}

// Cette fonction est appelée lorsqu'une nouvelle action est effectuée.
void Application::OnMove()
{
    // On lit ici l'état des entrées du clavier.
    ReadKeyboard();

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
