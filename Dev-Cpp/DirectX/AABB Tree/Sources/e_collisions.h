/*****************************************************************************
 * ==> Classe E_Collisions --------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe met à disposition un ensemble d'outils pour    *
 *               permettre le clacul des collisions dans un univers 3D.      *
 * Version     : 1.3                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_COLLISIONS_H
#define E_COLLISIONS_H

#include <windows.h>
#include <d3dx9.h>

#include "e_maths.h"
#include "e_polygon.h"

/*
 * Enumérateur des résultats possibles des tests de plans.
 */
enum E_Side
{
    E_FRONT    = 0,
    E_BACK     = 1,
    E_SPLITTED = 2,
    E_COPLANAR = 3,
};

// **************************** E_CollisionLayout *****************************
// Cette classe contient des outils pour faciliter le calcul d'une collision,
// ou pour calculer une collision entre deux objets simples.

/*
 * Cette classe contient des outils pour calculer une collisions.
 */
class E_CollisionLayout
{
  private:

    E_CollisionLayout();
    ~E_CollisionLayout();

  public:

    static D3DXVECTOR3 GetInversedPosition   ( D3DXVECTOR3   Position,
                                               D3DXMATRIXA16 Matrix );

    static bool        IsPointIntoTriangle   ( D3DXVECTOR3 Point,
                                               E_Polygon Polygon );

    static bool        IsPointIntoSphere     ( D3DXVECTOR3 Point,
                                               D3DXVECTOR3 SphereOrigin,
                                               float       SphereRadius );

    static bool        SphereIntersectPlane  ( D3DXVECTOR3 Center,
                                               float       Radius,
                                               D3DXPLANE   Plane );

    static bool        SphereIntersectSphere ( D3DXVECTOR3 Sphere1Center,
                                               float       Sphere1Radius,
                                               D3DXVECTOR3 Sphere2Center,
                                               float       Sphere2Radius );

    static bool        SphereIntersectBox    ( D3DXVECTOR3 SphereCenter,
                                               float       SphereRadius,
                                               D3DXVECTOR3 BoxMin,
                                               D3DXVECTOR3 BoxMax );

    static D3DXVECTOR3 ClosestPointOnLine    ( D3DXVECTOR3 SegmentStart,
                                               D3DXVECTOR3 SegmentEnd,
                                               D3DXVECTOR3 Point );

    static D3DXVECTOR3 ClosestPointOnTriangle( D3DXVECTOR3 Point,
                                               E_Polygon   Polygon );
};

// **************************** E_SphereCollision *****************************
// Cette classe calcule une collision entre une sphère et un polygone.

/*
 * Cette classe calcule une collision entre une sphère et un polygone.
 */
class E_SphereCollision
{
  private:

    E_SphereCollision();
    ~E_SphereCollision();

  public:

    static bool Check( D3DXVECTOR3 Position,
                       float       Radius,
                       E_Polygon   Polygon );
};

#endif // E_COLLISIONS_H
