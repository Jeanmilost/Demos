/*****************************************************************************
 * ==> QR_Sapphire ----------------------------------------------------------*
 *****************************************************************************
 * Description : Sapphire game piece                                         *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_SAPPHIREH
#define QR_SAPPHIREH

// interface
#include "QR_Piece.h"

/**
* Sapphire game piece
*@note Piece is shown as follow on play field
*       *
*      ***
*@author Jean-Milost Reymond
*/
class QR_Sapphire : public QR_Piece
{
    public:
        QR_Sapphire();
        virtual ~QR_Sapphire();
};

#endif // QR_SAPPHIREH

