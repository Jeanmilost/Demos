/*****************************************************************************
 * ==> Classe E_MD2Mesh -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un objet animé, en utilisant la     *
 *               méthode MD2 de Quake.                                       *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_md2mesh.h"

// ********************* Diverses données pour le rendu. **********************
// On déclare ici diverses structures et énumérateurs pour le rendu de l'objet.

// On initialise ici les propriétés des sommets des polygones de l'objet
const DWORD E_MD2_D3DVERTEX::FVF = D3DFVF_XYZ     |
                                   D3DFVF_NORMAL  |
                                   D3DFVF_DIFFUSE |
                                   D3DFVF_TEX1;

// *************************** Structure E_MD2Color ***************************
// Cette structure permet de contenir les données de la couleur de la lumière
// pré-calculée de l'objet. Ceci est valable pour la lumière d'ambiance et pour
// la lumière directionnelle.

// Constructeur de la classe E_MD2Color.
E_MD2Color::E_MD2Color()
{
    r = 0;
    g = 0;
    b = 0;
}

// Deuxième constructeur de la classe E_MD2Color.
E_MD2Color::E_MD2Color( int Red, int Green, int Blue )
{
    r = Red;
    g = Green;
    b = Blue;
}

// *************************** Structure E_MD2Light ***************************
// Cette structure permet de contenir les données pour créer la lumière pré-
// calculée de l'objet.

// Constructeur de la classe E_MD2Light.
E_MD2Light::E_MD2Light()
{
    Light     = E_MD2Color( 0, 0, 0 );
    Ambient   = E_MD2Color( 0, 0, 0 );
    Direction = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

// Deuxième constructeur de la classe E_MD2Light.
E_MD2Light::E_MD2Light( E_MD2Color  Light,
                        E_MD2Color  Ambient,
                        D3DXVECTOR3 Direction )
{
    this->Light     = Light;
    this->Ambient   = Ambient;
    this->Direction = Direction;
}

// ************************* Structure E_MD2Animation *************************
// Cette structure permet de contenir les données nécessaire pour configurer
// l'animation courante de l'objet.

// Constructeur par défaut de la structure E_MD2Animation.
E_MD2Animation::E_MD2Animation()
{
    Start = 0;
    End   = 0;
    Name  = NULL;
}

// Deuxième constructeur de la structure E_MD2Animation.
E_MD2Animation::E_MD2Animation( int Start, int End, LPSTR Name )
{
    this->Start = Start;
    this->End   = End;
    this->Name  = Name;
}

// ***************************** Classe E_MD2Mesh *****************************
// Cette classe représente un objet animé. Les algorythmes d'animation et de
// rendu sont ceux utilisés par les fichiers MD2 de Quake.

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
    // On libère les ressources utilisées par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // On libère les ressources utilisées par le VertexBuffer.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // On libère les ressources utilisées par la table des normales.
    if ( p_NormalsTable != NULL )
    {
        delete[] p_NormalsTable;
        p_NormalsTable = NULL;
    }

    // On libère les ressources utilisées par l'objet MD2 Skin.
    if ( p_Skin != NULL )
    {
        delete [] p_Skin;
        p_Skin = NULL;
    }

    // On libère les ressources utilisées par l'objet MD2 TextureCoords.
    if ( p_TexCoord != NULL )
    {
        delete [] p_TexCoord;
        p_TexCoord = NULL;
    }

    // On libère les ressources utilisées par l'objet MD2 Triangle.
    if ( p_Triangle != NULL )
    {
        delete [] p_Triangle;
        p_Triangle = NULL;
    }

    // On libère les ressources utilisées par l'objet MD2 Frame.
    if ( p_Frame != NULL )
    {
        // D'abord les données contenues dans le conteneur de Frames.
        for ( int i = 0; i < p_Header.Nb_Frames; i++ )
        {
            if ( p_Frame[i].Vertex != NULL )
            {
                delete[] p_Frame[i].Vertex;
                p_Frame[i].Vertex = NULL;
            }
        }

        // Puis, le conteneur de Frames lui-même.
        delete [] p_Frame;
        p_Frame = NULL;
    }

    // On libère les ressources utilisées par l'objet MD2 GlCmds.
    if ( p_GlCmds != NULL )
    {
        delete [] p_GlCmds;
        p_GlCmds = NULL;
    }
}

// Obtient le nom du fichier du modèle.
LPSTR E_MD2Mesh::GetModelFilename()
{
    return p_ModelFilename;
}

// Définit le nom du fichier du modèle.
void E_MD2Mesh::SetModelFilename( LPSTR Filename )
{
    p_ModelFilename = Filename;
}

// Obtient le nom du fichier de la texture.
LPSTR E_MD2Mesh::GetTextureFilename()
{
    return p_TextureFilename;
}

// Définit le nom du fichier de la texture.
void E_MD2Mesh::SetTextureFilename( LPSTR Filename )
{
    p_TextureFilename = Filename;
}

// Obtient le nom du fichier de la table des normales.
LPSTR E_MD2Mesh::GetNormalsTableFilename()
{
    return p_NormalsTableFilename;
}

// Définit le nom du fichier de la table des normales.
void E_MD2Mesh::SetNormalsTableFilename( LPSTR Filename )
{
    p_NormalsTableFilename = Filename;
}

// Obtient l'angle de rotation de l'objet sur l'axe X.
float E_MD2Mesh::GetRotationX()
{
    return p_rX;
}

// Définit l'angle de rotation de l'objet sur l'axe X.
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

// Définit l'angle de rotation de l'objet sur l'axe Y.
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

// Définit l'angle de rotation de l'objet sur l'axe Z.
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

// Définit la valeur de la position de l'objet sur l'axe X.
void E_MD2Mesh::SetTranslationX( float Value )
{
    p_tX = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Y.
float E_MD2Mesh::GetTranslationY()
{
    return p_tY;
}

// Définit la valeur de la position de l'objet sur l'axe Y.
void E_MD2Mesh::SetTranslationY( float Value )
{
    p_tY = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Z.
float E_MD2Mesh::GetTranslationZ()
{
    return p_tZ;
}

// Définit la valeur de la position de l'objet sur l'axe Z.
void E_MD2Mesh::SetTranslationZ( float Value )
{
    p_tZ = Value;
}

// Définit le moteur d'éclairage utilisé pour le rendu.
void E_MD2Mesh::SetLightningMode( E_MD2LightningMode Mode )
{
    p_LightningMode = Mode;
}

// Définit les paramètres de la lumière pré-calculée de l'objet.
void E_MD2Mesh::SetPreCalculatedLightParameters( E_MD2Light Light )
{
    p_PreCalculatedLight = Light;
}

// Définit les paramètres de l'animation courante de l'objet.
bool E_MD2Mesh::SetAnimationParameters( E_MD2Animation Animation )
{
    // On teste tout d'abord que les valeurs entrées ne soient pas hors de la
    // plage possible pour cet objet.
    if ( Animation.Start < 0 || Animation.End > p_Header.Nb_Frames )
    {
        // Si les valeurs sont inadmissibles, on retourne une erreur.
        return false;
    }

    // Si les valeurs sont correctes, on définit les paramètres de l'animation.
    p_Animation     = Animation;
    p_CurFrameID    = Animation.Start;
    p_NextFrameID   = Animation.Start + 1;
    p_Interpolation = 0.0f;

    // Pour finir, on retourne le message de réussite.
    return true;
}

// Définit l'état du moteur d'Aplha Blending.
void E_MD2Mesh::SetAlphaBlending( bool Value )
{
    p_IsAlphaBlended = Value;
}

// Retourne l'état du moteur d'aplha Blending.
bool E_MD2Mesh::IsAlphaBlended()
{
    return p_IsAlphaBlended;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_MD2Mesh::SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // En définissant la source et la destination de l'alpha blending à ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // désactivé pour cette opération, l'objet doit donc être rendu à la
    // fin de la scène, éventuellement trié par ordre de profondeur s'il y a
    // plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
    // correct.
    theD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    theD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
    theD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}

// Cette fonction sauvegarde l'état du Device Direct3D avant utilisation.
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

// Cette fonction restaure l'état du Device Direct3D après utilisation.
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

// Cette fonction crée la table des normales pré-calculées depuis le fichier.
bool E_MD2Mesh::CreateNormalsTable()
{
    // Pointeur vers le fichier.
    FILE* File = NULL;

    float Version = 0.0f;

    // On teste tout d'abord qu'un nom de fichier ait bien été défini.
    if ( p_NormalsTableFilename == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On tente d'ouvrir le fichier en lecture.
    File = fopen( p_NormalsTableFilename, "rb" );

    // On teste ensuite si le fichier a bien été ouvert.
    if( File == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on extrait le numéro de version du fichier.
    fread( &Version, sizeof(float), 1, File );

    // On teste si le numéro de version est correct.
    if ( Version != MD2_NVERSION )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // On lit ensuite la longeur de la table dans le fichier.
    fread( &p_NormalsTableLength, sizeof(int), 1, File );

    // Puis, on alloue l'espace nécessaire pour la table.
    p_NormalsTable = new D3DXVECTOR3[p_NormalsTableLength];

    // On teste que l'espace nécessaire soit bien alloué en mémoire.
    if ( p_NormalsTable == NULL )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // Puis, on copie les données de la table en mémoire, depuis le fichier.
    for ( int i = 0; i < p_NormalsTableLength; i++ )
    {
        fread( &p_NormalsTable[i], sizeof(D3DXVECTOR3), 1, File );
    }

    // Pour fnir, on clos le fichier, ...
    fclose( File );

    // ... et on retourne un message de réussite.
    return true;
}

// Cette fonction permet d'initialiser l'objet.
bool E_MD2Mesh::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // Pointeur vers le fichier.
    FILE* File = NULL;

    // On teste tout d'abord qu'un nom de fichier ait bien été défini.
    if ( p_ModelFilename == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On tente ensuite d'ouvrir le fichier.
    File = fopen( p_ModelFilename, "rb" );

    // On teste si le fichier a bien été ouvert.
    if( File == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on extrait l'en-tête MD2 du fichier.
    fread( &p_Header, sizeof(E_MD2Header), 1, File );

    // On teste ensuite que les paramètres de l'en-tête MD2 soient valides.
    if ( ( p_Header.ID != MD2_IDENT ) && ( p_Header.Version != MD2_VERSION ) )
    {
        // Si ce n'est pas le cas, on clos le fichier, ...
        fclose( File );

        // ... et on retourne une erreur.
        return false;
    }

    // Si le format du fichier est correct, on alloue la mémoire nécessaire aux
    // données à extraire.
    p_Skin     = new E_MD2Skin        [p_Header.Nb_Skins     + 1];
    p_TexCoord = new E_MD2TextureCoord[p_Header.Nb_UV        + 1];
    p_Triangle = new E_MD2Triangle    [p_Header.Nb_Triangles + 1];
    p_GlCmds   = new int              [p_Header.Nb_GlCmds    + 1];

    // On teste ensuite si la mémoire a été correctement allouée.
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

    // On lit les données depuis le fichier. D'abord le contenu de la classe
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

    // On alloue ensuite de la mémoire pour les Frames.
    p_Frame = new E_MD2Frame[p_Header.Nb_Frames];

    // Puis, on teste que cette mémoire ait été correctement allouée.
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
        // On alloue de la mémoire pour le frame à créer.
        p_Frame[i].Vertex = new E_MD2Vertex[p_Header.Nb_XYZ+1];

        // Puis, on teste si la mémoire a été correctement allouée.
        if ( p_Frame[i].Vertex == NULL )
        {
            // Si ce n'est pas le cas, on clos le fichier, ...
            fclose( File );

            // ... et on retourne une erreur.
            return false;
        }

        // On lit ensuite les données des sommets.
        fread( p_Frame[i].Scale,     sizeof(float) * 3, 1,  File );
        fread( p_Frame[i].Translate, sizeof(float) * 3, 1,  File );
        fread( p_Frame[i].Name,      sizeof(char),      16, File );
        fread( p_Frame[i].Vertex, sizeof(E_MD2Vertex), p_Header.Nb_XYZ, File );
    }

    // Après la lecture, on clos le fichier.
    fclose( File );

    // On tente de créer la table des normales en mémoire.
    if ( CreateNormalsTable() == false )
    {
        // Si la création a échoué, on retourne une erreur.
        return false;
    }

    // On crée ensuite un VertexBuffer capable de contenir les données.
    if( FAILED( theD3DDevice->CreateVertexBuffer
                         ( p_Header.Nb_Triangles * 3 * sizeof(E_MD2_D3DVERTEX),
                           D3DUSAGE_WRITEONLY,
                           E_MD2_D3DVERTEX::FVF,
                           D3DPOOL_MANAGED,
                           &p_VertexBuffer,
                           NULL ) ) )
    {
        // Si la création a échoué, on retourne une erreur.
        return false;
    }

    // Puis, on teste si un fichier de texture a été défini.
    if ( p_TextureFilename != NULL )
    {
        // Si c'est le cas, on tente de créer la texture.
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_TextureFilename,
                                               &p_Texture ) ) )
        {
            // Si la création a échoué, on retourne une erreur.
            return false;
        }
    }
    else
    {
        // Si aucun nom de texture n'est défini, on retourne une erreur.
        return false;
    }

    // On initialise un buffer de matériau, pour le cas où on utilise Direct3D
    // pour le rendu de la lumière.
    ZeroMemory( &p_Material, sizeof( p_Material ) );

    // Puis, on crée un matériau standard.
    p_Material.Diffuse.r = p_Material.Ambient.r = 1.0f;
    p_Material.Diffuse.g = p_Material.Ambient.g = 1.0f;
    p_Material.Diffuse.b = p_Material.Ambient.b = 1.0f;
    p_Material.Diffuse.a = p_Material.Ambient.a = 1.0f;

    // Pour finir, on retourne un message de réussite.
    return true;
}

// Cette fonction permet la gestion de l'animation de l'objet.
bool E_MD2Mesh::Animate()
{
    // On teste tout d'abord si le champ de lecture est correctement défini.
    if ( ( p_Animation.Start < 0 ) ||
         ( p_Animation.End   > p_Header.Nb_Frames - 1 ) )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Ensuite, on mets à jour les données temporelles.
    p_LastTime = p_CurTime;
    p_CurTime  = (double)timeGetTime() / 1000.0f;

    // Puis, on calcule le temps écoulé entre les rendus de l'objet, pour
    // permettre une interpolation correcte des Frames.
    p_Interpolation += 10 * ( p_CurTime - p_LastTime );

    // On teste si le pourcentage d'interpolation est correct.
    if ( p_Interpolation >= 1.0f )
    {
        // Si le pourcentage d'interpolation est dépassé, on mets à jour
        // les données de l'animation. On remet l'interpolation à 0.
        p_Interpolation = 0.0f;

        // Puis, on passe à la Frame suivante.
        p_CurFrameID++;
        p_NextFrameID++;

        // On teste ensuite si le paramètre p_NextFrameID est valide.
        if ( p_NextFrameID >= p_Animation.End )
        {
            // Si ce n'est pas le cas, on ajuste la valeur.
            p_NextFrameID = p_Animation.Start;
        }

        // Puis, on teste si le paramètre p_CurFrame est valide.
        if ( p_CurFrameID >= p_Animation.End )
        {
            // Si ce n'est pas le cas, on ajuste la valeur.
            p_CurFrameID = p_Animation.Start;
        }
    }

    // Pour finir, on retourne le message de réussite.
    return true;
}

// Cette fonction permet d'obtenir la valeur de l'éclairage pré-calculé.
DWORD E_MD2Mesh::GetPreCalculatedLightning( D3DXVECTOR3 PolygonNormal )
{
    // On effectue un produit scalaire entre l'angle de la lumière et l'angle
    // de la normale du sommet.
    float LightDot = D3DXVec3Dot( &PolygonNormal, &p_LightDirection );

    // On teste si le résultat est plus petit que 0, afin d'éviter le double
    // sens de l'éclairage.
    if ( LightDot < 0.0f )
    {
        // Si c'est le cas, on remet le résultat à 0.
        LightDot = 0.0f;
    }

    // Puis, on calcule la composante rouge, verte et bleue pour la couleur
    // du sommet. En multipliant la couleur de la lumière avec le produit
    // scalaire des angles, on obtient le pourcentage de cette couleur à mixer
    // dans la couleur finale pour donner un effet d'éclairage. C'est le même
    // principe pour le calcul de la couleur dans Direct3D, sauf que le calcul
    // est plus simple ici, donc plus rapide - théoriquement.
    int R = (int)( p_PreCalculatedLight.Light.r * LightDot ) +
                                                p_PreCalculatedLight.Ambient.r;
    int G = (int)( p_PreCalculatedLight.Light.g * LightDot ) +
                                                p_PreCalculatedLight.Ambient.g;
    int B = (int)( p_PreCalculatedLight.Light.b * LightDot ) +
                                                p_PreCalculatedLight.Ambient.b;

    // On teste que le résultat de la composante rouge soit correct.
    if ( R > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        R = 255;
    }

    // On teste que le résultat de la composante verte soit correct.
    if ( G > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        G = 255;
    }

    // On teste que le résultat de la composante bleue soit correct.
    if ( B > 255 )
    {
        // Si ce n'est pas le cas, on corrige la valeur.
        B = 255;
    }

    // Pour finir, on convertit les valeurs, et on retourne le résultat.
    return D3DCOLOR_XRGB( R, G, B );
}

// Cette fonction calcule l'interpolation entre deux vecteurs.
D3DXVECTOR3 E_MD2Mesh::GetInterpolation( D3DXVECTOR3 FirstVector,
                                         D3DXVECTOR3 SecondVector )
{
    D3DXVECTOR3 Interpolation;

    // En utilisant le temps écoulé entre le passage d'une Frame à l'autre, il
    // est possible de calculer une Frame intermédiaire. Cette technique,
    // appelée interpolation, permet d'éviter une animation saccadée - surtout
    // si les mouvements du personnage sont amples - tout en économisant
    // un maximum d'espace mémoire.
    Interpolation.x =
          FirstVector.x + p_Interpolation * ( SecondVector.x - FirstVector.x );
    Interpolation.y =
          FirstVector.y + p_Interpolation * ( SecondVector.y - FirstVector.y );
    Interpolation.z =
          FirstVector.z + p_Interpolation * ( SecondVector.z - FirstVector.z );

    return Interpolation;
}

// Cette fonction calcule un sommet à partir des données du fichier MD2.
D3DXVECTOR3 E_MD2Mesh::GetVertice( E_MD2Frame*  theFrame,
                                   E_MD2Vertex* theVertex )
{
    float Vertice[3];

    // On calcule les 3 sommets du polygone à partir des données compressées du
    // fichier MD2. La variable v[i] contient les données d'un sommet, mais si
    // vous regardez la structure E_MD2Vertex, vous constaterez que cette
    // variable est de type char, donc qu'elle contient des valeurs de 0 à 255.
    // Pour créer un polygone correct, on a besoin de décompresser cette donnée
    // en utilisant les variables Scale et Translate, afin de rendre au sommet
    // sa valeur d'origine.
    for ( int i = 0; i < 3; i++ )
    {
        Vertice[i] = ( theFrame->Scale[i] * theVertex->v[i] ) +
                                                        theFrame->Translate[i];
    }

    // On retourne la valeur de x. Parce-que le modèle est défini dans un
    // système de coordonnées RH, et que Direct3D utilise plus souvent le
    // système de coordonnées LH, on inverse la valeur des X pour convertir le
    // système de coordonnées, et obtenir un affichage correct.
    return D3DXVECTOR3( -Vertice[0], Vertice[1], Vertice[2] );
}

// Cette fonction permet de retourner la couleur à appliquer au polygone.
DWORD E_MD2Mesh::GetVertexColor( D3DXVECTOR3 PolygonNormal )
{
    // On teste la valeur du mode d'éclairage.
    switch ( p_LightningMode )
    {
        // Le moteur d'éclairage est inconnu.
        case E_MD2_LIGHTNING_UNKNOW:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Le moteur d'éclairage est inactif.
        case E_MD2_LIGHTNING_NONE:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Le moteur d'éclairage est pré-calculé.
        case E_MD2_LIGHTNING_PRECALCULATED:
        {
            // On calcule la valeur de la couleur pour le sommet courant.
            return GetPreCalculatedLightning( PolygonNormal );
        }

        // Le moteur d'éclairage utilise Direct3D.
        case E_MD2_LIGHTNING_DIRECT3D:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }

        // Aucune sélection n'est effectuée, ou la sélection est invalide.
        default:
        {
            // On retourne une couleur blanche.
            return 0xFFFFFF;
        }
    }
}

// Cette fonction permet de créer un VertexBuffer depuis les données MD2.
bool E_MD2Mesh::PopulateVertexBuffer()
{
    // On commence par contrôler que les données d'indexage soient correctes.
    if ( ( p_CurFrameID  < 0 ) || ( p_CurFrameID  > p_Header.Nb_Frames - 1 ) ||
         ( p_NextFrameID < 0 ) || ( p_NextFrameID > p_Header.Nb_Frames - 1 ) )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On teste ensuite si la table des normales pré-calculées existe en
    // mémoire.
    if ( p_NormalsTable == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On obtient les données de la Frame courante et de la Frame suivante.
    E_MD2Frame* theCurFrame  = &p_Frame[p_CurFrameID];
    E_MD2Frame* theNextFrame = &p_Frame[p_NextFrameID];

    // On teste ensuite que les pointeurs des Frames soient correctes.
    if ( theCurFrame == NULL || theNextFrame == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    E_MD2_D3DVERTEX* p_Vertices;

    // On verrouille le VertexBuffer, pour permettre l'accès en écriture.
    if( FAILED( p_VertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
    {
        // Si le verrouillage échoue, on retourne une erreur.
        return false;
    }

    // Puis, on traite chaque polygone de l'objet.
    for ( int i = 0; i < p_Header.Nb_Triangles; i++ )
    {
        // Le polygone contient 3 sommets.
        for ( int j = 0; j < 3; j++ )
        {
            // On obtient les données du sommet courant, ...
            E_MD2Vertex* theCurVertex =
                &theCurFrame->Vertex[p_Triangle[i].Index_XYZ[j]];

            // .. ainsi que celles du sommet suivant.
            E_MD2Vertex* theNextVertex =
                &theNextFrame->Vertex[p_Triangle[i].Index_XYZ[j]];

            // On teste si les pointeurs des sommets sont corrects.
            if ( theCurVertex == NULL || theNextVertex == NULL )
            {
                // Si ce n'est pas le cas, on déverrouille le VertexBuffer, et
                // on retourne une erreur.
                p_VertexBuffer->Unlock();
                return false;
            }

            // Puis, on calcule l'index du sommet auquel on veut accéder dans
            // le VertexBuffer.
            int vIndex = (i * 3 ) + j;

            // On obtient les données des normales des deux Frames à
            // interpoler.
            D3DXVECTOR3 CurNormal  = p_NormalsTable
                                              [theCurVertex->LightNormalIndex];
            D3DXVECTOR3 NextNormal = p_NormalsTable
                                             [theNextVertex->LightNormalIndex];

            // Puis, on calcule l'interpolation entre les deux normales, et on
            // inscrit le résultat dans le VertexBuffer.
            p_Vertices[vIndex].Normal =
                GetInterpolation( CurNormal, NextNormal );

            // On retourne la valeur de x. Parce-que le modèle est défini dans
            // un système de coordonnées RH, et que Direct3D utilise plus
            // souvent le système de coordonnées LH, on inverse la valeur des X
            // pour convertir le système de coordonnées, et obtenir un
            // affichage correct.
            p_Vertices[vIndex].Normal.x = - p_Vertices[vIndex].Normal.x;

            // On obtient ensuite les données des sommets des deux Frames à
            // interpoler.
            D3DXVECTOR3 CurVertice  = GetVertice( theCurFrame, theCurVertex );
            D3DXVECTOR3 NextVertice = GetVertice( theNextFrame,
                                                  theNextVertex );

            // Puis, on calcule l'interpolation entre les deux sommets,
            // et on inscrit le résultat dans le VertexBuffer.
            p_Vertices[vIndex].Position =
                GetInterpolation( CurVertice, NextVertice );

            // Ensuite, on calcule les coordonnées des textures, et on inscrit
            // le résultat dans le VertexBuffer.
            p_Vertices[vIndex].Tu    =
                (FLOAT)( p_TexCoord[p_Triangle[i].Index_UV[j]].u ) /
                                                            p_Header.SkinWidth;
            p_Vertices[vIndex].Tv    =
                (FLOAT)( p_TexCoord[p_Triangle[i].Index_UV[j]].v ) /
                                                           p_Header.SkinHeight;

            // Pour finir, on calcule la valeur de la couleur du sommet, et on
            // inscrit le résultat dans le VertexBuffer.
            p_Vertices[vIndex].Color =
                GetVertexColor( p_Vertices[vIndex].Normal );
        }
    }

    // Enfin, on libère l'accès au VertexBuffer, ...
    p_VertexBuffer->Unlock();

    // Puis, on retourne le message de réussite.
    return true;
}

// Cette fonction permet le rendu de l'objet.
void E_MD2Mesh::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On commence par sauvegarder l'état du Device Direct3D.
    PushDeviceProperties( theD3DDevice );

    // On calcule la matrice de monde pour le rendu, et on configure Direct3D.
    D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
    theD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

    // Puis, on inverse la matrice de monde, ...
    D3DXMATRIXA16 InvWorldMatrix;
    D3DXMatrixInverse( &InvWorldMatrix, NULL, &WorldMatrix );

    // ... ainsi que la valeur de la direction de la lumière, ...
    D3DXVECTOR3 Direction = -p_PreCalculatedLight.Direction;

    // ... pour effectuer la transformation de la direction de la lumière, afin
    // de travailler avec un repère des coordonnées correct.
    D3DXVec3TransformNormal( &p_LightDirection, &Direction, &InvWorldMatrix );

    // Ensuite, on commande la mise à jour des données d'animation, ...
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

    // Puis, on configure les propriétés des textures.
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

    // On indique le VertexBuffer comme source de primitive.
    theD3DDevice->SetStreamSource( 0,
                                   p_VertexBuffer,
                                   0,
                                   sizeof( E_MD2_D3DVERTEX ) );

    // On configure le Flexible Vertex Format, le matériau et la texture.
    theD3DDevice->SetFVF     ( E_MD2_D3DVERTEX::FVF );
    theD3DDevice->SetMaterial( &p_Material );
    theD3DDevice->SetTexture ( 0, p_Texture );

    // Puis, on commande le rendu de l'objet. Nous n'utiliserons pas les
    // commandes spécifiques à OpenGL, car celles-ci sont optimisées pour
    // l'utilisation de cette API. Avec Direct3D, les choses se compliquent.
    // Avec OpenGL, le développeur n'a pas à s'occuper de créer un VertexBuffer
    // pour son rendu, celui-ci est crée de façon interne. Mais avec Direct3D,
    // il faudrait créer un VertexBuffer par commande, d'une longeur
    // quelconque, et donc il faudrait détruire l'ancien buffer, puis
    // recalculer le nouveau buffer pour rendre la commande. Je ne sais pas si
    // le gain de temps et d'espace apporté par cette méthode est encore
    // d'actualité après une telle opération.
    theD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST,
                                 0,
                                 p_Header.Nb_Triangles );

    // Enfin, on restaure les propriétés d'affichage de Direct3D.
    PopDeviceProperties( theD3DDevice );
}

// Cette fonction permet de restaurer l'objet après un changement de Device.
bool E_MD2Mesh::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On libère les ressources utilisées par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // On libère les ressources utilisées par le VertexBuffer.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // On recrée ensuite le VertexBuffer.
    if( FAILED( theD3DDevice->CreateVertexBuffer
                         ( p_Header.Nb_Triangles * 3 * sizeof(E_MD2_D3DVERTEX),
                           D3DUSAGE_WRITEONLY,
                           E_MD2_D3DVERTEX::FVF,
                           D3DPOOL_MANAGED,
                           &p_VertexBuffer,
                           NULL ) ) )
    {
        // Si la création a échoué, on retourne une erreur.
        return false;
    }

    // Puis, on tente de recréer la texture.
    if ( p_TextureFilename != NULL )
    {
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_TextureFilename,
                                               &p_Texture ) ) )
        {
            // Si la création a échoué, on retourne une erreur.
            return false;
        }
    }

    // Pour finir, on retourne le message de réussite.
    return true;
}
