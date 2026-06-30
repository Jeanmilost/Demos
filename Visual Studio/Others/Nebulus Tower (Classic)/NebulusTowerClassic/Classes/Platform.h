#pragma once

/**
* Nebulus tower platform
*@author Jean-Milost Reymond
*/
class Platform
{
    public:
        float m_TowerAngle = 0.0f;

        Platform();

        /**
        * Constructor
        *@param width - platform width
        *@param height - platform height
        */
        Platform(int width, int height);

        virtual ~Platform();

        /**
        * Gets platform x position
        *@return platform x position
        */
        virtual int GetX() const;

        /**
        * Sets platform x position
        *@param value - platform x position
        */
        virtual void SetX(int value);

        /**
        * Gets platform y position
        *@return platform y position
        */
        virtual int GetY() const;

        /**
        * Sets platform y position
        *@param value - platform y position
        */
        virtual void SetY(int value);

        /**
        * Gets platform width
        *@return platform width
        */
        virtual int GetWidth() const;

        /**
        * Sets platform width
        *@param value - platform width
        */
        virtual void SetWidth(int value);

        /**
        * Gets platform height
        *@return platform height
        */
        virtual int GetHeight() const;

        /**
        * Sets platform height
        *@param value - platform height
        */
        virtual void SetHeight(int value);

        /**
        * Gets platform angle around the tower
        *@return platform angle around the tower
        */
        virtual float GetAngle() const;

        /**
        * Sets platform angle around the tower
        *@param angle - platform angle around the tower
        */
        virtual void SetAngle(float angle);

    protected:
        int   m_X      = 0;
        int   m_Y      = 0;
        int   m_Width  = 80;
        int   m_Height = 20;
        float m_Angle  = 0.0f;
};
