/*****************************************************************************
 * ==> QR_Citrine -----------------------------------------------------------*
 *****************************************************************************
 * Description : Citrine game piece                                          *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_CITRINEH
#define QR_CITRINEH

// interface
#include "QR_Piece.h"

/**
* Citrine game piece
*@note Piece is shown as follow on play field
*      *
*      ***
*@author Jean-Milost Reymond
*/
class QR_Citrine : public QR_Piece
{
    public:
        QR_Citrine();
        virtual ~QR_Citrine();
};

#endif // QR_CITRINEH

