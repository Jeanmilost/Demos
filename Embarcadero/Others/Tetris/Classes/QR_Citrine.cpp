/*****************************************************************************
 * ==> QR_Citrine -----------------------------------------------------------*
 *****************************************************************************
 * Description : Citrine game piece                                          *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Citrine.h"

//------------------------------------------------------------------------------
QR_Citrine::QR_Citrine() : QR_Piece()
{
    m_Right  = 3;
    m_Bottom = 2;
    m_Color  = QR_Cell::IE_LightYellow;
    m_Name   = L"Citrine";

    // create buffer indicating how piece should be filled
    char fillArray[] = {1 , 0 , 0,
                        1 , 1 , 1};

    // create piece
    Create(fillArray, 6);
}
//------------------------------------------------------------------------------
QR_Citrine::~QR_Citrine()
{}
//------------------------------------------------------------------------------

