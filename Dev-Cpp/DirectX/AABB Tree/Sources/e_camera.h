/*****************************************************************************
 * ==> Classe E_Camera ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente une caméra, ou un point de vue,     *
 *               dans le monde 3D.                                           *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_CAMERA_H
#define E_CAMERA_H

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

/*
 * Cette classe permet la gestion de la caméra.
 */
class E_Camera
{
  private:

    float p_rX;
    float p_rY;
    float p_rZ;
    float p_tX;
    float p_tY;
    float p_tZ;
    float p_FieldOfView;
    float p_AspectRatio;
    float p_ClipNear;
    float p_ClipFar;
    float p_Width;
    float p_Height;

  public:

    E_Camera();
    E_Camera( float Width, float Height );
    E_Camera( LPDIRECT3DDEVICE9 theD3DDevice );

    ~E_Camera();

  public:

    float GetWidth       ();
    void  SetWidth       ( float Value );
    float GetHeight      ();
    void  SetHeight      ( float Value );
    float GetRotationX   ();
    void  SetRotationX   ( float Value );
    float GetRotationY   ();
    void  SetRotationY   ( float Value );
    float GetRotationZ   ();
    void  SetRotationZ   ( float Value );
    float GetTranslationX();
    void  SetTranslationX( float Value );
    float GetTranslationY();
    void  SetTranslationY( float Value );
    float GetTranslationZ();
    void  SetTranslationZ( float Value );
    float GetFieldOfView ();
    void  SetFieldOfView ( float Value );
    float GetClipNear    ();
    void  SetClipNear    ( float Value );
    float GetClipFar     ();
    void  SetClipFar     ( float Value );
    float GetAspectRatio ();

    void SetCamera( LPDIRECT3DDEVICE9 theD3DDevice );
    void SetCameraOrtho( LPDIRECT3DDEVICE9 theD3DDevice );

    void Restore( LPDIRECT3DDEVICE9 theD3DDevice );

  private:

    void CalculateAspectRatio( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_CAMERA_H
