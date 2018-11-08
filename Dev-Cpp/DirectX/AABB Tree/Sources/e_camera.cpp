/*****************************************************************************
 * ==> Classe E_Camera ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente une caméra, ou un point de vue,     *
 *               dans le monde 3D.                                           *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_camera.h"

// Constructeur par défaut. Le ratio est défini à 1.
E_Camera::E_Camera()
{
    p_rX          = 0.0f;
    p_rY          = 0.0f;
    p_rZ          = 0.0f;
    p_tX          = 0.0f;
    p_tY          = 0.0f;
    p_tZ          = 0.0f;
    p_FieldOfView = (float)( D3DX_PI / 4 );
    p_AspectRatio = 1.0f;
    p_ClipNear    = 0.1f;
    p_ClipFar     = 100.0f;
    p_Width       = 0.0f;
    p_Height      = 0.0f;
}

// Deuxième constructeur. Construction d'un port de vue orthogonal.
E_Camera::E_Camera( float Width, float Height )
{
    p_rX          = 0.0f;
    p_rY          = 0.0f;
    p_rZ          = 0.0f;
    p_tX          = 0.0f;
    p_tY          = 0.0f;
    p_tZ          = 0.0f;
    p_FieldOfView = (float)( D3DX_PI / 4 );
    p_ClipNear    = 0.0f;
    p_ClipFar     = 1.0f;
    p_Width       = Width;
    p_Height      = Height;
    p_AspectRatio = Width / Height;
}

// Troisième constructeur. Le ratio est calculé en fonction du Device D3D.
E_Camera::E_Camera( LPDIRECT3DDEVICE9 theD3DDevice )
{
    p_rX          = 0.0f;
    p_rY          = 0.0f;
    p_rZ          = 0.0f;
    p_tX          = 0.0f;
    p_tY          = 0.0f;
    p_tZ          = 0.0f;
    p_FieldOfView = (float)( D3DX_PI / 4 );
    p_ClipNear    = 1.0f;
    p_ClipFar     = 100.0f;

    if ( theD3DDevice != NULL )
    {
        CalculateAspectRatio( theD3DDevice );
    }
}

// Destructeur de la classe E_Camera.
E_Camera::~E_Camera()
{
}

// Obtient la largeur du port de vue.
float E_Camera::GetWidth()
{
    return p_Width;
}

// Définit la largeur du port de vue.
void E_Camera::SetWidth( float Value )
{
    p_Width = Value;
}

// Obtient la hauteur du port de vue.
float E_Camera::GetHeight()
{
    return p_Height;
}

// Définit la hauteur du port de vue.
void E_Camera::SetHeight( float Value )
{
    p_Height = Value;
}

// Obtient l'angle de rotation de la caméra sur l'axe X.
float E_Camera::GetRotationX()
{
    return p_rX;
}

// Définit l'angle de rotation de la caméra sur l'axe X.
void E_Camera::SetRotationX( float Value )
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

// Obtient l'angle de rotation de la caméra sur l'axe Y.
float E_Camera::GetRotationY()
{
    return p_rY;
}

// Définit l'angle de rotation de la caméra sur l'axe Y.
void E_Camera::SetRotationY( float Value )
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

// Obtient l'angle de rotation de la caméra sur l'axe Z.
float E_Camera::GetRotationZ()
{
    return p_rZ;
}

// Définit l'angle de rotation de la caméra sur l'axe Z.
void E_Camera::SetRotationZ( float Value )
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

// Obtient la valeur de la position de la caméra sur l'axe X.
float E_Camera::GetTranslationX()
{
    return p_tX;
}

// Définit la valeur de la position de la caméra sur l'axe X.
void E_Camera::SetTranslationX( float Value )
{
    p_tX = Value;
}

// Obtient la valeur de la position de la caméra sur l'axe Y.
float E_Camera::GetTranslationY()
{
    return p_tY;
}

// Définit la valeur de la position de la caméra sur l'axe Y.
void E_Camera::SetTranslationY( float Value )
{
    p_tY = Value;
}

// Obtient la valeur de la position de la caméra sur l'axe Z.
float E_Camera::GetTranslationZ()
{
    return p_tZ;
}

// Définit la valeur de la position de la caméra sur l'axe Z.
void E_Camera::SetTranslationZ( float Value )
{
    p_tZ = Value;
}

// Obtient l'angle du champ de vue.
float E_Camera::GetFieldOfView()
{
    return p_FieldOfView;
}

// Définit l'angle du champ de vue.
void E_Camera::SetFieldOfView( float Value )
{
    p_FieldOfView = Value;
}

// Obtient l'aspect du ratio X/Y.
float E_Camera::GetAspectRatio()
{
    return p_AspectRatio;
}

// Obtient le point de coupe proche.
float E_Camera::GetClipNear()
{
    return p_ClipNear;
}

// Définit le point de coupe proche.
void E_Camera::SetClipNear( float Value )
{
    p_ClipNear = Value;
}

// Obtient le point de coupe lointain.
float E_Camera::GetClipFar()
{
    return p_ClipFar;
}

// Définit le point de coupe lointain.
void E_Camera::SetClipFar( float Value )
{
    p_ClipFar = Value;
}

// Cette fonction calcule la valeur du ratio X/Y.
void E_Camera::CalculateAspectRatio( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DDISPLAYMODE theDisplayMode;

    if ( theD3DDevice != NULL )
    {
        theD3DDevice->GetDisplayMode( 0, &theDisplayMode );

        p_Width  = (float)theDisplayMode.Width;
        p_Height = (float)theDisplayMode.Height;

        p_AspectRatio = p_Width / p_Height;
    }
}

// Cette fonction calcule un port de vue dans l'espace 3D.
void E_Camera::SetCamera( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 T;
    D3DXMATRIXA16 Camera;
    D3DXMATRIXA16 World;
    D3DXMATRIXA16 Projection;

    // On commence par initialiser toutes les matrices.
    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &T );
    D3DXMatrixIdentity( &Camera );
    D3DXMatrixIdentity( &World );
    D3DXMatrixIdentity( &Projection );

    // On crée ici les matrices de rotation et de translation, en utilisant
    // les valeurs entrées par l'utilisateur.
    D3DXMatrixRotationX( &RX, p_rX );
    D3DXMatrixRotationY( &RY, p_rY );
    D3DXMatrixRotationZ( &RZ, p_rZ );
    D3DXMatrixTranslation( &T, p_tX, p_tY, p_tZ );

    // Puis, on assemble ces différentes matrices, afin d'obtenir la matrice
    // de vue. Attention, l'ordre de combinaison est important.
    D3DXMatrixMultiply( &Camera, &RY, &RX );
    D3DXMatrixMultiply( &Camera, &Camera, &RZ );
    D3DXMatrixMultiply( &Camera, &T, &Camera );

    // On calcule ensuite la matrice de projection.
    D3DXMatrixPerspectiveFovLH( &Projection,
                                p_FieldOfView,
                                p_AspectRatio,
                                p_ClipNear,
                                p_ClipFar );

    // On teste si le Device Direct3D a été correctement initialisé.
    if ( theD3DDevice != NULL )
    {
        // Si oui, on indique à Direct3D la valeur des différentes matrices.
        // La matrice de monde, que l'on laisse à sa valeur par défaut, ...
        theD3DDevice->SetTransform( D3DTS_WORLD, &World );

        // ... puis, la matrice de vue, que l'on a calculé ci-dessus, ...
        theD3DDevice->SetTransform( D3DTS_VIEW, &Camera );

        // ... et enfin, la matrice de projection.
        theD3DDevice->SetTransform( D3DTS_PROJECTION, &Projection );
    }
}

// Cette fonction calcule un port de vue orthogonal dans l'espace 3D.
void E_Camera::SetCameraOrtho( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 T;
    D3DXMATRIXA16 Camera;
    D3DXMATRIXA16 World;
    D3DXMATRIXA16 Projection;

    // On commence par initialiser toutes les matrices.
    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &T );
    D3DXMatrixIdentity( &Camera );
    D3DXMatrixIdentity( &World );
    D3DXMatrixIdentity( &Projection );

    // On crée ici les matrices de rotation et de translation, en utilisant
    // les valeurs entrées par l'utilisateur.
    D3DXMatrixRotationX  ( &RX, p_rX );
    D3DXMatrixRotationY  ( &RY, p_rY );
    D3DXMatrixRotationZ  ( &RZ, p_rZ );
    D3DXMatrixTranslation( &T, p_tX, p_tY, p_tZ );

    // Puis, on assemble ces différentes matrices, afin d'obtenir la matrice
    // de vue. Attention, l'ordre de combinaison est important.
    D3DXMatrixMultiply( &Camera, &RY, &RX );
    D3DXMatrixMultiply( &Camera, &Camera, &RZ );
    D3DXMatrixMultiply( &Camera, &T, &Camera );

    // On calcule ensuite la matrice de projection. Il s'agit ici d'une
    // matrice orthogonale, optimisée pour un rendu 2D.
    D3DXMatrixOrthoLH( &Projection,
                       p_Width,
                       p_Height,
                       p_ClipNear,
                       p_ClipFar );

    // On teste si le Device Direct3D a été correctement initialisé.
    if ( theD3DDevice != NULL )
    {
        // Si oui, on indique à Direct3D la valeur des différentes matrices.
        // La matrice de monde, que l'on laisse à sa valeur par défaut, ...
        theD3DDevice->SetTransform( D3DTS_WORLD, &World );

        // ... puis, la matrice de vue, que l'on a calculé ci-dessus, ...
        theD3DDevice->SetTransform( D3DTS_VIEW, &Camera );

        // ... et enfin, la matrice de projection.
        theD3DDevice->SetTransform( D3DTS_PROJECTION, &Projection );
    }
}

// Restaure la caméra après la construction d'un nouveau Device Direct3D.
void E_Camera::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    CalculateAspectRatio( theD3DDevice );
}
