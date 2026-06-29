#pragma once

// std
#include <vector>

// classes
#include "Platform.h"

/**
* Nebulus tower platforms
*@author Jean-Milost Reymond
*/
class Platforms
{
    public:
        Platforms();
        virtual ~Platforms();

        /**
        * Adds a platform
        *@return added platform
        */
        virtual Platform* Add() = 0;

        /**
        * Adds a platform
        *@param width - platform width
        *@param height - platform height
        *@return added platform
        */
        virtual Platform* Add(int width, int height) = 0;

    protected:
        typedef std::vector<Platform*> IPlatforms;

        IPlatforms m_Platforms;
};
