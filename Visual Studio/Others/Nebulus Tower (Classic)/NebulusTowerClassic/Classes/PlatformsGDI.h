#pragma once

// classes
#include "Platforms.h"

// libraries
#include <windows.h>

/**
* GDI based Nebulus tower platforms
*@author Jean-Milost Reymond
*/
class PlatformsGDI : public Platforms
{
    public:
        PlatformsGDI();
        virtual ~PlatformsGDI();

        /**
        * Adds a platform
        *@return added platform
        */
        virtual Platform* Add();

        /**
        * Adds a platform
        *@param width - platform width
        *@param height - platform height
        *@return added platform
        */
        virtual Platform* Add(int width, int height);

        /**
        * Draws the platforms
        *@param elapsedTime - elapsed time since latest draw
        *@param angle - current tower angle
        *@param hDC - device context on which the player should be drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        *@param back - if true, the back part of the tower is drawing
        */
        virtual void Draw(double elapsedTime, float angle, HDC hDC, const RECT& clientRect, const RECT& towerRect, bool back) const;
};
