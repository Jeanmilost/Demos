/*****************************************************************************
 * ==> Classe E_Maths -------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe fournit une collection d'outils pour résoudre  *
 *               des équations classiques de la mathématique algébrique et   *
 *               vectorielle.                                                *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_maths.h"

// Constructeur de la classe E_Maths.
E_Maths::E_Maths()
{
}

// Destructeur de la classe E_Maths.
E_Maths::~E_Maths()
{
}

// Cette fonction retourne la valeur maximum entre deux valeurs entières.
int E_Maths::GetMaxValue( int First, int Second )
{
    // On compare les deux valeurs.
    if ( First > Second )
    {
        // Si First est plus grand que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne la valeur maximum entre deux valeurs flottantes.
float E_Maths::GetMaxValue( float First, float Second )
{
    // On compare les deux valeurs.
    if ( First > Second )
    {
        // Si First est plus grand que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne le vecteur maximisé de deux vecteurs.
D3DXVECTOR3 E_Maths::GetMaxValue( D3DXVECTOR3 First, D3DXVECTOR3 Second )
{
    // Pour chaque valeur X, Y et Z, on recherche la valeur la plus grande,
    // puis on retourne le vecteur maximisé.
    return D3DXVECTOR3( GetMaxValue( First.x, Second.x ),
                        GetMaxValue( First.y, Second.y ),
                        GetMaxValue( First.z, Second.z ) );
}

// Cette fonction retourne la valeur minimum entre deux valeurs entières.
int E_Maths::GetMinValue( int First, int Second )
{
    // On compare les deux valeurs.
    if ( First < Second )
    {
        // Si First est plus petit que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne la valeur minimum entre deux valeurs flottantes.
float E_Maths::GetMinValue( float First, float Second )
{
    // On compare les deux valeurs.
    if ( First < Second )
    {
        // Si First est plus petit que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne le vecteur minimisé de deux vecteurs.
D3DXVECTOR3 E_Maths::GetMinValue( D3DXVECTOR3 First, D3DXVECTOR3 Second )
{
    // Pour chaque valeur X, Y et Z, on recherche la valeur la plus petite,
    // puis on retourne le vecteur minimisé.
    return D3DXVECTOR3( GetMinValue( First.x, Second.x ),
                        GetMinValue( First.y, Second.y ),
                        GetMinValue( First.z, Second.z ) );
}

// Cette fonction retourne un vecteur avec des valeurs non signées.
D3DXVECTOR3 E_Maths::GetUnsignedValue( D3DXVECTOR3 Value )
{
    // On teste le signe de chaque valeur, et on inverse les valeurs négatives,
    // pour les rendre positives. Ensuite, on retourne la réponse.
    return D3DXVECTOR3( Value.x < 0.0f ? -Value.x : Value.x,
                        Value.y < 0.0f ? -Value.y : Value.y,
                        Value.z < 0.0f ? -Value.z : Value.z );
}

// Cette fonction calcule, puis retourne un vecteur normalisé.
D3DXVECTOR3 E_Maths::Normalize( D3DXVECTOR3 Vector )
{
    // On calcule tout d'abord la valeur de la norme du vecteur.
    float Len = sqrt( ( Vector.x * Vector.x ) +
                      ( Vector.y * Vector.y ) +
                      ( Vector.z * Vector.z ) );

    // Ensuite, on divise le vecteur par sa norme, et on retourne la réponse.
    return D3DXVECTOR3( ( Vector.x / Len ),
                        ( Vector.y / Len ),
                        ( Vector.z / Len ) );
}

// Cette fonction calcule, puis retourne la longeur d'un vecteur.
float E_Maths::GetVectorLength( D3DXVECTOR3 Vector )
{
    // On calcule la longeur du vecteur, et on retourne la réponse.
    return (float)sqrt( ( Vector.x * Vector.x ) +
                        ( Vector.y * Vector.y ) +
                        ( Vector.z * Vector.z ) );
}

// Cette fonction calcule, puis retourne le produit vectoriel de deux vecteurs.
D3DXVECTOR3 E_Maths::GetCrossProduct( D3DXVECTOR3 V1, D3DXVECTOR3 V2 )
{
    // On calcule le produit vectoriel des vecteurs, et on retourne la réponse.
    return D3DXVECTOR3( ( V1.y * V2.z ) - ( V2.y * V1.z ),
                        ( V1.z * V2.x ) - ( V2.z * V1.x ),
                        ( V1.x * V2.y ) - ( V2.x * V1.y ) );
}

// Cette fonction calcule, puis retourne le produit scalaire de deux vecteurs.
float E_Maths::GetDotProduct( D3DXVECTOR3 V1, D3DXVECTOR3 V2 )
{
    // On calcule le produit scalaire des vecteurs, et on retourne la réponse.
    return ( ( V1.x * V2.x ) + ( V1.y * V2.y ) + ( V1.z * V2.z ) );
}

// Calcule la distance entre un point de l'espace et un plan.
float E_Maths::GetDistanceToPlane( D3DXPLANE Plane, D3DXVECTOR3 Point )
{
    // On récupère la normale du plan.
    D3DXVECTOR3 N = D3DXVECTOR3( Plane.a, Plane.b, Plane.c );

    // Puis, on calcule la distance entre le plan et le point.
    return D3DXVec3Dot( &N, &Point ) + Plane.d;
}
