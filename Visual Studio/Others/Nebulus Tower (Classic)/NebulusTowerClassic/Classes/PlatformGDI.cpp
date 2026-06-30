#include "PlatformGDI.h"

// std
#include <cmath>

//------------------------------------------------------------------------------
// PlatformGDI
//------------------------------------------------------------------------------
PlatformGDI::PlatformGDI() :
    Platform()
{
    // create brushes
    m_hBlack     = ::CreateSolidBrush(RGB(0,   0,   0));
    m_hDarkBlue  = ::CreateSolidBrush(RGB(60,  57,  169));
    m_hLightBlue = ::CreateSolidBrush(RGB(124, 211, 205));
    m_hWhite     = ::CreateSolidBrush(RGB(255, 255, 255));
}
//------------------------------------------------------------------------------
PlatformGDI::PlatformGDI(int width, int height) :
    Platform(width, height)
{
    // create brushes
    m_hBlack     = ::CreateSolidBrush(RGB(0,   0,   0));
    m_hDarkBlue  = ::CreateSolidBrush(RGB(60,  57,  169));
    m_hLightBlue = ::CreateSolidBrush(RGB(124, 211, 205));
    m_hWhite     = ::CreateSolidBrush(RGB(255, 255, 255));
}
//------------------------------------------------------------------------------
PlatformGDI::~PlatformGDI()
{
    if (m_hBlack)
        ::DeleteObject(m_hBlack);

    if (m_hDarkBlue)
        ::DeleteObject(m_hDarkBlue);

    if (m_hLightBlue)
        ::DeleteObject(m_hLightBlue);

    if (m_hWhite)
        ::DeleteObject(m_hWhite);
}
//------------------------------------------------------------------------------
void PlatformGDI::Draw(double elapsedTime, HDC hDC) const
{
    if (!hDC)
        return;

    if (!m_hWhite)
        return;

    RECT platformRect;
    platformRect.left   = m_X - (m_Width >> 1);
    platformRect.top    = m_Y;
    platformRect.right  = platformRect.left + m_Width;
    platformRect.bottom = platformRect.top  + m_Height;

    // draw the platform background
    ::FillRect(hDC, &platformRect, m_hWhite);

    // draw the internal gradient
    const float step = m_Width / 16.0f;

    RECT internalRect;
    internalRect.left   = platformRect.left;
    internalRect.top    = platformRect.top;
    internalRect.right  = internalRect.left + (int)std::round(step);
    internalRect.bottom = platformRect.bottom;

    ::FillRect(hDC, &internalRect, m_hBlack);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hDarkBlue);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hLightBlue);

    internalRect.left  = internalRect.right + (int)std::round(step * 3.0f);
    internalRect.right = internalRect.left  + (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hLightBlue);

    internalRect.left  = internalRect.right + (int)std::round(step);
    internalRect.right = internalRect.left  + (int)std::round(step * 3.0f);

    ::FillRect(hDC, &internalRect, m_hLightBlue);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hDarkBlue);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hLightBlue);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step * 2.0f);

    ::FillRect(hDC, &internalRect, m_hDarkBlue);

    internalRect.left   = internalRect.right;
    internalRect.right += (int)std::round(step);

    ::FillRect(hDC, &internalRect, m_hBlack);

    // outline the platform
    ::FrameRect(hDC, &platformRect, m_hBlack);
}
//------------------------------------------------------------------------------
