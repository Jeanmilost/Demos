/*****************************************************************************
 * ==> QR_Random ------------------------------------------------------------*
 *****************************************************************************
 * Description : Class to generate random numbers                            *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "QR_Random.h"

//------------------------------------------------------------------------------
// class QR_Random - c++
//------------------------------------------------------------------------------
bool QR_Random::m_Initialized = false;
//------------------------------------------------------------------------------
void QR_Random::Initialize()
{
    std::srand((unsigned)std::time(0));

    m_Initialized = true;
}
//------------------------------------------------------------------------------
unsigned QR_Random::GetNumber(unsigned range)
{
    if (range == 0)
        return 0;

    if (!m_Initialized)
        Initialize();

    return (std::rand() % range);
}
//------------------------------------------------------------------------------