#include "Player.h"

//------------------------------------------------------------------------------
// Player
//------------------------------------------------------------------------------
Player::Player()
{}
//------------------------------------------------------------------------------
Player::~Player()
{}
//------------------------------------------------------------------------------
void Player::MoveLeft()
{
    // prevent while turning
    if (m_Turning)
        return;

    if (!m_MovingLeft)
        m_Turning = true;

    m_MovingLeft  = true;
    m_MovingRight = false;
    m_Stopped     = false;
}
//------------------------------------------------------------------------------
void Player::MoveRight()
{
    // prevent while turning
    if (m_Turning)
        return;

    if (!m_MovingRight)
        m_Turning = true;

    m_MovingLeft  = false;
    m_MovingRight = true;
    m_Stopped     = false;
}
//------------------------------------------------------------------------------
bool Player::IsTurning() const
{
    return m_Turning;
}
//------------------------------------------------------------------------------
void Player::Stop()
{
    m_Stopped = true;
}
//------------------------------------------------------------------------------
