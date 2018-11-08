#include "sotb_landscape.h"

SOTB_LandscapeContainer::SOTB_LandscapeContainer()
{
    p_Element = NULL;
    p_Next    = NULL;
}

SOTB_LandscapeContainer::~SOTB_LandscapeContainer()
{
    if ( p_Element != NULL )
    {
        delete p_Element;
        p_Element = NULL;
    }
}

SOTB_Element* SOTB_LandscapeContainer::GetElement()
{
    return p_Element;
}

bool SOTB_LandscapeContainer::CreateElement( int Index,
                                             LPDIRECT3DDEVICE9 theD3DDevice )
{
    p_Element = new SOTB_Element();

    if ( p_Element == NULL )
    {
        return false;
    }

    return p_Element->Initialize( LandscapeCoord[Index].NbElements,
                                  LandscapeCoord[Index].Width,
                                  LandscapeCoord[Index].Height,
                                  LandscapeCoord[Index].X,
                                  LandscapeCoord[Index].Y,
                                  LandscapeCoord[Index].Z,
                                  LandscapeCoord[Index].Velocity,
                                  LandscapeCoord[Index].Transparency,
                                  LandscapeCoord[Index].Filename,
                                  theD3DDevice );
}

SOTB_LandscapeContainer* SOTB_LandscapeContainer::GetNextContainer()
{
    return p_Next;
}

void SOTB_LandscapeContainer::SetNextContainer
    ( SOTB_LandscapeContainer* Container )
{
    p_Next = Container;
}

SOTB_Landscape::SOTB_Landscape()
{
    p_Trees        = NULL;
    p_Zeppelin     = NULL;
    p_First        = NULL;
    p_Current      = NULL;

    p_FogStart     = 1000.0f;
    p_FogEnd       = 1999.0f;
    p_Fog          = FALSE;

    p_TreesXOffset = -4000;
}

SOTB_Landscape::~SOTB_Landscape()
{
    if ( p_Trees != NULL )
    {
        delete p_Trees;
        p_Trees = NULL;
    }

    if ( p_Zeppelin != NULL )
    {
        delete p_Zeppelin;
        p_Zeppelin = NULL;
    }

    p_Current = p_First;

    while ( p_Current != NULL )
    {
        SOTB_LandscapeContainer* p_Next = p_Current->GetNextContainer();

        delete p_Current;

        p_Current = p_Next;
    }

    p_Current = NULL;
    p_First   = NULL;
}

bool SOTB_Landscape::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    int Index = 0;

    p_Zeppelin = new E_Sprite();

    if ( p_Zeppelin == NULL )
    {
        return false;
    }

    p_Zeppelin->SetFilename( "Graphics\\Zeppelin.png" );

    p_Zeppelin->SetWidth ( 128.0f );
    p_Zeppelin->SetHeight( 64.0f );

    p_Zeppelin->SetTranslationX( -500.0f );
    p_Zeppelin->SetTranslationY( 90.0f );
    p_Zeppelin->SetTranslationZ( 998.0f );

    p_Zeppelin->SetAlphaBlending( true );

    if ( p_Zeppelin->CreateTexturedSurface( theD3DDevice ) == false )
    {
        return false;
    }

    p_Trees = new E_Sprite();

    if ( p_Trees == NULL )
    {
        return false;
    }

    p_Trees->SetFilename( "Graphics\\Arbres.png" );

    p_Trees->SetWidth ( 1666.0f );
    p_Trees->SetHeight( 346.0f );

    p_Trees->SetTranslationX( -4000.0f );
    p_Trees->SetTranslationY( 17.0f );
    p_Trees->SetTranslationZ( 100.0f );

    p_Trees->SetAlphaBlending( true );

    if ( p_Trees->CreateTexturedSurface( theD3DDevice ) == false )
    {
        return false;
    }

    while ( LandscapeCoord[Index].IsLast == false )
    {
        SOTB_LandscapeContainer* p_New = new SOTB_LandscapeContainer();

        if ( p_New == NULL )
        {
            return false;
        }

        if ( p_New->CreateElement( Index, theD3DDevice ) == false )
        {
            return false;
        }

        if ( p_First == NULL )
        {
            p_First = p_New;
            p_Current = p_First;
        }
        else
        {
            p_Current->SetNextContainer( p_New );
            p_Current = p_Current->GetNextContainer();
        }

        Index++;
    }

    // On active ensuite la fonction de brouillard de Direct3D.
    theD3DDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );

    // Puis, on règle les paramètres du brouillard.
    theD3DDevice->SetRenderState( D3DRS_FOGCOLOR, 0xDF8A9A );

    theD3DDevice->SetRenderState( D3DRS_FOGDENSITY,     FToDW( 0.0f ) );
    theD3DDevice->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
    theD3DDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
    theD3DDevice->SetRenderState( D3DRS_RANGEFOGENABLE, TRUE );

    return true;
}

void SOTB_Landscape::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->SetRenderState( D3DRS_FOGSTART, FToDW( p_FogStart ) );
    theD3DDevice->SetRenderState( D3DRS_FOGEND,   FToDW( p_FogEnd ) );

    theD3DDevice->SetRenderState( D3DRS_FOGENABLE, p_Fog );

    p_Current = p_First;

    if ( p_Current != NULL )
    {
        p_Current->GetElement()->Render( theD3DDevice );
    }

    p_Current = p_Current->GetNextContainer();

    if ( p_Current != NULL )
    {
        p_Current->GetElement()->Render( theD3DDevice );
    }

    if ( p_Zeppelin != NULL )
    {
        p_Zeppelin->Render( theD3DDevice );
    }

    p_Current = p_Current->GetNextContainer();

    while ( p_Current->GetNextContainer() != NULL )
    {
        p_Current->GetElement()->Render( theD3DDevice );
        p_Current = p_Current->GetNextContainer();
    }

    theD3DDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

    if ( p_Trees != NULL )
    {
        p_Trees->Render( theD3DDevice );
    }

    p_Current->GetElement()->Render( theD3DDevice );
}

bool SOTB_Landscape::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    return true;
}

void SOTB_Landscape::Scroll()
{
    if ( p_Trees != NULL )
    {
        p_TreesXOffset += 12;

        p_Trees->SetTranslationX( p_Trees->GetTranslationX() + 12.0f );

        if ( p_Trees->GetTranslationX() > 4000.0f )
        {
            p_TreesXOffset = -4000;
            p_Trees->SetTranslationX( -4000.0f );
        }
    }

    if ( p_TreesXOffset < -833 || p_TreesXOffset > 833 )
    {
        p_Fog = FALSE;
    }
    else
    {
        int FogValue;

        if ( p_TreesXOffset < 0 )
        {
            FogValue = -p_TreesXOffset;
        }
        else
        {
            FogValue = p_TreesXOffset;
        }

        p_FogStart = 200.0f  + ( ( FogValue * 1000.0f ) / 833.0f );
        p_FogEnd   = 1000.0f + ( ( FogValue * 1000.0f ) / 833.0f );

        p_Fog = TRUE;
    }

    if ( p_Zeppelin != NULL )
    {
        p_Zeppelin->SetTranslationX( p_Zeppelin->GetTranslationX() + 0.2f );

        if ( p_Zeppelin->GetTranslationX() > 500.0f )
        {
            p_Zeppelin->SetTranslationX( -500.0f );
        }
    }

    p_Current = p_First;

    while ( p_Current != NULL )
    {
        p_Current->GetElement()->Scroll();
        p_Current = p_Current->GetNextContainer();
    }
}
