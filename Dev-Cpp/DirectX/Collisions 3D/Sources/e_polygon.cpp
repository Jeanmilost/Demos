/*****************************************************************************
 * ==> Classe E_Polygon -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un polygone à 3 sommets.            *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_polygon.h"

// Constructeur de la classe E_Polygon.
E_Polygon::E_Polygon()
{
    p_Vertex[0]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Vertex[1]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Vertex[2]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

// Deuxième constructeur de la classe E_Polygon.
E_Polygon::E_Polygon( D3DXVECTOR3 Vertex1,
                      D3DXVECTOR3 Vertex2,
                      D3DXVECTOR3 Vertex3 )
{
    p_Vertex[0]   = Vertex1;
    p_Vertex[1]   = Vertex2;
    p_Vertex[2]   = Vertex3;
}

// Destructeur de la classe E_Polygon.
E_Polygon::~E_Polygon()
{
}

// Obtient le sommet à l'index spécifié.
D3DXVECTOR3 E_Polygon::GetVertex( int Index )
{
    // On applique un masque binaire de 3 sur l'index (valeur binaire 0011),
    // ceci afin de garantir que l'utilisateur ne pourra jamais dépasser les
    // valeurs limites du tableau de sommets. C'est ce que l'on appelle un
    // tableau circulaire.
    return p_Vertex[Index&3];
}

// Définit le sommet à l'index spécifié.
void E_Polygon::SetVertex( int Index, D3DXVECTOR3 Vertex )
{
    // On applique un masque binaire de 3 sur l'index (valeur binaire 0011),
    // ceci afin de garantir que l'utilisateur ne pourra jamais dépasser les
    // valeurs limites du tableau de sommets. C'est ce que l'on appelle un
    // tableau circulaire.
    p_Vertex[Index&3] = Vertex;
}

// Obtient la coordonnée du premier sommet du polygone.
D3DXVECTOR3 E_Polygon::GetVertex1()
{
    return p_Vertex[0];
}

// Définit la coordonnée du premier sommet du polygone.
void E_Polygon::SetVertex1( D3DXVECTOR3 Value )
{
    p_Vertex[0] = Value;
}

// Obtient la coordonnée du deuxième sommet du polygone.
D3DXVECTOR3 E_Polygon::GetVertex2()
{
    return p_Vertex[1];
}

// Définit la coordonnée du deuxième sommet du polygone.
void E_Polygon::SetVertex2( D3DXVECTOR3 Value )
{
    p_Vertex[1] = Value;
}

// Obtient la coordonnée du troisième sommet du polygone.
D3DXVECTOR3 E_Polygon::GetVertex3()
{
    return p_Vertex[2];
}

// Définit la coordonnée du troisième sommet du polygone.
void E_Polygon::SetVertex3( D3DXVECTOR3 Value )
{
    p_Vertex[2] = Value;
}

// Obtient le plan du polygone.
D3DXPLANE E_Polygon::GetPlane()
{
    D3DXPLANE thePlane;

    // On calcule le plan à partir des valeurs des 3 sommets du polygone.
    D3DXPlaneFromPoints( &thePlane, &p_Vertex[0], &p_Vertex[1], &p_Vertex[2] );

    // Puis, on retourne le plan.
    return thePlane;
}

// Obtient le point central du polygone.
D3DXVECTOR3 E_Polygon::GetCenter()
{
    // On calcule, puis on retourne la valeur du point central du polygone.
    return D3DXVECTOR3
        ( ( ( p_Vertex[0].x + p_Vertex[1].x + p_Vertex[2].x ) / 3.0f ),
          ( ( p_Vertex[0].y + p_Vertex[1].y + p_Vertex[2].y ) / 3.0f ),
          ( ( p_Vertex[0].z + p_Vertex[1].z + p_Vertex[2].z ) / 3.0f ) );
}

// Obtient un clone du polygone.
E_Polygon E_Polygon::GetClone()
{
    // On copie le polygone, puis on retourne la copie.
    return E_Polygon( p_Vertex[0], p_Vertex[1], p_Vertex[2] );
}

// Obtient un clone du polygone transformé selon une matrice donnée.
E_Polygon E_Polygon::ApplyMatrix( D3DXMATRIXA16 Matrix )
{
    D3DXVECTOR3 TVertex[3];

    // On transforme chacun des sommets du polygone avec la matrice donnée.
    D3DXVec3TransformCoord( &TVertex[0], &p_Vertex[0], &Matrix );
    D3DXVec3TransformCoord( &TVertex[1], &p_Vertex[1], &Matrix );
    D3DXVec3TransformCoord( &TVertex[2], &p_Vertex[2], &Matrix );

    // Puis, on retourne une copie du polygone, avec les nouvelles valeurs.
    return E_Polygon( TVertex[0], TVertex[1], TVertex[2] );
}
