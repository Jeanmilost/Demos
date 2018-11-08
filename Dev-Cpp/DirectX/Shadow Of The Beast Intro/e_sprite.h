/*****************************************************************************
 * ==> Classe E_Sprite ------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe gère les sprites dans le monde 3D.             *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_SPRITE_H
#define E_SPRITE_H

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

/*
 * Enumération des surfaces du sprite.
 */
enum VertexMode
{
    UNKNOW_SURFACE         = 0,
    SIMPLE_COLORED_SURFACE = 1,
    TEXTURED_SURFACE       = 2
};

/*
 * Cette structure permet la création d'une surface texturée.
 */
struct TEXTURED_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
    FLOAT       Tu;
    FLOAT       Tv;

    static const DWORD FVF;
};

/*
 * Cette structure permet la création d'une surface colorée.
 */
struct COLORED_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;

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

    DWORD                   p_Color[4];

    float                   p_Width;
    float                   p_Height;
    float                   p_rX;
    float                   p_rY;
    float                   p_rZ;
    float                   p_tX;
    float                   p_tY;
    float                   p_tZ;

    bool                    p_AlphaBlended;
    bool                    p_ExternalTexture;

    VertexMode              p_VertexMode;

    LPSTR                   p_Filename;

  public:

    E_Sprite();
    ~E_Sprite();

  public:

    D3DXVECTOR3 GetVertex( int Value );

    LPSTR GetFilename     ();
    void  SetFilename     ( LPSTR Filename );
    void  SetTexture      ( LPDIRECT3DTEXTURE9 Texture );
    float GetWidth        ();
    void  SetWidth        ( float Value );
    float GetHeight       ();
    void  SetHeight       ( float Value );
    float GetRotationX    ();
    void  SetRotationX    ( float Value );
    float GetRotationY    ();
    void  SetRotationY    ( float Value );
    float GetRotationZ    ();
    void  SetRotationZ    ( float Value );
    float GetTranslationX ();
    void  SetTranslationX ( float Value );
    float GetTranslationY ();
    void  SetTranslationY ( float Value );
    float GetTranslationZ ();
    void  SetTranslationZ ( float Value );
    bool  IsAlphaBlended  ();
    void  SetAlphaBlending( bool Value );

    D3DXMATRIXA16 GetWorldMatrix();

    DWORD GetVertexColor( int Index );
    void  SetVertexColor( int Index, DWORD Value );

    bool CreateTexturedSurface( LPDIRECT3DDEVICE9 theD3DDevice );
    bool CreateSimpleColoredSurface( LPDIRECT3DDEVICE9 theD3DDevice );
    bool Restore( LPDIRECT3DDEVICE9 theD3DDevice );

    void Render( LPDIRECT3DDEVICE9 theD3DDevice );

  private:

    void OnAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice );
    void RestoreNormalProperties( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_SPRITE_H
