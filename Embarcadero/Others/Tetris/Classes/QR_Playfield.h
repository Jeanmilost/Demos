/*****************************************************************************
 * ==> QR_Playfield ---------------------------------------------------------*
 *****************************************************************************
 * Description : Play field, it's the space on which game can be played      *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_PLAYFIELDH
#define QR_PLAYFIELDH

// std
#include <vector>

// interface
#include "QR_Cell.h"

/**
* Play field
*@author Jean-Milost Reymond
*/
class QR_Playfield
{
    public:
        /**
        * Constructor
        *@param width - play field width
        *@param height - play field height
        */
        QR_Playfield(unsigned width, unsigned height);

        virtual ~QR_Playfield();

        /**
        * Clears play field
        */
        virtual void Clear();

        /**
        * Resets play field to initial state
        */
        virtual void Reset();

        /**
        * Tests lines and select action to do
        */
        virtual void TestLines();

        /**
        * Gets cell at location
        *@param x - x position in chars from play field origin
        *@param y - y position in chars from play field origin
        *@param[out] pCell - cell
        *@return true if cell was found, false otherwise        */
        virtual bool GetCell(int x, int y, QR_Cell*& pCell) const;

        /**
        * Sets cell
        *@param x - x position in chars from play field origin
        *@param y - y position in chars from play field origin
        *@param pCell - cell to set
        *@return true if cell was set, otherwise false
        */
        virtual bool SetCell(int x, int y, const QR_Cell* pCell);

        /**
        * Gets play field width
        *@return width
        */
        virtual unsigned GetWidth() const;

        /**
        * Gets play field height
        *@return height
        */
        virtual unsigned GetHeight() const;

        /**
        * Gets scpre
        *@return score
        */
        virtual unsigned GetScore() const;

    private:
        typedef std::vector<QR_Cell*> IRow;
        typedef std::vector<IRow>     ITable;

        ITable   m_PlayField;
        unsigned m_Score;

        /**
        * Deletes line
        *@param index - line index to delete
        */
        void DeleteLine(unsigned index);
};

#endif // QR_PLAYFIELDH

