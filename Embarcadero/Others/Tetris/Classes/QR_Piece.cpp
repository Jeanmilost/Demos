/*****************************************************************************
 * ==> QR_Piece -------------------------------------------------------------*
 *****************************************************************************
 * Description : Basic game piece class, contains common functions as e.g.   *
 *               rotation or move                                            *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Piece.h"

// std
#include <memory>

//------------------------------------------------------------------------------
QR_Piece::QR_Piece() :
    m_Color(QR_Cell::IE_White),
    m_Left(0),
    m_Top(0),
    m_Right(0),
    m_Bottom(0),
    m_Position(0)
{}
//------------------------------------------------------------------------------
QR_Piece::~QR_Piece()
{
    // get cell count
    const unsigned cellCount = m_Cells.size();

    // iterate through cells to delete
    for (unsigned i = 0; i < cellCount; ++i)
        // delete cell
        delete m_Cells[i];
}
//------------------------------------------------------------------------------
std::wstring QR_Piece::GetName()
{
    return m_Name;
}
//------------------------------------------------------------------------------
QR_Cell::IEColor QR_Piece::GetColor()
{
    return m_Color;
}
//------------------------------------------------------------------------------
unsigned QR_Piece::GetWidth()
{
    return m_Right - m_Left;
}
//------------------------------------------------------------------------------
unsigned QR_Piece::GetHeight()
{
    return m_Bottom - m_Top;
}
//------------------------------------------------------------------------------
bool QR_Piece::SetPiece(int left, int top, bool doFreeze, QR_Playfield* pPlayfield)
{
    // play field is set?
    if (!pPlayfield)
        return false;

    // search for piece position
    switch (m_Position)
    {
        // 0 degrees rotated (default position)
        case 0:
            // iterate through piece cells (by accessing cell array directly)
            for (unsigned y = m_Top; y < m_Bottom; ++y)
                for (unsigned x = m_Left; x < m_Right; ++x)
                {
                    // calculate cell index in array
                    unsigned index = y * m_Right + x;

                    // set only filled cell on play field
                    if (m_Cells[index]->GetStatus() != QR_Cell::IE_Empty)
                        SetCell(left + x, top + y, index, doFreeze, pPlayfield);
                }

            return true;

        // 90 degrees rotated
        case 1:
            // iterate through piece cells (by accessing cell array directly)
            for (unsigned y = m_Left; y < m_Right; ++y)
                for (unsigned x = m_Bottom; x > m_Top; --x)
                {
                    // calculate cell index in array
                    unsigned index = (x * m_Right - y) - 1;

                    // set only filled cell on play field
                    if (m_Cells[index]->GetStatus() != QR_Cell::IE_Empty)
                        SetCell((left + m_Bottom) - x, (top + m_Right) - y - 1,
                                index, doFreeze, pPlayfield);
                }

            return true;

        // 180 degrees rotated
        case 2:
            // iterate through piece cells (by accessing cell array directly)
            for (unsigned y = m_Bottom; y > m_Top; --y)
                for (unsigned x = m_Right; x > m_Left; --x)
                {
                    // calculate cell index in array
                    unsigned index = (y - 1) * m_Right + (x - 1);

                    // set only filled cell on play field
                    if (m_Cells[index]->GetStatus() != QR_Cell::IE_Empty)
                        SetCell((left + m_Right) - x, (top + m_Bottom) - y,
                                index, doFreeze, pPlayfield);
                }

            return true;

        // 270 degrees rotated
        case 3:
            // iterate through piece cells (by accessing cell array directly)
            for (unsigned y = m_Right; y > m_Left; --y)
                for (unsigned x = m_Top; x < m_Bottom; ++x)
                {
                    // calculate cell index in array
                    unsigned index = (x * m_Right + y) - 1;

                    // set only filled cell on play field
                    if (m_Cells[index]->GetStatus() != QR_Cell::IE_Empty)
                        SetCell(left + x, (top + m_Right) - y, index, doFreeze,
                                pPlayfield);
                }

            return true;

        default:
            return false;
    }
}
//------------------------------------------------------------------------------
bool QR_Piece::TestNextPosition(int left, int top, const QR_Playfield* pPlayfield)
{
    // play field is set?
    if (!pPlayfield)
        return false;

    // search for piece position
    switch (m_Position)
    {        // 0 degrees rotated (default position)
        case 0:
        {
            // is piece exceeds play field?
            if (left < 0 || left + m_Right > pPlayfield->GetWidth()
                    || top < 0 || top + m_Bottom > pPlayfield->GetHeight())
                return false;

            // iterate through cells to test
            for (unsigned y = m_Top; y < m_Bottom; ++y)
                for (unsigned x = m_Left; x < m_Right; ++x)
                {
                    // calculate source cell index in array
                    unsigned index = y * m_Right + x;

                    // is piece cell filled?
                    if (m_Cells[index]->GetStatus() == QR_Cell::IE_Empty)
                        continue;
                    QR_Cell* pCell;

                    // get destination cell on play field
                    if (!pPlayfield->GetCell(left + x, top + y, pCell))
                        continue;

                    // is play field cell already filled? (NOTE filled state should be considered as
                    // empty, because only pieces that reached their final state, i.e. with frozen
                    // state, are intersting)
                    if (pCell->GetStatus() == QR_Cell::IE_Frozen)
                        return false;
                }

            return true;
        }

        // 90 degrees rotated
        case 1:
        {
            // is piece exceeds play field?
            if (left < 0 || left + m_Bottom > pPlayfield->GetWidth()
                    || top < 0 || top + m_Right > pPlayfield->GetHeight())
                return false;

            // iterate through cells to test
            for (unsigned y = m_Left; y < m_Right; ++y)
                for (unsigned x = m_Bottom; x > m_Top; --x)
                {
                    // calculate cell index in array
                    unsigned index = (x * m_Right - y) - 1;

                    // is piece cell filled?
                    if (m_Cells[index]->GetStatus() == QR_Cell::IE_Empty)
                        continue;
                    QR_Cell* pCell;

                    // get destination cell on play field
                    if (!pPlayfield->GetCell((left + m_Bottom ) - x,
                            (top + m_Right ) - y - 1, pCell))
                        continue;

                    // is play field cell already filled? (NOTE filled state should be considered as
                    // empty, because only pieces that reached their final state, i.e. with frozen
                    // state, are intersting)
                    if (pCell->GetStatus() == QR_Cell::IE_Frozen)
                        return false;
                }

            return true;
        }

        // 180 degrees rotated
        case 2:
        {
            // is piece exceeds play field?
            if (left < 0 || left + m_Right > pPlayfield->GetWidth() || top < 0
                    || top + m_Bottom > pPlayfield->GetHeight())
                return false;

            // iterate through cells to test
            for (unsigned y = m_Bottom; y > m_Top; --y)
                for (unsigned x = m_Right; x > m_Left; --x)
                {
                    // calculate cell index in array
                    unsigned index = (y - 1) * m_Right + (x - 1);

                    // is piece cell filled?
                    if (m_Cells[index]->GetStatus() == QR_Cell::IE_Empty)
                        continue;
                    QR_Cell* pCell;

                    // get destination cell on play field
                    if (!pPlayfield->GetCell((left + m_Right ) - x,
                            (top + m_Bottom ) - y, pCell))
                        continue;

                    // is play field cell already filled? (NOTE filled state should be considered as
                    // empty, because only pieces that reached their final state, i.e. with frozen
                    // state, are intersting)
                    if (pCell->GetStatus() == QR_Cell::IE_Frozen)
                        return false;
                }

            return true;
        }

        // 270 degrees rotated
        case 3:
        {
            // is piece exceeds play field?
            if (left < 0 || left + m_Bottom > pPlayfield->GetWidth()
                    || top < 0 || top + m_Right > pPlayfield->GetHeight())
                return false;

            // iterate through cells to test
            for (unsigned y = m_Right; y > m_Left; --y)
                for (unsigned x = m_Top; x < m_Bottom; ++x)
                {
                    // calculate cell index in array
                    unsigned index = (x * m_Right + y) - 1;

                    // is piece cell filled?
                    if (m_Cells[index]->GetStatus() == QR_Cell::IE_Empty)
                        continue;
                    QR_Cell* pCell;

                    // get destination cell on play field
                    if (!pPlayfield->GetCell(left + x, (top + m_Right) - y,
                            pCell))
                        continue;

                    // is play field cell already filled? (NOTE filled state should be considered as
                    // empty, because only pieces that reached their final state, i.e. with frozen
                    // state, are intersting)
                    if (pCell->GetStatus() == QR_Cell::IE_Frozen)
                        return false;
                }

            return true;
        }

        default:
            return false;
    }
}
//------------------------------------------------------------------------------
void QR_Piece::RotateClockwise()
{
    m_Position = ++m_Position & 3;
}
//------------------------------------------------------------------------------
void QR_Piece::RotateCounterclockwise()
{
    m_Position = --m_Position & 3;
}
//------------------------------------------------------------------------------
bool QR_Piece::GetCell(int x, int y, QR_Cell*& pCell)
{
    if (x < 0 || y < 0 || x > (int)m_Right - 1 || y > (int)m_Bottom - 1)
        return false;

    pCell = m_Cells[y * (m_Right - m_Left) + x];

    return true;
}
//------------------------------------------------------------------------------
void QR_Piece::Create(const char* pFillArray, unsigned count)
{
    // iterate through cells to create
    for (unsigned i = 0; i < count; ++i)
    {
        // create and configure new piece cell
        std::auto_ptr<QR_Cell> pCell(new QR_Cell());        pCell->SetColor(m_Color);
        pCell->SetStatus(pFillArray[i] ? QR_Cell::IE_Filled : QR_Cell::IE_Empty);
        m_Cells.push_back(pCell.release());
    }
}
//------------------------------------------------------------------------------
void QR_Piece::SetCell(int x, int y, unsigned index, bool doFreeze,
        QR_Playfield* pPlayfield)
{
    // set only filled cell on play field
    if (m_Cells[index]->GetStatus() == QR_Cell::IE_Empty)
        return;

    // do freeze cell?
    if (doFreeze)
        m_Cells[index]->SetStatus(QR_Cell::IE_Frozen);

    // set cell on play field
    pPlayfield->SetCell(x, y, m_Cells[index]);}
//------------------------------------------------------------------------------

