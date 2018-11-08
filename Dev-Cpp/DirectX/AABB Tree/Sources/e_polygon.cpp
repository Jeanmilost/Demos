/*****************************************************************************
 * ==> Classe E_Polygon -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un polygone à 3 sommets.            *
 * Version     : 1.1                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_polygon.h"

// Constructeur de la classe E_Polygon.
E_Polygon::E_Polygon()
{
    p_BoundingBox = NULL;

    p_Vertex[0]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Vertex[1]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Vertex[2]   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

// Deuxième constructeur de la classe E_Polygon.
E_Polygon::E_Polygon( D3DXVECTOR3 Vertex1,
                      D3DXVECTOR3 Vertex2,
                      D3DXVECTOR3 Vertex3 )
{
    p_BoundingBox = NULL;

    p_Vertex[0]   = Vertex1;
    p_Vertex[1]   = Vertex2;
    p_Vertex[2]   = Vertex3;
}

// Destructeur de la classe E_Polygon.
E_Polygon::~E_Polygon()
{
    // On nettoie les ressources de l'objet BoundingBox.
    if ( p_BoundingBox != NULL )
    {
        delete p_BoundingBox;
        p_BoundingBox = NULL;
    }
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
bool E_Polygon::SetVertex( int Index, D3DXVECTOR3 Vertex )
{
    // On applique un masque binaire de 3 sur l'index (valeur binaire 0011),
    // ceci afin de garantir que l'utilisateur ne pourra jamais dépasser les
    // valeurs limites du tableau de sommets. C'est ce que l'on appelle un
    // tableau circulaire.
    p_Vertex[Index&3] = Vertex;

    UpdateBoundingBox();

    return true;
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
    UpdateBoundingBox();
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
    UpdateBoundingBox();
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
    UpdateBoundingBox();
}

// Obtient la boîte de collision du polygone.
E_BoundingBox* E_Polygon::GetBoundingBox()
{
    if ( p_BoundingBox == NULL )
    {
        p_BoundingBox = new E_BoundingBox();

        if ( p_BoundingBox == NULL )
        {
            return NULL;
        }
    }

    return p_BoundingBox;
}

// Cette fonction met à jour les données de la boîte de collisions.
void E_Polygon::UpdateBoundingBox()
{
    if ( p_BoundingBox != NULL )
    {
        for ( int i = 0; i < 3; i++ )
        {
            p_BoundingBox->SetMax
                ( E_Maths::GetMaxValue( p_Vertex[i],
                                        p_BoundingBox->GetMax() ) );

            p_BoundingBox->SetMin
                ( E_Maths::GetMinValue( p_Vertex[i],
                                        p_BoundingBox->GetMin() ) );
        }
    }
}

// Obtient le plan du polygone.
D3DXPLANE E_Polygon::GetPlane()
{
    D3DXPLANE thePlane;

    D3DXPlaneFromPoints( &thePlane, &p_Vertex[0], &p_Vertex[1], &p_Vertex[2] );

    return thePlane;
}

// Obtient le point central du polygone.
D3DXVECTOR3 E_Polygon::GetCenter()
{
    return D3DXVECTOR3
        ( ( ( p_Vertex[0].x + p_Vertex[1].x + p_Vertex[2].x ) / 3.0f ),
          ( ( p_Vertex[0].y + p_Vertex[1].y + p_Vertex[2].y ) / 3.0f ),
          ( ( p_Vertex[0].z + p_Vertex[1].z + p_Vertex[2].z ) / 3.0f ) );
}

// Obtient un clone du polygone.
E_Polygon E_Polygon::GetClone()
{
    return E_Polygon( p_Vertex[0], p_Vertex[1], p_Vertex[2] );
}

// Obtient un clone du polygone transformé selon une matrice donnée.
E_Polygon E_Polygon::ApplyMatrix( D3DXMATRIXA16 Matrix )
{
    D3DXVECTOR3 TVertex[3];

    D3DXVec3TransformCoord( &TVertex[0], &p_Vertex[0], &Matrix );
    D3DXVec3TransformCoord( &TVertex[1], &p_Vertex[1], &Matrix );
    D3DXVec3TransformCoord( &TVertex[2], &p_Vertex[2], &Matrix );

    return E_Polygon( TVertex[0], TVertex[1], TVertex[2] );
}
