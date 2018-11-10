/*****************************************************************************
 * ==> QR_GfxCommon ---------------------------------------------------------*
 *****************************************************************************
 * Description : Some basic graphic types, as e.g. point, size, ...          *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_GfxCommon_h
#define QR_GfxCommon_h

/**
* Structure representing point
*/
struct QR_Point
{
    int m_X;
    int m_Y;

    QR_Point();

    /**
    * Constructor
    *@param x - x coordinate in pixels
    *@param y - y coordinate in pixels
    */
    QR_Point(int x, int y);
};

/**
* Structure representing size
*/
struct QR_Size
{
    int m_Width;
    int m_Height;

    QR_Size();

    /**
    * Constructor
    *@param width - width in pixels
    *@param height - height in pixels
    */
    QR_Size(unsigned width, unsigned height);
};

/**
* Structure representing screen
*/
struct QR_Screen
{
    unsigned m_Width;
    unsigned m_Height;

    QR_Screen();

    /**
    * Constructor
    *@param width - width in pixels
    *@param height - height in pixels
    */
    QR_Screen(unsigned width, unsigned height);
};

#endif
