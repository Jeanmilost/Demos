/*****************************************************************************
 * ==> QR_Agathe ------------------------------------------------------------*
 *****************************************************************************
 * Description : Agathe game piece                                           *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_AGATHEH
#define QR_AGATHEH

// interface
#include "QR_Piece.h"

/**
* Agathe game piece
*@note Piece is shown as follow on play field
*       *
*      **
*      *
*@author Jean-Milost Reymond
*/
class QR_Agathe : public QR_Piece
{
    public:
        QR_Agathe();
        virtual ~QR_Agathe();
};

#endif // QR_AGATHEH

