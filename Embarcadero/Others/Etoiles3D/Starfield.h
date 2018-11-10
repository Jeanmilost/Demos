#ifndef StarfieldH
#define StarfieldH

/**
* 3D Starfield class
*@author Jean-Milost Reymond
*/
class Starfield
{
    public :
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
        Starfield();

        /**
        * Destructor
        */
        ~Starfield();

        /**
        * Rasterize and get star position in 2D coordinates
        *@param resolution - screen resolution
        *@param focale - value for focale, cannot be little than 1
        *@param[out] isVisible - if true, pixel is visible on screen after rasterization
        *@returns star position
        */
        const IVector2D& GetPosition(const IResolution& resolution, float focale, bool& isVisible);

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

        /**
        * Get luminance of pixel
        *@param z - z position of pixel
        *@param deep - max deep of starfield
        *@returns color representing luminance of star
        */
        static unsigned GetLuminance(float z, float deep);

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

        /**
        * Apply screen corrections and check if pixel is always visible
        *@param resolution - screen resolution
        *@param pixel - pixel to check and correct
        *@param[out] result - corrected pixel
        *@returns true if pixel is always vilible on screen after correction, otherwise false
        */
        bool ApplyCorrection(const IResolution& resolution, const IVector2D& pixel, IVector2D& result);
};
#endif
