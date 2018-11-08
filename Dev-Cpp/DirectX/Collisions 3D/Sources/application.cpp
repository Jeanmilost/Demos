/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente l'infrastructure de l'application.  *
 * Version     : 1.1                                                         *
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
        // On teste si le menu a envoyé une commande.
        case WM_COMMAND:
        {
            // Si oui, on recherche quel entrée a été sélectionnée.
            switch( wParam )
            {
                // L'entrée "Sans collisions" a été sélectionnée.
                case IDM_NOCOLLISIONS:
                {
                    // On met à jour les entrées du menu.
                    // "Sans collisions" est coché.
                    Application::GetInstance()->UpdateMenu
                                                    ( IDM_NOCOLLISIONS, true );

                    // "Collisions uniquement" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                                 ( IDM_COLLISIONSONLY, false );

                    // "Collisions et sliding" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                           ( IDM_COLLISIONSANDSLIDING, false );

                    // Puis, on met à jour le type de moteur de collisions.
                    Application::GetInstance()->SetCollisionType
                                                              ( NO_COLLISION );

                    break;
                }

                // L'entrée "Collisions seulement" a été sélectionnée.
                case IDM_COLLISIONSONLY:
                {
                    // On met à jour les entrées du menu.
                    // "Sans collisions" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                                   ( IDM_NOCOLLISIONS, false );

                    // "Collisions uniquement" est coché.
                    Application::GetInstance()->UpdateMenu
                                                  ( IDM_COLLISIONSONLY, true );

                    // "Collisions et sliding" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                           ( IDM_COLLISIONSANDSLIDING, false );

                    // Puis, on met à jour le type de moteur de collisions.
                    Application::GetInstance()->SetCollisionType
                                                            ( COLLISION_ONLY );

                    break;
                }

                // L'entrée "Collisions et sliding" a été sélectionnée.
                case IDM_COLLISIONSANDSLIDING:
                {
                    // On met à jour les entrées du menu.
                    // "Sans collisions" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                                   ( IDM_NOCOLLISIONS, false );

                    // "Collisions uniquement" n'est pas coché.
                    Application::GetInstance()->UpdateMenu
                                                 ( IDM_COLLISIONSONLY, false );

                    // "Collisions et sliding" est coché.
                    Application::GetInstance()->UpdateMenu
                                            ( IDM_COLLISIONSANDSLIDING, true );

                    // Puis, on met à jour le type de moteur de collisions.
                    Application::GetInstance()->SetCollisionType
                                                     ( COLLISION_AND_SLIDING );

                    break;
                }

                // L'application doit être quittée.
                case IDM_APPEXIT:
                {
                    PostQuitMessage( 0 );
                    break;
                }
            }

            return 0;
        }

        // La fenêtre doit être redessinnée.
        case WM_PAINT:
        {
            Application::GetInstance()->Render();
            ValidateRect( hWnd, NULL );
            return 0;
        }

        // L'application doit être détruite.
        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            return 0;
        }

        // Par défaut, on parse simplement les messages.
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
    p_WindowName     = "Collisions";
    p_ClassName      = "Application";

    p_WinWidth       = 300;
    p_WinHeight      = 300;

    p_D3D            = NULL;
    p_D3DDevice      = NULL;
    p_DI             = NULL;
    p_Keyboard       = NULL;

    p_CurPos         = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_PosVelocity    = 0.5f;
    p_DirVelocity    = 0.1f;
    p_CurDir         = 0.0f;
    p_AvatarHeight   = 2.0f;

    p_CollisionType  = COLLISION_AND_SLIDING;

    // On crée ici le nom et le chemin d'accès des textures.
    p_TextureName[0] = "Graphics\\Floor.jpg";
    p_TextureName[1] = "Graphics\\Wall.jpg";
    p_TextureName[2] = "Graphics\\Roof.jpg";

    p_Camera         = NULL;

    for ( int i = 0; i < NB_WALLS; i++ )
    {
        p_Wall[i]    = NULL;
    }

    for ( int i = 0; i < NB_TEXTURES; i++ )
    {
        p_Texture[i] = NULL;
    }
}

