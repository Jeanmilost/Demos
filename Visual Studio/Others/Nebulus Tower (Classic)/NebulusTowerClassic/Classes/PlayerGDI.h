#pragma once

// std
#include <string>

// classes
#include "Player.h"

// libraries
#include <windows.h>

/**
* GDI based Nebulus player
*@author Jean-Milost Reymond
*/
class PlayerGDI : public Player
{
    public:
        PlayerGDI();
        virtual ~PlayerGDI();

        /**
        * Loads the player resources
        *@param fileName - png file name containing the player resources
        *@return true on success, otherwise false
        */
        virtual bool Load(const std::wstring& fileName);

        /**
        * Draws the player
        *@param elapsedTime - elapsed time since latest draw
        *@param hDC - device context on which the player should be drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        */
        virtual void Draw(double elapsedTime, HDC hDC, const RECT& clientRect, const RECT& towerRect) const;

    private:
        ULONG_PTR m_Token    = 0;
        HBITMAP   m_hBitmap  = nullptr;
        int       m_Index    = 0;
        int       m_Count    = 0;
        double    m_CurTime  = 0.0;
        double    m_AnimTime = 60.0;
};
