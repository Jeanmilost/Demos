/*****************************************************************************
 * ==> Classe E_Sprite ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe gère les sprites dans le monde 3D.             *
 * Version     : 1.1                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_sprite.h"

// On initialise ici les propriétés des sommets des polygones du sprite.
const DWORD SPRITE_VERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

// Constructeur de la classe E_Sprite.
E_Sprite::E_Sprite()
{
    p_PolygonList     = NULL;
    p_VertexBuffer    = NULL;
    p_Texture         = NULL;
    p_Filename        = NULL;

    p_Width           = 1.0f;
    p_Height          = 1.0f;
    p_rX              = 0.0f;
    p_rY              = 0.0f;
    p_rZ              = 0.0f;
    p_tX              = 0.0f;
    p_tY              = 0.0f;
    p_tZ              = 0.0f;

    p_ExternalTexture = false;
}

// Destructeur de la classe E_Sprite.
E_Sprite::~E_Sprite()
{
    // Nettoie les ressources utilisées par la liste de polygones.
    if ( p_PolygonList != NULL )
    {
        delete p_PolygonList;
        p_PolygonList = NULL;
    }

    // Nettoie les ressources utilisées par le VertexBuffer.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // Nettoie les ressources utilisées par la texture, si elle est interne.
    if ( p_Texture != NULL && p_ExternalTexture == false )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }
}

// Obtient la liste de polygones du sprite.
E_PolygonList* E_Sprite::GetPolygonList()
{
    return p_PolygonList;
}

// Obtient le nom du fichier des textures.
LPSTR E_Sprite::GetFilename()
{
    return p_Filename;
}

// Définit le nom du fichier des textures.
void E_Sprite::SetFilename( LPSTR Filename )
{
    p_Filename = Filename;
}

// Définit la texture du sprite.
void E_Sprite::SetTexture( LPDIRECT3DTEXTURE9 Texture )
{
    // On s'assure que l'utilisateur n'essaie pas d'écraser une texture
    // interne déjà existante. Si c'est le cas, on nettoie la ressource
    // avant de copier le pointeur vers la texture externe.
    if ( p_Texture != NULL && p_ExternalTexture == false )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // On copie ensuite le pointeur de la texture, puis on indique à la classe
    // que la texture est externe, donc qu'elle n'est plus responsable de la
    // gestion des ressources de cette texture.
    p_Texture         = Texture;
    p_ExternalTexture = true;
}

// Obtient la largeur du sprite.
float E_Sprite::GetWidth()
{
    return p_Width;
}

// Définit la largeur du sprite.
void E_Sprite::SetWidth( float Value )
{
    p_Width = Value;
}

// Obtient la hauteur du sprite.
float E_Sprite::GetHeight()
{
    return p_Height;
}

// Définit la hauteur du sprite.
void E_Sprite::SetHeight( float Value )
{
    p_Height = Value;
}

// Obtient l'angle de rotation du sprite sur l'axe X.
float E_Sprite::GetRotationX()
{
    return p_rX;
}

// Définit l'angle de rotation du sprite sur l'axe X.
void E_Sprite::SetRotationX( float Value )
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

// Obtient l'angle de rotation du sprite sur l'axe Y.
float E_Sprite::GetRotationY()
{
    return p_rY;
}

// Définit l'angle de rotation du sprite sur l'axe Y.
void E_Sprite::SetRotationY( float Value )
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

// Obtient l'angle de rotation du sprite sur l'axe Z.
float E_Sprite::GetRotationZ()
{
    return p_rZ;
}

// Définit l'angle de rotation du sprite sur l'axe Z.
void E_Sprite::SetRotationZ( float Value )
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

// Obtient la valeur de la position du sprite sur l'axe X.
float E_Sprite::GetTranslationX()
{
    return p_tX;
}

// Définit la valeur de la position du sprite sur l'axe X.
void E_Sprite::SetTranslationX( float Value )
{
    p_tX = Value;
}

// Obtient la valeur de la position du sprite sur l'axe Y.
float E_Sprite::GetTranslationY()
{
    return p_tY;
}

// Définit la valeur de la position du sprite sur l'axe Y.
void E_Sprite::SetTranslationY( float Value )
{
    p_tY = Value;
}

// Obtient la valeur de la position du sprite sur l'axe Z.
float E_Sprite::GetTranslationZ()
{
    return p_tZ;
}

// Définit la valeur de la position du sprite sur l'axe Z.
void E_Sprite::SetTranslationZ( float Value )
{
    p_tZ = Value;
}

// Cette fonction construit la matrice d'affichage locale pour le sprite.
D3DXMATRIXA16 E_Sprite::GetWorldMatrix()
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 SpriteMatrix;

    // On initialise toutes les matrices.
    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &SpriteMatrix );

    D3DXMatrixRotationX( &RX, p_rX );
    D3DXMatrixRotationY( &RY, p_rY );
    D3DXMatrixRotationZ( &RZ, p_rZ );

    // Ensuite, on combine les différentes matrices, pour créer la matrice
    // finale. Attention, l'ordre des opérations est important.
    D3DXMatrixMultiply( &SpriteMatrix, &RX, &SpriteMatrix );
    D3DXMatrixMultiply( &SpriteMatrix, &RY, &SpriteMatrix );
    D3DXMatrixMultiply( &SpriteMatrix, &RZ, &SpriteMatrix );

    // On combine la translation directement dans la matrice.
    SpriteMatrix._41 = p_tX;
    SpriteMatrix._42 = p_tY;
    SpriteMatrix._43 = p_tZ;

    // Pour finir, on retourne la matrice finale.
    return SpriteMatrix;
}

// Cette fonction initialise le sprite.
bool E_Sprite::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DXVECTOR3 Vertex[4];

    // On commence par créer les coordonnées du sprite.
    Vertex[0] = D3DXVECTOR3( -p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    Vertex[1] = D3DXVECTOR3( -p_Width / 2.0f,  p_Height / 2.0f, 0.0f );
    Vertex[2] = D3DXVECTOR3(  p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    Vertex[3] = D3DXVECTOR3(  p_Width / 2.0f,  p_Height / 2.0f, 0.0f );

    // On crée ensuite la liste de polygones.
    p_PolygonList = new E_PolygonList();

    // Puis, on teste que la liste soit correctement crée en mémoire.
    if ( p_PolygonList == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Ensuite, on copie les données dans la liste de polygones.
    for ( int i = 0; i < 2; i++ )
    {
        // On tente l'ajout d'un nouveau polygone.
        if ( p_PolygonList->AddPolygon( Vertex[i],
                                        Vertex[i+1],
                                        Vertex[i+2] ) == false )
        {
            // Si l'ajout échoue, on retourne une erreur.
            return false;
        }
    }

    // Puis, on initialise le VertexBuffer.
    if( FAILED( theD3DDevice->CreateVertexBuffer( 4 * sizeof( SPRITE_VERTEX ),
                                                  D3DUSAGE_WRITEONLY,
                                                  SPRITE_VERTEX::FVF,
                                                  D3DPOOL_MANAGED,
                                                  &p_VertexBuffer,
                                                  NULL ) ) )
    {
        // Si l'initialisation échoue, on retourne une erreur.
        return false;
    }

    SPRITE_VERTEX* p_Vertices;

    // On verrouille ensuite le VertexBuffer, pour écrire les données.
    if( FAILED( p_VertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On crée ici le polygone, en copiant les données dans le VertexBuffer.
    p_Vertices[0].Position = Vertex[0];
    p_Vertices[0].Tu       = 0.0f;
    p_Vertices[0].Tv       = 1.0f;
    p_Vertices[0].Color    = 0xFFFFFFFF;
    p_Vertices[1].Position = Vertex[1];
    p_Vertices[1].Tu       = 0.0f;
    p_Vertices[1].Tv       = 0.0f;
    p_Vertices[1].Color    = 0xFFFFFFFF;
    p_Vertices[2].Position = Vertex[2];
    p_Vertices[2].Tu       = 1.0f;
    p_Vertices[2].Tv       = 1.0f;
    p_Vertices[2].Color    = 0xFFFFFFFF;
    p_Vertices[3].Position = Vertex[3];
    p_Vertices[3].Tu       = 1.0f;
    p_Vertices[3].Tv       = 0.0f;
    p_Vertices[3].Color    = 0xFFFFFFFF;

    // Puis, on libère l'accès au VertexBuffer.
    p_VertexBuffer->Unlock();

    // On teste si un fichier de texture a été spécifié.
    if ( p_Filename != NULL )
    {
        // Si c'est le cas, on tente de charger celui-ci.
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_Filename,
                                               &p_Texture ) ) )
        {
            // Si l'opération échoue, on retourne une erreur.
            return false;
        }

        // On indique à la classe que la texture est interne, donc qu'elle est
        // responsable de la gestion de cette ressource.
        p_ExternalTexture = false;
    }

    // Si tout s'est bien passé, on retourne un message de réussite.
    return true;
}

// Cette fonction permet le rendu du sprite.
void E_Sprite::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On met à jour la matrice de monde, pour placer l'objet dans l'espace.
    D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
    theD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

    // Puis, on indique à Direct3D l'emplacement du VertexBuffer.
    theD3DDevice->SetStreamSource( 0,
                                   p_VertexBuffer,
                                   0,
                                   sizeof( SPRITE_VERTEX ) );

    // On indique ensuite le type de FVF (Flexible Vertex Format) à utiliser.
    theD3DDevice->SetFVF( SPRITE_VERTEX::FVF );

    // On indique l'emplacement de la texture à appliquer.
    theD3DDevice->SetTexture( 0, p_Texture );

    // Pour finir, on commande à Direct3D le rendu de la primitive.
    theD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

// Restaure le sprite après la construction d'un nouveau Device Direct3D.
bool E_Sprite::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On détruit le VertexBuffer, si celui-ci existe.
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    // On détruit la texture, si celle-ci existe, et qu'elle est interne.
    if ( p_Texture != NULL && p_ExternalTexture == false )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // Enfin, on reconstruit l'objet.
    return Initialize( theD3DDevice );
}
