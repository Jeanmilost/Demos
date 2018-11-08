/*********************************************
 * Cette classe représente le terrain de jeu *
 *********************************************/

#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include "Cell.h"

#define PLAYFIELD_X 15
#define PLAYFIELD_Y 24

/*
 * Cette classe représente le terrain de jeu.
 */
class Playfield
{
  private:

    int  p_X;
    int  p_Y;
    int  p_Score;

    Cell p_Playfield[PLAYFIELD_X][PLAYFIELD_Y];

  public:

    // Constructeur
    Playfield();

    // Destructeur
    ~Playfield();

  public:

    void Clean();
    void Reset();
    void TestLines();

    bool GetCell( int x, int y, Cell& theCell );
    bool SetCell( int x, int y, Cell theCell );

    int  GetLenght();
    int  GetHeight();
    int  GetScore();

  private:

    void DeleteLine( int Line );
};

#endif // PLAYFIELD_H
