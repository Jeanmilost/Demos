/*****************************************************************************
 * ==> Classe E_Maths -------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe fournit une collection d'outils pour r�soudre  *
 *               des �quations classiques de la math�matique alg�brique et   *
 *               vectorielle.                                                *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_MATHS_H
#define E_MATHS_H

#include <d3dx9.h>

/*
 * Enum�ration des axes possibles dans un syst�me d'axes 3D.
 */
enum E_Axis
{
    E_X_AXIS     = 0,
    E_Y_AXIS     = 1,
    E_Z_AXIS     = 2
};

/*
 * Cette classe est une collection d'outils pour la r�solution d'�quations.
 */
class E_Maths
{
  private:

    E_Maths();
    ~E_Maths();

  public:

    static int         GetMaxValue       ( int First, int Second );
    static float       GetMaxValue       ( float First, float Second );
    static D3DXVECTOR3 GetMaxValue       ( D3DXVECTOR3 First,
                                           D3DXVECTOR3 Second );

    static int         GetMinValue       ( int First, int Second );
    static float       GetMinValue       ( float First, float Second );
    static D3DXVECTOR3 GetMinValue       ( D3DXVECTOR3 First,
                                           D3DXVECTOR3 Second );

    static D3DXVECTOR3 GetUnsignedValue  ( D3DXVECTOR3 Value );

    static D3DXVECTOR3 Normalize         ( D3DXVECTOR3 Vector );

    static float       GetVectorLength   ( D3DXVECTOR3 Vector );
    static D3DXVECTOR3 GetCrossProduct   ( D3DXVECTOR3 V1, D3DXVECTOR3 V2 );
    static float       GetDotProduct     ( D3DXVECTOR3 V1, D3DXVECTOR3 V2 );
    static float       GetDistanceToPlane( D3DXPLANE Plane, D3DXVECTOR3 Point );
};

#endif // E_MATHS_H
