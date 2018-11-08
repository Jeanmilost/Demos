/*****************************************************************************
 * ==> Classe E_Text --------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet l'affichage d'un texte sur la surface   *
 *               de rendu de la scène 3D.                                    *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_TEXT_H
#define E_TEXT_H

#include <d3dx9.h>

/*
 * Cette classe permet l'affichage d'un texte.
 */
class E_Text
{
  private:

    LPD3DXFONT p_Font;

    LPSTR      p_FontName;

    DWORD      p_Size;

  public:

    E_Text();
    ~E_Text();

  public:

    void  SetFont   ( LPSTR FontName );
    LPSTR GetFont   ();
    void  SetSize   ( DWORD Size );
    DWORD GetSize   ();

    bool  Initialize( LPDIRECT3DDEVICE9 theD3DDevice );
    bool  Restore   ( LPDIRECT3DDEVICE9 theD3DDevice );

    void  Render    ( LPDIRECT3DDEVICE9 theD3DDevice,
                      LPSTR             Text,
                      int               X             = 1,
                      int               Y             = 1,
                      unsigned char     a             = 255,
                      unsigned char     r             = 255,
                      unsigned char     g             = 255,
                      unsigned char     b             = 255 );
};

#endif // E_TEXT_H
