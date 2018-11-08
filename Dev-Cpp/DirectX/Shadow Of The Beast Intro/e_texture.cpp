/*****************************************************************************
 * ==> Classe E_Texture -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la cr�ation et la gestion d'une texture,*
 *               utilisable pour un ou plusieurs objet(s) 3D.                *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_texture.h"

// Constructeur par d�faut de la classe E_Texture.
E_Texture::E_Texture()
{
    p_Texture  = NULL;
    p_Filename = NULL;
}

// Deuxi�me constructeur de la classe E_Texture.
E_Texture::E_Texture( LPSTR Filename )
{
    p_Texture  = NULL;
    p_Filename = Filename;
}

// Destructeur de la classe E_Texture.
E_Texture::~E_Texture()
{
    // On nettoie les ressources utilis�es par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }
}

// Obtient le pointeur vers l'objet texture.
LPDIRECT3DTEXTURE9 E_Texture::GetTexture()
{
    return p_Texture;
}

// Obtient le nom du fichier de l'image de la texture.
LPSTR E_Texture::GetFilename()
{
    return p_Filename;
}

// D�finit le nom du fichier de l'image de la texture.
void E_Texture::SetFilename( LPSTR Filename )
{
    p_Filename = Filename;
}

// Cette fonction initialise la texture.
bool E_Texture::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On teste si un fichier d'image a �t� d�fini.
    if ( p_Filename != NULL )
    {
        // Si oui, on tente de charger la texture � partir de ce fichier.
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_Filename,
                                               &p_Texture ) ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        p_Texture = NULL;
        return false;
    }
}

// Restaure la texture apr�s la construction d'un nouveau Device Direct3D.
bool E_Texture::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On nettoie les ressources utilis�es par la texture.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // Puis, on tente de r�initialiser la texture.
    return Initialize( theD3DDevice );
}
