/*****************************************************************************
 * ==> Classe E_Sprite ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe gère les sprites dans le monde 3D.             *
 * Version     : 1.1                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_SPRITE_H
#define E_SPRITE_H

#include <d3dx9.h>

#include "e_polygonList.h"

/*
 * Cette structure permet la création du VertexBuffer.
 */
struct SPRITE_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
    FLOAT       Tu;
    FLOAT       Tv;

    static const DWORD FVF;
};

/*
 * Cette classe permet la gestion des sprites.
 */
class E_Sprite
{
  private:

    LPDIRECT3DVERTEXBUFFER9 p_VertexBuffer;

    LPDIRECT3DTEXTURE9      p_Texture;

    E_PolygonList*          p_PolygonList;

    float                   p_Width;
    float                   p_Height;
    float                   p_rX;
    float                   p_rY;
    float                   p_rZ;
    float                   p_tX;
    float                   p_tY;
    float                   p_tZ;

    LPSTR                   p_Filename;

    bool                    p_ExternalTexture;

  public:

    E_Sprite();
    ~E_Sprite();

  public:

    E_PolygonList* GetPolygonList();

    LPSTR GetFilename    ();
    void  SetFilename    ( LPSTR Filename );
    void  SetTexture     ( LPDIRECT3DTEXTURE9 Texture );
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

    D3DXMATRIXA16 GetWorldMatrix();

    bool Initialize( LPDIRECT3DDEVICE9 theD3DDevice );
    bool Restore( LPDIRECT3DDEVICE9 theD3DDevice );

    void Render( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_SPRITE_H
