/**********************************************************
 * Cette classe repr�sente une pi�ce Diamant dans le jeu. *
 **********************************************************/

#ifndef DIAMOND_H
#define DIAMOND_H

#include "piece.h"

/*
 * Cette classe repr�sente la pi�ce Diamant.
 */
class Diamond : public Piece
{
  public:

    // Constructeur
    Diamond();

    // Destructeur
    ~Diamond();
};

#endif // DIAMOND_H
