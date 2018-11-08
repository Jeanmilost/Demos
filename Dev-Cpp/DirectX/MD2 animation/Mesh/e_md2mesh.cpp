/*****************************************************************************
 * ==> Classe E_MD2Mesh -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe repr�sente un objet anim�, en utilisant la     *
 *               m�thode MD2 de Quake.                                       *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_md2mesh.h"

// ********************* Diverses donn�es pour le rendu. **********************
// On d�clare ici diverses structures et �num�rateurs pour le rendu de l'objet.

// On initialise ici les propri�t�s des sommets des polygones de l'objet
const DWORD E_MD2_D3DVERTEX::FVF = D3DFVF_XYZ     |
                                   D3DFVF_NORMAL  |
                                   D3DFVF_DIFFUSE |
                                   D3DFVF_TEX1;

// *************************** Structure E_MD2Color ***************************
// Cette structure permet de contenir les donn�es de la couleur de la lumi�re
// pr�-calcul�e de l'objet. Ceci est valable pour la lumi�re d'ambiance et pour
// la lumi�re directionnelle.

// Constructeur de la classe E_MD2Color.
E_MD2Color::E_MD2Color()
{
    r = 0;
    g = 0;
    b = 0;
}

// Deuxi�me constructeur de la classe E_MD2Color.
E_MD2Color::E_MD2Color( int Red, int Green, int Blue )
{
    r = Red;
    g = Green;
    b = Blue;
}

// *************************** Structure E_MD2Light ***************************
// Cette structure permet de contenir les donn�es pour cr�er la lumi�re pr�-
// calcul�e de l'objet.

// Constructeur de la classe E_MD2Light.
E_MD2Light::E_MD2Light()
{
    Light     = E_MD2Color( 0, 0, 0 );
    Ambient   = E_MD2Color( 0, 0, 0 );
    Direction = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

// Deuxi�me constructeur de la classe E_MD2Light.
E_MD2Light::E_MD2Light( E_MD2Color  Light,
                        E_MD2Color  Ambient,
                        D3DXVECTOR3 Direction )
{
    this->Light     = Light;
    this->Ambient   = Ambient;
    this->Direction = Direction;
}

// ************************* Structure E_MD2Animation *************************
// Cette structure permet de contenir les donn�es n�cessaire pour configurer
// l'animation courante de l'objet.

// Constructeur par d�faut de la structure E_MD2Animation.
E_MD2Animation::E_MD2Animation()
{
    Start = 0;
    End   = 0;
    Name  = NULL;
}

// Deuxi�me constructeur de la structure E_MD2Animation.
E_MD2Animation::E_MD2Animation( int Start, int End, LPSTR Name )
{
    this->Start = Start;
    this->End   = End;
    this->Name  = Name;
}

// ***************************** Classe E_MD2Mesh *****************************
// Cette classe repr�sente un objet anim�. Les algorythmes d'animation et de
// rendu sont ceux utilis�s par les fichiers MD2 de Quake.

// Constructeur de la classe E_MD2Mesh.
E_MD2Mesh::E_MD2Mesh()
{
    p_Skin                 = NULL;
    p_TexCoord             = NULL;
    p_Triangle             = NULL;
    p_Frame                = NULL;
    p_GlCmds               = NULL;
    p_VertexBuffer         = NULL;
    p_Texture              = NULL;
    p_NormalsTable         = NULL;
    p_ModelFilename        = NULL;
    p_TextureFilename      = NULL;

    p_NumMaterials         = 0;
    p_AdressU              = 0;
    p_AdressV              = 0;
    p_CullMode             = 0;
    p_ZWriteEnable         = 0;
    p_AlphaBlendEnable     = 0;
    p_AlphaTestEnable      = 0;
    p_SrcBlend             = 0;
    p_DestBlend            = 0;

    p_Interpolation        = 0.0f;
    p_rX                   = 0.0f;
    p_rY                   = 0.0f;
    p_rZ                   = 0.0f;
    p_tX                   = 0.0f;
    p_tY                   = 0.0f;
    p_tZ                   = 0.0f;

    p_CurTime              = 0L;
    p_LastTime             = 0L;

    p_CurFrameID           = 0;
    p_NextFrameID          = 0;
    p_NormalsTableLength   = 0;

    p_LightDirection       = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_LightningMode        = E_MD2_LIGHTNING_NONE;

    p_NormalsTableFilename = ".\\NormalsTable\\MD2NormalsTable.bin";

    p_IsAlphaBlended       = false;
}

// Destructeur de la classe E_MD2Mesh.
E_MD2Mesh::~E_MD2Mesh()
{
    // On lib�re les ressources utilis�es par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // On lib�re les ressources utilis�es par le VertexBuffer.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // On lib�re les ressources utilis�es par la table des normales.
    if ( p_NormalsTable != NULL )
    {
        delete[] p_NormalsTable;
        p_NormalsTable = NULL;
    }

    // On lib�re les ressources utilis�es par l'objet MD2 Skin.
    if ( p_Skin != NULL )
    {
        delete [] p_Skin;
        p_Skin = NULL;
    }

    // On lib�re les ressources utilis�es par l'objet MD2 TextureCoords.
    if ( p_TexCoord != NULL )
    {
        delete [] p_TexCoord;
        p_TexCoord = NULL;
    }

    // On lib�re les ressources utilis�es par l'objet MD2 Triangle.
    if ( p_Triangle != NULL )
    {
        delete [] p_Triangle;
        p_Triangle = NULL;
    }

    // On lib�re les ressources utilis�es par l'objet MD2 Frame.
    if ( p_Frame != NULL )
    {
        // D'abord les donn�es contenues dans le conteneur de Frames.
        for ( int i = 0; i < p_Header.Nb_Frames; i++ )
        {
            if ( p_Frame[i].Vertex != NULL )
            {
                delete[] p_Frame[i].Vertex;
                p_Frame[i].Vertex = NULL;
            }
        }

        // Puis, le conteneur de Frames lui-m�me.
        delete [] p_Frame;
        p_Frame = NULL;
    }

    // On lib�re les ressources utilis�es par l'objet MD2 GlCmds.
    if ( p_GlCmds != NULL )
    {
        delete [] p_GlCmds;
        p_GlCmds = NULL;
    }
}

// Obtient le nom du fichier du mod�le.
LPSTR E_MD2Mesh::GetModelFilename()
{
    return p_ModelFilename;
}

// D�finit le nom du fichier du mod�le.
void E_MD2Mesh::SetModelFilename( LPSTR Filename )
{
    p_ModelFilename = Filename;
}

// Obtient le nom du fichier de la texture.
LPSTR E_MD2Mesh::GetTextureFilename()
{
    return p_TextureFilename;
}

// D�finit le nom du fichier de la texture.
void E_MD2Mesh::SetTextureFilename( LPSTR Filename )
{
    p_TextureFilename = Filename;
}

// Obtient le nom du fichier de la table des normales.
LPSTR E_MD2Mesh::GetNormalsTableFilename()
{
    return p_NormalsTableFilename;
}

// D�finit le nom du fichier de la table des normales.
void E_MD2Mesh::SetNormalsTableFilename( LPSTR Filename )
{
    p_NormalsTableFilename = Filename;
}

// Obtient l'angle de rotation de l'objet sur l'axe X.
float E_MD2Mesh::GetRotationX()
{
    return p_rX;
}

// D�finit l'angle de rotation de l'objet sur l'axe X.
void E_MD2Mesh::SetRotationX( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rX = Value;
    }
    else
    {
        p_rX = 0;
    }
}

// Obtient l'angle de rotation de l'objet sur l'axe Y.
float E_MD2Mesh::GetRotationY()
{
    return p_rY;
}

// D�finit l'angle de rotation de l'objet sur l'axe Y.
void E_MD2Mesh::SetRotationY( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rY = Value;
    }
    else
    {
        p_rY = 0;
    }
}

// Obtient l'angle de rotation de l'objet sur l'axe Z.
float E_MD2Mesh::GetRotationZ()
{
    return p_rZ;
}

// D�finit l'angle de rotation de l'objet sur l'axe Z.
void E_MD2Mesh::SetRotationZ( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rZ = Value;
    }
    else
    {
        p_rZ = 0;
    }
}

// Obtient la valeur de la position de l'objet sur l'axe X.
float E_MD2Mesh::GetTranslationX()
{
    return p_tX;
}

// D�finit la valeur de la position de l'objet sur l'axe X.
void E_MD2Mesh::SetTranslationX( float Value )
{
    p_tX = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Y.
float E_MD2Mesh::GetTranslationY()
{
    return p_tY;
}

// D�finit la valeur de la position de l'objet sur l'axe Y.
void E_MD2Mesh::SetTranslationY( float Value )
{
    p_tY = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Z.
float E_MD2Mesh::GetTranslationZ()
{
    return p_tZ;
}

// D�finit la valeur de la position de l'objet sur l'axe Z.
void E_MD2Mesh::SetTranslationZ( float Value )
{
    p_tZ = Value;
}

// D�finit le moteur d'�clairage utilis� pour le rendu.
void E_MD2Mesh::SetLightningMode( E_MD2LightningMode Mode )
{
    p_LightningMode = Mode;
}

// D�finit les param�tres de la lumi�re pr�-calcul�e de l'objet.
void E_MD2Mesh::SetPreCalculatedLightParameters( E_MD2Light Light )
{
    p_PreCalculatedLight = Light;
}

// D�finit les param�tres de l'animation courante de l'objet.
bool E_MD2Mesh::SetAnimationParameters( E_MD2Animation Animation )
{
    // On teste tout d'abord que les valeurs entr�es ne soient pas hors de la
    // plage possible pour cet objet.
    if ( Animation.Start < 0 || Animation.End > p_Header.Nb_Frames )
    {
        // Si les valeurs sont inadmissibles, on retourne une erreur.
        return false;
    }

    // Si les valeurs sont correctes, on d�finit les param�tres de l'animation.
    p_Animation     = Animation;
    p_CurFrameID    = Animation.Start;
    p_NextFrameID   = Animation.Start + 1;
    p_Interpolation = 0.0f;

    // Pour finir, on retourne le message de r�ussite.
    return true;
}

// D�finit l'�tat du moteur d'Aplha Blending.
void E_MD2Mesh::SetAlphaBlending( bool Value )
{
    p_IsAlphaBlended = Value;
}

// Retourne l'�tat du moteur d'aplha Blending.
bool E_MD2Mesh::IsAlphaBlended()
{
    return p_IsAlphaBlended;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_MD2Mesh::SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // En d�finissant la source et la destination de l'alpha blending � ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // d�sactiv� pour cette op�ration, l'objet doit donc �tre rendu � la
    // fin de la sc�ne, �ventuellement tri� par ordre de profondeur s'il y a
    // plusieurs objets de ce type � dessiner, afin d'obtenir un affichage
    // correct.
    theD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    theD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
    theD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}

// Cette fonction sauvegarde l'�tat du Device Direct3D avant utilisation.
void E_MD2Mesh::PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSU,   &p_AdressU );
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSV,   &p_AdressV );
    theD3DDevice->GetRenderState ( D3DRS_CULLMODE,        &p_CullMode );
    theD3DDevice->GetRenderState ( D3DRS_ZWRITEENABLE,    &p_ZWriteEnable );
    theD3DDevice->GetRenderState ( D3DRS_SRCBLEND,        &p_SrcBlend );
    theD3DDevice->GetRenderState ( D3DRS_DESTBLEND,       &p_DestBlend );
    theD3DDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &p_AlphaTestEnable );
    theD3DDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,
                                   &p_AlphaBlendEnable );
}

// Cette fonction restaure l'�tat du Device Direct3D apr�s utilisation.
void E_MD2Mesh::PopDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,    p_AdressU );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,    p_AdressV );
    theD3DDevice->SetRenderState ( D3DRS_CULLMODE,         p_CullMode );
    theD3DDevice->SetRenderState ( D3DRS_ZWRITEENABLE,     p_ZWriteEnable );
    theD3DDevice->SetRenderState ( D3DRS_SRCBLEND,         p_SrcBlend );
    theD3DDevice->SetRenderState ( D3DRS_DESTBLEND,        p_DestBlend );
    theD3DDevice->SetRenderState ( D3DRS_ALPHATESTENABLE,  p_AlphaTestEnable );
    theD3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, p_AlphaBlendEnable );
}

// Cette fonction construit la matrice d'affichage locale pour le sprite.
D3DXMATRIXA16 E_MD2Mesh::GetWorldMatrix()
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 SpriteMatrix;

    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &SpriteMatrix );

    D3DXMatrixRotationX( &RX, p_rX );
    D3DXMatrixRotationY( &RY, p_rY );
    D3DXMatrixRotationZ( &RZ, p_rZ );

    D3DXMatrixMultiply( &SpriteMatrix, &RX, &SpriteMatrix );
    D3DXMatrixMultiply( &SpriteMatrix, &RY, &SpriteMatrix );
    D3DXMatrixMultiply( &SpriteMatrix, &RZ, &SpriteMatrix );

    SpriteMatrix._41 = p_tX;
    SpriteMatrix._42 = p_tY;
    SpriteMatrix._43 = p_tZ;

    return SpriteMatrix;
}

// Cette fonction cr�e la table des normales pr�-calcul�es depuis le fichier.
bool E_MD2Mesh::CreateNormalsTable()
{
    // Pointeur vers le fichier.
    FILE* File = NULL;

    float Version = 0.0f;

    // On teste tout d'abord qu'un nom de fichier ait bien �t� d�fini.
    if ( p_NormalsTableFilename == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On tente d'ouvrir le fichier en lecture.
    File = fopen( p_NormalsTableFilename, "rb" );

    // On teste ensuite si le fichier a bien �t� ouvert.
    if( File == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on extrait le num�ro de version du fichier.
    fread( &Version, sizeof(float), 1, File );

    // On teste si le num�ro de version est correct.
    if ( Version != MD2_NVERSION )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // On lit ensuite la longeur de la table dans le fichier.
    fread( &p_NormalsTableLength, sizeof(int), 1, File );

    // Puis, on alloue l'espace n�cessaire pour la table.
    p_NormalsTable = new D3DXVECTOR3[p_NormalsTableLength];

    // On teste que l'espace n�cessaire soit bien allou� en m�moire.
    if ( p_NormalsTable == NULL )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // Puis, on copie les donn�es de la table en m�moire, depuis le fichier.
    for ( int i = 0; i < p_NormalsTableLength; i++ )
    {
        fread( &p_NormalsTable[i], sizeof(D3DXVECTOR3), 1, File );
    }

    // Pour fnir, on clos le fichier, ...
    fclose( File );

    // ... et on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet d'initialiser l'objet.
bool E_MD2Mesh::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // Pointeur vers le fichier.
    FILE* File = NULL;

    // On teste tout d'abord qu'un nom de fichier ait bien �t� d�fini.
    if ( p_ModelFilename == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On tente ensuite d'ouvrir le fichier.
    File = fopen( p_ModelFilename, "rb" );

    // On teste si le fichier a bien �t� ouvert.
    if( File == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on extrait l'en-t�te MD2 du fichier.
    fread( &p_Header, sizeof(E_MD2Header), 1, File );

    // On teste ensuite que les param�tres de l'en-t�te MD2 soient valides.
    if ( ( p_Header.ID != MD2_IDENT ) && ( p_Header.Version != MD2_VERSION ) )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // Si le format du fichier est correct, on alloue la m�moire n�cessaire aux
    // donn�es � extraire.
    p_Skin     = new E_MD2Skin        [p_Header.Nb_Skins     + 1];
    p_TexCoord = new E_MD2TextureCoord[p_Header.Nb_UV        + 1];
    p_Triangle = new E_MD2Triangle    [p_Header.Nb_Triangles + 1];
    p_GlCmds   = new int              [p_Header.Nb_GlCmds    + 1];

    // On teste ensuite si la m�moire a �t� correctement allou�e.
    if ( p_Skin     == NULL ||
         p_TexCoord == NULL ||
         p_Triangle == NULL ||
         p_GlCmds   == NULL )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // On lit les donn�es depuis le fichier. D'abord le contenu de la classe
    // E_MD2Skin, ...
    fseek( File, p_Header.Offset_Skins, SEEK_SET );
    fread( p_Skin, sizeof(E_MD2Skin), p_Header.Nb_Skins, File );

    // ... ensuite, le contenu de la classe E_MD2TextureCoord, ...
    fseek( File, p_Header.Offset_UV, SEEK_SET );
    fread( p_TexCoord, sizeof(E_MD2TextureCoord), p_Header.Nb_UV, File );

    // ... puis, le contenu de la classe E_MD2Triangle, ...
    fseek( File, p_Header.Offset_Triangles, SEEK_SET );
    fread( p_Triangle, sizeof(E_MD2Triangle), p_Header.Nb_Triangles, File );

    // ... et enfin, le contenu des commandes OpenGL.
    fseek( File, p_Header.Offset_GlCmds, SEEK_SET );
    fread( p_GlCmds, sizeof(int), p_Header.Nb_GlCmds, File );

    // On alloue ensuite de la m�moire pour les Frames.
    p_Frame = new E_MD2Frame[p_Header.Nb_Frames];

    // Puis, on teste que cette m�moire ait �t� correctement allou�e.
    if ( p_Frame == NULL )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // On pointe ensuite le curseur du fichier sur les frames.
    fseek( File, p_Header.Offset_Frames, SEEK_SET );

    // On lit chaque frame depuis le fichier.
    for ( int i = 0; i < p_Header.Nb_Frames; i++ )
    {
        // On alloue de la m�moire pour le frame � cr�er.
        p_Frame[i].Vertex = new E_MD2Vertex[p_Header.Nb_XYZ+1];

        // Puis, on teste si la m�moire a �t� correctement allou�e.
        if ( p_Frame[i].Vertex == NULL )
        {
            // Si ce n'est pas le cas, on clos le fichier, ...
            fclose( File );

            // ... et on retourne une erreur.
            return false;
        }

        // On lit ensuite les donn�es des sommets.
        fread( p_Frame[i].Scale,     sizeof(float) * 3, 1,  File );
        fread( p_Frame[i].Translate, sizeof(float) * 3, 1,  File );
        fread( p_Frame[i].Name,      sizeof(char),      16, File );
        fread( p_Frame[i].Vertex, sizeof(E_MD2Vertex), p_Header.Nb_XYZ, File );
    }

    // Apr�s la lecture, on clos le fichier.
    fclose( File );

    // On tente de cr�er la table des normales en m�moire.
    if ( CreateNormalsTable() == false )
    {
        // Si la cr�ation a �chou�, on retourne une erreur.
        return false;
    }

    // On cr�e ensuite un VertexBuffer capable de contenir les donn�es.
    if( FAILED( theD3DDevice->CreateVertexBuffer
                         ( p_Header.Nb_Triangles * 3 * sizeof(E_MD2_D3DVERTEX),
                           D3DUSAGE_WRITEONLY,
                           E_MD2_D3DVERTEX::FVF,
                           D3DPOOL_MANAGED,
                           &p_VertexBuffer,
                           NULL ) ) )
    {
        // Si la cr�ation a �chou�, on retourne une erreur.
        return false;
    }

    // Puis, on teste si un fichier de texture a �t� d�fini.
    if ( p_TextureFilename != NULL )
    {
        // Si c'est le cas, on tente de cr�er la texture.
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_TextureFilename,
                                               &p_Texture ) ) )
        {
            // Si la cr�ation a �chou�, on retourne une erreur.
            return false;
        }
    }
    else
    {
        // Si aucun nom de texture n'est d�fini, on retourne une erreur.
        return false;
    }

    // On initialise un buffer de mat�riau, pour le cas o� on utilise Direct3D
    // pour le rendu de la lumi�re.
    ZeroMemory( &p_Material, sizeof( p_Material ) );

    // Puis, on cr�e un mat�riau standard.
    p_Material.Diffuse.r = p_Material.Ambient.r = 1.0f;
    p_Material.Diffuse.g = p_Material.Ambient.g = 1.0f;
    p_Material.Diffuse.b = p_Material.Ambient.b = 1.0f;
    p_Material.Diffuse.a = p_Material.Ambient.a = 1.0f;

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet la gestion de l'animation de l'objet.
bool E_MD2Mesh::Animate()
{
    // On teste tout d'abord si le champ de lecture est correctement d�fini.
    if ( ( p_Animation.Start < 0 ) ||
         ( p_Animation.End   > p_Header.Nb_Frames - 1 ) )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Ensuite, on mets � jour les donn�es temporelles.
    p_LastTime = p_CurTime;
    p_CurTime  = (double)timeGetTime() / 1000.0f;

    // Puis, on calcule le temps �coul� entre les rendus de l'objet, pour
    // permettre une interpolation correcte des Frames.
    p_Interpolation += 10 * ( p_CurTime - p_LastTime );

    // On teste si le pourcentage d'interpolation est correct.
    if ( p_Interpolation >= 1.0f )
    {
        // Si le pourcentage d'interpolation est d�pass�, on mets � jour
        // les donn�es de l'animation. On remet l'interpolation � 0.
        p_Interpolation = 0.0f;

        // Puis, on passe � la Frame suivante.
        p_CurFrameID++;
        p_NextFrameID++;

        // On teste ensuite si le param�tre p_NextFrameID est valide.
        if ( p_NextFrameID >= p_Animation.End )
        {
            // Si ce n'est pas le cas, on ajuste la valeur.
            p_NextFrameID = p_Animation.Start;
        }

        // Puis, on teste si le param�tre p_CurFrame est valide.
        if ( p_CurFrameID >= p_Animation.End )
        {
            // Si ce n'est pas le cas, on ajuste la valeur.
            p_CurFrameID = p_Animation.Start;
        }
    }

    // Pour finir, on retourne le message de r�ussite.
    return true;
}

// Cette fonction permet d'obtenir la valeur de l'�clairage pr�-calcul�.
DWORD E_MD2Mesh::GetPreCalculatedLightning( D3DXVECTOR3 PolygonNormal )
{
    // On effectue un produit scalaire entre l'angle de la lumi�re et l'angle
    // de la normale du sommet.
    float LightDot = D3DXVec3Dot( &PolygonNormal, &p_LightDirection );

    // On teste si le r�sultat est plus petit que 0, afin d'�viter le double
    // sens de l'�clairage.
    if ( LightDot < 0.0f )
    {
        // Si c'est le cas, on remet le r�sultat � 0.
        LightDot = 0.0f;
    }

    // Puis, on calcule la composante rouge, verte et bleue pour la couleur
    // du sommet. En multipliant la couleur de la lumi�re avec le produit
    // scalaire des angles, on obtient le pourcentage de cette couleur � mixer
    // dans la couleur finale pour donner un effet d'�clairage. C'est le m�me
    // principe pour le calcul de la couleur dans Direct3D, sauf que le calcul
    // est plus simple ici, donc plus rapide - th�oriquement.
    int R = (int)( p_PreCalculatedLight.Light.r * LightDot ) +
                                                p_PreCalculatedLight.Ambient.r;
    int G = (int)( p_PreCalculatedLight.Light.g * LightDot ) +
                                                p_PreCalculatedLight.Ambient.g;
    int B = (int)( p_PreCalculatedLight.Light.b * LightDot ) +
                                                p_PreCalculatedLight.Ambient.b;

    // On teste que le r�sultat de la composante rouge soit correct.
    if ( R > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        R = 255;
    }

    // On teste que le r�sultat de la composante verte soit correct.
    if ( G > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        G = 255;
    }

    // On teste que le r�sultat de la composante bleue soit correct.
    if ( B > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        B = 255;
    }

    // Pour finir, on convertit les valeurs, et on retourne le r�sultat.
    return D3DCOLOR_XRGB( R, G, B );
}

// Cette fonction calcule l'interpolation entre deux vecteurs.
D3DXVECTOR3 E_MD2Mesh::GetInterpolation( D3DXVECTOR3 FirstVector,
                                         D3DXVECTOR3 SecondVector )
{
    D3DXVECTOR3 Interpolation;

    // En utilisant le temps �coul� entre le passage d'une Frame � l'autre, il
    // est possible de calculer une Frame interm�diaire. Cette technique,
    // appel�e interpolation, permet d'�viter une animation saccad�e - surtout
    // si les mouvements du personnage sont amples - tout en �conomisant
    // un maximum d'espace m�moire.
    Interpolation.x =
          FirstVector.x + p_Interpolation * ( SecondVector.x - FirstVector.x );
    Interpolation.y =
          FirstVector.y + p_Interpolation * ( SecondVector.y - FirstVector.y );
    Interpolation.z =
          FirstVector.z + p_Interpolation * ( SecondVector.z - FirstVector.z );

    return Interpolation;
}

// Cette fonction calcule un sommet � partir des donn�es du fichier MD2.
D3DXVECTOR3 E_MD2Mesh::GetVertice( E_MD2Frame*  theFrame,
                                   E_MD2Vertex* theVertex )
{
    float Vertice[3];

    // On calcule les 3 sommets du polygone � partir des donn�es compress�es du
    // fichier MD2. La variable v[i] contient les donn�es d'un sommet, mais si
    // vous regardez la structure E_MD2Vertex, vous constaterez que cette
    // variable est de type char, donc qu'elle contient des valeurs de 0 � 255.
    // Pour cr�er un polygone correct, on a besoin de d�compresser cette donn�e
    // en utilisant les variables Scale et Translate, afin de rendre au sommet
    // sa valeur d'origine.
    for ( int i = 0; i < 3; i++ )
    {
        Vertice[i] = ( theFrame->Scale[i] * theVertex->v[i] ) +
                                                        theFrame->Translate[i];
    }

    // On retourne la valeur de x. Parce-que le mod�le est d�fini dans un
    // syst�me de coordonn�es RH, et que Direct3D utilise plus souvent le
    // syst�me de coordonn�es LH, on inverse la valeur des X pour convertir le
    // syst�me de coordonn�es, et obtenir un affichage correct.
    return D3DXVECTOR3( -Vertice[0], Vertice[1], Vertice[2] );
}

// Cette fonction permet de retourner la couleur � appliquer au polygone.
DWORD E_MD2Mesh::GetVertexColor( D3DXVECTOR3 PolygonNormal )
{
    // On teste la valeur du mode d'�clairage.
    switch ( p_LightningMode )
    {
        // Le moteur d'�clairage est inconnu.
        case E_MD2_LIGHTNING_UNKNOW:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Le moteur d'�clairage est inactif.
        case E_MD2_LIGHTNING_NONE:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Le moteur d'�clairage est pr�-calcul�.
        case E_MD2_LIGHTNING_PRECALCULATED:
        {
            // On calcule la valeur de la couleur pour le sommet courant.
            return GetPreCalculatedLightning( PolygonNormal );
        }

        // Le moteur d'�clairage utilise Direct3D.
        case E_MD2_LIGHTNING_DIRECT3D:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Aucune s�lection n'est effectu�e, ou la s�lection est invalide.
        default:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }
    }
}

// Cette fonction permet de cr�er un VertexBuffer depuis les donn�es MD2.
bool E_MD2Mesh::PopulateVertexBuffer()
{
    // On commence par contr�ler que les donn�es d'indexage soient correctes.
    if ( ( p_CurFrameID  < 0 ) || ( p_CurFrameID  > p_Header.Nb_Frames - 1 ) ||
         ( p_NextFrameID < 0 ) || ( p_NextFrameID > p_Header.Nb_Frames - 1 ) )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On teste ensuite si la table des normales pr�-calcul�es existe en
    // m�moire.
    if ( p_NormalsTable == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On obtient les donn�es de la Frame courante et de la Frame suivante.
    E_MD2Frame* theCurFrame  = &p_Frame[p_CurFrameID];
    E_MD2Frame* theNextFrame = &p_Frame[p_NextFrameID];

    // On teste ensuite que les pointeurs des Frames soient correctes.
    if ( theCurFrame == NULL || theNextFrame == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    E_MD2_D3DVERTEX* p_Vertices;

    // On verrouille le VertexBuffer, pour permettre l'acc�s en �criture.
    if( FAILED( p_VertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
    {
        // Si le verrouillage �choue, on retourne une erreur.
        return false;
    }

    // Puis, on traite chaque polygone de l'objet.
    for ( int i = 0; i < p_Header.Nb_Triangles; i++ )
    {
        // Le polygone contient 3 sommets.
        for ( int j = 0; j < 3; j++ )
        {
            // On obtient les donn�es du sommet courant, ...
            E_MD2Vertex* theCurVertex =
                &theCurFrame->Vertex[p_Triangle[i].Index_XYZ[j]];

            // .. ainsi que celles du sommet suivant.
            E_MD2Vertex* theNextVertex =
                &theNextFrame->Vertex[p_Triangle[i].Index_XYZ[j]];

            // On teste si les pointeurs des sommets sont corrects.
            if ( theCurVertex == NULL || theNextVertex == NULL )
            {
                // Si ce n'est pas le cas, on d�verrouille le VertexBuffer, et
                // on retourne une erreur.
                p_VertexBuffer->Unlock();
                return false;
            }

            // Puis, on calcule l'index du sommet auquel on veut acc�der dans
            // le VertexBuffer.
            int vIndex = (i * 3 ) + j;

            // On obtient les donn�es des normales des deux Frames �
            // interpoler.
            D3DXVECTOR3 CurNormal  = p_NormalsTable
                                              [theCurVertex->LightNormalIndex];
            D3DXVECTOR3 NextNormal = p_NormalsTable
                                             [theNextVertex->LightNormalIndex];

            // Puis, on calcule l'interpolation entre les deux normales, et on
            // inscrit le r�sultat dans le VertexBuffer.
            p_Vertices[vIndex].Normal =
                GetInterpolation( CurNormal, NextNormal );

            // On retourne la valeur de x. Parce-que le mod�le est d�fini dans
            // un syst�me de coordonn�es RH, et que Direct3D utilise plus
            // souvent le syst�me de coordonn�es LH, on inverse la valeur des X
            // pour convertir le syst�me de coordonn�es, et obtenir un
            // affichage correct.
            p_Vertices[vIndex].Normal.x = - p_Vertices[vIndex].Normal.x;

            // On obtient ensuite les donn�es des sommets des deux Frames �
            // interpoler.
            D3DXVECTOR3 CurVertice  = GetVertice( theCurFrame, theCurVertex );
            D3DXVECTOR3 NextVertice = GetVertice( theNextFrame,
                                                  theNextVertex );

            // Puis, on calcule l'interpolation entre les deux sommets,
            // et on inscrit le r�sultat dans le VertexBuffer.
            p_Vertices[vIndex].Position =
                GetInterpolation( CurVertice, NextVertice );

            // Ensuite, on calcule les coordonn�es des textures, et on inscrit
            // le r�sultat dans le VertexBuffer.
            p_Vertices[vIndex].Tu    =
                (FLOAT)( p_TexCoord[p_Triangle[i].Index_UV[j]].u ) /
                                                            p_Header.SkinWidth;
            p_Vertices[vIndex].Tv    =
                (FLOAT)( p_TexCoord[p_Triangle[i].Index_UV[j]].v ) /
                                                           p_Header.SkinHeight;

            // Pour finir, on calcule la valeur de la couleur du sommet, et on
            // inscrit le r�sultat dans le VertexBuffer.
            p_Vertices[vIndex].Color =
                GetVertexColor( p_Vertices[vIndex].Normal );
        }
    }

    // Enfin, on lib�re l'acc�s au VertexBuffer, ...
    p_VertexBuffer->Unlock();

    // Puis, on retourne le message de r�ussite.
    return true;
}

// Cette fonction permet le rendu de l'objet.
void E_MD2Mesh::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On commence par sauvegarder l'�tat du Device Direct3D.
    PushDeviceProperties( theD3DDevice );

    // On calcule la matrice de monde pour le rendu, et on configure Direct3D.
    D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
    theD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

    // Puis, on inverse la matrice de monde, ...
    D3DXMATRIXA16 InvWorldMatrix;
    D3DXMatrixInverse( &InvWorldMatrix, NULL, &WorldMatrix );

    // ... ainsi que la valeur de la direction de la lumi�re, ...
    D3DXVECTOR3 Direction = -p_PreCalculatedLight.Direction;

    // ... pour effectuer la transformation de la direction de la lumi�re, afin
    // de travailler avec un rep�re des coordonn�es correct.
    D3DXVec3TransformNormal( &p_LightDirection, &Direction, &InvWorldMatrix );

    // Ensuite, on commande la mise � jour des donn�es d'animation, ...
    if ( Animate() == false )
    {
        return;
    }

    // ... ainsi que le calcul d'un nouveau VertexBuffer.
    if ( PopulateVertexBuffer() == false )
    {
        return;
    }

    // On teste ensuite si le moteur d'alpha blending est actif.
    if ( p_IsAlphaBlended == true )
    {
        // Si oui, on configure l'alpha blending pour le rendu.
        SetAlphaBlending( theD3DDevice );
    }

    // Puis, on configure les propri�t�s des textures.
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

    // On indique le VertexBuffer comme source de primitive.
    theD3DDevice->SetStreamSource( 0,
                                   p_VertexBuffer,
                                   0,
                                   sizeof( E_MD2_D3DVERTEX ) );

    // On configure le Flexible Vertex Format, le mat�riau et la texture.
    theD3DDevice->SetFVF     ( E_MD2_D3DVERTEX::FVF );
    theD3DDevice->SetMaterial( &p_Material );
    theD3DDevice->SetTexture ( 0, p_Texture );

    // Puis, on commande le rendu de l'objet. Nous n'utiliserons pas les
    // commandes sp�cifiques � OpenGL, car celles-ci sont optimis�es pour
    // l'utilisation de cette API. Avec Direct3D, les choses se compliquent.
    // Avec OpenGL, le d�veloppeur n'a pas � s'occuper de cr�er un VertexBuffer
    // pour son rendu, celui-ci est cr�e de fa�on interne. Mais avec Direct3D,
    // il faudrait cr�er un VertexBuffer par commande, d'une longeur
    // quelconque, et donc il faudrait d�truire l'ancien buffer, puis
    // recalculer le nouveau buffer pour rendre la commande. Je ne sais pas si
    // le gain de temps et d'espace apport� par cette m�thode est encore
    // d'actualit� apr�s une telle op�ration.
    theD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST,
                                 0,
                                 p_Header.Nb_Triangles );

    // Enfin, on restaure les propri�t�s d'affichage de Direct3D.
    PopDeviceProperties( theD3DDevice );
}

// Cette fonction permet de restaurer l'objet apr�s un changement de Device.
bool E_MD2Mesh::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On lib�re les ressources utilis�es par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // On lib�re les ressources utilis�es par le VertexBuffer.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // On recr�e ensuite le VertexBuffer.
    if( FAILED( theD3DDevice->CreateVertexBuffer
                         ( p_Header.Nb_Triangles * 3 * sizeof(E_MD2_D3DVERTEX),
                           D3DUSAGE_WRITEONLY,
                           E_MD2_D3DVERTEX::FVF,
                           D3DPOOL_MANAGED,
                           &p_VertexBuffer,
                           NULL ) ) )
    {
        // Si la cr�ation a �chou�, on retourne une erreur.
        return false;
    }

    // Puis, on tente de recr�er la texture.
    if ( p_TextureFilename != NULL )
    {
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_TextureFilename,
                                               &p_Texture ) ) )
        {
            // Si la cr�ation a �chou�, on retourne une erreur.
            return false;
        }
    }

    // Pour finir, on retourne le message de r�ussite.
    return true;
}
