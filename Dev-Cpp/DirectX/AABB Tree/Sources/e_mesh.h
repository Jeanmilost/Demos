/*****************************************************************************
 * ==> Classe E_Mesh --------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de charger et d'afficher un objet       *
 *               depuis un fichier .x.                                       *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_MESH_H
#define E_MESH_H

#include <d3dx9.h>

#include "e_boundingbox.h"
#include "e_polygonlist.h"

/*
 * Classe E_Mesh.
 */
class E_Mesh
{
  private:

    LPD3DXMESH          p_Mesh;
    D3DMATERIAL9*       p_MeshMaterials;
    LPDIRECT3DTEXTURE9* p_MeshTextures;

    E_PolygonList*      p_PolygonList;

    DWORD               p_NumMaterials;
    DWORD               p_AdressU;
    DWORD               p_AdressV;
    DWORD               p_ZWriteEnable;
    DWORD               p_AlphaBlendEnable;
    DWORD               p_AlphaTestEnable;
    DWORD               p_SrcBlend;
    DWORD               p_DestBlend;

    LPSTR               p_Filename;

    float               p_rX;
    float               p_rY;
    float               p_rZ;
    float               p_tX;
    float               p_tY;
    float               p_tZ;

    bool                p_IsAlphaBlended;

  public:

    E_Mesh();
    ~E_Mesh();

  public:

    E_PolygonList*   GetPolygonList   ();
    E_BoundingBox    GetBoundingBox   ();
    E_BoundingSphere GetBoundingSphere();

    LPSTR GetFilename     ();
    void  SetFilename     ( LPSTR Filename );
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

    void  SetAlphaBlending( bool Value );
    bool  IsAlphaBlended  ();

    D3DXMATRIXA16 GetWorldMatrix();

    bool Initialize( LPDIRECT3DDEVICE9 theD3DDevice );
    bool Restore   ( LPDIRECT3DDEVICE9 theD3DDevice );

    void Render( LPDIRECT3DDEVICE9 theD3DDevice );

  private:

    void SetAlphaBlending    ( LPDIRECT3DDEVICE9 theD3DDevice );
    void PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice );
    void PopDeviceProperties ( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_MESH_H
