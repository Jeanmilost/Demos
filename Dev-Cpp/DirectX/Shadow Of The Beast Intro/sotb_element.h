// Class automatically generated by Dev-C++ New Class wizard

#ifndef SOTB_ELEMENT_H
#define SOTB_ELEMENT_H

#include "e_sprite.h"
#include "e_texture.h"

class SOTB_ElementContainer
{
  private:

    E_Sprite*              p_Sprite;
    SOTB_ElementContainer* p_Next;

  public:

    SOTB_ElementContainer();
    SOTB_ElementContainer( E_Sprite* Sprite );
    ~SOTB_ElementContainer();

  public:

    E_Sprite* GetSprite();

    bool CreateSprite( float             Width,
                       float             Height,
                       float             X,
                       float             Y,
                       float             Z,
                       bool              Transparency,
                       E_Texture*        Texture,
                       LPDIRECT3DDEVICE9 theD3DDevice );

    SOTB_ElementContainer* GetNextContainer();
    void SetNextContainer( SOTB_ElementContainer* Container );
};

/*
 * No description
 */
class SOTB_Element
{
  private:

    bool                   p_IsScrollable;

    float                  p_X;
    float                  p_ScrollVelocity;

    E_Texture*             p_Texture;

    SOTB_ElementContainer* p_First;
    SOTB_ElementContainer* p_Current;

  public:

    SOTB_Element();
    ~SOTB_Element();

  public:

    bool Initialize  ( int               NbElements,
                       float             Width,
                       float             Height,
                       float             X,
                       float             Y,
                       float             Z,
                       float             ScrollVelocity,
                       bool              Transparency,
                       LPSTR             Filename,
                       LPDIRECT3DDEVICE9 theD3DDevice );

    bool IsScrollable();

    void Render      ( LPDIRECT3DDEVICE9 theD3DDevice );
    void Scroll      ();
};

#endif // SOTB_ELEMENT_H