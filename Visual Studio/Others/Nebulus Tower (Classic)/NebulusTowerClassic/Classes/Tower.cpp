#include "Tower.h"

// std
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

//------------------------------------------------------------------------------
Tower::Tower()
{}
//------------------------------------------------------------------------------
Tower::Tower(std::size_t width, std::size_t rowHeight, float division) :
    m_Width(width),
    m_RowHeight(rowHeight),
    m_Division(division)
{}
//------------------------------------------------------------------------------
Tower::~Tower()
{}
//------------------------------------------------------------------------------
void Tower::Draw(double elapsedTime) const
{
    DrawTower(elapsedTime, m_Angle);
}
//------------------------------------------------------------------------------
std::size_t Tower::GetWidth() const
{
    return m_Width;
}
//------------------------------------------------------------------------------
std::size_t Tower::GetRowHeight() const
{
    return m_RowHeight;
}
//------------------------------------------------------------------------------
float Tower::GetDivision() const
{
    return m_Division;
}
//------------------------------------------------------------------------------
void Tower::RotateLeft(float velocity)
{
    m_Angle -= velocity;

    while (m_Angle < 0.0f)
        m_Angle += (float)M_PI;
}
//------------------------------------------------------------------------------
void Tower::RotateRight(float velocity)
{
    m_Angle += velocity;

    while (m_Angle >= (float)M_PI)
        m_Angle -= (float)M_PI;
}
//------------------------------------------------------------------------------
