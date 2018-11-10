/*****************************************************************************
 * ==> QR_Aquamarine --------------------------------------------------------*
 *****************************************************************************
 * Description : Aquamarine game piece                                       *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Aquamarine.h"

//------------------------------------------------------------------------------
QR_Aquamarine::QR_Aquamarine() : QR_Piece()
{
    m_Right  = 3;
    m_Bottom = 2;
    m_Color  = QR_Cell::IE_Marine;
    m_Name   = L"Aquamarine";

    // create buffer indicating how piece should be filled
    char fillArray[] = {0, 0, 1,
                        1, 1, 1};

    // create piece
    Create(fillArray, 6);
}
//------------------------------------------------------------------------------
QR_Aquamarine::~QR_Aquamarine()
{}
//------------------------------------------------------------------------------

