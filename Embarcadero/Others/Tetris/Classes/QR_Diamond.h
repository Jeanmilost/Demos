/*****************************************************************************
 * ==> QR_Diamond -----------------------------------------------------------*
 *****************************************************************************
 * Description : Diamond game piece                                          *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_DIAMONDH
#define QR_DIAMONDH

// interface
#include "QR_Piece.h"

/**
* Diamond game piece
*@note Piece is shown as follow on play field
*      ****
*@author Jean-Milost Reymond
*/
class QR_Diamond : public QR_Piece
{
    public:
        QR_Diamond();
        virtual ~QR_Diamond();
};

#endif // QR_DIAMONDH

