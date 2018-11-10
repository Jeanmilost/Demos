/*****************************************************************************
 * ==> QR_Playfield ---------------------------------------------------------*
 *****************************************************************************
 * Description : Play field, it's the space on which game can be played      *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Playfield.h"

// std
#include <memory>

//------------------------------------------------------------------------------
QR_Playfield::QR_Playfield(unsigned width, unsigned height) : m_Score(0)
{
    // reserve enough space to create all rows
    m_PlayField.reserve(height);

    // iterate through rows to create
    for (unsigned i = 0; i < height; ++i)
    {
        IRow row;

        // reserve enough space to create all cells
        row.reserve(width);

        // iterate through cells to create
        for (unsigned j = 0; j < width; ++j)
        {
            // create new cell and add to row
            std::auto_ptr<QR_Cell> pCell(new QR_Cell());
            row.push_back(pCell.release());
        }

        // add line to play field
        m_PlayField.push_back(row);
    }
}
//------------------------------------------------------------------------------
QR_Playfield::~QR_Playfield()
{
    // get row count
    const unsigned rowCount = m_PlayField.size();

    // iterate through rows to delete
    for (unsigned i = 0; i < rowCount; ++i)
    {
        // get cell count
        const unsigned cellCount = m_PlayField[i].size();

        // iterate through cells to delete
        for (unsigned j = 0; j < cellCount; ++j)
            // delete cell            delete m_PlayField[i][j];
    }

    m_PlayField.clear();
}
//------------------------------------------------------------------------------
void QR_Playfield::Clear()
{
    // get row count
    const unsigned rowCount = m_PlayField.size();

    // iterate through rows
    for (unsigned i = 0; i < rowCount; ++i)
    {
        // get cell count
        const unsigned cellCount = m_PlayField[i].size();

        // iterate through cells
        for (unsigned j = 0; j < cellCount; ++j)
        {
            // clear only cells belonging to moving piece
            if (m_PlayField[i][j]->GetStatus() != QR_Cell::IE_Filled)
                continue;

            // clear cell
            m_PlayField[i][j]->SetStatus(QR_Cell::IE_Empty);
        }
    }
}
//------------------------------------------------------------------------------
void QR_Playfield::Reset()
{
    // get row count
    const unsigned rowCount = m_PlayField.size();

    // iterate through rows
    for (unsigned i = 0; i < rowCount; ++i)
    {
        // get cell count
        const unsigned cellCount = m_PlayField[i].size();

        // iterate through cells
        for (unsigned j = 0; j < cellCount; ++j)
            // clear cell
            m_PlayField[i][j]->Clear();
    }

    m_Score = 0;
}
//------------------------------------------------------------------------------
void QR_Playfield::TestLines()
{
    // get row count
    const unsigned rowCount = m_PlayField.size();

    // iterate through rows
    for (unsigned i = 0; i < rowCount; ++i)
    {
        // get cell count
        const unsigned cellCount   = m_PlayField[i].size();
              unsigned staticCount = 0;

        // iterate through cells
        for (unsigned j = 0; j < cellCount; ++j)
            // is cell part of play field background? (i.e. piece previously played)
            if (m_PlayField[i][j]->GetStatus() == QR_Cell::IE_Frozen)
                ++staticCount;

        // is line filled?
        if (staticCount == cellCount)
        {
            // increase score and delete filled line
            m_Score += staticCount;
            DeleteLine(i);
        }
    }
}
//------------------------------------------------------------------------------
bool QR_Playfield::GetCell(int x, int y, QR_Cell*& pCell) const
{
    // is location out of bounds?
    if (x < 0 || y < 0 || x > (int)GetWidth() - 1 || y > (int)GetHeight() - 1)
        return false;

    // get cell at location (NOTE array is organized in row, then cell, for this
    // reason y position should be found before x)
    pCell = m_PlayField[y][x];

    return true;
}
//------------------------------------------------------------------------------
bool QR_Playfield::SetCell(int x, int y, const QR_Cell* pCell)
{
    // is location out of bounds?
    if (x < 0 || y < 0 || x > (int)GetWidth() - 1 || y > (int)GetHeight() - 1)
        return false;

    // set cell at location (NOTE array is organized in row, then cell, for this
    // reason y position should be found before x)
    *m_PlayField[y][x] = *pCell;

    return true;
}
//------------------------------------------------------------------------------
unsigned QR_Playfield::GetWidth() const
{
    // play field contains no line?
    if (!m_PlayField.size())
        return 0;

    // return play field line width
    return m_PlayField[0].size();
}
//------------------------------------------------------------------------------
unsigned QR_Playfield::GetHeight() const
{
    // return play field height
    return m_PlayField.size();
}
//------------------------------------------------------------------------------
unsigned QR_Playfield::GetScore() const
{
    return m_Score;
}
//------------------------------------------------------------------------------
void QR_Playfield::DeleteLine(unsigned index)
{
    // get play field width
    const unsigned width = GetWidth();

    // iterate through play field lines from index to delete
    for (int y = index - 1; y >= 0; --y)
        for (unsigned x = 0; x < width; ++x)
            // erase line at index by copying line above
            *m_PlayField[y + 1][x] = *m_PlayField[y][x];

    // clear play field first line
    for (unsigned x = 0; x < width; ++x)
        m_PlayField[0][x]->Clear();
}
//------------------------------------------------------------------------------

