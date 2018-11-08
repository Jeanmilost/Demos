/*****************************************************************************
 * ==> Classe E_BoundingSphere ----------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente une sphère de collisions pour un    *
 *               objet.                                                      *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_BOUNDINGSPHERE_H
#define E_BOUNDINGSPHERE_H

#include <d3dx9.h>

/*
 * Cette structure permet d'accéder aux données de la boîte virtuelle.
 */
struct E_SPHERE_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
};

/*
 * Cette classe représente une sphère de collisions pour un objet.
 */
class E_BoundingSphere
{
    D3DXVECTOR3         p_Center;
    float               p_Radius;

    D3DMATERIAL9        p_SphereMaterial;

    LPD3DXMESH          p_Sphere;

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

    E_BoundingSphere();
    E_BoundingSphere( D3DXVECTOR3 Center, float Radius );

    ~E_BoundingSphere();

  public:

    void        SetCenter           ( D3DXVECTOR3 Center );
    D3DXVECTOR3 GetCenter           ();
    void        SetRadius           ( float Radius );
    float       GetRadius           ();

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

#endif // E_BOUNDINGSPHERE_H
