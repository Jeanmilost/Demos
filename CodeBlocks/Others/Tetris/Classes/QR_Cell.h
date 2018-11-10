/*****************************************************************************
 * ==> QR_Cell --------------------------------------------------------------*
 *****************************************************************************
 * Description : Basic game cell                                             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_CELLH
#define QR_CELLH

/**
* Basic game cell
*@author Jean-Milost Reymond
*/
class QR_Cell
{
    public:
        /**
        * Game color enumeration
        *@note Values are used by Windows console to select color, don't modify them
        */
        enum IEColor
        {
            IE_Red         = 1,
            IE_Green       = 2,
            IE_Blue        = 3,
            IE_Yellow      = 4,
            IE_Violet      = 5,
            IE_Marine      = 6,
            IE_White       = 7,
            IE_Gray        = 8,
            IE_LightRed    = 9,
            IE_LightGreen  = 10,
            IE_LightBlue   = 11,
            IE_LightYellow = 12,
            IE_LightViolet = 13,
            IE_LightMarine = 14,
            IE_LightWhite  = 15
        };

        /**
        * Cell status enumeration
        */
        enum IEStatus
        {
            IE_Empty = 0,
            IE_Filled,
            IE_Frozen,
        };

        QR_Cell();
        virtual ~QR_Cell();

        /**
        * Clear cell
        */
        virtual void Clear();

        /**
        * Gets color
        *@return color
        */
        virtual IEColor GetColor();

        /**
        * Sets color
        *@param color - color
        */
        virtual void SetColor(IEColor color);

        /**
        * Gets status
        *@return status
        */
        virtual IEStatus GetStatus();
        /**
        * Sets status
        *@param status - status
        */
        virtual void SetStatus(IEStatus status);

    private:
        IEColor  m_Color;
        IEStatus m_Status;
};
#endif // QR_CELLH

