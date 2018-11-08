/*****************************************************************************
 * ==> Classe E_Polygon -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un polygone à 3 sommets.            *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_POLYGON_H
#define E_POLYGON_H

#include <d3dx9.h>

/*
 * Cette classe représente un polygone à 3 sommets.
 */
class E_Polygon
{
  private:

    D3DXVECTOR3    p_Vertex[3];

  public:

    E_Polygon();
    E_Polygon( D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3 );

    ~E_Polygon();

  public:

    D3DXVECTOR3 GetVertex  ( int Index );
    void        SetVertex  ( int Index, D3DXVECTOR3 Vertex );

    D3DXVECTOR3 GetVertex1 ();
    void        SetVertex1 ( D3DXVECTOR3 Value );
    D3DXVECTOR3 GetVertex2 ();
    void        SetVertex2 ( D3DXVECTOR3 Value );
    D3DXVECTOR3 GetVertex3 ();
    void        SetVertex3 ( D3DXVECTOR3 Value );

    D3DXPLANE   GetPlane   ();
    D3DXVECTOR3 GetCenter  ();
    E_Polygon   GetClone   ();
    E_Polygon   ApplyMatrix( D3DXMATRIXA16 Matrix );
};

#endif // E_POLYGON_H
