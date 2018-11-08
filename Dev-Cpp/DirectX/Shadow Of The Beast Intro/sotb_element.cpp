
#include "sotb_element.h"

SOTB_ElementContainer::SOTB_ElementContainer()
{
    p_Sprite = NULL;
    p_Next   = NULL;
}

SOTB_ElementContainer::~SOTB_ElementContainer()
{
    if ( p_Sprite != NULL )
    {
        delete p_Sprite;
        p_Sprite = NULL;
    }
}

E_Sprite* SOTB_ElementContainer::GetSprite()
{
    return p_Sprite;
}

bool SOTB_ElementContainer::CreateSprite( float             Width,
                                          float             Height,
                                          float             X,
                                          float             Y,
                                          float             Z,
                                          bool              Transparency,
                                          E_Texture*        Texture,
                                          LPDIRECT3DDEVICE9 theD3DDevice )
{
    p_Sprite = new E_Sprite();

    if ( p_Sprite == NULL )
    {
        return false;
    }

    p_Sprite->SetWidth        ( Width );
    p_Sprite->SetHeight       ( Height );
    p_Sprite->SetTranslationX ( X );
    p_Sprite->SetTranslationY ( Y );
    p_Sprite->SetTranslationZ ( Z );

    if ( Texture != NULL )
    {
        p_Sprite->SetAlphaBlending( Transparency );
        p_Sprite->SetTexture  ( Texture->GetTexture() );

        return p_Sprite->CreateTexturedSurface( theD3DDevice );
    }
    else
    {
        DWORD VertexColor[] =
        {
            0x00F866B3,
            0x0013B6F7,
            0x00F866B3,
            0x0013B6F7
        };

        for ( int i = 0; i < 4; i++ )
        {
            p_Sprite->SetVertexColor( i, VertexColor[i] );
        }

        p_Sprite->SetAlphaBlending( false );

        return p_Sprite->CreateSimpleColoredSurface( theD3DDevice );
    }
}

SOTB_ElementContainer* SOTB_ElementContainer::GetNextContainer()
{
    return p_Next;
}

void SOTB_ElementContainer::SetNextContainer( SOTB_ElementContainer* Container )
{
    p_Next = Container;
}

SOTB_Element::SOTB_Element()
{
    p_Texture        = NULL;
    p_First          = NULL;
    p_Current        = NULL;

    p_IsScrollable   = false;

    p_ScrollVelocity = 0.0f;
    p_X              = 0.0f;
}

SOTB_Element::~SOTB_Element()
{
    if ( p_Texture != NULL )
    {
        delete p_Texture;
        p_Texture = NULL;
    }

    p_Current = p_First;

    while ( p_Current != NULL )
    {
        SOTB_ElementContainer* p_Next = p_Current->GetNextContainer();

        delete p_Current;

        p_Current = p_Next;
    }

    p_Current = NULL;
    p_First   = NULL;
}

bool SOTB_Element::Initialize( int               NbElements,
                               float             Width,
                               float             Height,
                               float             X,
                               float             Y,
                               float             Z,
                               float             ScrollVelocity,
                               bool              Transparency,
                               LPSTR             Filename,
                               LPDIRECT3DDEVICE9 theD3DDevice )
{
    p_X = X;

    if ( NbElements <= 0 )
    {
        return false;
    }

    if ( Filename == NULL )
    {
        p_Texture = NULL;
    }
    else
    {
        p_Texture = new E_Texture( Filename );

        if ( p_Texture == NULL )
        {
            return false;
        }
        

        if ( p_Texture->Initialize( theD3DDevice ) == false )
        {
            return false;
        }
    }

    p_First = new SOTB_ElementContainer();

    if ( p_First == NULL )
    {
        return false;
    }

    if ( p_First->CreateSprite( Width,
                                Height,
                                X,
                                Y,
                                Z,
                                Transparency,
                                p_Texture,
                                theD3DDevice ) == false )
    {
        return false;
    }

    if ( NbElements == 1 )
    {
        p_IsScrollable = false;
        return true;
    }
    else
    {
        p_IsScrollable   = true;
        p_ScrollVelocity = ScrollVelocity;
    }

    SOTB_ElementContainer* p_New = NULL;
    p_Current = p_First;

    for ( int i = 0; i < NbElements - 1; i++ )
    {
        p_New = new SOTB_ElementContainer();

        if ( p_New == NULL )
        {
            return false;
        }

        if ( p_New->CreateSprite( Width,
                                  Height,
                                  X + ( Width * ( i + 1 ) ),
                                  Y,
                                  Z,
                                  Transparency,
                                  p_Texture,
                                  theD3DDevice ) == false )
        {
            return false;
        }

        p_Current->SetNextContainer( p_New );
        p_Current = p_Current->GetNextContainer();
    }

    return true;
}

void SOTB_Element::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    p_Current = p_First;

    while ( p_Current != NULL )
    {
        p_Current->GetSprite()->Render( theD3DDevice );
        p_Current = p_Current->GetNextContainer();
    }
}

bool SOTB_Element::IsScrollable()
{
    return p_IsScrollable;
}

void SOTB_Element::Scroll()
{
    if ( p_IsScrollable == false )
    {
        return;
    }

    int Offset = 0;

    p_Current = p_First;

    while ( p_Current != NULL )
    {
        E_Sprite* CurSprite = p_Current->GetSprite();

        CurSprite->SetTranslationX
            ( CurSprite->GetTranslationX() + p_ScrollVelocity );

        if ( p_ScrollVelocity > 0.0f )
        {
            if ( CurSprite->GetTranslationX() >
                p_X + ( CurSprite->GetWidth() * Offset ) +
                        CurSprite->GetWidth() )
            {
                CurSprite->SetTranslationX
                    ( CurSprite->GetTranslationX() - CurSprite->GetWidth() );
            }
        }
        else
        {
            if ( CurSprite->GetTranslationX() <
                p_X + ( CurSprite->GetWidth() * Offset ) -
                        CurSprite->GetWidth() )
            {
                CurSprite->SetTranslationX
                    ( CurSprite->GetTranslationX() + CurSprite->GetWidth() );
            }
        }

        p_Current = p_Current->GetNextContainer();

        Offset++;
    }
}
