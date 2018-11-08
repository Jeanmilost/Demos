/**********************************************************
 * Cette classe représente une pièce Diamant dans le jeu. *
 **********************************************************/

#ifndef DIAMOND_H
#define DIAMOND_H

#include "piece.h"

/*
 * Cette classe représente la pièce Diamant.
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
