/*****************************************************************************
 * ==> QR_GameEngine --------------------------------------------------------*
 *****************************************************************************
 * Description : Game engine, select and execute all game events             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_GAMEENGINEH
#define QR_GAMEENGINEH

// std
#include <ctime>

// interface
#include "QR_Playfield.h"
#include "QR_Ruby.h"
#include "QR_Emerald.h"
#include "QR_Sapphire.h"
#include "QR_Diamond.h"
#include "QR_Agathe.h"
#include "QR_Citrine.h"
#include "QR_Aquamarine.h"

//------------------------------------------------------------------------------
// Global defines
//------------------------------------------------------------------------------
//REM #define M_Playfield_X 15
//REM #define M_Playfield_Y 24
//REM #define M_Nb_Pieces   7
//------------------------------------------------------------------------------

/**
* Game engine, select and execute all game events
*@author Jean-Milost Reymond
*/
class QR_GameEngine
{
    public:
        QR_GameEngine();
        virtual ~QR_GameEngine();

        /**
        * Resets game to initial status (and thus is ready for next play)
        */
        virtual void Reset();

        /**
        * Plays the game
        *@note This function is to be considered as the main game function. Will
        *      be called by main loop each time an action should be made
        */
        virtual void Play();

        /**
        * Gets play field
        *@return play field
        */
        virtual QR_Playfield* GetPlayfield();

        /**
        * Gets level
        *@return level
        */
        virtual unsigned GetLevel();

        /**
        * Clockwise rotates moving piece        */
        virtual void RotateClockwise();

        /**
        * Counterclockwise rotates moving piece        */
        virtual void RotateCounterclockwise();

        /**
        * Moves moving piece on the left
        */
        virtual void MoveLeft();

        /**
        * Moves moving piece on the right
        */
        virtual void MoveRight();

        /**
        * Bypasses the internal play timer
        */
        virtual void BypassTimer();

        /**
        * Tests next piece position
        *@return true if next position is valid, otherwise false
        */
        virtual bool TestNextPosition();

        /**
        * Checks if game is over
        *@return true if game is over, otherwise false
        */
        virtual bool IsGameOver();

        /**
        * Gets next piece that will be shown after current piece will be placed        *@return next piece
        */
        virtual QR_Piece* GetNextPiece();

    private:
        int           m_PositionX;
        int           m_PositionY;
        unsigned      m_NbPiece;
        unsigned      m_Level;
        bool          m_GameOver;
        bool          m_BypassTimer;
        std::clock_t  m_PrevTime;
        std::clock_t  m_TimerTick;
        QR_Piece*     m_pNextPiece;
        QR_Piece*     m_pCurPiece;
        QR_Playfield* m_pPlayfield;

        /**
        * Sets next piece that will be shown after current piece will be placed        */
        void SetNextPiece();

        /**
        * Creates new piece
        *@return newly created piece
        */
        QR_Piece* CreateNewPiece();
};

#endif // QR_GAMEENGINEH

