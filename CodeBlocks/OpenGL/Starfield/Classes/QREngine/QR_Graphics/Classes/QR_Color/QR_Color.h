/*****************************************************************************
 * ==> QR_Color -------------------------------------------------------------*
 *****************************************************************************
 * Description : Universal color class                                       *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_Color_h
#define QR_Color_h

/**
* Universal color class
*@author Jean-Milost Reymond
*/
class QR_Color
{
    public:
        QR_Color();

        /**
        * Constructor
        *@param r - color red component
        *@param g - color green component
        *@param b - color blue component
        *@param a - color alpha component
        */
        QR_Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

        /**
        * Sets color
        *@param r - color red component
        *@param g - color green component
        *@param b - color blue component
        *@param a - color alpha component
        */
        void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

        /**
        * Gets color as 32 bit value
        *@return color as 32 bit value
        */
        inline unsigned GetRGB() const;
        inline unsigned GetBGR() const;
        inline unsigned GetARGB() const;
        inline unsigned GetRGBA() const;
        inline unsigned GetABGR() const;
        inline unsigned GetBGRA() const;

        /**
        * Sets color from 32 bit value
        *@return color as 32 bit value
        */
        inline void SetRGB(unsigned value);
        inline void SetBGR(unsigned value);
        inline void SetARGB(unsigned value);
        inline void SetRGBA(unsigned value);
        inline void SetABGR(unsigned value);
        inline void SetBGRA(unsigned value);

        /**
        * Gets red component
        *@return red component
        */
        unsigned char GetRed() const;

        /**
        * Gets green component
        *@return green component
        */
        unsigned char GetGreen() const;

        /**
        * Gets blue component
        *@return blue component
        */
        unsigned char GetBlue() const;

        /**
        * Gets alpha component
        *@return alpha component
        */
        unsigned char GetAlpha() const;

        /**
        * Sets red component
        *@param value - component value
        */
        void SetRed(unsigned char value);

        /**
        * Sets green component
        *@param value - component value
        */
        void SetGreen(unsigned char value);

        /**
        * Sets blue component
        *@param value - component value
        */
        void SetBlue(unsigned char value);

        /**
        * Sets alpha component
        *@param value - component value
        */
        void SetAlpha(unsigned char value);

        /**
        * Blend color with another color
        *@param other - other color to blend with
        *@param offset - blend offset (from 0.0f to 1.0f)
        *@return blended color
        */
        QR_Color Blend(const QR_Color& other, float offset) const;

    private:
        unsigned char m_R;
        unsigned char m_G;
        unsigned char m_B;
        unsigned char m_A;
};
#endif
