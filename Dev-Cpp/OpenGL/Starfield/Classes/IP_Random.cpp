/*****************************************************************************
 * ==> IP_Random ------------------------------------------------------------*
 * ***************************************************************************
 * Description : Random numbers generator                                    *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "IP_Random.h"

//------------------------------------------------------------------------------
// IP_Random
//------------------------------------------------------------------------------
void IP_Random::Initialize()
{
    std::srand((unsigned)std::time(0));
}
//------------------------------------------------------------------------------
unsigned IP_Random::GetNumber(unsigned range)
{
    return (std::rand() % range);
}
//------------------------------------------------------------------------------
