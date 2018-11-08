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
        // Le syst�me envoie WM_PAINT lorsque la fen�tre doit �tre redessin�e.
        case WM_PAINT:
        {
            // On ex�cute la fonction de rendu, ...
            Application::GetInstance()->Render();

            // ... puis, on valide la zone d'affichage de l'application.
            ValidateRect( hWnd, NULL );
            return 0;
        }

        // Le syst�me envoie WM_DESTROY lorsque l'application doit �tre quitt�e.
        case WM_DESTROY:
        {
            // On envoie le message de fermeture � l'application.
            PostQuitMessage( 0 );
            break;
        }

        // Par d�faut, on appelle la fonction DefWindowProc.
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
    // D�salloue les ressources de l'objet cam�ra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // D�salloue les ressources du personnage.
    if ( p_AnimMesh != NULL )
    {
        delete p_AnimMesh;
        p_AnimMesh = NULL;
    }

    // D�salloue les ressources de l'arme du personnage.
    if ( p_Weapon != NULL )
    {
        delete p_Weapon;
        p_Weapon = NULL;
    }

    // D�salloue les ressources du clavier.
    if ( p_Keyboard != NULL )
    {
        p_Keyboard->Unacquire();
        p_Keyboard->Release();
        p_Keyboard = NULL;
    }

    // D�salloue l'objet DirectInput utilis� par cette application.
    if ( p_DI != NULL )
    {
         p_DI->Release();
         p_DI = NULL;
     }

    // D�salloue le device Direct3D utilis� par cette application.
    if( p_D3DDevice != NULL )
    {
        p_D3DDevice->Release();
        p_D3DDevice = NULL;
    }

    // D�salloue l'objet Direct3D utilis� par cette application.
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
    // La fonction MessageBox de l'API Windows permet d'afficher une bo�te
    // de dialogue simple, avec un texte, pour faciliter l'affichage des
    // messages d'information de l'application.
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

    // On teste si l'objet Direct3D est bien cr�e en m�moire.
    if( p_D3D == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
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

    // Puis, on tente la cr�ation de l'objet Device.
    if( FAILED( p_D3D->CreateDevice( D3DADAPTER_DEFAULT,
                                     D3DDEVTYPE_HAL,
                                     hWnd,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                     &D3Dpp,
                                     &p_D3DDevice ) ) )
    {
        // Si la fonction �choue, on retourne une erreur.
        return false;
    }

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction initialise DirectInput.
bool Application::InitDInput()
{
    // On cr�e d'abord les param�tres d'acquision du clavier.
    DWORD CoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND;

    // On cr�e ici une instance de la classe DirectInput8.
    if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),
                                    DIRECTINPUT_VERSION, 
                                    IID_IDirectInput8,
                                    (VOID**)&p_DI,
                                    NULL ) ) )
    {
        // Si la fonction �choue, on retourne une erreur.
        return false;
    }

    // Puis, on tente la cr�ation du Device.
    if( FAILED( p_DI->CreateDevice( GUID_SysKeyboard, &p_Keyboard, NULL ) ) )
    {
        // Si la fonction �choue, on retourne une erreur.
        return false;
    }

    // Ensuite, on initialise le buffer de r�ception pour les donn�es
    // du clavier.
    if( FAILED( p_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        // Si la fonction �choue, on retourne une erreur.
        return false;
    }

    // On d�termine ici le niveau de coop�ration de DirectInput.
    if ( FAILED ( p_Keyboard->SetCooperativeLevel( hWnd, CoopFlags ) ) )
    {
        // Si la fonction �choue, on retourne une erreur.
        return false;
    }

    // Puis, on tente l'acquision du clavier.
    p_Keyboard->Acquire();

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction est appel�e lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    // On cr�e ici l'objet Cam�ra. Cet objet repr�sente aussi le port de vue.
    p_Camera = new E_Camera( p_D3DDevice );

    // On teste ensuite que l'objet cam�ra soit bien cr�e en m�moire.
    if ( p_Camera == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On param�tre les points de coupe proche et lointain de la cam�ra.
    p_Camera->SetClipNear( 1.0f );
    p_Camera->SetClipFar( 1000.0f );

    // Puis, on cr�e l'objet personnage.
    p_AnimMesh = new E_MD2Mesh();

    // On teste que l'objet personnage soit bien cr�e en m�moire.
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

    // On param�tre ensuite l'animation courante du personnage.
    p_AnimMesh->SetAnimationParameters( AnimValues[0] );

    // Puis, on cr�e l'objet arme.
    p_Weapon = new E_MD2Mesh();

    // On teste que l'objet arme soit bien cr�e en m�moire.
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

    // On param�tre ensuite l'animation courante de l'arme.
    p_Weapon->SetAnimationParameters( AnimValues[0] );

    // On cr�e les valeurs de l'�clairage pr�-calcul�.
    p_PreCalcLight.Light.r = 255;
    p_PreCalcLight.Light.g = 255;
    p_PreCalcLight.Light.b = 255;
    p_PreCalcLight.Ambient.r = 0;
    p_PreCalcLight.Ambient.g = 0;
    p_PreCalcLight.Ambient.b = 255;

    // Puis, on cr�e les valeurs de l'�clairage Direct3D.
    ZeroMemory( &p_D3DLight, sizeof(D3DLIGHT9) );
    p_D3DLight.Type      = D3DLIGHT_DIRECTIONAL;
    p_D3DLight.Diffuse.r = 1.0f;
    p_D3DLight.Diffuse.g = 1.0f;
    p_D3DLight.Diffuse.b = 1.0f;
    p_D3DLight.Range     = 1000.0f;

    // Pour finir, on retourne le message de r�ussite.
    return true;
}

// Cette fonction est appel�e � la fin de l'initialisation de l'application.
bool Application::OnPostInitialize( int nFunsterStil )
{
    // On initialise les op�rations � appliquer sur les textures.
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    p_D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    // Puis,on initialise les filtres � appliquer sur les textures.
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

    // Ensuite, on initialise les options de rendu de Direct3D.
    p_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    p_D3DDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    p_D3DDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );

    // Pour finir, on affiche la fen�tre, ...
    ShowWindow( hWnd, nFunsterStil );

    // ... et on retourne un message de r�ussite.
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
        ShowError( "Erreur lors de l'initialisation de la fen�tre." );
        return false;
    }

    // Enfin, on initialise DirectInput.
    if ( InitDInput() == false )
    {
        ShowError( "Erreur lors de l'initialisation de DirectInput." );
        return false;
    }

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction est appel�e lorsque la sc�ne doit �tre dessin�e.
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

    // Ensuite, on commande le rendu des objets anim�s.
    p_AnimMesh->Render( p_D3DDevice );
    p_Weapon->Render( p_D3DDevice );
}

