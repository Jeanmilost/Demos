/******************************************************************************
 * Cette classe est le moteur du jeu. C'est ici que les différentes phases du *
 * jeu seront appelées et gérées.                                             *
 ******************************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>

#include "playfield.h"

#include "rubis.h"
#include "emerald.h"
#include "saphyr.h"
#include "diamond.h"
#include "agatha.h"
#include "citrin.h"
#include "aquamarine.h"

#define NB_PIECES 7

/*
 * Cette classe représente le moteur du jeu.
 */
class Engine
{
  private:

    Playfield* p_Playfield;

    int        p_NbPieces;
    int        p_PositionX;
    int        p_PositionY;
    int        p_Tick;
    int        p_Level;

    Piece*     p_NextPiece;
    Piece*     p_CurPiece;

    clock_t    p_StartTime;
    clock_t    p_EndTime;

    bool       p_GameOver;
    bool       p_BypassClock;

  public:

    Engine();
    ~Engine();

  public:

    Playfield* GetPlayfield();

    void       Play();
    void       Reset();
    void       RotateRight();
    void       RotateLeft();
    void       MoveRight();
    void       MoveLeft();
    void       BypassClock();

    int        GetLevel();

    bool       TestNextPosition();
    bool       IsGameOver();

    Piece*     GetNextPiece();

  private:

    void       SetNextPiece();
    Piece*     GetNewPiece();
};

#endif // ENGINE_H