// Destructeur de la classe Application.
Application::~Application()
{
    // Détruit l'objet caméra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // Détruit les objets murs.
    for ( int i = 0; i < NB_WALLS; i++ )
    {
        if ( p_Wall[i] != NULL )
        {
            delete p_Wall[i];
            p_Wall[i] = NULL;
        }
    }

    // Détruit les objets textures.
    for ( int i = 0; i < NB_TEXTURES; i++ )
    {
        if ( p_Texture[i] != NULL )
        {
            delete p_Texture[i];
            p_Texture[i] = NULL;
        }
    }

    // Désalloue les ressources du clavier.
    if ( p_Keyboard != NULL )
    {
        p_Keyboard->Unacquire();
        p_Keyboard->Release();
        p_Keyboard = NULL;
    }

    // Détruit l'objet DirectInput utilisé par cette application.
    if ( p_DI != NULL )
    {
         p_DI->Release();
         p_DI = NULL;
     }

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

// Cette fonction met à jour l'entrée de menu spécifiée.
void Application::UpdateMenu( int MenuID, bool Checked )
{
    // On teste si l'entrée doit être cochée ou pas.
    if ( Checked == true )
    {
        // Si elle doit être cochée, on demande l'ajout de la coche.
        CheckMenuItem( GetMenu( hWnd ),
                       MenuID,
                       MF_CHECKED );
    }
    else
    {
        // Sinon, on demande la suppression de la coche.
        CheckMenuItem( GetMenu( hWnd ),
                       MenuID,
                       MF_UNCHECKED );
    }
}

// Cette fonction permet d'afficher un message d'erreur.
void Application::ShowError( LPSTR ErrorMessage )
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
    WinCl.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
    WinCl.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
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
                           WS_VISIBLE,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           p_WinWidth,
                           p_WinHeight,
                           HWND_DESKTOP,
                           NULL,
                           hInstance,
                           NULL );

    // Ensuite, on ajoute les menus de l'application.
    HMENU Menu = LoadMenu( hInstance, MAKEINTRESOURCE( ID_MENU ) );
    SetMenu( hWnd, Menu );

    // Puis, on met à jour les entrées des menus en fonction des paramètres de
    // départ.
    switch ( p_CollisionType )
    {
        // Au départ, "sans collisions" est sélectionné.
        case NO_COLLISION:
        {
            // On demande la mise à jour du menu.
            UpdateMenu( IDM_NOCOLLISIONS, true );
            break;
        }

        // Au départ, "collisions uniquement" est sélectionné.
        case COLLISION_ONLY:
        {
            // On demande la mise à jour du menu.
            UpdateMenu( IDM_COLLISIONSONLY, true );
            break;
        }

        // Au départ, "collisions et sliding" est sélectionné.
        case COLLISION_AND_SLIDING:
        {
            // On demande la mise à jour du menu.
            UpdateMenu( IDM_COLLISIONSANDSLIDING, true );
            break;
        }

        // Si aucun paramètre ne correspond, il s'agit d'une erreur.
        default:
        {
            // Dans ce cas, on retourne une erreur.
            return false;
        }
    }

    // Si tout s'est bien passé, on retourne un message de réussite.
    return true;
}

