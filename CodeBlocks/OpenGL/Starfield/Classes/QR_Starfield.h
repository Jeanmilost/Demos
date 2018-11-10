/*****************************************************************************
 * ==> QR_Starfield ---------------------------------------------------------*
 * ***************************************************************************
 * Description : Starfield class                                             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_StarfieldH
#define QR_StarfieldH
#include <vector>
#include <gl/gl.h>
#include "QR_Random.h"
#include "QR_GfxCommon.h"
#include "QR_Color.h"
#include "QR_Star.h"

#define M_NbStars   1000
#define M_StarDepth 1000.0f

/**
* Contains the 3D object data for displaying
*/
struct QR_3DObject
{
    static const GLfloat m_Vertices[];
    static       GLubyte m_Colors[];
};

/**
* Starfield object
*/
class QR_Starfield
{
    public:
        /**
        * Constructor
        */
        QR_Starfield();

        /**
        * Destructor
        */
        ~QR_Starfield();

        /**
        * Set background color
        *@param color - color
        */
        void SetBackgroundColor(const QR_Color& color);

        /**
        * Sets stars color
        *@param color - color
        */
        void SetStarsColor(const QR_Color& color);

        /**
        * Sets speed velocity
        *@param value - velocity
        */
        void SetSpeedVelocity(float value);

        /**
        * Sets rotation velocity
        *@param value - velocity
        */
        void SetRotateVelocity(float value);

        /**
        * Initializes starfield
        *@param nbStars - number of stars in the starfield
        *@param screen - screen data
        */
        void InitializeStars(unsigned nbStars, const QR_Screen& screen);

        /**
        * Displays the scene
        */
        void Render();

        /**
        * Calculates color
        *@param deep - star deep
        */
        void CalculateColor(float deep);

    private:
        std::vector<QR_Star> m_Starfield;
        QR_Color*            m_pBackgroundColor;
        QR_Color*            m_pStarsColor;
        float                m_Angle;
        float                m_SpeedVelocity;
        float                m_RotateVelocity;
};
#endif
