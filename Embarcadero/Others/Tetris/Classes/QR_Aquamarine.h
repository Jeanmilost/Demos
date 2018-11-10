/*****************************************************************************
 * ==> QR_Aquamarine --------------------------------------------------------*
 *****************************************************************************
 * Description : Aquamarine game piece                                       *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_AQUAMARINEH
#define QR_AQUAMARINEH

// interface
#include "QR_Piece.h"

/**
* Aquamarine game piece
*@note Piece is shown as follow on play field
*        *
*      ***
*@author Jean-Milost Reymond
*/
class QR_Aquamarine : public QR_Piece
{
    public:
        QR_Aquamarine();
        virtual ~QR_Aquamarine();
};

#endif // QR_AQUAMARINEH

