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


#include <cstdlib>
#include <iostream>
#include <conio.h>

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
    float p_Width;
    float p_Height;
    float p_ClipNear;
    float p_ClipFar;

  public:

    E_Camera( float Width, float Height );

    ~E_Camera();

  public:

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
    float GetClipNear    ();
    void  SetClipNear    ( float Value );
    float GetClipFar     ();
    void  SetClipFar     ( float Value );

    void SetCamera( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_CAMERA_H
