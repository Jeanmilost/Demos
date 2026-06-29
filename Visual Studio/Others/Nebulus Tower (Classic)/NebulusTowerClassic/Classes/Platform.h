#pragma once

/**
* Nebulus tower platform
*@author Jean-Milost Reymond
*/
class Platform
{
    public:
        Platform();

        /**
        * Constructor
        *@param width - platform width
        *@param height - platform height
        */
        Platform(int width, int height);

        virtual ~Platform();

        /**
        * Get platform width
        *@return platform width
        */
        virtual int GetWidth() const;

        /**
        * Get platform height
        *@return platform height
        */
        virtual int GetHeight() const;

        /**
        * Get platform angle around the tower
        *@return platform angle around the tower
        */
        virtual float GetAngle() const;

        /**
        * Set platform angle around the tower
        *@param angle - platform angle around the tower
        */
        virtual void SetAngle(float angle);

    protected:
        int   m_Width  = 80;
        int   m_Height = 20;
        float m_Angle  = 0.0f;
};
