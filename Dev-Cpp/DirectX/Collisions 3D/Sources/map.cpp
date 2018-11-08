/*****************************************************************************
 * ==> Fichier Map.cpp ------------------------------------------------------*
 * ***************************************************************************
 * Description : Ce fichier contient les donn�es de construction de la salle.*
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "map.h"

// Constructeur de la structure Wall.
Wall::Wall( float       Width,
            float       Height,
            D3DXVECTOR3 Position,
            D3DXVECTOR3 Direction,
            int         TextureID )
{
    this->Width     = Width;
    this->Height    = Height;
    this->Position  = Position;
    this->Direction = Direction;
    this->TextureID = TextureID;
}

// Voici le plan de la salle :
//               _
//             /  |_
//            |_   _|
//              |_|

// Cette variable contient les donn�es de la salle.
Wall Map[] =
{
    // On cr�e ici les donn�es des murs. Il y en a 11.
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, 0.0f, 30.0f ),
          D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 10.0f, 0.0f, 20.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI / 2, 0.0f ),
          1 ),
    Wall( 28.3f,
          20.0f,
          D3DXVECTOR3( -20.0f, 0.0f, 20.0f ),
          D3DXVECTOR3( 0.0f, -(float)D3DX_PI / 4, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 30.0f, 0.0f, 0.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI / 2, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 20.0f, 0.0f, 10.0f ),
          D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 20.0f, 0.0f, -10.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, 0.0f, -30.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 10.0f, 0.0f, -20.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI / 2, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -10.0f, 0.0f, -20.0f ),
          D3DXVECTOR3( 0.0f, -(float)D3DX_PI / 2, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -30.0f, 0.0f, 0.0f ),
          D3DXVECTOR3( 0.0f, -(float)D3DX_PI / 2, 0.0f ),
          1 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -20.0f, 0.0f, -10.0f ),
          D3DXVECTOR3( 0.0f, (float)D3DX_PI, 0.0f ),
          1 ),

    // On cr�e ici les donn�es des sols. Il y en a 6.
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, -10.0f, -20.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, -10.0f, 0.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, -10.0f, 20.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -20.0f, -10.0f, 0.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 20.0f, -10.0f, 0.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -20.0f, -10.0f, 20.0f ),
          D3DXVECTOR3( (float)D3DX_PI / 2, 0.0f, 0.0f ),
          0 ),

    // On cr�e ici les donn�es des plafonds. Il y en a 6.
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, 10.0f, -20.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, 10.0f, 0.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 0.0f, 10.0f, 20.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -20.0f, 10.0f, 0.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( 20.0f, 10.0f, 0.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 ),
    Wall( 20.0f,
          20.0f,
          D3DXVECTOR3( -20.0f, 10.0f, 20.0f ),
          D3DXVECTOR3( -(float)D3DX_PI / 2, 0.0f, 0.0f ),
          2 )
};
