/*****************************************************************************
 * ==> E_Maze ---------------------------------------------------------------*
 * ***************************************************************************
 * Description : Maze generator                                              *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "E_Maze.h"
#include "E_Random.h"

//------------------------------------------------------------------------------
// class E_Maze - C++
//------------------------------------------------------------------------------
E_Maze::E_Maze(unsigned width, unsigned height)
{
    // initialize maze array with blank cells
    for (unsigned j = 0; j < height; ++j)
    {
        std::vector<ICell> line;

        for (unsigned i = 0; i < width; ++i)
        {
            ICell cell;
            cell.m_Type = ICell::IE_Empty;

            line.push_back(cell);
        }

        m_Maze.push_back(line);
    }

    ICoords min;
    min.m_X = 0;
    min.m_Y = 0;

    ICoords max;
    max.m_X = width;
    max.m_Y = height;

    // create maze
    Split(min, max);

    CreateSpecialCell(ICell::IE_Start);
    CreateSpecialCell(ICell::IE_End);
    CreateSpecialCell(ICell::IE_Key);
}
//------------------------------------------------------------------------------
unsigned E_Maze::GetWidth()
{
    if (m_Maze.size() == 0)
        return 0;

    return m_Maze[0].size();
}
//------------------------------------------------------------------------------
unsigned E_Maze::GetHeight()
{
    return m_Maze.size();
}
//------------------------------------------------------------------------------
E_Maze::ICell E_Maze::Get(unsigned x, unsigned y)
{
    if (y >= GetHeight())
        throw "Get - y is out of bounds";
    else
    if (x >= GetWidth())
        throw "Get - x is out of bounds";

    return m_Maze[y][x];
}
//------------------------------------------------------------------------------
E_Maze::ICell E_Maze::Get(unsigned index)
{
    if (GetWidth() == 0)
        throw "Get - invalid width";
    else
    if (index >= GetWidth() * GetHeight())
        throw "Get - index is out of bounds";

    unsigned y = index / GetWidth();
    unsigned x = index % (y * GetWidth());

    return m_Maze[y][x];
}
//------------------------------------------------------------------------------
void E_Maze::Set(unsigned x, unsigned y, const ICell& cell)
{
    if (y >= GetHeight())
        throw "Set - y is out of bounds";
    else
    if (x >= GetWidth())
        throw "Set - x is out of bounds";

    m_Maze[y][x] = cell;
}
//------------------------------------------------------------------------------
void E_Maze::Set(unsigned index, const ICell& cell)
{
    if (GetWidth() == 0)
        throw "Set - invalid width";
    else
    if (index >= GetWidth() * GetHeight())
        throw "Set - index is out of bounds";

    unsigned y = index / GetWidth();
    unsigned x = index % (y * GetWidth());

    m_Maze[y][x] = cell;
}
//------------------------------------------------------------------------------
void E_Maze::CreateSpecialCell(const ICell::IE_Type& type)
{
    unsigned count = 0;
    unsigned max   = 100000;

    // create end point
    do
    {
        unsigned x = E_Random::GetNumber(GetWidth() - 1);
        unsigned y = E_Random::GetNumber(GetHeight() - 1);

        ICell cell;

        cell = Get(x, y);

        if (cell.m_Type == ICell::IE_Empty)
        {
            cell.m_Type = type;
            Set(x, y, cell);
            break;
        }
        else
        if (count > max)
            throw L"Unable to create special cell";
        else
            ++count;
    }
    while (1);
}
//------------------------------------------------------------------------------
void E_Maze::Split(const ICoords& min, const ICoords& max, unsigned deep, unsigned maxDeep)
{
    unsigned deltaX = max.m_X - min.m_X;
    unsigned deltaY = max.m_Y - min.m_Y;

    ICoords childMinRight;
    ICoords childMaxRight;
    ICoords childMinLeft;
    ICoords childMaxLeft;

    ICell wallCell;
    wallCell.m_Type = ICell::IE_Wall;

    ICell doorCell;
    doorCell.m_Type = ICell::IE_Door;

    // determine the orientation of space to split
    if (deltaX > deltaY)
    {
        if (deltaX < 3 || deltaY < 3)
            return;

        // calculate the split point and the door position
        unsigned splitX = min.m_X + 1 + ((deltaX == 3) ? 0 : E_Random::GetNumber(deltaX - 2));
        unsigned door   = min.m_Y + E_Random::GetNumber(deltaY - 2);

        // create the wall
        for (unsigned i = min.m_Y; i < max.m_Y; ++i)
            if (i != door && i != door + 1)
                Set(splitX, i, wallCell);
            else
                Set(splitX, i, doorCell);

        // create children spaces to divide
        childMinRight.m_X = min.m_X;
        childMinRight.m_Y = min.m_Y;
        childMaxRight.m_X = splitX;
        childMaxRight.m_Y = max.m_Y;

        childMinLeft.m_X = ((splitX >= max.m_X) ? max.m_X : splitX + 1);
        childMinLeft.m_Y = min.m_Y;
        childMaxLeft.m_X = max.m_X;
        childMaxLeft.m_Y = max.m_Y;
    }
    else
    {
        if (deltaY < 3 || deltaX < 3)
            return;

        // calculate the split point and the door position
        unsigned splitY = min.m_Y + 1 + ((deltaY == 3) ? 0 : E_Random::GetNumber(deltaY - 2));
        unsigned door   = min.m_X + E_Random::GetNumber(deltaX - 2);

        // create the wall
        for (unsigned i = min.m_X; i < max.m_X; ++i)
            if (i != door && i != door + 1)
                Set(i, splitY, wallCell);
            else
                Set(i, splitY, doorCell);

        // create children spaces to divide
        childMinRight.m_X = min.m_X;
        childMinRight.m_Y = min.m_Y;
        childMaxRight.m_X = max.m_X;
        childMaxRight.m_Y = splitY;

        childMinLeft.m_X = min.m_X;
        childMinLeft.m_Y = ((splitY >= max.m_Y) ? max.m_Y : splitY + 1);
        childMaxLeft.m_X = max.m_X;
        childMaxLeft.m_Y = max.m_Y;
    }

    if (deep < maxDeep)
    {
        ++deep;

        // split right child
        if ((childMaxRight.m_X - childMinRight.m_X) > 1 && (childMaxRight.m_Y - childMinRight.m_Y) > 1)
            Split(childMinRight, childMaxRight, deep);

        // split left child
        if ((childMaxLeft.m_X - childMinLeft.m_X) > 1 && (childMaxLeft.m_Y - childMinLeft.m_Y) > 1)
            Split(childMinLeft, childMaxLeft, deep);
    }
}
//------------------------------------------------------------------------------
