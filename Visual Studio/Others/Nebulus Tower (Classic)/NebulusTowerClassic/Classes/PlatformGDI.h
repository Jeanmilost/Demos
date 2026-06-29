#pragma once

// classes
#include "Platform.h"

// libraries
#include <windows.h>

/**
* GDI based Nebulus tower platform
*@author Jean-Milost Reymond
*/
class PlatformGDI : public Platform
{
    public:
        PlatformGDI();

        /**
        * Constructor
        *@param width - platform width
        *@param height - platform height
        */
        PlatformGDI(int width, int height);

        virtual ~PlatformGDI();

        /**
        * Draws the platform
        *@param elapsedTime - elapsed time since latest draw
        *@param hDC - device context on which the player should be drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        *@param x - platform x position (located at the platform middle)
        */
        virtual void Draw(double elapsedTime, HDC hDC, const RECT& clientRect, const RECT& towerRect, int x) const;

    private:
        HBRUSH m_hBlack     = nullptr;
        HBRUSH m_hDarkBlue  = nullptr;
        HBRUSH m_hLightBlue = nullptr;
        HBRUSH m_hWhite     = nullptr;
};
