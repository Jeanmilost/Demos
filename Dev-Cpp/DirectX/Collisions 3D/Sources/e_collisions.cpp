/*****************************************************************************
 * ==> Classe E_Collisions --------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la d�tection des collisions entre le    *
 *               joueur et les objets du monde 3D.                           *
 * Version     : 1.3                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_collisions.h"

// Calcule la collision entre une sph�re et un polygone.
bool E_Collisions::GetTriSphereCollision( D3DXVECTOR3 Position,
                                          float       Radius,
                                          E_Polygon   Polygon,
                                          D3DXPLANE*  SlidePlane )
{
    // On commence par d�finir 2 variables pour les points de tests.
    float TestPoint1 = 0.0f;
    float TestPoint2 = 0.0f;

    // On cr�e ici un plan � partir des 3 sommets du polygone.
    D3DXPLANE PolygonPlane = Polygon.GetPlane();

    // Puis, on calcule la distance entre le centre de la sph�re et le plan.
    TestPoint1 = GetDistanceToPlane( PolygonPlane, Position );

    D3DXVECTOR3 SphereNormal;

    // En fonction de la valeur positive ou n�gative de la distance entre le
    // centre de la sph�re et le plan du polygone, on calcule la normale de la
    // sph�re par rapport au plan.
    if ( TestPoint1 < 0.0f )
    {
        SphereNormal = D3DXVECTOR3( PolygonPlane.a,
                                    PolygonPlane.b,
                                    PolygonPlane.c );
    }
    else
    {
        SphereNormal = D3DXVECTOR3( -PolygonPlane.a,
                                    -PolygonPlane.b,
                                    -PolygonPlane.c );
    }

    // On calcule ici le point o� la droite partant du centre de la sph�re
    // en direction du plan va croiser le bord de la sph�re.
    D3DXVECTOR3 PointOnSphere =
        D3DXVECTOR3( Position.x + ( Radius * SphereNormal.x ),
                     Position.y + ( Radius * SphereNormal.y ),
                     Position.z + ( Radius * SphereNormal.z ) );

    // Puis, on calcule la distance entre le bord de la sph�re et le plan.
    TestPoint2 = GetDistanceToPlane( PolygonPlane, PointOnSphere );

    // Si les deux points de tests sont situ�s de chaque c�t� du plan, alors
    // la sph�re touche le plan. On assume le fait que, puisque le segment
    // partant du centre de la sph�re est calcul� en direction du plan du
    // polygone, ces deux objets ne peuvent jamais �tre co-planaires.
    if ( ( TestPoint1 <= 0.0f && TestPoint2 >= 0.0f ) ||
         ( TestPoint2 <= 0.0f && TestPoint1 >= 0.0f ) )
    {
        D3DXVECTOR3 PointOnPlane;

        // On commence par calculer o� le segment de droite a crois� le plan.
        if ( TestPoint1 == 0.0f )
        {
            // Si TestPoint1 vaut 0, le centre de la sph�re touche le plan.
            PointOnPlane = Position;
        }
        else if ( TestPoint2 == 0.0f )
        {
            // Si TestPoint2 vaut 0, le bord de la sph�re touche le plan.
            PointOnPlane = PointOnSphere;
        }
        else
        {
            // Sinon, on calcule le point d'intersection.
            D3DXPlaneIntersectLine( &PointOnPlane,
                                    &PolygonPlane,
                                    &Position,
                                    &PointOnSphere );
        }

        // Puis, on teste si ce point se trouve � l'int�rieur du polygone.
        if ( IsPointIntoTriangle( PointOnPlane, Polygon ) == true )
        {
            // Si oui, la sph�re est en collision avec le polygone.
            // Dans ce cas, on copie le plan, puis on retourne la r�ponse.
            SlidePlane->a = PolygonPlane.a;
            SlidePlane->b = PolygonPlane.b;
            SlidePlane->c = PolygonPlane.c;
            SlidePlane->d = PolygonPlane.d;

            return true;
        }
        else
        {
            // Si ce n'est pas le cas, on teste si la sph�re est en collision
            // avec le bord du polygone. On commence par calculer le point de
            // test sur le bord du polygone.
            D3DXVECTOR3 PointOnTriangle = ClosestPointOnTriangle( PointOnPlane,
                                                                  Polygon );

            // Puis, on teste si ce point est � l'int�rieur de la sph�re.
            if ( IsPointIntoSphere( PointOnTriangle,
                                    Position,
                                    Radius ) == true )
            {
                // Si oui, la sph�re est en collision avec le polygone.
                // Dans ce cas, on copie le plan, puis on retourne la r�ponse.
                SlidePlane->a = PolygonPlane.a;
                SlidePlane->b = PolygonPlane.b;
                SlidePlane->c = PolygonPlane.c;
                SlidePlane->d = PolygonPlane.d;

                return true;
            }
        }
    }

    // Si aucune collision n'a �t� trouv�e, on retourne une r�ponse n�gative.
    return false;
}

// Calcule la prochaine position valide par rapport au plan de collision.
D3DXVECTOR3 E_Collisions::GetSlidingPoint( D3DXPLANE   SlidingPlane,
                                           D3DXVECTOR3 Position,
                                           float       Radius )
{
    // On commence par calculer la distance entre le centre de la
    // sph�re et le plan.
    float DistanceToPlane = GetDistanceToPlane( SlidingPlane, Position );

    // On teste si la valeur est n�gative.
    if ( DistanceToPlane < 0.0f )
    {
        // Si la valeur est n�gative, il faut retourner le plan.
        SlidingPlane.a = -SlidingPlane.a;
        SlidingPlane.b = -SlidingPlane.b;
        SlidingPlane.c = -SlidingPlane.c;
        SlidingPlane.d = -SlidingPlane.d;
    }

    // Calcule la direction du vecteur position-plan.
    D3DXVECTOR3 PlaneRatio = D3DXVECTOR3( Radius * SlidingPlane.a,
                                          Radius * SlidingPlane.b,
                                          Radius * SlidingPlane.c );

    // Calcule le point o� le vecteur perpendiculaire au plan, en
    // partant du centre de la sph�re, croise la sph�re de collision.
    // Normalement, ce point est situ� apr�s le plan.
    D3DXVECTOR3 PointBehindPlane = D3DXVECTOR3( Position.x - PlaneRatio.x,
                                                Position.y - PlaneRatio.y,
                                                Position.z - PlaneRatio.z );

    D3DXVECTOR3 PointOnPlane;

    // Calcule le point o� le vecteur "centre de la sph�re"-"point
    // apr�s le plan" croise le plan de collision.
    D3DXPlaneIntersectLine( &PointOnPlane,
                            &SlidingPlane,
                            &Position,
                            &PointBehindPlane );

    // Depuis ce point, on ajoute le rayon de la sph�re, puis on retourne
    // la valeur.
    return D3DXVECTOR3( PointOnPlane.x + PlaneRatio.x,
                        PointOnPlane.y + PlaneRatio.y,
                        PointOnPlane.z + PlaneRatio.z );
}

// Teste si un point de l'espace se trouve � l'int�rieur d'une sph�re.
bool E_Collisions::IsPointIntoSphere( D3DXVECTOR3 Point,
                                      D3DXVECTOR3 SphereOrigin,
                                      float       SphereRadius )
{
    // On calcule la distance entre le point � tester et le centre de la sph�re.
    D3DXVECTOR3 Length   = Point - SphereOrigin;
    float       Distance = D3DXVec3Length( &Length );

    // Puis, on teste si la distance est plus courte que le rayon de la sph�re.
    if( Distance <= SphereRadius )
    {
        // Si c'est le cas, le point est � l'int�rieur de la sph�re. On
        // retourne une r�ponse positive.
        return true;
    }

    // Sinon, on retourne une r�ponse n�gative.
    return false;
}

/// Teste si un point de l'espace se trouve � l'int�rieur d'un polygone.
bool E_Collisions::IsPointIntoTriangle( D3DXVECTOR3 Point, E_Polygon Polygon )
{
    // On doit ici tester si le point P se trouve au centre du polygone.
    // Imaginons le sch�ma suivant:
    //
    //              Collision               Pas de collision
    //                  V1                         V1
    //                  /\                         /\
    //                 /  \                       /  \
    //                / *P \                  *P /    \
    //               /      \                   /      \
    //            V2 -------- V3             V2 -------- V3
    //
    // Nous savons ici que le point P est sur le plan du polygone, puisque
    // nous l'avons clacul� avant d'appeler cette fonction. Donc pour
    // calculer ce dont nous avons besoin, il suffit de cr�er trois 
    // vecteurs, entre le point P et chacun des sommets du polygone. Je
    // rappelle que le produit scalaire de 2 vecteurs permet d'obtenir l'angle
    // entre ceux-ci, donc en calculant par exemple le produit scalaire de
    // P-V1 et P-V2, on trouve l'angle A1. Si on fait la m�me chose avec les
    // deux autre combinaisons possibles (P-V2 et P-V3, ainsi que P-V3 et
    // P-V1), on obtient 3 angles DONT LA SOMME NE VAUT 360� QU'A CONDITION
    // QUE LE POINT P SE TROUVE A L'INTERIEUR DU TRIANGLE. Vous pouvez le
    // v�rifier visuellement en imaginant les vecteurs sur les sch�mas
    // ci-dessus.

    D3DXVECTOR3 V1 = Polygon.GetVertex1();
    D3DXVECTOR3 V2 = Polygon.GetVertex2();
    D3DXVECTOR3 V3 = Polygon.GetVertex3();

    // D'abord, On calcule le segment de droite entre le point P et
    // chacun des sommets du polygone, puis on normalise ce segment.
    // Pour cela, nous utilisons les formules suivantes :
    // PToV1 = Polygon.Vertex1 - Point;
    // PToV2 = Polygon.Vertex2 - Point;
    // PToV3 = Polygon.Vertex3 - Point;
    D3DXVECTOR3 PToV1;
    D3DXVECTOR3 NPToV1;

    D3DXVec3Subtract ( &PToV1, &V1, &Point );
    D3DXVec3Normalize( &NPToV1, &PToV1 );

    D3DXVECTOR3 PToV2;
    D3DXVECTOR3 NPToV2;

    D3DXVec3Subtract ( &PToV2, &V2, &Point );
    D3DXVec3Normalize( &NPToV2, &PToV2 );

    D3DXVECTOR3 PToV3;
    D3DXVECTOR3 NPToV3;

    D3DXVec3Subtract ( &PToV3, &V3, &Point );
    D3DXVec3Normalize( &NPToV3, &PToV3 );

    // Puis, on calcule les angles en utilisant le produit scalaire de chaque
    // paire de vecteurs. Pour cela, on utilise les formules suivantes :
    // A1 = NPToV1.x * NPToV2.x + NPToV1.y * NPToV2.y + NPToV1.z * NPToV2.z
    // A2 = NPToV2.x * NPToV3.x + NPToV2.y * NPToV3.y + NPToV2.z * NPToV3.z
    // A3 = NPToV3.x * NPToV1.x + NPToV3.y * NPToV1.y + NPToV3.z * NPToV1.z
    float A1 = D3DXVec3Dot( &NPToV1, &NPToV2 );
    float A2 = D3DXVec3Dot( &NPToV2, &NPToV3 );
    float A3 = D3DXVec3Dot( &NPToV3, &NPToV1 );

    // Ensuite, on calcule la somme des angles. Notez l'utilisation de l'arc-
    // cosinus, les valeurs contenues dans Ax sont les cosinus des angles
    // que nous recherchons.
    float AngleResult = (float)acosf( A1 ) +
                        (float)acosf( A2 ) +
                        (float)acosf( A3 );

    // Si la somme vaut 360�, alors P est au centre du polygone.
    if ( AngleResult >= 6.28f )
    {
        // Dans ce cas, on retourne une r�ponse positive.
        return true;
    }

    // Sinon, on retourne une r�ponse n�gative.
    return false;
}

// Calcule la coordonn�e projet�e d'un point sur un segment de droite.
D3DXVECTOR3 E_Collisions::ClosestPointOnLine( D3DXVECTOR3 SegmentStart,
                                              D3DXVECTOR3 SegmentEnd,
                                              D3DXVECTOR3 Point )
{
    // On commence par calculer la distance entre le point � tester et le point
    // de d�part du segment de droite, et la longeur du segment de droite.
    D3DXVECTOR3 PToStart      = Point      - SegmentStart;
    D3DXVECTOR3 Length        = SegmentEnd - SegmentStart;
    float       SegmentLength = D3DXVec3Length( &Length );

    // Puis, on calcule la direction du segment.
    D3DXVECTOR3 NormalizedLength;
    D3DXVec3Normalize( &NormalizedLength ,&Length );

    // On calcule ensuite la projection du segment de droite form� par la
    // distance Start -> Point sur le segment de droite Start -> End.
    float Angle = D3DXVec3Dot( &NormalizedLength, &PToStart );

    // On teste si la projection du point se trouve avant le d�but du segment.
    if ( Angle < 0.0f )
    {
        // Si c'est le cas, on retourne le point de d�part du segment.
        return SegmentStart;
    }

    // On teste si la projection du point se trouve apr�s la fin du segment.
    if ( Angle > SegmentLength )
    {
        // Si c'est le cas, on retourne le point de fin du segment.
        return SegmentEnd;
    }

    D3DXVECTOR3 P;

    // Sinon, on calcule la position de la projection du point sur le segment.
    P.x = NormalizedLength.x * Angle;
    P.y = NormalizedLength.y * Angle;
    P.z = NormalizedLength.z * Angle;

    // Puis, on retourne la coordonn�e du point sur le segment.
    return ( SegmentStart + P );
}

// Calcule la coordonn�e projet�e d'un point sur un polygone.
D3DXVECTOR3 E_Collisions::ClosestPointOnTriangle( D3DXVECTOR3 Point,
                                                  E_Polygon   Polygon )
{
    // On commence par calculer les points de projection sur chacun des c�t�s
    // du polygone.
    D3DXVECTOR3 Rab = ClosestPointOnLine( Polygon.GetVertex1(),
                                          Polygon.GetVertex2(),
                                          Point );
    D3DXVECTOR3 Rbc = ClosestPointOnLine( Polygon.GetVertex2(),
                                          Polygon.GetVertex3(),
                                          Point );
    D3DXVECTOR3 Rca = ClosestPointOnLine( Polygon.GetVertex3(),
                                          Polygon.GetVertex1(),
                                          Point );

    // Puis, on calcule les distances entre ces points de projections et le
    // point � tester.
    D3DXVECTOR3 vAB = Point - Rab;
    D3DXVECTOR3 vBC = Point - Rbc;
    D3DXVECTOR3 vCA = Point - Rca;

    // On calcule ensuite la longeur de chacun de ces segments de droite.
    float dAB = D3DXVec3Length( &vAB );
    float dBC = D3DXVec3Length( &vBC );
    float dCA = D3DXVec3Length( &vCA );

    // Puis, on recherche la distance la plus courte.
    float Min = dAB;

    D3DXVECTOR3 Result = Rab;

    // On teste si la distance dBC est plus courte que le r�sultat trouv�.
    if ( dBC < Min )
    {
        // Si c'est le cas, on met � jour les valeurs.
        Min   = dBC;
        Result = Rbc;
    }

    // On teste si la distance dCA est plus courte que le r�sultat trouv�.
    if ( dCA < Min )
    {
        // Si c'est le cas, on met � jour les valeurs.
        Result = Rca;
    }

    // Puis, on retourne le r�sultat.
    return Result;
}

// Calcule la distance entre un point de l'espace et un plan.
float E_Collisions::GetDistanceToPlane( D3DXPLANE   thePlane,
                                        D3DXVECTOR3 thePoint )
{
    // On r�cup�re la normale du plan.
    D3DXVECTOR3 N = D3DXVECTOR3( thePlane.a, thePlane.b, thePlane.c );

    // Puis, on calcule la distance entre le plan et le point.
    return D3DXVec3Dot( &N, &thePoint ) + thePlane.d;
}
