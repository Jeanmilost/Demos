/*****************************************************************************
 * ==> Classe E_Camera ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente une caméra, ou un point de vue,     *
 *               dans le monde 3D.                                           *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_camera.h"

// Constructeur de la classe E_Camera.
E_Camera::E_Camera( float Width, float Height )
{
    p_rX       = 0.0f;
    p_rY       = 0.0f;
    p_rZ       = 0.0f;
    p_tX       = 0.0f;
    p_tY       = 0.0f;
    p_tZ       = 0.0f;
    p_Width    = Width;
    p_Height   = Height;
    p_ClipNear = 0.0f;
    p_ClipFar  = 1.0f;
}

// Destructeur de la classe E_Camera.
E_Camera::~E_Camera()
{
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

// Place la caméra dans le monde 3D.
void E_Camera::SetCamera( LPDIRECT3DDEVICE9 theD3DDevice )
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 T;
    D3DXMATRIXA16 Camera;
    D3DXMATRIXA16 World;
    D3DXMATRIXA16 Projection;

    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &T );
    D3DXMatrixIdentity( &Camera );
    D3DXMatrixIdentity( &World );
    D3DXMatrixIdentity( &Projection );

    D3DXMatrixRotationX( &RX, p_rX );
    D3DXMatrixRotationY( &RY, p_rY );
    D3DXMatrixRotationZ( &RZ, p_rZ );

    D3DXMatrixTranslation( &T, p_tX, p_tY, p_tZ );

    D3DXMatrixMultiply( &Camera, &RY, &RX );
    D3DXMatrixMultiply( &Camera, &Camera, &RZ );
    D3DXMatrixMultiply( &Camera, &T, &Camera );

    if ( theD3DDevice != NULL )
    {
        theD3DDevice->SetTransform( D3DTS_WORLD, &World );
        theD3DDevice->SetTransform( D3DTS_VIEW, &Camera );

        D3DXMatrixOrthoLH( &Projection,
                           p_Width,
                           p_Height,
                           p_ClipNear,
                           p_ClipFar );

        theD3DDevice->SetTransform( D3DTS_PROJECTION, &Projection );
    }
}
