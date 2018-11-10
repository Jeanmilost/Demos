/*****************************************************************************
 * ==> QR_Emerald -----------------------------------------------------------*
 *****************************************************************************
 * Description : Emerald game piece                                          *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_EMERALDH
#define QR_EMERALDH

// interface
#include "QR_Piece.h"

/**
* Emerald game piece
*@note Piece is shown as follow on play field
*      *
*      **
*       *
*@author Jean-Milost Reymond
*/
class QR_Emerald : public QR_Piece
{
    public:
        QR_Emerald();
        virtual ~QR_Emerald();
};

#endif // QR_EMERALDH

