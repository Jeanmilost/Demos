/*****************************************************************************
 * ==> Classe Application ---------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe repr�sente l'infrastructure de l'application.  *
 * Version     : 1.1                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "application.h"

#include <cstdio>

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
        // On teste si le menu a envoy� une commande.
        case WM_COMMAND:
        {
            // Si oui, on recherche quel entr�e a �t� s�lectionn�e.
            switch( wParam )
            {
                // L'utilisateur veut charger un nouveau fichier.
                case IDM_OPEN:
                {
                    // On tente d'ouvrir un nouveau fichier.
                    if ( Application::GetInstance()->OnOpenFile() == false )
                    {
                        // Si l'op�ration �choue, on affiche une erreur.
                        Application::GetInstance()->ShowError
                            ( "Erreur lors du chargement du fichier." );
                    }

                    break;
                }

                // L'application doit �tre quitt�e.
                case IDM_APPEXIT:
                {
                    PostQuitMessage( 0 );
                    break;
                }

                // Le status de la bo�te de collision de l'objet est modifi�.
                case IDM_SHOWBBOBJECT:
                {
                    // On met � jour le status.
                    Application::GetInstance()->OnShowBBObject();
                    break;
                }

                // Le status de la sph�re de collision de l'objet est modifi�.
                case IDM_SHOWBSOBJECT:
                {
                    // On met � jour le status.
                    Application::GetInstance()->OnShowBSObject();
                    break;
                }

                // Le status de la sph�re de collision du joueur est modifi�.
                case IDM_SHOWBSPLAYER:
                {
                    // On met � jour le status.
                    Application::GetInstance()->OnShowBSPlayer();
                    break;
                }

                // Le status des diagnostiques est modifi�.
                case IDM_SHOWDIAGS:
                {
                    // On met � jour le status.
                    Application::GetInstance()->OnShowDiags();
                    break;
                }

                // Le status de l'aide est modifi�.
                case IDM_SHOWHELP:
                {
                    // On met � jour le status.
                    Application::GetInstance()->OnShowHelp();
                    break;
                }
            }

            return 0;
        }

        // La fen�tre doit �tre redessinn�e.
        case WM_PAINT:
        {
            Application::GetInstance()->Render();
            ValidateRect( hWnd, NULL );
            return 0;
        }

        // L'application doit �tre d�truite.
        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            return 0;
        }

        // Par d�faut, on parse simplement les messages.
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
    p_WindowName            = "AABB Tree d�mo";
    p_ClassName             = "Application";

    p_WinWidth              = 300;
    p_WinHeight             = 300;

    p_D3D                   = NULL;
    p_D3DDevice             = NULL;
    p_DI                    = NULL;
    p_Keyboard              = NULL;

    p_CurPos                = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_PosVelocity           = 0.5f;
    p_DirVelocity           = 0.1f;
    p_CurDir                = 0.0f;

    p_Camera                = NULL;
    p_Text                  = NULL;
    p_Mesh                  = NULL;
    p_Tree                  = NULL;

    p_PolygonsTested        = 0;

    p_ShowObjectSphere      = false;
    p_ShowObjectBox         = false;
    p_ShowPlayerSphere      = false;
    p_ShowDiags             = true;
    p_ShowHelp              = true;
    p_SphereSphereCollision = false;
    p_SphereBoxCollision    = false;
    p_SphereObjCollision    = false;

    p_TextColorA            = 255;
    p_TextColorR            = 134;
    p_TextColorG            = 34;
    p_TextColorB            = 186;

    p_Player.SetCenter( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
    p_Player.SetRadius( 1.5f );
}

// Destructeur de la classe Application.
Application::~Application()
{
    // D�truit les objets virtuels.
    p_Sphere.ReleaseVirtualObject();
    p_Box.ReleaseVirtualObject();
    p_Player.ReleaseVirtualObject();

    // D�truit l'objet cam�ra.
    if ( p_Camera != NULL )
    {
        delete p_Camera;
        p_Camera = NULL;
    }

    // D�truit l'objet texte.
    if ( p_Text != NULL )
    {
        delete p_Text;
        p_Text = NULL;
    }

    // D�truit l'arbre.
    if ( p_Tree != NULL )
    {
        delete p_Tree;
        p_Tree = NULL;
    }

    // D�truit l'objet 3D.
    if ( p_Mesh != NULL )
    {
        delete p_Mesh;
        p_Mesh = NULL;
    }

    // D�salloue les ressources du clavier.
    if ( p_Keyboard != NULL )
    {
        p_Keyboard->Unacquire();
        p_Keyboard->Release();
        p_Keyboard = NULL;
    }

    // D�truit l'objet DirectInput utilis� par cette application.
    if ( p_DI != NULL )
    {
         p_DI->Release();
         p_DI = NULL;
     }

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

// Cette fonction permet de montrer/cacher la bo�te de collision de l'objet.
void Application::OnShowBBObject()
{
    p_ShowObjectBox = !p_ShowObjectBox;
    UpdateMenu( IDM_SHOWBBOBJECT, p_ShowObjectBox );
}

// Cette fonction permet de montrer/cacher la sph�re de collision de l'objet.
void Application::OnShowBSObject()
{
    p_ShowObjectSphere = !p_ShowObjectSphere;
    UpdateMenu( IDM_SHOWBSOBJECT, p_ShowObjectSphere );
}

// Cette fonction permet de montrer/cacher la sph�re de collision du joueur.
void Application::OnShowBSPlayer()
{
    p_ShowPlayerSphere = !p_ShowPlayerSphere;
    UpdateMenu( IDM_SHOWBSPLAYER, p_ShowPlayerSphere );
}

// Cette fonction permet de montrer/cacher les diagnostiques.
void Application::OnShowDiags()
{
    p_ShowDiags = !p_ShowDiags;
    UpdateMenu( IDM_SHOWDIAGS, p_ShowDiags );
}

// Cette fonction permet de montrer/cacher l'aide.
void Application::OnShowHelp()
{
    p_ShowHelp = !p_ShowHelp;
    UpdateMenu( IDM_SHOWHELP, p_ShowHelp );
}

// Cette fonction permet de charger un objet 3D en m�moire.
bool Application::LoadMesh( LPSTR Filename )
{
    // On teste d'abord si un autre objet a d�j� �t� charg� en m�moire.
    if ( p_Mesh != NULL )
    {
        // Si c'est le cas, on lib�re les ressources de cet objet avant de
        // tenter d'en charger un autre.
        delete p_Mesh;
        p_Mesh = NULL;
    }

    // On teste ensuite si un arbre existe.
    if ( p_Tree != NULL )
    {
        // Si c'est le cas, on lib�re les ressources de l'arbre existant avant
        // de tenter d'en cr�er un autre.
        delete p_Tree;
        p_Tree = NULL;
    }

    // On cr�e un nouvel objet en m�moire.
    p_Mesh = new E_Mesh();

    // Puis, on teste si l'objet a �t� correctement cr�e en m�moire.
    if ( p_Mesh == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On d�finit le nom du fichier X � charger.
    p_Mesh->SetFilename( Filename );

    // Puis, on tente d'initialiser l'objet 3D.
    if ( p_Mesh->Initialize( p_D3DDevice ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On obtient les valeurs non sign�es de la bo�te de collision de l'objet.
    D3DXVECTOR3 UnsignedMax =
        E_Maths::GetUnsignedValue( p_Mesh->GetBoundingBox().GetMax() );
    D3DXVECTOR3 UnsignedMin =
        E_Maths::GetUnsignedValue( p_Mesh->GetBoundingBox().GetMin() );

    // Puis, on cherche la valeur non sign�e la plus grande.
    D3DXVECTOR3 Max = E_Maths::GetMaxValue( UnsignedMax, UnsignedMin );

    // On utilise cette valeur pour calculer la position de l'objet sur l'axe Z.
    p_Mesh->SetTranslationZ( Max.z * 3.0f );

    // Puis, on tente de cr�er un nouvel arbre.
    p_Tree = new E_AABBTree();

    // On teste ensuite si l'arbre a �t� correctement cr�e en m�moire.
    if ( p_Tree == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on tente d'initialiser l'arbre.
    if ( p_Tree->Initialize( p_Mesh->GetPolygonList() ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On cr�e ensuite une sph�re virtuelle pour l'objet.
    p_Sphere.ReleaseVirtualObject();
    p_Sphere = p_Mesh->GetBoundingSphere();
    p_Sphere.CreateVirtualObject( p_D3DDevice, 255, 0, 0 );

    // Puis, on cr�e une bo�te virtuelle pour l'objet.
    p_Box.ReleaseVirtualObject();
    p_Box = p_Mesh->GetBoundingBox();
    p_Box.CreateVirtualObject( p_D3DDevice, 0, 0, 255 );

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet � l'utilisateur de choisir un fichier � ouvrir.
bool Application::OnOpenFile()
{
    // On cr�e un objet pour param�trer Windows Explorer.
    OPENFILENAME OpenFileName;
    char FileName[MAX_PATH];

    // Puis, on initialise l'objet en m�moire.
    ZeroMemory( &OpenFileName, sizeof( OpenFileName ) );
    FileName[0] = 0;

    // On configure ensuite les param�tres de l'explorateur de fichiers.
    OpenFileName.lStructSize     = sizeof( OpenFileName );
    OpenFileName.hwndOwner       = hWnd;
    OpenFileName.lpstrFilter     = "X Files (*.x)\0*.x\0\0";
    OpenFileName.lpstrFile       = FileName;
    OpenFileName.nMaxFile        = MAX_PATH;
    OpenFileName.lpstrInitialDir = "Meshes";
    OpenFileName.lpstrDefExt     = "x";
    OpenFileName.Flags           = OFN_EXPLORER      |
                                   OFN_FILEMUSTEXIST |
                                   OFN_HIDEREADONLY;

    // Puis, on ouvre l'explorateur, et on r�cup�re le choix de l'utilisateur.
    if( GetOpenFileName( &OpenFileName ) )
    {
        // On tente d'ouvrir le fichier.
        if ( LoadMesh( FileName ) == false )
        {
            // Si l'op�ration �choue, on retourne une erreur.
            return false;
        }
    }

    // Pour finir, on retourne une r�ponse positive.
    return true;
}

// Cette fonction met � jour l'entr�e de menu sp�cifi�e.
void Application::UpdateMenu( int MenuID, bool Checked )
{
    // On teste si l'entr�e doit �tre coch�e ou pas.
    if ( Checked == true )
    {
        // Si elle doit �tre coch�e, on demande l'ajout de la coche.
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
    MessageBox( hWnd,
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
    WinCl.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
    WinCl.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
    WinCl.hCursor       = LoadCursor( NULL, IDC_ARROW );
    WinCl.lpszMenuName  = NULL;
    WinCl.cbClsExtra    = 0;
    WinCl.cbWndExtra    = 0;
    WinCl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    // Puis, on tente d'enregistrer la classe fen�tre.
    if ( !RegisterClassEx( &WinCl ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
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

    // Puis, on met � jour les menus en fonction des param�tres de d�part.
    UpdateMenu( IDM_SHOWBBOBJECT, p_ShowObjectSphere );
    UpdateMenu( IDM_SHOWBSOBJECT, p_ShowObjectBox );
    UpdateMenu( IDM_SHOWBSPLAYER, p_ShowPlayerSphere );
    UpdateMenu( IDM_SHOWDIAGS,    p_ShowDiags );
    UpdateMenu( IDM_SHOWHELP,     p_ShowHelp );

    // Si tout s'est bien pass�, on retourne un message de r�ussite.
    return true;
}

// Cette fonction initialise Direct3D.
bool Application::InitD3D()
{
    // On cr�e ici une instance de la classe Direct3D9.
    p_D3D = Direct3DCreate9( D3D_SDK_VERSION );

    // Puis, on teste si l'instance a �t� correctement cr�e en m�moire.
    if( p_D3D == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On initialise ensuite les param�tres du device Direct3D.
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
        // Si la cr�ation �choue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien pass�, on retourne un message de r�ussite.
    return true;
}

// Cette fonction initialise DirectInput.
bool Application::InitDInput()
{
    // On cr�e d'abord les param�tres d'acquision du clavier.
    DWORD CoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;

    // Puis, on tente de cr�er une instance de la classe DirectInput8.
    if( FAILED( DirectInput8Create( GetModuleHandle( NULL ),
                                    DIRECTINPUT_VERSION, 
                                    IID_IDirectInput8,
                                    (VOID**)&p_DI,
                                    NULL ) ) )
    {
        // Si la cr�ation �choue, on retourne une erreur.
        return false;
    }

    // Ensuite, on tente la cr�ation du Device.
    if( FAILED( p_DI->CreateDevice( GUID_SysKeyboard, &p_Keyboard, NULL ) ) )
    {
        // Si la cr�ation �choue, on retourne une erreur.
        return false;
    }

    // Puis, on initialise le buffer de r�ception pour les donn�es du clavier.
    if( FAILED( p_Keyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
    {
        // Si la cr�ation �choue, on retourne une erreur.
        return false;
    }

    // On d�termine ici le niveau de coop�ration de DirectInput.
    if ( FAILED ( p_Keyboard->SetCooperativeLevel( hWnd, CoopFlags ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on tente l'acquision du clavier.
    p_Keyboard->Acquire();

    // Pour finir, on retourne le message de r�ussite.
    return true;
}

// Cette fonction est appel�e lorsque l'application s'initialise.
bool Application::OnInitialize()
{
    // On cr�e ici l'objet Cam�ra. Cet objet est le port de vue.
    p_Camera = new E_Camera( p_D3DDevice );

    // Ensuite, on teste que l'objet cam�ra soit bien cr�e en m�moire.
    if ( p_Camera == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On d�finit le point de coupe lointain � 1000 unit�s.
    p_Camera->SetClipFar( 1000.0f );

    // On cr�e ensuite l'objet pour le rendu du texte.
    p_Text = new E_Text();

    // Puis, on teste si l'objet a �t� correctement cr�e en m�moire.
    if ( p_Text == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On d�finit la police du texte.
    p_Text->SetFont( "Verdana" );

    // Puis, on tente d'initialiser l'objet texte.
    if ( p_Text->Initialize( p_D3DDevice ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On tente ensuite de charge un objet par d�faut en m�moire.
    if ( LoadMesh( "Meshes\\Tiger.x" ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on cr�e un objet virtuel pour repr�senter le joueur.
    if ( p_Player.CreateVirtualObject( p_D3DDevice, 0, 255, 0 ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction est appel�e � la fin de l'initialisation de l'application.
bool Application::OnPostInitialize()
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
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP );
    p_D3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP );

    // Enfin, on initialise les options de rendu de Direct3D.
    p_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    p_D3DDevice->SetRenderState( D3DRS_ZENABLE,      TRUE );
    p_D3DDevice->SetRenderState( D3DRS_LIGHTING,     FALSE );

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
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

    // Puis, on appelle la fonction d'initialisation de l'application.
    if ( OnInitialize() == false )
    {
        ShowError( "Erreur lors de l'initialisation de l'application." );
        return false;
    }

    // Ensuite, on appelle la fonction de post-initialisation.
    if ( OnPostInitialize() == false )
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

    // Pour finir, on affiche la fen�tre.
    ShowWindow( hWnd, nFunsterStil );

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction est appel�e lorsque la sc�ne doit �tre dessin�e.
void Application::Render()
{
    // On teste d'abord si l'objet Direct3D exite.
    if( p_D3DDevice == NULL )
    {
        // Si ce n'est pas le cas, on ne fait rien.
        return;
    }

    HRESULT Result;

    // On teste le niveau coop�ratif de Direct3D.
    Result = p_D3DDevice->TestCooperativeLevel();

    if( FAILED( Result ) )
    {
        // Si le Device est perdu, on ne fait rien.
        if( Result == D3DERR_DEVICELOST )
        {
            return;
        }

        // On teste si le Device doit �tre restaur�.
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

// Cette fonction convertit une valeur bool�enne en texte.
LPSTR Application::ConvertBoolToText( bool Value )
{
    // On teste la valeur demand�e.
    if ( Value == true )
    {
        // Si la valeur vaut true, on retourne la conversion.
        return ": Oui";
    }
    else
    {
        // Si la valeur vaut false, on retourne la conversion.
        return ": Non";
    }
}

// Cette fonction est appel�e lorsque la console de diags doit �tre dessin�e.
void Application::OnRenderDiags()
{
    // On teste tout d'abord si l'objet texte a �t� correctement initialis�.
    if ( p_Text != NULL )
    {
        // Si c'est le cas, on effectue le rendu des textes de l�gende.
        p_Text->Render( p_D3DDevice,
                        "Profondeur de l'arbre",
                        1,
                        1,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Nombre de noeuds dans l'arbre",
                        1,
                        12,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Collision sph�re-sph�re",
                        1,
                        24,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Collision sph�re-bo�te",
                        1,
                        36,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Collision sph�re-objet",
                        1,
                        48,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Nombre de polygones du Mesh",
                        1,
                        60,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "Nombre de polygones test�s",
                        1,
                        72,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        TCHAR Text[20];

        // Ensuite, on convertit les valeurs � afficher en texte, et on
        // effectue le rendu des valeurs.
        std::sprintf( Text, ": %d", p_Tree->GetDeep() );
        p_Text->Render( p_D3DDevice,
                        Text,
                        185,
                        1,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        sprintf( Text, ": %d", p_Tree->GetChildCount() );
        p_Text->Render( p_D3DDevice,
                        Text,
                        185,
                        12,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        ConvertBoolToText( p_SphereSphereCollision ),
                        185,
                        24,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        ConvertBoolToText( p_SphereBoxCollision ),
                        185,
                        36,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        ConvertBoolToText( p_SphereObjCollision ),
                        185,
                        48,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        sprintf( Text, ": %d", p_Mesh->GetPolygonList()->GetCount() );
        p_Text->Render( p_D3DDevice,
                        Text,
                        185,
                        60,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        sprintf( Text, ": %d", p_PolygonsTested );
        p_Text->Render( p_D3DDevice,
                        Text,
                        185,
                        72,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );
    }
}

// Cette fonction est appel�e lorsque l'aide doit �tre dessin�e.
void Application::OnRenderHelp()
{
    // On teste tout d'abord si l'objet texte a �t� correctement initialis�.
    if ( p_Text != NULL )
    {
        // Si c'est le cas, on commande le rendu de l'aide.
        p_Text->Render( p_D3DDevice,
                        "Aide :",
                        1,
                        218,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "<HAUT><BAS> = D�placer cam�ra",
                        1,
                        230,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );

        p_Text->Render( p_D3DDevice,
                        "<GAUCHE><DROITE> = Tourner objet.\0",
                        1,
                        242,
                        p_TextColorA,
                        p_TextColorR,
                        p_TextColorG,
                        p_TextColorB );
    }
}

// Cette fonction est appel�e lorsque la sc�ne est en cours de dessin.
void Application::OnRender()
{
    // On commence par placer le port de vue dans le monde 3D.
    p_Camera->SetCamera( p_D3DDevice );

    // On teste si les objets n�cessaires pour le rendu sont correctement
    // initialis�s.
    if ( p_Mesh                   == NULL ||
         p_Mesh->GetPolygonList() == NULL ||
         p_Tree                   == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'op�ration
        return;
    }

    // Puis, on commande le rendu de l'objet 3D.
    p_Mesh->Render( p_D3DDevice );

    // On teste si la bo�te de l'objet doit �tre affich�e.
    if ( p_ShowObjectSphere == true )
    {
        // Si c'est le cas, on commande le rendu de l'objet.
        p_Sphere.RenderVirtualObject( p_D3DDevice, p_Mesh->GetWorldMatrix() );
    }

    // On teste si la sph�re de l'objet doit �tre affich�e.
    if ( p_ShowObjectBox == true )
    {
        // Si c'est le cas, on commande le rendu de l'objet.
        p_Box.RenderVirtualObject( p_D3DDevice, p_Mesh->GetWorldMatrix() );
    }

    // On teste si la sph�re du joueur doit �tre affich�e.
    if ( p_ShowPlayerSphere == true )
    {
        // Si c'est le cas, on cr�e une matrice de r�f�rence.
        D3DXMATRIXA16 Identity;
        D3DXMatrixIdentity( &Identity );

        // Puis, on commande le rendu de l'objet avec cette matrice.
        p_Player.RenderVirtualObject( p_D3DDevice, Identity );
    }

    // On teste si la console de diagnostiques doit �tre affich�e.
    if ( p_ShowDiags == true )
    {
        // Si c'est le cas, on commande le rendu de la console.
        OnRenderDiags();
    }

    // On teste si la console d'aide doit �tre affich�e.
    if ( p_ShowHelp == true )
    {
        // Si c'est le cas, on commande le rendu de la console.
        OnRenderHelp();
    }
}

// Cette fonction teste la validit� de la position du joueur.
bool Application::CheckPosition( D3DXVECTOR3 Position )
{
    // On teste si l'objet 3D et si l'arbre sont correctement initialis�s.
    if ( p_Mesh == NULL || p_Tree == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    p_PolygonsTested        = 0;
    p_SphereSphereCollision = false;
    p_SphereBoxCollision    = false;
    p_SphereObjCollision    = false;

    // On transforme le point � tester selon l'inverse de la matrice de
    // l'objet. Ceci permet de convertir le point � tester dans le syst�me
    // d'axes de l'objet. Vous remarquerez que l'on utilise la coordonn�e
    // invers�e du point � tester. La raison de ceci est que, lorsque l'on
    // applique la matrice de monde au port de vue dans la cam�ra, on inverse
    // le syst�me de coordonn�es. Il faut donc le r�-inverser pour que les
    // calculs correspondent � la position physique r�elle de la cam�ra dans
    // l'univers 3D.
    D3DXVECTOR3 CheckPosition =
        E_CollisionLayout::GetInversedPosition( -Position,
                                                p_Mesh->GetWorldMatrix() );

    // On cr�e ensuite une liste de polygones, pour r�cup�rer la r�ponse de
    // l'arbre.
    E_PolygonList* Result = new E_PolygonList();

    // On teste si la liste a �t� correctement cr�e en m�moire.
    if ( Result == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On cr�e une sph�re de collision pour le test.
    E_BoundingSphere Sphere( CheckPosition, p_Player.GetRadius() );

    // Puis, on r�cup�re les polygones � tester dans l'arbre.
    p_Tree->Resolve( Result, Sphere );

    // On teste si la sph�re du joueur touche la sph�re de l'objet.
    if ( E_CollisionLayout::SphereIntersectSphere
             ( Sphere.GetCenter(),
               Sphere.GetRadius(),
               p_Mesh->GetBoundingSphere().GetCenter(),
               p_Mesh->GetBoundingSphere().GetRadius() ) == true )
    {
        // Si c'est le cas, on met � jour la valeur pour les diagnostiques.
        p_SphereSphereCollision = true;
    }

    // On teste si la bo�te de collision de l'objet touche la sph�re du joueur.
    if ( E_CollisionLayout::SphereIntersectBox
             ( Sphere.GetCenter(),
               Sphere.GetRadius(),
               p_Mesh->GetBoundingBox().GetMin(),
               p_Mesh->GetBoundingBox().GetMax() ) == true )
    {
        // Si c'est le cas, on met � jour la valeur pour les diagnostiques.
        p_SphereBoxCollision = true;
    }

    // On met � jour la valeur du nombre de polygones test�s.
    p_PolygonsTested = Result->GetCount();

    // Puis, on obtient le pointeur du premier conteneur de la liste.
    E_PolygonContainer* Current = Result->GetFirst();

    // On passe toute la liste-r�ponse en revue.
    while ( Current != NULL )
    {
        // On obtient la copie du polygone courant.
        E_Polygon Polygon = Current->GetPolygon()->GetClone();

        // Puis, on teste si ce polygone est en collision avec la sph�re du
        // joueur.
        if ( E_SphereCollision::Check( CheckPosition,
                                       p_Player.GetRadius(),
                                       Polygon ) == true )
        {
            // Si c'est le cas, on met � jour la valeur de diagnostique.
            p_SphereObjCollision = true;

            // Puis, on lib�re les ressources de la liste-r�ponse.
            delete Result;
            Result = NULL;

            // Pour finir, on retourne la r�ponse.
            return true;
        }

        // On passe au polygone suivant.
        Current = Current->GetNext();
    }

    // Si aucune collision n'a �t� trouv�e, on lib�re les ressources de la
    // liste-r�ponse.
    delete Result;
    Result = NULL;

    // Pour finir, on retourne la r�ponse.
    return false;
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
        // Si ce n'est pas le cas, on indique que la lecture n'a pas eu lieu.
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
        // on attend que le focus soit � nouveau attribu�.
        while( p_Keyboard->Acquire() == DIERR_INPUTLOST )
        {
        }

        // Ensuite, on indique que la lecture n'a pas eu lieu.
        return true; 
    }

    // On teste si la touche press�e est la touche fl�ch�e gauche.
    if ( KEYDOWN( Diks, DIK_LEFT ) )
    {
        // Si oui, on incr�mente la valeur de la direction du joueur.
        p_CurDir += p_DirVelocity;

        // Puis on teste si la valeur de la direction est acceptable.
        if ( p_CurDir > (float)D3DX_PI * 2 )
        {
            // Si ce n'est pas le cas, on corrige.
            p_CurDir -= (float)D3DX_PI * 2;
        }

        // On teste aussi si la rotation de l'objet met le joueur en collision.
        while ( CheckPosition( p_CurPos ) == true )
        {
            // Si c'est le cas, on corrige la position, jusqu'� ce qu'on ne
            // soit plus en collision avec l'objet.
            p_CurPos.z += p_PosVelocity;
        }
    }

    // On teste si la touche press�e est la touche fl�ch�e droite.
    if ( KEYDOWN( Diks, DIK_RIGHT ) )
    {
        // Si oui, on d�cr�mente la valeur de la direction du joueur.
        p_CurDir -= p_DirVelocity;

        // Puis on teste si la valeur de la direction est acceptable.
        if ( p_CurDir < 0.0f )
        {
            // Si ce n'est pas le cas, on corrige.
            p_CurDir += (float)D3DX_PI * 2;
        }

        // On teste aussi si la rotation de l'objet met le joueur en collision.
        while ( CheckPosition( p_CurPos ) == true )
        {
            // Si c'est le cas, on corrige la position, jusqu'� ce qu'on ne
            // soit plus en collision avec l'objet.
            p_CurPos.z += p_PosVelocity;
        }
    }

    // On teste si la touche press�e est la touche fl�ch�e haut.
    if( KEYDOWN( Diks, DIK_UP ) )
    {
        // On commence par copier la valeur actuelle de la position.
        D3DXVECTOR3 NextPosition = p_CurPos;

        // Puis, on calcule la nouvelle position.
        NextPosition.z -= p_PosVelocity;

        // Enfin, on teste la validit� de la prochaine position.
        if ( CheckPosition( NextPosition ) == false )
        {
            // Si la prochaine position est valide, on met � jour la valeur.
            p_CurPos = NextPosition;
        }
    }

    // On teste si la touche press�e est la touche fl�ch�e bas.
    if( KEYDOWN( Diks, DIK_DOWN ) )
    {
        // On commence par copier la valeur actuelle de la position.
        D3DXVECTOR3 NextPosition = p_CurPos;

        // Puis, on calcule la nouvelle position.
        NextPosition.z += p_PosVelocity;

        // Enfin, on teste la validit� de la prochaine position.
        if ( CheckPosition( NextPosition ) == false )
        {
            // Si la prochaine position est valide, on met � jour la valeur.
            p_CurPos = NextPosition;
        }
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
    // On teste tout d'abord si les objets 3D ont �t� correctement cr�es en
    // m�moire.
    if ( p_Mesh == NULL || p_Camera == NULL )
    {
        // Si ce n'est pas le cas, on ne fait rien.
        return;
    }

    // On lit ici l'�tat des entr�es du clavier.
    ReadKeyboard();

    // On ajuste ensuite les donn�es du point de vue.
    p_Mesh->SetRotationY     ( p_CurDir );

    // On applique la nouvelle position � la cam�ra.
    p_Camera->SetTranslationX( p_CurPos.x );
    p_Camera->SetTranslationY( p_CurPos.y );
    p_Camera->SetTranslationZ( p_CurPos.z );

    // On applique la nouvelle position � la sph�re de collision du joueur.
    p_Player.SetCenter       ( -p_CurPos );

    // Enfin, on commande un rendu de la sc�ne.
    Render();
}

// Cette fonction restaure les objets apr�s un changement de Device Direct3D.
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
        // Si la restauration �choue, on retourne une erreur.
        return false;
    }

    // On teste si la cam�ra a �t� correctement initialis�e.
    if ( p_Camera != NULL )
    {
        // On restaure la cam�ra.
        p_Camera->Restore( p_D3DDevice );
    }

    // On teste si le mesh a �t� correctement initialis�.
    if ( p_Mesh != NULL )
    {
        // On tente de restaurer le mesh.
        if ( p_Mesh->Restore( p_D3DDevice ) == false )
        {
            // Si l'op�ration �choue, on retourne une erreur.
            return false;
        }
    }

    // On restaure la sph�re de collision virtuelle de l'objet.
    if ( p_Sphere.RestoreVirtualObject( p_D3DDevice ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On restaure la bo�te de collision virtuelle de l'objet.
    if ( p_Box.RestoreVirtualObject( p_D3DDevice ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On restaure la sph�re de collision virtuelle du joueur.
    if ( p_Player.RestoreVirtualObject( p_D3DDevice ) == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On teste si l'objet texte a �t� correctement initialis�.
    if ( p_Text != NULL )
    {
        // On tente de restaurer l'objet texte.
        if ( p_Text->Restore( p_D3DDevice ) == false )
        {
            // Si l'op�ration �choue, on retourne une erreur.
            return false;
        }
    }

    // Ensuite, on appelle la fonction de post-initialisation.
    if ( OnPostInitialize() == false )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Si tout s'est bien pass�, on retourne un message de r�ussite.
    return true;
}

// Cette fonction est la fonction principale de l'application.
int Application::Run()
{
    // On appelle ici le moteur de messages de Windows.
    while( Messages.message != WM_QUIT )
    {
        // Si un message est arriv�, on le traite.
        PeekMessage     ( &Messages, NULL, 0U, 0U, PM_REMOVE );
        TranslateMessage( &Messages );
        DispatchMessage ( &Messages );

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
    // On lib�re ici les ressources utilis�es par l'application.
    if ( p_Application != NULL )
    {
        delete p_Application;
        p_Application = NULL;
    }
}
