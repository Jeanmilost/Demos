/*****************************************************************************
 * ==> Classe E_BoundingBox -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente une boîte de collisions pour un     *
 *               objet.                                                      *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_BOUNDINGBOX_H
#define E_BOUNDINGBOX_H

#include <d3dx9.h>

#include "e_maths.h"

/*
 * Cette structure permet d'accéder aux données de la boîte virtuelle.
 */
struct E_BOX_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
};

/*
 * Cette classe représente une boîte de collisions pour un objet.
 */
class E_BoundingBox
{
  private:

    D3DXVECTOR3         p_Min;
    D3DXVECTOR3         p_Max;

    D3DMATERIAL9        p_BoxMaterial;

    LPD3DXMESH          p_Box;

    unsigned char       p_R;
    unsigned char       p_G;
    unsigned char       p_B;

    DWORD               p_NumMaterials;
    DWORD               p_AdressU;
    DWORD               p_AdressV;
    DWORD               p_ZWriteEnable;
    DWORD               p_AlphaBlendEnable;
    DWORD               p_AlphaTestEnable;
    DWORD               p_SrcBlend;
    DWORD               p_DestBlend;
    DWORD               p_CullMode;

  public:

    E_BoundingBox();
    E_BoundingBox( D3DXVECTOR3 Min, D3DXVECTOR3 Max );

    ~E_BoundingBox();

  public:

    D3DXVECTOR3 GetMin              ();
    void        SetMin              ( D3DXVECTOR3 Min );
    D3DXVECTOR3 GetMax              ();
    void        SetMax              ( D3DXVECTOR3 Max );

    D3DXVECTOR3 GetCenter           ();
    E_Axis      GetLongestAxis      ();
    D3DXPLANE   GetSplittingPlane   ( E_Axis Axis );
    D3DXPLANE   GetSplittingPlane   ( E_Axis Axis, D3DXVECTOR3 Center );

    bool        CreateVirtualObject ( LPDIRECT3DDEVICE9 theD3DDevice,
                                      unsigned char     r,
                                      unsigned char     g,
                                      unsigned char     b );
    void        ReleaseVirtualObject();
    void        RenderVirtualObject ( LPDIRECT3DDEVICE9 theD3DDevice,
                                      D3DXMATRIXA16     ObjectMatrix );
    bool        RestoreVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice );

  private:

    void        SetAlphaBlending    ( LPDIRECT3DDEVICE9 theD3DDevice );
    void        PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice );
    void        PopDeviceProperties ( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_BOUNDINGBOX_H
