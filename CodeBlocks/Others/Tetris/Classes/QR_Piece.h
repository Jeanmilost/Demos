/*****************************************************************************
 * ==> QR_Piece -------------------------------------------------------------*
 *****************************************************************************
 * Description : Basic game piece class, contains common functions as e.g.   *
 *               rotation or move                                            *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include <cstdlib>

#ifndef QR_PIECEH
#define QR_PIECEH

// std
#include <string>

// interface
#include "QR_Cell.h"
#include "QR_Playfield.h"

/**
* Basic piece class, contains common functions as e.g. rotation or move
*@author Jean-Milost Reymond
*/
class QR_Piece
{
    public:
        QR_Piece();
        virtual ~QR_Piece();

        /**
        * Gets name
        *@return name
        */
        virtual std::wstring GetName();

        /**
        * Gets color
        *@return color
        */
        virtual QR_Cell::IEColor GetColor();

        /**
        * Gets width
        *@return width in chars
        */
        virtual unsigned GetWidth();

        /**
        * Gets height
        *@return height in chars
        */
        virtual unsigned GetHeight();

        /**
        * Sets piece on play field
        *@param left - piece left position in chars from play field origin
        *@param top - piece top position in chars from play field origin
        *@param doFreeze - if true, piece reached his final position and can't be moved
        *@param pPlayfield - play field on which piece will be set
        */
        virtual bool SetPiece(int left, int top, bool doFreeze,
                QR_Playfield* pPlayfield);

        /**
        * Tests next piece position
        *@param left - piece left position in chars from play field origin
        *@param top - piece top position in chars from play field origin
        *@param pPlayfield - play field on which piece will be set
        *@return true if next position is valid, otherwise false
        */
        virtual bool TestNextPosition(int left, int top,
                const QR_Playfield* pPlayfield);

        /**
        * Clockwise rotates piece
        */
        virtual void RotateClockwise();

        /**
        * Counterclockwise rotates piece
        */
        virtual void RotateCounterclockwise();

        /**
        * Gets cell
        *@param x - x position in chars from piece left top origin
        *@param y - y position in chars from piece left top origin
        *@param[out] pCell - cell        *@return true on success, otherwise false
        */
        virtual bool GetCell(int x, int y, QR_Cell*& pCell);

    protected:
        typedef std::vector<QR_Cell*> ICells;

        ICells           m_Cells;
        QR_Cell::IEColor m_Color;
        unsigned         m_Left;
        unsigned         m_Top;
        unsigned         m_Right;
        unsigned         m_Bottom;
        std::wstring     m_Name;

        /**
        * Creates piece from fill array
        *@param pFillArray - fill array indicating which cell should be filled
        *@param count - cell count
        */
        virtual void Create(const char* pFillArray, unsigned count);

    private:
        int m_Position;

        /**
        * Sets cell on play field
        *@param x - cell x position in chars from play field origin
        *@param y - cell y position in chars from play field origin
        *@param doFreeze - if true, cell reached his final position and can't be moved
        */
        void SetCell(int x, int y, unsigned index, bool doFreeze,
                QR_Playfield* pPlayfield);
};
#endif // QR_PIECEH