// Cette fonction initialise Direct3D.
bool Application::InitD3D()
{
    // On crée ici une instance de la classe Direct3D9.
    p_D3D = Direct3DCreate9( D3D_SDK_VERSION );

    // Puis, on teste si l'instance a été correctement crée en mémoire.
    if( p_D3D == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On initialise ensuite les paramètres du device Direct3D.
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
        // Si la création échoue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien passé, on retourne un message de réussite.
    return true;
}

// Cette fonction initialise DirectInput.
bool Application::InitDInput()
{
    // On crée d'abord les paramètres d'acquision du clavier.
    DWORD CoopFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND;

    // Puis, on tente de créer une instance de la classe DirectInput8.
    if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),
                                    DIRECTINPUT_VERSION, 
                                    IID_IDirectInput8,
                                    (VOID**)&p_DI,
                                    NULL ) ) )
    {
        // Si la création échoue, on retourne une erreur.
        return false;
    }

    // Ensuite, on tente la création du Device.
    if( FAILED( p_DI->CreateDevice( GUID_SysKeyboard, &p_Keyboard, NULL ) ) )
    {
        // Si la création échoue, on retourne une erreur.
        return false;
    }

    // Puis, on initialise le buffer de réception pour les données du clavier.
    if( FAILED( p_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        // Si la création échoue, on retourne une erreur.
        return false;
    }

    // On détermine ici le niveau de coopération de DirectInput.
    if ( FAILED ( p_Keyboard->SetCooperativeLevel( hWnd, CoopFlags ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // Puis, on tente l'acquision du clavier.
    p_Keyboard->Acquire();

    // Pour finir, on retourne le message de réussite.
    return true;
}

// Cette fonction est appelée lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    // On crée ici l'objet Caméra. Cet objet est le port de vue.
    p_Camera = new E_Camera( p_D3DDevice );

    // Ensuite, on teste que l'objet caméra soit bien crée en mémoire.
    if ( p_Camera == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on charge les textures en mémoire.
    for ( int i = 0; i < NB_TEXTURES; i++ )
    {
        // On crée d'abord l'objet texture.
        p_Texture[i] = new E_Texture( p_TextureName[i] );

        // Ensuite, on teste que l'objet texture soit bien crée en mémoire.
        if ( p_Texture[i] == NULL )
        {
            // Si ce n'est pas le cas, on retourne une erreur.
            return false;
        }

        // Puis, on tente d'initialiser l'objet texture.
        if ( p_Texture[i]->Initialize( p_D3DDevice ) == false )
        {
            // Si l'initialisation échoue, on retourne une erreur.
            return false;
        }
    }

    // On crée ensuite les sprites, pour aficher les murs.
    for ( int i = 0; i < NB_WALLS; i++ )
    {
        // On crée d'abord l'objet sprite.
        p_Wall[i] = new E_Sprite();

        // Ensuite, on teste que l'objet sprite soit bien crée en mémoire.
        if ( p_Wall[i] == NULL )
        {
            // Si ce n'est pas le cas, on retourne une erreur.
            return false;
        }

        // Puis, on configure les propriétés des murs.
        p_Wall[i]->SetWidth ( Map[i].Width );
        p_Wall[i]->SetHeight( Map[i].Height );

        p_Wall[i]->SetTranslationX( Map[i].Position.x );
        p_Wall[i]->SetTranslationY( Map[i].Position.y );
        p_Wall[i]->SetTranslationZ( Map[i].Position.z );

        p_Wall[i]->SetRotationX( Map[i].Direction.x );
        p_Wall[i]->SetRotationY( Map[i].Direction.y );
        p_Wall[i]->SetRotationZ( Map[i].Direction.z );

        p_Wall[i]->SetTexture( p_Texture[Map[i].TextureID]->GetTexture() );

        // On tente ensuite d'initialiser l'objet mur.
        if ( p_Wall[i]->Initialize( p_D3DDevice ) != true )
        {
            // Si l'initialisation échoue, on retourne une erreur.
            return false;
        }
    }

    // Si tout s'est bien passé, on retourne le message de réussite.
    return true;
}

// Cette fonction est appelée à la fin de l'initialisation de l'application.
bool Application::OnPostInitialize()
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
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP );

    // Enfin, on initialise les options de rendu de Direct3D.
    p_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    p_D3DDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    p_D3DDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );

    // Si tout s'est bien passé, on retourne le message de réussite.
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

    // Puis, on appelle la fonction d'initialisation de l'application.
    if ( OnInitialize() == false )
    {
        ShowError( "Erreur lors de l'initialisation de l'application." );
        return false;
    }

    // Ensuite, on appelle la fonction de post-initialisation.
    if ( OnPostInitialize() == false )
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

    // Pour finir, on affiche la fenêtre.
    ShowWindow( hWnd, nFunsterStil );

    // Si tout s'est bien passé, on retourne le message de réussite.
    return true;
}

