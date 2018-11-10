/*****************************************************************************
 * ==> IP_Star --------------------------------------------------------------*
 * ***************************************************************************
 * Description : Star and starfield objects                                  *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef IP_StarH
#define IP_StarH
#include <vector>
#include <gl/gl.h>
#include "IP_Random.h"

#define M_NbStars   1000
#define M_StarDepth 1000.0f

/**
* Star class
*@author Jean-Milost Reymond
*/
class IP_Star
{
    public:
        /**
        * Structure representing a 3D vector
        */
        struct IVector3D
        {
            float m_X; // x coordinate
            float m_Y; // y coordinate
            float m_Z; // z coordinate
        };

        /**
        * Structure representing a 2D vector
        */
        struct IVector2D
        {
            float m_X; // x coordinate
            float m_Y; // y coordinate
        };

        /**
        * Structure representing screen resolution
        */
        struct IResolution
        {
            unsigned m_X;
            unsigned m_Y;
        };

        /**
        * Constructor
        */
        IP_Star();

        /**
        * Destructor
        */
        ~IP_Star();

        /**
        * Rasterize and get star position in 2D coordinates
        *@param resolution - screen resolution
        *@param focale - value for focale, cannot be little than 1
        *@returns star position
        */
        const IVector2D& GetPosition(const IResolution& resolution, float focale);

        /**
        * Rasterize and get star position in 2D coordinates
        *@returns star position
        */
        const IVector3D& GetPosition();

        /**
        * Set star position
        *@param value - star position
        */
        void SetPosition(const IVector3D& value);

    private:
        IVector3D m_Position;      // star position in 3D coordinate
        IVector2D m_RasterizedPos; // converted position of star in 2D coordinate

        /**
        * Convert internal 3D vector and get 2D coordinate
        *@param focale - value for focale
        *@param position - value of original 3D position to convert
        *@param[out] result - resulting value
        */
        void Rasterize(float focale, const IVector3D& position, IVector2D& result);
};

/**
* Color object
*/
struct IP_Color
{
    unsigned char m_R;
    unsigned char m_G;
    unsigned char m_B;
};

/**
* Contains the 3D object data for displaying
*/
struct IP_3DObject
{
    static const GLfloat m_Vertices[];
    static       GLubyte m_Colors[];
};

/**
* Starfield object
*/
class IP_Starfield
{
    public:
        /**
        * Constructor
        */
        IP_Starfield();

        /**
        * Destructor
        */
        ~IP_Starfield();

        /**
        * Set background color
        *@param r - red component
        *@param g - green component
        *@param b - blue component
        */
        void SetBackgroundColor(unsigned char r, unsigned char g, unsigned char b);

        /**
        * Sets stars color
        *@param r - red component
        *@param g - green component
        *@param b - blue component
        */
        void SetStarsColor(unsigned char r, unsigned char g, unsigned char b);

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
        *@param screenWidth - screen width
        *@param screenHeight - screen height
        */
        void InitializeStars(unsigned nbStars, unsigned screenWidth, unsigned screenHeight);

        /**
        * Displays the scene
        *@param screenWidth - screen width
        *@param screenHeight - screen height
        */
        void Render(unsigned screenWidth, unsigned screenHeight);

        /**
        * Calculates color
        *@param deep - star deep
        */
        void CalculateColor(float deep);

    private:
        std::vector<IP_Star> m_Starfield;
        IP_Color*            m_pBackgroundColor;
        IP_Color*            m_pStarsColor;
        float                m_Angle;
        float                m_SpeedVelocity;
        float                m_RotateVelocity;
};
#endif
