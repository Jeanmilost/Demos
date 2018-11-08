/*****************************************************************************
 * ==> Classe E_Collisions --------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe met à disposition un ensemble d'outils pour    *
 *               permettre le clacul des collisions dans un univers 3D.      *
 * Version     : 1.3                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_collisions.h"

// **************************** E_CollisionLayout *****************************
// Cette classe contient des outils pour faciliter le calcul d'une collision,
// ou pour calculer une collision entre deux objets simples.

// Retourne le point transformé selon l'inverse de la matrice donnée.
D3DXVECTOR3 E_CollisionLayout::GetInversedPosition( D3DXVECTOR3   Position,
                                                    D3DXMATRIXA16 Matrix )
{
    // On calcule l'inverse de la matrice de transformation.
    D3DXMATRIXA16 InvMat;
    D3DXMatrixInverse( &InvMat, NULL, &Matrix );

    // Puis, on transforme le point avec la nouvelle matrice.
    D3DXVECTOR3 Result;
    D3DXVec3TransformCoord( &Result, &Position, &InvMat );

    // Pour finir, on retourne la réponse.
    return Result;
}

/// Teste si un point de l'espace se trouve à l'intérieur d'un polygone.
bool E_CollisionLayout::IsPointIntoTriangle( D3DXVECTOR3 Point,
                                             E_Polygon   Polygon )
{
    // On doit ici tester si le point P se trouve au centre du polygone.
    // Imaginons le schéma suivant:
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
    // nous l'avons claculé avant d'appeler cette fonction. Donc pour
    // calculer ce dont nous avons besoin, il suffit de créer trois 
    // vecteurs, entre le point P et chacun des sommets du polygone. Je
    // rappelle que le produit scalaire de 2 vecteurs permet d'obtenir l'angle
    // entre ceux-ci, donc en calculant par exemple le produit scalaire de
    // P-V1 et P-V2, on trouve l'angle A1. Si on fait la même chose avec les
    // deux autre combinaisons possibles (P-V2 et P-V3, ainsi que P-V3 et
    // P-V1), on obtient 3 angles DONT LA SOMME NE VAUT 360° QU'A CONDITION
    // QUE LE POINT P SE TROUVE A L'INTERIEUR DU TRIANGLE. Vous pouvez le
    // vérifier visuellement en imaginant les vecteurs sur les schémas
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

    // Puis, on calcule la somme des angles. Notez l'utilisation de l'arc-
    // cosinus, les valeurs contenues dans Ax sont les cosinus des angles
    // que nous recherchons.
    float AngleResult = (float)acosf( A1 ) +
                        (float)acosf( A2 ) +
                        (float)acosf( A3 );

    // Si la somme vaut 360°, alors P est au centre du polygone.
    if ( AngleResult >= 6.28f )
    {
        // Si c'est le cas, on retourne une réponse positive.
        return true;
    }

    // Sinon, on retourne une réponse négative.
    return false;
}

/// Teste si un point de l'espace se trouve à l'intérieur d'une sphère.
bool E_CollisionLayout::IsPointIntoSphere( D3DXVECTOR3 Point,
                                           D3DXVECTOR3 SphereOrigin,
                                           float       SphereRadius )
{
    // On calcule la distance entre le point à tester et le centre de la sphère.
    D3DXVECTOR3 Length   = Point - SphereOrigin;
    float       Distance = D3DXVec3Length( &Length );

    // Puis, on teste si la distance est plus courte que le rayon de la sphère.
    if( Distance <= SphereRadius )
    {
        // Si c'est le cas, le point est à l'intérieur de la sphère. On
        // retourne une réponse positive.
        return true;
    }

    // Sinon, on retourne une réponse négative.
    return false;
}

// Cette fonction détermine si une sphère croise un plan.
bool E_CollisionLayout::SphereIntersectPlane( D3DXVECTOR3 Center,
                                              float       Radius,
                                              D3DXPLANE   Plane )
{
    // On calcule la distance entre le centre de la sphère et le plan.
    float CenterToPlane = E_Maths::GetDistanceToPlane( Plane, Center );

    D3DXVECTOR3 SphereNormal;

    // En fonction de la valeur positive ou négative de la distance
    // entre le centre de la sphère et le plan du polygone, on
    // calcule la normale de la sphère.
    if ( CenterToPlane < 0.0f )
    {
        SphereNormal = D3DXVECTOR3( Plane.a, Plane.b, Plane.c );
    }
    else
    {
        SphereNormal = D3DXVECTOR3( -Plane.a, -Plane.b, -Plane.c );
    }

    // On calcule ensuite le point où la droite partant du centre de la sphère
    // en direction du plan va croiser le bord de la sphère.
    D3DXVECTOR3 PointOnSphere =
        D3DXVECTOR3( Center.x + ( Radius * SphereNormal.x ),
                     Center.y + ( Radius * SphereNormal.y ),
                     Center.z + ( Radius * SphereNormal.z ) );

    // ... et du point ou la droite a croisé la sphère.
    float RadiusToPlane = E_Maths::GetDistanceToPlane( Plane, PointOnSphere );

    // Si ces deux points sont situés de chaque côté du plan, alors la
    // sphère croise le plan.
    if ( ( CenterToPlane < 0.0f && RadiusToPlane > 0.0f ) ||
         ( CenterToPlane < 0.0f && RadiusToPlane > 0.0f ) )
    {
        // Dans ce cas, on retourne une réponse positive.
        return true;
    }

    // Sinon, on retourne une réponse négative.
    return false;
}

// Cette fonction détermine si deux sphères se touchent.
bool E_CollisionLayout::SphereIntersectSphere( D3DXVECTOR3 Sphere1Center,
                                               float       Sphere1Radius,
                                               D3DXVECTOR3 Sphere2Center,
                                               float       Sphere2Radius )
{
    // On calcule tout d'abord la distance entre les deux sphères.
    D3DXVECTOR3 SphereDist = Sphere2Center - Sphere1Center;
    float Distance = D3DXVec3Length( &SphereDist );

    // Si la distance est plus courte ou égale à l'addition des deux rayons,
    // alors les deux sphères sont en collision.
    if ( Distance <= Sphere1Radius + Sphere2Radius )
    {
        // Dans ce cas, on retourne une réponse positive.
        return true;
    }

    // Sinon, on retourne une réponse négative.
    return false;
}

// Cette fonction détermine si une sphère croise une boîte.
bool E_CollisionLayout::SphereIntersectBox( D3DXVECTOR3 SphereCenter,
                                            float       SphereRadius,
                                            D3DXVECTOR3 BoxMin,
                                            D3DXVECTOR3 BoxMax )
{
    // Soit A la longeur, largeur ou hauteur de la boîte, B le rayon de la
    // sphère, et C la longeur entre le centre de la sphère et le centre de la
    // boîte. On peut considérer que si C <= A + B, alors la sphère touche la
    // boîte de collisions.
    //
    //      A         B
    // |---------| |-----|
    // |-----------------|
    //          C

    // On calcule la longeur du segment A sur les 3 axes.
    D3DXVECTOR3 ALength = D3DXVECTOR3( ( BoxMax.x - BoxMin.x ) / 2.0f,
                                       ( BoxMax.y - BoxMin.y ) / 2.0f,
                                       ( BoxMax.z - BoxMin.z ) / 2.0f );

    float A[3];

    // Puis, on s'assure que les résultats soit plus grand que 0.
    A[0] = ALength.x < 0.0f ? -ALength.x : ALength.x;
    A[1] = ALength.y < 0.0f ? -ALength.y : ALength.y;
    A[2] = ALength.z < 0.0f ? -ALength.z : ALength.z;

    // On calcule la longeur du segment B.
    float B = SphereRadius;

    // On calcule le centre de la boîte de collisions.
    D3DXVECTOR3 BoxCenter = D3DXVECTOR3( ( BoxMin.x + BoxMax.x ) / 2.0f,
                                         ( BoxMin.y + BoxMax.y ) / 2.0f,
                                         ( BoxMin.z + BoxMax.z ) / 2.0f );

    // On calcule la longeur du segment C.
    D3DXVECTOR3 CLength = SphereCenter - BoxCenter;

    float C = D3DXVec3Length( &CLength );

    // Puis, on passe en revue les 3 axes.
    for ( int i = 0; i < 3; i++ )
    {
        // Sur chaque axe, on teste si C est plus petit ou égal à A + B.
        if ( C <= ( A[i] + B ) )
        {
            // Si c'est le cas, la sphère touche la boîte. Dans ce cas, on
            // retourne une réponse positive.
            return true;
        }
    }

    // Sinon, on retourne une réponse négative.
    return false;
}

// Calcule la coordonnée projetée d'un point sur un segment de droite.
D3DXVECTOR3 E_CollisionLayout::ClosestPointOnLine( D3DXVECTOR3 SegmentStart,
                                                   D3DXVECTOR3 SegmentEnd,
                                                   D3DXVECTOR3 Point )
{
    // On commence par calculer la distance entre le point à tester et le point
    // de départ du segment de droite, et la longeur du segment de droite.
    D3DXVECTOR3 PToStart      = Point      - SegmentStart;
    D3DXVECTOR3 Length        = SegmentEnd - SegmentStart;
    float       SegmentLength = D3DXVec3Length( &Length );

    // Puis, on calcule la direction du segment.
    D3DXVECTOR3 NormalizedLength;
    D3DXVec3Normalize( &NormalizedLength ,&Length );

    // On calcule ensuite la projection du segment de droite à tester sur le
    // côté du polygone. Je rappelle ici que le produit scalaire permet aussi
    // de calculer la projection d'un vecteur sur un autre vecteur.
    float Angle = D3DXVec3Dot( &NormalizedLength, &PToStart );

    // On teste si la projection du point se trouve avant le début du segment.
    if ( Angle < 0.0f )
    {
        // Si c'est le cas, on retourne le point de départ du segment.
        return SegmentStart;
    }

    // On teste si la projection du point se trouve après la fin du segment.
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

    // Puis, on retourne la coordonnée du point projeté sur le segment.
    return ( SegmentStart + P );
}

// Calcule la coordonnée projetée d'un point sur un polygone.
D3DXVECTOR3 E_CollisionLayout::ClosestPointOnTriangle( D3DXVECTOR3 Point,
                                                       E_Polygon   Polygon )
{
    // On commence par calculer les points de projection sur chacun des côtés
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
    // point à tester.
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

    // On teste si la distance dBC est plus courte que le résultat trouvé.
    if ( dBC < Min )
    {
        // Si c'est le cas, on met à jour les valeurs.
        Min   = dBC;
        Result = Rbc;
    }

    // On teste si la distance dCA est plus courte que le résultat trouvé.
    if ( dCA < Min )
    {
        // Si c'est le cas, on met à jour les valeurs.
        Result = Rca;
    }

    // Puis, on retourne le résultat.
    return Result;
}

// **************************** E_SphereCollision *****************************
// Cette classe calcule une collision entre une sphère et un polygone.

// Cette fonction teste si une sphère est en collision avec un polygone.
bool E_SphereCollision::Check( D3DXVECTOR3 Position,
                               float       Radius,
                               E_Polygon   Polygon )
{
    // On commence par définir 2 variables pour les points de tests.
    float TestPoint1 = 0.0f;
    float TestPoint2 = 0.0f;

    // On crée ici un plan à partir des 3 sommets du polygone.
    D3DXPLANE PolygonPlane = Polygon.GetPlane();

    // Puis, on calcule la distance entre le centre de la sphère et le plan.
    TestPoint1 = E_Maths::GetDistanceToPlane( PolygonPlane, Position );

    D3DXVECTOR3 SphereNormal;

    // En fonction de la valeur positive ou négative de la distance entre le
    // centre de la sphère et le plan du polygone, on calcule la normale de la
    // sphère par rapport au plan.
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

    // On calcule ici le point où la droite partant du centre de la sphère
    // en direction du plan va croiser le bord de la sphère.
    D3DXVECTOR3 PointOnSphere =
        D3DXVECTOR3( Position.x + ( Radius * SphereNormal.x ),
                     Position.y + ( Radius * SphereNormal.y ),
                     Position.z + ( Radius * SphereNormal.z ) );

    // Puis, on calcule la distance entre le bord de la sphère et le plan.
    TestPoint2 = E_Maths::GetDistanceToPlane( PolygonPlane, PointOnSphere );

    // Si les deux points de tests sont situés de chaque côté du plan, alors
    // la sphère touche le plan. On assume le fait que, puisque le segment
    // partant du centre de la sphère est calculé en direction du plan du
    // polygone, ces deux objets ne peuvent jamais être co-planaire.
    if ( ( TestPoint1 <= 0.0f && TestPoint2 >= 0.0f ) ||
         ( TestPoint2 <= 0.0f && TestPoint1 >= 0.0f ) )
    {
        D3DXVECTOR3 PointOnPlane;

        // On commence par calculer où le segment de droite a croisé le plan.
        if ( TestPoint1 == 0.0f )
        {
            // Si TestPoint1 vaut 0, le centre de la sphère touche le plan.
            PointOnPlane = Position;
        }
        else if ( TestPoint2 == 0.0f )
        {
            // Si TestPoint2 vaut 0, le bord de la sphère touche le plan.
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

        // Puis, on teste si ce point se trouve à l'intérieur du polygone.
        if ( E_CollisionLayout::IsPointIntoTriangle( PointOnPlane,
                                                     Polygon ) == true )
        {
            // Si oui, la sphère est en collision avec le polygone.
            // Dans ce cas, on retourne la réponse.
            return true;
        }
        else
        {
            // Si ce n'est pas le cas, on teste si la sphère est en collision
            // avec le bord du polygone. On commence par calculer le point de
            // test sur le bord du polygone.
            D3DXVECTOR3 PointOnTriangle =
                E_CollisionLayout::ClosestPointOnTriangle( PointOnPlane,
                                                           Polygon );

            // Puis, on teste si ce point est à l'intérieur de la sphère.
            if ( E_CollisionLayout::IsPointIntoSphere( PointOnTriangle,
                                                       Position,
                                                       Radius ) == true )
            {
                // Si oui, la sphère est en collision avec le polygone.
                // Dans ce cas, on retourne la réponse.
                return true;
            }
        }
    }

    // Si aucune collision n'a été trouvée, on retourne la réponse.
    return false;
}
