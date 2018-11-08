/*****************************************************************************
 * Classe de base pour les pi�ces du jeu. Les routines communes � toutes les *
 * pi�ces, comme par exemple la rotation d'une pi�ce, sont �crites ici.      *
 *****************************************************************************/

#include <cstdlib>

#ifndef PIECE_H
#define PIECE_H

#include "cell.h"
#include "playfield.h"

/*
 * Classe de base pour les pi�ces du jeu.
 */
class Piece
{
  private:

    int        p_Position;

  protected:

    Game_Color p_Color;

    Cell*      p_Cells;
    Cell*      p_Tests;

    int        p_Top;
    int        p_Bottom;
    int        p_Left;
    int        p_Right;
    int        p_NbCells;
    int        p_NameLenght;

    char*      p_Name;

  public:

    // Constructeur
    Piece();

    // Destructeur
    ~Piece();

  public:

    char*      GetName         ();

    Game_Color GetColor        ();

    int        GetHeight       ();
    int        GetLenght       ();
    int        GetNameLenght   ();

    bool       SetPiece        ( int        Left,
                                 int        Top,
                                 bool       SetLock,
                                 Playfield* thePlayfield );

    bool       TestNextPosition( int Left, int Top, Playfield* thePlayfield );

    void       RotateLeft      ();
    void       RotateRight     ();

    bool       GetCell         ( int x, int y, Cell& theCell );
};

#endif // PIECE_H
