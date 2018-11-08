/*****************************************************************************
 * ==> Classe E_Polygon -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un polygone à 3 sommets.            *
 * Version     : 1.1                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_POLYGON_H
#define E_POLYGON_H

#include <d3dx9.h>

#include "e_boundingbox.h"
#include "e_maths.h"

/*
 * Cette classe représente un polygone à 3 sommets.
 */
class E_Polygon
{
  private:

    D3DXVECTOR3    p_Vertex[3];
    E_BoundingBox* p_BoundingBox;

  public:

    E_Polygon();
    E_Polygon( D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3 );

    ~E_Polygon();

  public:

    D3DXVECTOR3    GetVertex        ( int Index );
    bool           SetVertex        ( int Index, D3DXVECTOR3 Vertex );

    D3DXVECTOR3    GetVertex1       ();
    void           SetVertex1       ( D3DXVECTOR3 Value );
    D3DXVECTOR3    GetVertex2       ();
    void           SetVertex2       ( D3DXVECTOR3 Value );
    D3DXVECTOR3    GetVertex3       ();
    void           SetVertex3       ( D3DXVECTOR3 Value );

    E_BoundingBox* GetBoundingBox   ();
    D3DXPLANE      GetPlane         ();
    D3DXVECTOR3    GetCenter        ();
    E_Polygon      GetClone         ();
    E_Polygon      ApplyMatrix      ( D3DXMATRIXA16 Matrix );

  private:

    void           UpdateBoundingBox();
};

#endif // E_POLYGON_H
