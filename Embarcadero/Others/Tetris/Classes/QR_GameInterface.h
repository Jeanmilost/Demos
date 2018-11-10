/*****************************************************************************
 * ==> QR_GameInterface -----------------------------------------------------*
 *****************************************************************************
 * Description : Game interface, draws game on screen                        *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_GAMEINTERFACEH
#define QR_GAMEINTERFACEH

// std
#include <cstdlib>
#include <iostream>
#include <conio.h>

// windows
#include <windows.h>

// interface
#include "QR_GameEngine.h"

#define M_Console_Color_Red         FOREGROUND_RED
#define M_Console_Color_Green       FOREGROUND_GREEN
#define M_Console_Color_Blue        FOREGROUND_BLUE
#define M_Console_Color_Yellow      FOREGROUND_RED | FOREGROUND_GREEN
#define M_Console_Color_Violet      FOREGROUND_RED | FOREGROUND_BLUE
#define M_Console_Color_Marine      FOREGROUND_GREEN | FOREGROUND_BLUE
#define M_Console_Color_White       FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define M_Console_Color_Gray        FOREGROUND_INTENSITY
#define M_Console_Color_LightRed    FOREGROUND_INTENSITY | M_Console_Color_Red
#define M_Console_Color_LightGreen  FOREGROUND_INTENSITY | M_Console_Color_Green
#define M_Console_Color_LightBlue   FOREGROUND_INTENSITY | M_Console_Color_Blue
#define M_Console_Color_LightYellow FOREGROUND_INTENSITY | M_Console_Color_Yellow
#define M_Console_Color_LightViolet FOREGROUND_INTENSITY | M_Console_Color_Violet
#define M_Console_Color_LightMarine FOREGROUND_INTENSITY | M_Console_Color_Marine
#define M_Console_Color_LightWhite  FOREGROUND_INTENSITY | M_Console_Color_White

//REM using namespace std;

/**
* Game interface, draws game on screen
*@author Jean-Milost Reymond
*/
class QR_GameInterface
{
    public:
        QR_GameInterface();
        virtual ~QR_GameInterface();

        /**
        * Initializes Windows console
        */
        void InitConsole();

        /**
        * Draws current game state
        *@param pEngine - game engine
        */
        void DrawGame(QR_GameEngine* pEngine);

        /**
        * Draws game over
        *@param pEngine - game engine
        */
        void DrawGameOver(QR_GameEngine* pGameEngine);

        /**
        * Shows message at location
        *@param message - message to show
        *@param x - x location in chars from console left edge
        *@param y - y location in chars from console top edge
        */
        void ShowMessage(const std::wstring& message, int x, int y,
                bool doResetCursor = false);

        /**
        * Check if a key is pressed and run appropriate action if yes
        *@param pEngine - game engine
        */
        void CheckKeyPressed(QR_GameEngine* pEngine);

        /**
        * Checks if application should quit
        *@return true if application should quit, otherwise false
        */
        bool DoQuit();

    private:
        typedef std::vector<QR_Cell*> ICells;

        HANDLE m_hWinConsole;
        bool   m_Quit;
        ICells m_NextPieceCells;

        /**
        * Sets console cursor color
        *@param color - color
        */
        void SetCursorColor (QR_Cell::IEColor color);
};

#endif // QR_GAMEINTERFACEH

