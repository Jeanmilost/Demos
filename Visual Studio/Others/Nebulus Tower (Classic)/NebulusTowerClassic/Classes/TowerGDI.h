#pragma once

// classes
#include "Tower.h"

// libraries
#include <windows.h>

/**
* GDI based Nebulus tower
*@author Jean-Milost Reymond
*/
class TowerGDI : public Tower
{
    public:
        TowerGDI();

        /**
        * Constructor
        *@param width - tower width
        *@param rowHeight - tower row height
        *@param division - division angle which defines an individual brick on the row
        */
        TowerGDI(std::size_t width, std::size_t rowHeight, float division);

        virtual ~TowerGDI();

        /**
        * Sets the device context
        *@param hWnd - Windows handle from which the device context was get
        *@param hDC - device context to set
        */
        virtual void SetDC(HWND hWnd, HDC hDC);

    protected:
        /**
        * Draws the tower
        *@param elapsedTime - elapsed time since latest draw
        *@param angle - tower view angle
        */
        virtual void DrawTower(double elapsedTime, float angle) const;

    private:
        HWND        m_hWnd         = nullptr;
        HDC         m_hDC          = nullptr;
        HDC         m_hMemDC       = nullptr;
        HBITMAP     m_hCanvas      = nullptr;
        HBITMAP     m_hOldBitmap   = nullptr;
        HBRUSH      m_hBlack       = nullptr;
        HBRUSH      m_hBlue        = nullptr;
        RECT        m_ClientRect   = { 0 };
        std::size_t m_ScreenWidth  = 0;
        std::size_t m_ScreenHeight = 0;
};