// Effectue une action en fonction de la touche press�e sur le clavier.
bool Application::ReadKeyboard()
{
    BYTE Diks[256];

    TCHAR strNewText[256*5+1] = TEXT( "" );
    TCHAR strElement[10];

    // On teste d'abord si le Device clavier existe.
    if( p_Keyboard == NULL )
    {
        // Si ce n'est pas le cas, le clavier ne peut pas �tre lu.
        return false;
    }

    // Si oui, on pr�pare la m�moire de r�ception des touches.
    ZeroMemory( Diks, sizeof( Diks ) );

    // Puis, on tente une lecture du buffer clavier.
    HRESULT hr = p_Keyboard->GetDeviceState( sizeof( Diks ), Diks );

    // On teste si l'op�ration a r�ussi.
    if ( hr != DI_OK )
    {
        // Si non, l'application a peut-�tre perdu le focus. Dans ce cas,
        // on attend que le focus soit � nouveau attribu� ...
        while( p_Keyboard->Acquire() == DIERR_INPUTLOST )
        {
        }

        // ... et on retourne un message pour indiquer que le clavier n'a pas
        // �t� lu cette fois-ci. A la prochaine lecture, le Device devrait �tre
        // disponible.
        return false;
    }

    // On teste si la touche O a �t� press�e.
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

    // On teste si la touche P a �t� press�e.
    if( KEYDOWN( Diks, DIK_P ) )
    {
        // On passe au set d'animation pr�c�dent.
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

    // On teste si la touche 1 a �t� press�e.
    if( KEYDOWN( Diks, DIK_1 ) )
    {
        // Aucun �clairage ne doit �tre rendu.
        p_LightningMode = E_MD2_LIGHTNING_NONE;

        // On param�tre les objets � E_MD2_LIGHTNING_NONE.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // Puis, on d�sactive le moteur de lumi�res de Direct3D.
        p_D3DDevice->LightEnable( 0, FALSE );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    }


    // On teste si la touche 2 a �t� press�e.
    if( KEYDOWN( Diks, DIK_2 ) )
    {
        // On demande un �clairage pr�-calcul�.
        p_LightningMode = E_MD2_LIGHTNING_PRECALCULATED;

        // La lumi�re directionnelle a besoin d'une direction pour fonctionner.
        // On cr�e ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // Puis, on normalise ce vecteur, et on inscrit le r�sultat.
        D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction, &VecDir );

        // On param�tre les objets � E_MD2_LIGHTNING_PRECALCULATED.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // Puis, on indique aux objets les param�tres de la lumi�re.
        p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
        p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );

        // Enfin, on s'assure que le moteur de lumi�res de Direct3D soit bien
        // d�sactiv�.
        p_D3DDevice->LightEnable( 0, FALSE );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    }

    // On teste si la touche 3 a �t� press�e.
    if( KEYDOWN( Diks, DIK_3 ) )
    {
        // On demande un �clairage g�r� par Direct3D.
        p_LightningMode = E_MD2_LIGHTNING_DIRECT3D;

        // La lumi�re directionnelle a besoin d'une direction pour fonctionner.
        // On cr�e ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // Puis, on normalise cette direction, et on inscrit la valeur.
        D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

        // On param�tre les objets � E_MD2_LIGHTNING_DIRECT3D.
        p_AnimMesh->SetLightningMode( p_LightningMode );
        p_Weapon->SetLightningMode( p_LightningMode );

        // On indique ensuite les param�tres de la lumi�re � Direct3D.
        p_D3DDevice->SetLight   ( 0, &p_D3DLight );
        p_D3DDevice->LightEnable( 0, TRUE );

        // Pour finir, on configure la lumi�re d'ambiance avec une couleur
        // rouge, et on d�marre le moteur de lumi�res de Direct3D.
        p_D3DDevice->SetRenderState( D3DRS_AMBIENT,  0xFF0000 );
        p_D3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    }

    // On teste si la touche fl�ch�e haut a �t� press�e.
    if( KEYDOWN( Diks, DIK_UP ) )
    {
        // On incr�mente la direction de la lumi�re.
        p_CurLightDir += 0.1f;

        // Puis, on teste si la nouvelle valeur est correcte.
        if ( p_CurLightDir >= ( D3DX_PI * 2 ) )
        {
            // Si ce n'est pas le cas, on corrige la valeur.
            p_CurLightDir = ( D3DX_PI * 2 );
        }

        // La lumi�re directionnelle a besoin d'une direction pour fonctionner.
        // On cr�e ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // On teste ensuite quel mode de lumi�re est actuellement en service.
        if ( p_LightningMode == E_MD2_LIGHTNING_PRECALCULATED )
        {
            // Si c'est la lumi�re pr�-calcul�e, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction,
                               &VecDir );

            // On configure ensuite les objets avec la nouvelle valeur.
            p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
            p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );
        }
        else if ( p_LightningMode == E_MD2_LIGHTNING_DIRECT3D )
        {
            // Si c'est la lumi�re Direct3D, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

            // On configure ensuite Direct3D avec la nouvelle valeur.
            p_D3DDevice->SetLight   ( 0, &p_D3DLight );
            p_D3DDevice->LightEnable( 0, TRUE );
        }
    }

    // On teste si la touche fl�ch�e bas a �t� press�e.
    if( KEYDOWN( Diks, DIK_DOWN ) )
    {
        // On d�cr�mente la direction de la lumi�re.
        p_CurLightDir -= 0.1f;

        // Puis, on teste si la nouvelle valeur est correcte.
        if ( p_CurLightDir <= 0.0f )
        {
            // Si ce n'est pas le cas, on corrige la valeur.
            p_CurLightDir = 0.0f;
        }

        // La lumi�re directionnelle a besoin d'une direction pour fonctionner.
        // On cr�e ici le vecteur de direction.
        D3DXVECTOR3 VecDir = D3DXVECTOR3( sinf( p_CurLightDir ),
                                          0.0f,
                                          cosf( p_CurLightDir ) );

        // On teste ensuite quel mode de lumi�re est actuellement en service.
        if ( p_LightningMode == E_MD2_LIGHTNING_PRECALCULATED )
        {
            // Si c'est la lumi�re pr�-calcul�e, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_PreCalcLight.Direction,
                               &VecDir );

            // On configure ensuite les objets avec la nouvelle valeur.
            p_AnimMesh->SetPreCalculatedLightParameters( p_PreCalcLight );
            p_Weapon->SetPreCalculatedLightParameters( p_PreCalcLight );
        }
        else if ( p_LightningMode == E_MD2_LIGHTNING_DIRECT3D )
        {
            // Si c'est la lumi�re Direct3D, on normalise la nouvelle
            // valeur de direction, puis on enregistre celle-ci.
            D3DXVec3Normalize( (D3DXVECTOR3*)&p_D3DLight.Direction, &VecDir );

            // On configure ensuite Direct3D avec la nouvelle valeur.
            p_D3DDevice->SetLight   ( 0, &p_D3DLight );
            p_D3DDevice->LightEnable( 0, TRUE );
        }
    }

    // On teste si la touche fl�ch�e gauche a �t� press�e.
    if( KEYDOWN( Diks, DIK_LEFT ) )
    {
        // On configure la nouvelle position dans les objets.
        p_AnimMesh->SetRotationZ( p_AnimMesh->GetRotationZ() - 0.1f );
        p_Weapon->SetRotationZ( p_Weapon->GetRotationZ() - 0.1f );
    }

    // On teste si la touche fl�ch�e droite a �t� press�e.
    if( KEYDOWN( Diks, DIK_RIGHT ) )
    {
        // On configure la nouvelle position dans les objets.
        p_AnimMesh->SetRotationZ( p_AnimMesh->GetRotationZ() + 0.1f );
        p_Weapon->SetRotationZ( p_Weapon->GetRotationZ() + 0.1f );
    }

    // On teste si la touche press�e est la touche Escape.
    if( KEYDOWN( Diks, DIK_ESCAPE ) )
    {
        // Si oui, on envoie un message de fermeture au syst�me.
        SendMessage( hWnd, WM_CLOSE, 0, 0 );
    }

    return true;
}

// Cette fonction est appel�e lorsqu'une nouvelle action est effectu�e.
void Application::OnMove()
{
    // On lit ici l'�tat des entr�es du clavier.
    ReadKeyboard();

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
