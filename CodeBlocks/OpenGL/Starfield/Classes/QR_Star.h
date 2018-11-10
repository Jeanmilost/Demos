/*****************************************************************************
 * ==> QR_Star --------------------------------------------------------------*
 *****************************************************************************
 * Description : Class to create and manage a star                           *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_Star_h
#define QR_Star_h

// qr engine
#include "QR_Vector2D.h"
#include "QR_Vector3D.h"

/**
* Star class
*@author Jean-Milost Reymond
*/
class QR_Star
{
    public :
        /**
        * Constructor
        */
        QR_Star();

        /**
        * Destructor
        */
        ~QR_Star();

        /**
        * Converts and gets star position in 2D coordinates
        *@param focal - value for focal, cannot be little than 1
        *@return star position
        */
        const QR_Vector2D& GetPosition(float focal);

        /**
        * Gets star position in 3D coordinates
        *@return star position
        */
        const QR_Vector3D& GetPosition();

        /**
        * Sets star position
        *@param value - star position
        */
        void SetPosition(const QR_Vector3D& value);

    private:
        QR_Vector3D m_Position;      // star position in 3D coordinate
        QR_Vector2D m_RasterizedPos; // converted position of star in 2D coordinate

        /**
        * Converts internal 3D vector to 2D vector
        *@param focal - focal value
        *@param position - original 3D position to convert
        *@param[out] result - converted value
        */
        void Rasterize(float focal, const QR_Vector3D& position, QR_Vector2D& result);
};
#endif
