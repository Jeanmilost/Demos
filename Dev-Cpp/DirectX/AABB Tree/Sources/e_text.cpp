/*****************************************************************************
 * ==> Classe E_Text --------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet l'affichage d'un texte sur la surface   *
 *               de rendu de la sc�ne 3D.                                    *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_text.h"

// Constructeur de la classe E_Text.
E_Text::E_Text()
{
    p_Font     = NULL;
    p_FontName = "Arial";
    p_Size     = 8;
}

// Destructeur de la classe E_Text.
E_Text::~E_Text()
{
    // On lib�re les ressources utilis�es par l'objet D3DXFONT.
    if ( p_Font != NULL )
    {
        p_Font->Release();
        p_Font = NULL;
    }
}

// D�finit la police d'affichage.
void E_Text::SetFont( LPSTR FontName )
{
    p_FontName = FontName;
}

// Obtient la police d'affichage.
LPSTR E_Text::GetFont()
{
    return p_FontName;
}

// D�finit la taille de la police.
void E_Text::SetSize( DWORD Size )
{
    p_Size = Size;
}

// Obtient la taille de la police.
DWORD E_Text::GetSize()
{
    return p_Size;
}

// Cette fonction initialise la classe pour le rendu d'un texte.
bool E_Text::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    HDC hDC;
    INT nHeight;

    // On obtient un pointeur vers le Device Context de Windows.
    hDC = GetDC( NULL );

    // Puis, on obtient la hauteur format�e pour la police.
    nHeight = -MulDiv( p_Size, GetDeviceCaps( hDC, LOGPIXELSY ), 72 );

    // Ensuite, on lib�re le Device Context.
    ReleaseDC( NULL, hDC );

    // On tente ensuite d'initialiser l'objet D3DXFONT.
    if ( FAILED( D3DXCreateFont( theD3DDevice, 
                                 nHeight,
                                 0,
                                 FW_NORMAL,
                                 1,
                                 false,
                                 DEFAULT_CHARSET,
                                 OUT_DEFAULT_PRECIS,
                                 ANTIALIASED_QUALITY,
                                 DEFAULT_PITCH | FF_DONTCARE,
                                 p_FontName,
                                 &p_Font ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // sinon on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet d'afficher un texte sur la surface de rendu.
void E_Text::Render( LPDIRECT3DDEVICE9 theD3DDevice,
                     LPSTR             Text,
                     int               X,
                     int               Y,
                     unsigned char     a,
                     unsigned char     r,
                     unsigned char     g,
                     unsigned char     b )
{
    // On teste d'abord si le Device Direct3D est correctement initialis�, et
    // si un texte a �t� pass� en param�tre.
    if ( theD3DDevice == NULL || Text == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'op�ration.
        return;
    }

    // On d�finit ensuite un rectangle d'affichage. Les deux derni�res valeurs
    // sont � 0, ceci force Direct3D a calculer la surface pour nous, en
    // fonction de la longeur du texte.
    RECT Rect;
    SetRect( &Rect, X, Y, 800, 50 );

    // On commande le rendu du texte.
    p_Font->DrawText( NULL,
                      Text,
                      -1,
                      &Rect,
                      DT_SINGLELINE,
                      D3DCOLOR_ARGB( a, r, g, b ) );
}

// Cette fonction restaure l'objet apr�s un changement de Device Direct3D.
bool E_Text::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On lib�re les ressources utilis�es par l'objet D3DXFONT.
    if ( p_Font != NULL )
    {
        p_Font->Release();
        p_Font = NULL;
    }

    // Puis, on tente d'initialiser l'objet, et on retourne la r�ponse.
    return Initialize( theD3DDevice );
}
