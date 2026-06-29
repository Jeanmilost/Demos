#pragma once

// std
#include <functional>

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
        /**
        * Called before the tower is drawn
        *@param elapsedTime - elapsed time since latest draw
        *@param angle - current tower angle
        *@param hDC - device context on which the tower is drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        */
        using ITfOnBeforeDrawTower = std::function<void(double elapsedTime, float angle, HDC hDC, const RECT& clientRect,
                const RECT& towerRect)>;

        /**
        * Called after the tower is drawn
        *@param elapsedTime - elapsed time since latest draw
        *@param angle - current tower angle
        *@param hDC - device context on which the tower is drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        */
        using ITfOnAfterDrawTower = std::function<void(double elapsedTime, float angle, HDC hDC, const RECT& clientRect,
                const RECT& towerRect)>;

        /**
        * Called when the player should be drawn
        *@param elapsedTime - elapsed time since latest draw
        *@param hDC - device context on which the tower is drawn
        *@param clientRect - client rectangle
        *@param towerRect - tower rectangle
        */
        using ITfOnDrawPlayer = std::function<void(double elapsedTime, HDC hDC, const RECT& clientRect, const RECT& towerRect)>;

        ITfOnBeforeDrawTower m_fOnBeforeDrawTower = nullptr;
        ITfOnAfterDrawTower  m_fOnAfterDrawTower  = nullptr;
        ITfOnDrawPlayer      m_fOnDrawPlayer      = nullptr;

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
