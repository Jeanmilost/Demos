/*****************************************************************************
 * ==> Classe E_Texture -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la création et la gestion d'une texture,*
 *               utilisable pour un ou plusieurs objet(s) 3D.                *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_TEXTURE_H
#define E_TEXTURE_H

#include <windows.h>
#include <d3dx9.h>

/*
 * Cette classe permet la gestion des textures.
 */
class E_Texture
{
  private:

    LPDIRECT3DTEXTURE9 p_Texture;

    LPSTR              p_Filename;

  public:

    E_Texture();
    E_Texture( LPSTR Filename );

    ~E_Texture();

  public:

    LPDIRECT3DTEXTURE9 GetTexture();
    LPSTR GetFilename();
    void SetFilename( LPSTR Filename );

    bool Initialize( LPDIRECT3DDEVICE9 theD3DDevice );
    bool Restore( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_TEXTURE_H