// Cette fonction est appelée lorsque la scène doit être dessinée.
void Application::Render()
{
    // On teste d'abord si l'objet Direct3D exite.
    if( p_D3DDevice == NULL )
    {
        // Si ce n'est pas le cas, on ne fait rien.
        return;
    }

    HRESULT Result;

    // On teste le niveau coopératif de Direct3D.
    Result = p_D3DDevice->TestCooperativeLevel();

    if( FAILED( Result ) )
    {
        // Si le Device est perdu, on ne fait rien.
        if( Result == D3DERR_DEVICELOST )
        {
            return;
        }

        // On teste si le Device doit être restauré.
        if( Result == D3DERR_DEVICENOTRESET )
        {
            // Si c'est le cas, on tente de restaurer le Device.
            if( Restore() == false )
            {
                // En cas d'erreur, on quitte.
                return;
            }
        }

        // Tant que le Device n'est pas disponible, on ne fait rien.
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
void Application::OnRender()
{
    // On commence par placer le port de vue dans le monde 3D.
    p_Camera->SetCamera( p_D3DDevice );

    // Puis, on commande l'affichage des murs.
    for ( int i = 0; i < NB_WALLS; i++ )
    {
        p_Wall[i]->Render( p_D3DDevice );
    }
}

// Cette fonction teste la validité de la prochaine position.
D3DXVECTOR3 Application::CheckNextPosition( D3DXVECTOR3 CurrentPosition,
                                            D3DXVECTOR3 NextPosition )
{
    // Teste si le moteur de collision est activé.
    if ( p_CollisionType == NO_COLLISION )
    {
        // Si le moteur de collision est ignoré, on retourne simplement la
        // prochaine position.
        return NextPosition;
    }
    else
    {
        // Sinon, il faut tester s'il y a eu une collision.
        bool      Collision          = false;
        bool      MultipleCollisions = false;
        D3DXPLANE SlidingPlane;
        D3DXPLANE CollisionPlane;

        // On teste tous les murs du terrain de jeu. Dans un programme lourd,
        // il faudrait réduire au maximum le nombre de polygones à tester, en
        // utilisant par exemple un arbre BSP (Binary Space Partitionning).
        // Ici, il s'agit d'une démo, donc le code doit rester simple, et le
        // faible nombre de polygones à tester (22 au total) permet d'ignorer
        // ce test. Remarquez tout de même que le sol et le plafond ne sont pas
        // testés, puisqu'il est impossible d'entrer en collision avec eux.
        for ( int i = FIRST_COLLIDABLE_WALL; i < LAST_COLLIDABLE_WALL; i++ )
        {
            // On obtient le pointeur sur la liste de polygones de l'objet.
            E_PolygonContainer* CurrentPolygon =
                p_Wall[i]->GetPolygonList()->GetFirst();

            // On recherche une collision dans toute la liste de polygones.
            while ( CurrentPolygon != NULL )
            {
                // On obtient le polygone courant, et on lui applique une
                // transformation pour le placer dans le bon système d'axes.
                // Une meilleure méthode serait de convertir uniquement la
                // position à tester dans le système d'axes du polygone, en
                // appliquant l'inverse de la matrice du polygone sur la
                // position, afin d'économiser quelques calculs. Mais parce-
                // qu'on devra utiliser le plan du polygone pour le Sliding,
                // plus tard, il vaut mieux pour notre cas convertir les
                // coordonnées du polygone, et obtenir directement le bon plan.
                E_Polygon TestPolygon =
                    CurrentPolygon->GetPolygon()->ApplyMatrix
                                               ( p_Wall[i]->GetWorldMatrix() );

                // On teste ensuite si l'avatar est en collision avec le
                // polygone. Au passage, on récupère le plan de collision,
                // si celui-ci existe. Remarquez que la position à tester est
                // inversée, afin de travailler avec le bon système d'axes.
                if ( E_Collisions::
                         GetTriSphereCollision( -NextPosition,
                                                p_AvatarHeight,
                                                TestPolygon,
                                                &SlidingPlane ) == true )
                {
                    // Si une collision a déjà eu lieu, et si le plan trouvé
                    // n'est pas identique au plan de la collision précédente,
                    // ni à l'inverse du plan de la collision précédente,
                    // alors on indique que le joueur est en collision avec
                    // plusieurs murs.
                    if ( ( SlidingPlane.a !=  CollisionPlane.a   ||
                           SlidingPlane.b !=  CollisionPlane.b   ||
                           SlidingPlane.c !=  CollisionPlane.c   ||
                           SlidingPlane.d !=  CollisionPlane.d ) &&
                         ( SlidingPlane.a != -CollisionPlane.a   ||
                           SlidingPlane.b != -CollisionPlane.b   ||
                           SlidingPlane.c != -CollisionPlane.c   ||
                           SlidingPlane.d != -CollisionPlane.d ) &&
                         Collision        == true )
                    {
                        MultipleCollisions = true;
                    }

                    // On copie le plan de collision.
                    CollisionPlane = SlidingPlane;

                    // Puis, on indique qu'une collision a eu lieu.
                    Collision = true;
                }

                // On passe au polygone suivant, pour parcourir toute la liste.
                CurrentPolygon = CurrentPolygon->GetNext();
            }
        }

        // On teste l'état du moteur de collision.
        if ( p_CollisionType == COLLISION_ONLY )
        {
            // Si le moteur doit tester uniquement les collisions, on teste
            // les valeurs retournées par la fonction de collision.
            if ( Collision == false )
            {
                // Si aucune collision n'a été détectée, alors la prochaine
                // position est valide. Dans ce cas, on la retoune simplement.
                return NextPosition;
            }

            // Si une collision a été trouvée, la nouvelle position n'est
            // pas valide. Dans ce cas, on reste à la position courante.
            return CurrentPosition;
        }
        else
        {
            // Si le moteur doit tester les collisions et gérer le sliding,
            // on teste les valeurs retournées par la fonction de collision.
            if ( Collision == false )
            {
                // Si aucune collision n'a été détectée, alors la prochaine
                // position est valide. Dans ce cas, on la retourne simplement.
                return NextPosition;
            }
            else if ( Collision == true && MultipleCollisions == false )
            {
                // Si une seule collision a été détectée, on corrige la
                // position du joueur en fonction du plan du mur touché.
                // Remarquez que la position est inversée, pour qu'elle
                // corresponde au système d'axes du plan, et que la réponse est
                // à nouveau inversée, pour la reconvertir aux systèmes d'axes
                // de la caméra.
                return -( E_Collisions::GetSlidingPoint( CollisionPlane,
                                                         -NextPosition,
                                                         p_AvatarHeight ) );
            }

            // Si plus d'une collision a été trouvée, la nouvelle position
            // n'est pas valide. Dans ce cas, on reste à la position courante.
            return CurrentPosition;
        }
    }

    // Ce point du code n'est jamais atteint, mais il vaut mieux le traiter
    // quand même, pour éviter des erreurs de compilation.
    return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
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
        // Si ce n'est pas le cas, on indique que la lecture n'a pas eu lieu.
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
        // on attend que le focus soit à nouveau attribué.
        while( p_Keyboard->Acquire() == DIERR_INPUTLOST )
        {
        }

        // Ensuite, on indique que la lecture n'a pas eu lieu.
        return true; 
    }

    // On teste si la touche pressée est la touche fléchée gauche.
    if ( KEYDOWN( Diks, DIK_LEFT ) )
    {
        // Si oui, on incrémente la valeur de la direction du joueur.
        p_CurDir += p_DirVelocity;

        // Puis on teste si la valeur de la direction est acceptable.
        if ( p_CurDir > (float)D3DX_PI * 2 )
        {
            // Si ce n'est pas le cas, on corrige.
            p_CurDir -= (float)D3DX_PI * 2;
        }
    }

    // On teste si la touche pressée est la touche fléchée droite.
    if ( KEYDOWN( Diks, DIK_RIGHT ) )
    {
        // Si oui, on décrémente la valeur de la direction du joueur.
        p_CurDir -= p_DirVelocity;

        // Puis on teste si la valeur de la direction est acceptable.
        if ( p_CurDir < 0.0f )
        {
            // Si ce n'est pas le cas, on corrige.
            p_CurDir += (float)D3DX_PI * 2;
        }
    }

    // On teste si la touche pressée est la touche fléchée haut.
    if( KEYDOWN( Diks, DIK_UP ) )
    {
        // On commence par copier la valeur actuelle de la position.
        D3DXVECTOR3 NextPosition = p_CurPos;

        // Puis, on calcule la nouvelle position.
        NextPosition.x -=
            ( p_PosVelocity * ( (float)cosf( p_CurDir + D3DX_PI / 2 ) ) );
        NextPosition.z -=
            ( p_PosVelocity * ( (float)sinf( p_CurDir + D3DX_PI / 2 ) ) );

        // Enfin, on teste la validité de la prochaine position.
        p_CurPos = CheckNextPosition( p_CurPos, NextPosition );
    }

    // On teste si la touche pressée est la touche fléchée bas.
    if( KEYDOWN( Diks, DIK_DOWN ) )
    {
        // On commence par copier la valeur actuelle de la position.
        D3DXVECTOR3 NextPosition = p_CurPos;

        // Puis, on calcule la nouvelle position.
        NextPosition.x +=
            ( p_PosVelocity * ( (float)cosf( p_CurDir + D3DX_PI / 2 ) ) );
        NextPosition.z +=
            ( p_PosVelocity * ( (float)sinf( p_CurDir + D3DX_PI / 2 ) ) );

        // Enfin, on teste la validité de la prochaine position.
        p_CurPos = CheckNextPosition( p_CurPos, NextPosition );
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

    // On ajuste ensuite les données du point de vue.
    p_Camera->SetRotationY   ( p_CurDir );
    p_Camera->SetTranslationX( p_CurPos.x );
    p_Camera->SetTranslationY( p_CurPos.y );
    p_Camera->SetTranslationZ( p_CurPos.z );

    // Enfin, on commande un rendu de la scène.
    Render();
}

// Cette fonction permet de configurer le moteur de collisions.
void Application::SetCollisionType( CollisionType Type )
{
    // On réinitialise la position et la direction du joueur.
    p_CurPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_CurDir = 0.0f;

    // Puis on inscrit le type de moteur de collision.
    p_CollisionType = Type;
}

// Cette fonction restaure les objets après un changement de Device Direct3D.
bool Application::Restore()
{
    // On teste tout d'abord si un Device Direct3D existe.
    if ( p_D3DDevice == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on tente de restaurer le Device.
    if( FAILED( p_D3DDevice->Reset( &D3Dpp ) ) )
    {
        // Si la restauration échoue, on retourne une erreur.
        return false;
    }

    // On restaure la caméra.
    p_Camera->Restore( p_D3DDevice );

    for ( int i = 0; i < NB_TEXTURES; i++ )
    {
        if ( p_Texture[i]->Restore( p_D3DDevice ) == false )
        {
            // Si la restauration échoue, on retourne une erreur.
            return false;
        }
    }

    // On restaure tous les murs.
    for ( int i = 0; i < NB_WALLS; i++ )
    {
        // On tente de restaurer le mur.
        if ( p_Wall[i]->Restore( p_D3DDevice ) == false )
        {
            // Si la restauration échoue, on retourne une erreur.
            return false;
        }

        // Puis, on réattribue le pointeur de la texture restaurée.
        p_Wall[i]->SetTexture( p_Texture[Map[i].TextureID]->GetTexture() );
    }

    // Ensuite, on appelle la fonction de post-initialisation.
    if ( OnPostInitialize() == false )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien passé, on retourne un message de réussite.
    return true;
}

// Cette fonction est la fonction principale de l'application.
int Application::Run()
{
    // On appelle ici le moteur de messages de Windows.
    while( Messages.message != WM_QUIT )
    {
        // Si un message est arrivé, on le traite.
        PeekMessage     ( &Messages, NULL, 0U, 0U, PM_REMOVE );
        TranslateMessage( &Messages );
        DispatchMessage ( &Messages );

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
    // On libère ici les ressources utilisées par l'application.
    if ( p_Application != NULL )
    {
        delete p_Application;
        p_Application = NULL;
    }
}
