#pragma once

// std
#include <string>

/**
* Nebulus tower
*@author Jean-Milost Reymond
*/
class Tower
{
    public:
        Tower();

        /**
        * Constructor
        *@param width - tower width
        *@param rowHeight - tower row height
        *@param division - division angle which defines an individual brick on the row
        */
        Tower(std::size_t width, std::size_t rowHeight, float division);

        virtual ~Tower();

        /**
        * Gets the tower width
        *@return the tower width
        */
        virtual std::size_t GetWidth() const;

        /**
        * Gets the tower row height
        *@return the tower row height
        */
        virtual std::size_t GetRowHeight() const;

        /**
        * Gets the division angle which defines an individual brick on the row
        *@return the division angle
        */
        virtual float GetDivision() const;

        /**
        * Rotates the tower on the left
        *@param velocity - rotation velocity
        */
        virtual void RotateLeft(float velocity = 0.05f);

        /**
        * Rotates the tower on the right
        *@param velocity - rotation velocity
        */
        virtual void RotateRight(float velocity = 0.05f);

        /**
        * Draws the tower
        *@param elapsedTime - elapsed time since latest draw
        */
        virtual void Draw(double elapsedTime) const;

    protected:
        /**
        * Draws the tower
        *@param elapsedTime - elapsed time since latest draw
        *@param angle - tower view angle
        */
        virtual void DrawTower(double elapsedTime, float angle) const = 0;

    private:
        std::size_t m_Width     = 400;
        std::size_t m_RowHeight = 24;
        float       m_Division  = 20.0f;
        float       m_Angle     = 0.0f;
};
