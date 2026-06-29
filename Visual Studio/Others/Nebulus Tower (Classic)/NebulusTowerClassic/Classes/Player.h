#pragma once

/**
* Nebulus player
*@author Jean-Milost Reymond
*/
class Player
{
    public:
        Player();
        virtual ~Player();

        /**
        * Notifies that player is moving to the left
        */
        virtual void MoveLeft();

        /**
        * Notifies that player is moving to the right
        */
        virtual void MoveRight();

        /**
        * Notifies that player stopped to move
        */
        virtual void Stop();

        /**
        * Checks if player is turning
        *@return true if player is turning, otherwise false
        */
        virtual bool IsTurning() const;

    protected:
        bool m_Turning     = false;
        bool m_MovingLeft  = false;
        bool m_MovingRight = true;
        bool m_Stopped     = true;
};
