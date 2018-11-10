/*****************************************************************************
 * ==> QR_GameEngine --------------------------------------------------------*
 *****************************************************************************
 * Description : Game engine, select and execute all game events             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_GameEngine.h"

//------------------------------------------------------------------------------
// Global defines
//------------------------------------------------------------------------------
#define M_GameEngine_Playfield_X 15
#define M_GameEngine_Playfield_Y 24
#define M_GameEngine_Nb_Piece    7
#define M_GameEngine_Start_Level 1
#define M_GameEngine_Timer_Tick  999
//------------------------------------------------------------------------------
QR_GameEngine::QR_GameEngine()
{
    m_NbPiece     = M_GameEngine_Nb_Piece;
    m_PositionX   = 0;
    m_PositionY   = 0;
    m_Level       = M_GameEngine_Start_Level;
    m_GameOver    = false;
    m_BypassTimer = false;
    m_PrevTime    = std::clock();
    m_TimerTick   = M_GameEngine_Timer_Tick;
    m_pCurPiece   = NULL;
    m_pNextPiece  = NULL;
    m_pPlayfield  = new QR_Playfield(M_GameEngine_Playfield_X, M_GameEngine_Playfield_Y);

    std::srand((unsigned)std::time(0));
}
//------------------------------------------------------------------------------
QR_GameEngine::~QR_GameEngine()
{
    if (m_pPlayfield)
        delete m_pPlayfield;

    if (m_pNextPiece)
         delete m_pNextPiece;

    if (m_pCurPiece)
         delete m_pCurPiece;
}
//------------------------------------------------------------------------------
void QR_GameEngine::Reset()
{
    m_NbPiece     = M_GameEngine_Nb_Piece;
    m_PositionX   = 0;
    m_PositionY   = 0;
    m_Level       = M_GameEngine_Start_Level;
    m_GameOver    = false;
    m_BypassTimer = false;
    m_PrevTime    = std::clock();
    m_TimerTick   = M_GameEngine_Timer_Tick;
    m_pCurPiece   = NULL;
    m_pNextPiece  = NULL;

    if (m_pNextPiece)
    {
         delete m_pNextPiece;
         m_pNextPiece = NULL;
    }

    if (m_pCurPiece)
    {
         delete m_pCurPiece;
         m_pCurPiece = NULL;
    }

    if (m_pPlayfield)
        m_pPlayfield->Reset();

    std::srand((unsigned)std::time(0));
}
//------------------------------------------------------------------------------
void QR_GameEngine::Play()
{
    // is game over?
    if (m_GameOver)
        return;

    // Sauvegarde l'ancienne hauteur de la pièce, on en aura peut-être besoin
    // plus tard.
    // store piece y position
    int oldY = m_PositionY;

    // clear play field before starting new draw
    m_pPlayfield->Clear();

    // current piece exists?
    if (!m_pCurPiece)
    {
        // select next piece to show. Current piece will be selected too if needed
        SetNextPiece();

        // set piece start position in play field
        m_PositionX = (m_pPlayfield->GetWidth() >> 1) - (m_pCurPiece->GetWidth() >> 1);
        m_PositionY = 0;

        return;
    }
    else
    {
        // get elapsed time
        std::clock_t elapsedTime = std::clock() - m_PrevTime;

        // time to move piece down?
        if (elapsedTime > m_TimerTick || m_BypassTimer)
        {
            // update timer values
            m_PrevTime    = std::clock();
            m_BypassTimer = false;

            // can modify game velocity? (only if timer tick is higher than 0)
            if (m_TimerTick)
            {
                // get current score
                unsigned score = m_pPlayfield->GetScore();

                // do change level (and game velocity)?
                if (score > 900)
                {
                    m_Level = 10;
                    m_TimerTick  = 0;
                }
                else
                if (score > 800)
                {
                    m_Level     = 9;
                    m_TimerTick = 100;
                }
                else
                if (score > 700)
                {
                    m_Level     = 8;
                    m_TimerTick = 200;
                }
                else
                if (score > 600)
                {
                    m_Level     = 7;
                    m_TimerTick = 300;
                }
                else
                if (score > 500)
                {
                    m_Level     = 6;
                    m_TimerTick = 400;
                }
                else
                if (score > 400)
                {
                    m_Level     = 5;
                    m_TimerTick = 500;
                }
                else
                if (score > 300)
                {
                    m_Level     = 4;
                    m_TimerTick = 600;
                }
                else
                if (score > 200)
                {
                    m_Level     = 3;
                    m_TimerTick = 700;
                }
                else
                if (score > 100)
                {
                    m_Level     = 2;
                    m_TimerTick = 800;
                }
            }

            // set next piece position
            ++m_PositionY;

            // is next piece position valid?
            if (!TestNextPosition())
            {
                // piece has moved down at least 1 time?
                if (!oldY)
                    // piece cannot move, game is over
                    m_GameOver = true;

                // revert piece to previous position and notify that final piece
                // position is reached (thus piece become part of play field
                // background)
                m_pCurPiece->SetPiece(m_PositionX, oldY, true, m_pPlayfield);

                // delete current cell to get new on next time
                if (m_pCurPiece)
                {
                    delete m_pCurPiece;
                    m_pCurPiece = NULL;
                }
            }
        }
    }

    // check if completed lines exist on play field, remove them and update
    // score if yes
    m_pPlayfield->TestLines();

    // playing piece to set?
    if (m_pCurPiece)
        // set playing piece in play field, don't freeze piece cells
        m_pCurPiece->SetPiece(m_PositionX, m_PositionY, false, m_pPlayfield);
}
//------------------------------------------------------------------------------
QR_Playfield* QR_GameEngine::GetPlayfield()
{
    return m_pPlayfield;
}
//------------------------------------------------------------------------------
unsigned QR_GameEngine::GetLevel()
{
    return m_Level;
}
//------------------------------------------------------------------------------
void QR_GameEngine::RotateClockwise()
{
    // no piece to rotate?
    if (!m_pCurPiece)
        return;

    // rotate piece 1/4 clockwise
    m_pCurPiece->RotateClockwise();

    // is new piece position valid?
    if (!m_pCurPiece->TestNextPosition(m_PositionX, m_PositionY, m_pPlayfield))
    {
        // clear play field and restore piece to previous position
        m_pPlayfield->Clear();
        m_pCurPiece->RotateCounterclockwise();
    }
}
//------------------------------------------------------------------------------
void QR_GameEngine::RotateCounterclockwise()
{
    // no piece to rotate?
    if (!m_pCurPiece)
        return;

    // rotate piece 1/4 clockwise
    m_pCurPiece->RotateCounterclockwise();

    // is new piece position valid?
    if (!m_pCurPiece->TestNextPosition(m_PositionX, m_PositionY, m_pPlayfield))
    {
        // clear play field and restore piece to previous position
        m_pPlayfield->Clear();
        m_pCurPiece->RotateClockwise();
    }
}
//------------------------------------------------------------------------------
void QR_GameEngine::MoveLeft()
{
    // no piece to move?
    if (!m_pCurPiece)
        return;

    // move piece to the left
    --m_PositionX;

    // is new piece position valid?
    if (!m_pCurPiece->TestNextPosition(m_PositionX, m_PositionY, m_pPlayfield))
        // restore piece to previous position
        ++m_PositionX;
}
//------------------------------------------------------------------------------
void QR_GameEngine::MoveRight()
{
    // no piece to move?
    if (!m_pCurPiece)
        return;

    // move piece to the right
    ++m_PositionX;

    // is new piece position valid?
    if (!m_pCurPiece->TestNextPosition(m_PositionX, m_PositionY, m_pPlayfield))
        // restore piece to previous position
        --m_PositionX;
}
//------------------------------------------------------------------------------
void QR_GameEngine::BypassTimer()
{
    m_BypassTimer = true;
}
//------------------------------------------------------------------------------
bool QR_GameEngine::TestNextPosition()
{
    // no piece to test?
    if (!m_pCurPiece)
        return false;

    // test piece and return result
    return m_pCurPiece->TestNextPosition(m_PositionX, m_PositionY, m_pPlayfield);
}
//------------------------------------------------------------------------------
bool QR_GameEngine::IsGameOver()
{
    return m_GameOver;
}
//------------------------------------------------------------------------------
QR_Piece* QR_GameEngine::GetNextPiece()
{
    return m_pNextPiece;
}
//------------------------------------------------------------------------------
void QR_GameEngine::SetNextPiece()
{
    // next piece does no exists?
    if (!m_pNextPiece)
        // creates one
        m_pNextPiece = CreateNewPiece();

    // current piece already exists?
    if (m_pCurPiece)
    {
        // delete it
        delete m_pCurPiece;
        m_pCurPiece = NULL;
    }

    // copy next piece to current piece and get new next piece
    m_pCurPiece  = m_pNextPiece;
    m_pNextPiece = CreateNewPiece();
}
//------------------------------------------------------------------------------
QR_Piece* QR_GameEngine::CreateNewPiece()
{
    // get next random number
    switch (std::rand() % m_NbPiece)
    {
        case 0:
            // create ruby
            return new QR_Ruby();

        case 1:
            // create emerald
            return new QR_Emerald();

        case 2:
            // create diamond
            return new QR_Diamond();

        case 3:
            // create sapphire
            return new QR_Sapphire();

        case 4:
            // create agathe
            return new QR_Agathe();

        case 5:
            // create citrine
            return new QR_Citrine();

        case 6:
            // create aquamarine
            return new QR_Aquamarine();

        default:
            // error
            return NULL;
    }
}
//------------------------------------------------------------------------------

