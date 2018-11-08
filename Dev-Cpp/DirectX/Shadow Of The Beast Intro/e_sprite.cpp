/*****************************************************************************
 * ==> Classe E_Sprite ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe gère les sprites dans le monde 3D.             *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_sprite.h"

// On initialise ici les propriétés des sommets de la surface texturée ...
const DWORD TEXTURED_VERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

// ... ainsi que celles de la surface colorée.
const DWORD COLORED_VERTEX::FVF  = D3DFVF_XYZ | D3DFVF_DIFFUSE;

// Constructeur de la classe E_Sprite.
E_Sprite::E_Sprite()
{
    p_VertexBuffer    = NULL;

    p_Texture         = NULL;

    p_VertexMode      = UNKNOW_SURFACE;

    p_Width           = 1.0f;
    p_Height          = 1.0f;
    p_rX              = 0.0f;
    p_rY              = 0.0f;
    p_rZ              = 0.0f;
    p_tX              = 0.0f;
    p_tY              = 0.0f;
    p_tZ              = 0.0f;

    p_ExternalTexture = false;
    p_AlphaBlended    = false;

    p_Filename        = NULL;

    for ( int i = 0; i < 4; i++ )
    {
        p_Color[i]    = 0x0;
    }
}

// Destructeur de la classe E_Sprite.
E_Sprite::~E_Sprite()
{
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    if ( p_Texture != NULL && p_ExternalTexture == false )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }
}

// Obtient le nom du fichier des textures.
LPSTR E_Sprite::GetFilename()
{
    return p_Filename;
}

// Définit le nom du fichier des textures.
void E_Sprite::SetFilename( LPSTR Filename )
{
    p_Filename        = Filename;
    p_ExternalTexture = false;
}

// Définit la texture du sprite.
void E_Sprite::SetTexture( LPDIRECT3DTEXTURE9 Texture )
{
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

// Obtient la couleur d'un sommet du sprite.
DWORD E_Sprite::GetVertexColor( int Index )
{
    if ( Index < 0 || Index > 3 )
    {
        return 0x0;
    }

    return p_Color[Index];
}

// Définit la couleur d'un sommet du sprite.
void E_Sprite::SetVertexColor( int Index, DWORD Value )
{
    if ( Index < 0 || Index > 3 )
    {
        return;
    }

    p_Color[Index] = Value;
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

// Obtient l'état du moteur d'aplha blending pour le rendu du sprite.
bool E_Sprite::IsAlphaBlended()
{
    return p_AlphaBlended;
}

// Configure le moteur d'aplha blending pour le rendu du sprite.
void E_Sprite::SetAlphaBlending( bool Value )
{
    p_AlphaBlended = Value;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_Sprite::OnAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DCAPS9 D3DCaps;
    theD3DDevice->GetDeviceCaps( &D3DCaps );

    // Configure le blending diffus pour les valeurs alpha dans les sommets.
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    theD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    theD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    // Active le test alpha. Celui-ci permet d'ignorer les pixels plus petits
    // qu'une certaine valeur alpha.
    if( D3DCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
    {
        theD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        theD3DDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
        theD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
    }
}

// Cette fonction restaure les paramètres normaux d'affichage.
void E_Sprite::RestoreNormalProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

// Cette fonction construit la matrice d'affichage locale pour le sprite.
D3DXMATRIXA16 E_Sprite::GetWorldMatrix()
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

// Cette fonction permet la création d'une surface texturée.
bool E_Sprite::CreateTexturedSurface( LPDIRECT3DDEVICE9 theD3DDevice )
{
    if( FAILED( theD3DDevice->CreateVertexBuffer( 4 * sizeof( TEXTURED_VERTEX ),
                                                  D3DUSAGE_WRITEONLY,
                                                  TEXTURED_VERTEX::FVF,
                                                  D3DPOOL_MANAGED,
                                                  &p_VertexBuffer,
                                                  NULL ) ) )
    {
        return false;
    }

    TEXTURED_VERTEX* p_Vertices;

    if( FAILED( p_VertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
    {
        return false;
    }

    p_Vertices[0].Position =
        D3DXVECTOR3( -p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    p_Vertices[0].Tu       = 0.0f;
    p_Vertices[0].Tv       = 1.0f;
    p_Vertices[0].Color    = 0xFFFFFFFF;
    p_Vertices[1].Position =
        D3DXVECTOR3( -p_Width / 2.0f, p_Height / 2.0f, 0.0f );
    p_Vertices[1].Tu       = 0.0f;
    p_Vertices[1].Tv       = 0.0f;
    p_Vertices[1].Color    = 0xFFFFFFFF;
    p_Vertices[2].Position =
        D3DXVECTOR3( p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    p_Vertices[2].Tu       = 1.0f;
    p_Vertices[2].Tv       = 1.0f;
    p_Vertices[2].Color    = 0xFFFFFFFF;
    p_Vertices[3].Position =
        D3DXVECTOR3( p_Width / 2.0f, p_Height / 2.0f, 0.0f );
    p_Vertices[3].Tu       = 1.0f;
    p_Vertices[3].Tv       = 0.0f;
    p_Vertices[3].Color    = 0xFFFFFFFF;

    p_VertexBuffer->Unlock();

    p_VertexMode   = TEXTURED_SURFACE;

    if ( p_Filename != NULL )
    {
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_Filename,
                                               &p_Texture ) ) )
        {
            return false;
        }
    }

    return true;
}

// Cette fonction permet la création d'une surface colorée.
bool E_Sprite::CreateSimpleColoredSurface( LPDIRECT3DDEVICE9 theD3DDevice )
{
    if( FAILED( theD3DDevice->CreateVertexBuffer( 4 * sizeof( COLORED_VERTEX ),
                                                  D3DUSAGE_WRITEONLY,
                                                  COLORED_VERTEX::FVF,
                                                  D3DPOOL_MANAGED,
                                                  &p_VertexBuffer,
                                                  NULL ) ) )
    {
        return false;
    }

    COLORED_VERTEX* p_Vertices;

    if( FAILED( p_VertexBuffer->Lock( 0, 0, (void**)&p_Vertices, 0 ) ) )
    {
        return false;
    }

    p_Vertices[0].Position =
        D3DXVECTOR3( -p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    p_Vertices[0].Color    = p_Color[0];
    p_Vertices[1].Position =
        D3DXVECTOR3( -p_Width / 2.0f, p_Height / 2.0f, 0.0f );
    p_Vertices[1].Color    = p_Color[1];
    p_Vertices[2].Position =
        D3DXVECTOR3( p_Width / 2.0f, -p_Height / 2.0f, 0.0f );
    p_Vertices[2].Color    = p_Color[2];
    p_Vertices[3].Position =
        D3DXVECTOR3( p_Width / 2.0f, p_Height / 2.0f, 0.0f );
    p_Vertices[3].Color    = p_Color[3];

    p_VertexBuffer->Unlock();

    p_VertexMode   = SIMPLE_COLORED_SURFACE;

    return true;
}

// Cette fonction permet le rendu du sprite.
void E_Sprite::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    if ( p_AlphaBlended == true )
    {
        OnAlphaBlending( theD3DDevice );
    }

    switch ( p_VertexMode )
    {
        case SIMPLE_COLORED_SURFACE:
        {
            theD3DDevice->SetStreamSource( 0,
                                           p_VertexBuffer,
                                           0,
                                           sizeof( COLORED_VERTEX ) );

            theD3DDevice->SetFVF( COLORED_VERTEX::FVF );
            theD3DDevice->SetTexture( 0, NULL );

            break;
        }

        case TEXTURED_SURFACE:
        {
            theD3DDevice->SetStreamSource( 0,
                                           p_VertexBuffer,
                                           0,
                                           sizeof( TEXTURED_VERTEX ) );

            theD3DDevice->SetFVF( TEXTURED_VERTEX::FVF );
            theD3DDevice->SetTexture( 0, p_Texture );

            break;
        }

        case UNKNOW_SURFACE:
        {
            return;
        }

        default:
        {
            return;
        }
    }

    D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
    theD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

    theD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    RestoreNormalProperties( theD3DDevice );
}

// Restaure le sprite après la construction d'un nouveau Device Direct3D.
bool E_Sprite::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    if ( p_VertexBuffer != NULL )
    {
        p_VertexBuffer->Release();
        p_VertexBuffer = NULL;
    }

    if ( p_Texture != NULL && p_ExternalTexture == false )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    switch ( p_VertexMode )
    {
        case SIMPLE_COLORED_SURFACE:
        {
            return CreateSimpleColoredSurface( theD3DDevice );
        }

        case TEXTURED_SURFACE:
        {
            return CreateTexturedSurface( theD3DDevice );
        }

        case UNKNOW_SURFACE:
        {
            return false;
        }

        default:
        {
            return false;
        }
    }

    return false;
}
