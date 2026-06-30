#include "PlatformsGDI.h"

// std
#include <algorithm>
#include <memory>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

// classes
#include "PlatformGDI.h"

//------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------
bool OnSortPlatforms(Platform* pFirst, Platform* pSecond)
{
    // calculate current angles
    const float firstAngle  = pFirst->m_TowerAngle  + pFirst->GetAngle();
    const float secondAngle = pSecond->m_TowerAngle + pSecond->GetAngle();

    // smaller cos = further back (assuming 0° faces the camera)
    return std::cosf(firstAngle) < std::cosf(secondAngle);
}
//------------------------------------------------------------------------------
// PlatformsGDI
//------------------------------------------------------------------------------
PlatformsGDI::PlatformsGDI() :
    Platforms()
{}
//------------------------------------------------------------------------------
PlatformsGDI::~PlatformsGDI()
{}
//------------------------------------------------------------------------------
Platform* PlatformsGDI::Add()
{
    std::unique_ptr<PlatformGDI> pPlatform = std::make_unique<PlatformGDI>();

    m_Platforms.push_back(pPlatform.get());

    return pPlatform.release();
}
//------------------------------------------------------------------------------
Platform* PlatformsGDI::Add(int width, int height)
{
    std::unique_ptr<PlatformGDI> pPlatform = std::make_unique<PlatformGDI>(width, height);

    m_Platforms.push_back(pPlatform.get());

    return pPlatform.release();
}
//------------------------------------------------------------------------------
void PlatformsGDI::Draw(double elapsedTime, float angle, HDC hDC, const RECT& clientRect, const RECT& towerRect, bool back) const
{
    // calculate the middle of the horizontal axis
    const LONG halfX = (clientRect.right - clientRect.left) >> 1;

    // calculate the half width of the tower
    const LONG halfWidth = (LONG)((towerRect.right - towerRect.left) >> 1);

    IPlatforms platforms;
    platforms.reserve(m_Platforms.size());

    for (std::size_t i = 0; i < m_Platforms.size(); ++i)
    {
        const int   platformWidth = m_Platforms[i]->GetWidth();
        const float platformAngle = m_Platforms[i]->GetAngle() + (float)M_PI;

        // clamp current angle between 0 and 2 * PI
        const float curAngle = std::fmodf(angle + platformAngle, (float)M_PI * 2.0f);

        // check if platform is located on tower back or front face
        const bool isBackFace = (curAngle <= (float)(M_PI / 2.0) || curAngle >= (float)(M_PI + (M_PI / 2.0)));

        // calculate angle sinus
        const float sinAngle = std::sinf(curAngle);

        // add the platform to the tower half width
        const LONG halfWidthAndPlatform = halfWidth + (platformWidth >> 1);

        // calculate the platform x position
        const int x = (int)std::roundf((float)halfX + sinAngle * (float)halfWidthAndPlatform);

        // currently drawing back face?
        if (back)
        {
            // back pass, only draw if on back face and bleeding out beyond the tower edges
            if (!isBackFace)
                continue;

            const bool bleedsLeft  = (x - platformWidth) < (halfX - halfWidthAndPlatform);
            const bool bleedsRight = (x + platformWidth) > (halfX + halfWidthAndPlatform);

            if (!bleedsLeft && !bleedsRight)
                continue;
        }
        else
        if (isBackFace)
            // front pass, draw all front-face platforms
            continue;

        // set platform position and update tower angle
        m_Platforms[i]->SetX(x);
        m_Platforms[i]->m_TowerAngle = angle;

        platforms.push_back(m_Platforms[i]);
    }

    std::sort(platforms.begin(), platforms.end(), OnSortPlatforms);

    for (std::size_t i = 0; i < platforms.size(); ++i)
        static_cast<PlatformGDI*>(platforms[i])->Draw(elapsedTime, hDC);
}
//------------------------------------------------------------------------------
