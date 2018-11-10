/*****************************************************************************
 * ==> QR_Ruby --------------------------------------------------------------*
 *****************************************************************************
 * Description : Ruby game piece                                             *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_RUBYH
#define QR_RUBYH

// interface
#include "QR_Piece.h"

/**
* Ruby game piece
*@note Piece is shown as follow on play field
*      **
*      **
*@author Jean-Milost Reymond
*/
class QR_Ruby : public QR_Piece
{
    public:
        QR_Ruby();
        virtual ~QR_Ruby();
};

#endif // QR_RUBYH

