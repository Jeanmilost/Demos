#include "TowerGDI.h"

// std
#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// TowerGDI
//------------------------------------------------------------------------------
TowerGDI::TowerGDI() :
    Tower()
{}
//------------------------------------------------------------------------------
TowerGDI::TowerGDI(std::size_t width, std::size_t rowHeight, float division) :
    Tower(width, rowHeight, division)
{}
//------------------------------------------------------------------------------
TowerGDI::~TowerGDI()
{
    if (m_hBlack)
        ::DeleteObject(m_hBlack);

    if (m_hBlue)
        ::DeleteObject(m_hBlue);

    if (m_hCanvas)
        ::DeleteObject(m_hCanvas);

    if (m_hOldBitmap && m_hMemDC)
        ::SelectObject(m_hMemDC, m_hOldBitmap);

    if (m_hMemDC)
        ::DeleteDC(m_hMemDC);
}
//------------------------------------------------------------------------------
void TowerGDI::SetDC(HWND hWnd, HDC hDC)
{
    if (!hWnd)
        return;

    m_hWnd = hWnd;

    if (!hDC)
        return;

    m_hDC = hDC;

    ::GetClientRect(hWnd, &m_ClientRect);

    // calculate the canvas width and height
    m_ScreenWidth  = (std::size_t)m_ClientRect.right  - (std::size_t)m_ClientRect.left;
    m_ScreenHeight = (std::size_t)m_ClientRect.bottom - (std::size_t)m_ClientRect.top;

    if (!m_ScreenWidth || !m_ScreenHeight)
        return;

    // create memory DC
    m_hMemDC = ::CreateCompatibleDC(hDC);

    if (!m_hMemDC)
        return;

    // create a compatible bitmap for the back buffer
    m_hCanvas = ::CreateCompatibleBitmap(hDC, (int)m_ScreenWidth, (int)m_ScreenHeight);

    if (!m_hCanvas)
        return;

    // attach the back buffer to its device context
    m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hCanvas);

    // create brushes
    m_hBlack = ::CreateSolidBrush(RGB(0, 0, 0));
    m_hBlue  = ::CreateSolidBrush(RGB(0, 0, 255));
}
//------------------------------------------------------------------------------
void TowerGDI::DrawTower(double elapsedTime, float angle) const
{
    if (!m_hDC)
        return;

    if (!m_hMemDC)
        return;

    if (!m_hBlack)
        return;

    if (!m_hBlue)
        return;

    // draw the background
    ::FillRect(m_hMemDC, &m_ClientRect, m_hBlack);

    // calculate the middle of the horizontal axis
    const LONG halfX = (m_ClientRect.right - m_ClientRect.left) >> 1;

    // calculate the half width of the tower
    const LONG halfWidth = (LONG)(GetWidth() >> 1);

    RECT towerRect;

    // calculate the tower background rect
    towerRect.left   = halfX - halfWidth;
    towerRect.right  = halfX + halfWidth;
    towerRect.top    = m_ClientRect.top;
    towerRect.bottom = m_ClientRect.bottom;

    // draw the tower background rect
    ::FillRect(m_hMemDC, &towerRect, m_hBlue);

    // select the black pen
    ::SelectObject(m_hMemDC, m_hBlack);

    const int   rowHeight = (int)GetRowHeight();
    const float division  =      GetDivision();

    // iterate through rows to draw
    for (int i = 0; i < (int)(m_ScreenHeight / rowHeight) + 1; ++i)
    {
        // draw vertical line
        ::MoveToEx(m_hMemDC, halfX - halfWidth, i * rowHeight, nullptr);
        ::LineTo  (m_hMemDC, halfX + halfWidth, i * rowHeight);

        // calculate brick step and current angle
        const float step     = (float)(M_PI * 2.0f) / division;
              float curAngle = (i % 2) ? angle : angle + (step / 2.0f);

        // iterate through bricks to draw
        for (std::size_t j = 0; j < (std::size_t)division; ++j)
        {
            // calculate current angle sinus
            const float sinAngle = sinf(curAngle);

            // is brick located on the not visible tower face?
            if (sinAngle >= 0.0f)
            {
                // calculate the brick x position
                const int x = (halfX - halfWidth) + (int)roundf(curAngle < ((float)M_PI / 2.0f)
                        ? (float)halfWidth * (1.0f - sinAngle) : (float)halfWidth + ((float)halfWidth * sinAngle));

                // draw brick horizontal line
                ::MoveToEx(m_hMemDC, x,  i      * (int)rowHeight, nullptr);
                ::LineTo  (m_hMemDC, x, (i + 1) * (int)rowHeight);
            }

            // increase the current angle and clamp it between 0 and 2 * PI
            curAngle = fmodf(curAngle + step, (float)M_PI * 2.0f);
        }
    }

    // draw the back buffer onto the main one
    ::BitBlt(m_hDC,
             m_ClientRect.left,
             m_ClientRect.top,
             (int)m_ScreenWidth,
             (int)m_ScreenHeight,
             m_hMemDC,
             m_ClientRect.left,
             m_ClientRect.top,
             SRCCOPY);
}
//------------------------------------------------------------------------------
