/*****************************************************************************
 * ==> Classe E_Texture -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la création et la gestion d'une texture,*
 *               utilisable pour un ou plusieurs objet(s) 3D.                *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_texture.h"

// Constructeur par défaut de la classe E_Texture.
E_Texture::E_Texture()
{
    p_Texture  = NULL;
    p_Filename = NULL;
}

// Deuxième constructeur de la classe E_Texture.
E_Texture::E_Texture( LPSTR Filename )
{
    p_Texture  = NULL;
    p_Filename = Filename;
}

// Destructeur de la classe E_Texture.
E_Texture::~E_Texture()
{
    // Nettoie les ressources utilisées par la texture.
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

// Définit le nom du fichier de l'image de la texture.
void E_Texture::SetFilename( LPSTR Filename )
{
    p_Filename = Filename;
}

// Cette fonction initialise la texture.
bool E_Texture::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On teste si le nom du fichier de l'image de la texture a été défini.
    if ( p_Filename != NULL )
    {
        // Si c'est le cas, on tente de charger la texture en mémoire.
        if( FAILED( D3DXCreateTextureFromFile( theD3DDevice,
                                               p_Filename,
                                               &p_Texture ) ) )
        {
            // Si l'opération échoue, on définit le pointeur à NULL,
            // et on retourne une erreur.
            p_Texture = NULL;
            return false;
        }

        // Sinon, on retourne un message de réussite.
        return true;
    }
    else
    {
        // Sinon, on définit le pointeur à NULL, et on retourne une erreur.
        p_Texture = NULL;
        return false;
    }

    // Ce point du code n'est jamais atteint, mais on le traite quand même,
    // pour éviter des erreurs de compilation.
    return false;
}

// Restaure la texture après la construction d'un nouveau Device Direct3D.
bool E_Texture::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On détruit l'objet texture, si celui-ci existe.
    if ( p_Texture != NULL )
    {
        p_Texture->Release();
        p_Texture = NULL;
    }

    // Puis on reconstruit l'objet.
    return Initialize( theD3DDevice );
}
