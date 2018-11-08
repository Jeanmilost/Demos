/***************************************************************************
 * Cette classe gère l'interface du jeu, c'est-à-dire la façon dont le jeu *
 * sera affiché à l'écran, les interactions avec le joueur, etc...         *
 ***************************************************************************/

#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <windows.h>

#include "engine.h"

#define GAME_RED         FOREGROUND_RED
#define GAME_GREEN       FOREGROUND_GREEN
#define GAME_BLUE        FOREGROUND_BLUE
#define GAME_YELLOW      FOREGROUND_RED | FOREGROUND_GREEN
#define GAME_VIOLET      FOREGROUND_RED | FOREGROUND_BLUE
#define GAME_MARINE      FOREGROUND_GREEN | FOREGROUND_BLUE
#define GAME_WHITE       FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define GAME_GREY        FOREGROUND_INTENSITY
#define GAME_LIGHTRED    FOREGROUND_INTENSITY | GAME_RED
#define GAME_LIGHTGREEN  FOREGROUND_INTENSITY | GAME_GREEN
#define GAME_LIGHTBLUE   FOREGROUND_INTENSITY | GAME_BLUE
#define GAME_LIGHTYELLOW FOREGROUND_INTENSITY | GAME_YELLOW
#define GAME_LIGHTVIOLET FOREGROUND_INTENSITY | GAME_VIOLET
#define GAME_LIGHTMARINE FOREGROUND_INTENSITY | GAME_MARINE
#define GAME_LIGHTWHITE  FOREGROUND_INTENSITY | GAME_WHITE

using namespace std;

/*
 * Interfrace du jeu.
 */
class GameInterface
{
  private:

    HANDLE p_Win;
    COORD  p_PosCur;

    bool   p_Quit;

    Cell   p_NextPieceCells[15];

  public:

    // Constructeur
    GameInterface();

    // Destructeur
    ~GameInterface();

  public:

    void Init           ();
    void DisplayGame    ( Engine* theEngine );
    void DisplayGameOver( Engine* theEngine );
    void OnKeyPressed   ( Engine* theEngine );

    void DisplayMessage ( LPSTR theMessage,
                          int   x,
                          int   y,
                          bool  DoResetCursor = false );

    bool DoQuit         ();

  private:

    void SetCursorColor ( Game_Color Color );
};

#endif // GAMEINTERFACE_H
