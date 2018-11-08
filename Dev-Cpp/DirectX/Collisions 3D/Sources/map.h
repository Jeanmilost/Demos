/*****************************************************************************
 * ==> Fichier Map.h --------------------------------------------------------*
 * ***************************************************************************
 * Description : Ce fichier contient les donn�es de construction de la salle.*
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef MAP_H
#define MAP_H

#include <d3dx9.h>

// D�finitions pour l'affichage et le test des collisions de la salle.
#define NB_WALLS              23
#define FIRST_COLLIDABLE_WALL 0
#define LAST_COLLIDABLE_WALL  11
#define NB_TEXTURES           3

/*
 * Cette structure contient les prototypes des donn�es d'affichage d'un mur.
 */
struct Wall
{
    float       Width;
    float       Height;
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Direction;
    int         TextureID;

    Wall( float       Width,
          float       Height,
          D3DXVECTOR3 Position,
          D3DXVECTOR3 Direction,
          int         TextureID );
};

// Prototype de la variable Map, qui contient les donn�es de la salle.
extern Wall Map[];

#endif // MAP_H
