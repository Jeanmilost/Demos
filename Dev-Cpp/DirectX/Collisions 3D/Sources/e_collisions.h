/*****************************************************************************
 * ==> Classe E_Collisions --------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la détection des collisions entre le    *
 *               joueur et les objets du monde 3D.                           *
 * Version     : 1.3                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_COLLISIONS_H
#define E_COLLISIONS_H

#include <d3dx9.h>

#include "e_polygon.h"

/*
 * Cette classe permet la gestion des collisions.
 */
class E_Collisions
{
  private:

    E_Collisions();
    ~E_Collisions();

  public:

    static bool        GetTriSphereCollision ( D3DXVECTOR3 Position,
                                               float       Radius,
                                               E_Polygon   Polygon,
                                               D3DXPLANE*  SlidePlane );

    static D3DXVECTOR3 GetSlidingPoint       ( D3DXPLANE   SlidingPlane,
                                               D3DXVECTOR3 Position,
                                               float       Radius );

    static bool        IsPointIntoSphere     ( D3DXVECTOR3 Point,
                                               D3DXVECTOR3 SphereOrigin,
                                               float       SphereRadius );

    static bool        IsPointIntoTriangle   ( D3DXVECTOR3 Point,
                                               E_Polygon   Polygon );

    static D3DXVECTOR3 ClosestPointOnLine    ( D3DXVECTOR3 SegmentStart,
                                               D3DXVECTOR3 SegmentEnd,
                                               D3DXVECTOR3 Point );

    static D3DXVECTOR3 ClosestPointOnTriangle( D3DXVECTOR3 Point,
                                               E_Polygon   Polygon );

    static float       GetDistanceToPlane    ( D3DXPLANE   thePlane,
                                               D3DXVECTOR3 thePoint );
};

#endif // E_COLLISIONS_H
