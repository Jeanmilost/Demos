/*****************************************************************************
 * ==> E_Random -------------------------------------------------------------*
 * ***************************************************************************
 * Description : To generate random numbers                                  *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "E_Random.h"

//------------------------------------------------------------------------------
// class E_Random - c++
//------------------------------------------------------------------------------
bool E_Random::m_Initialized = false;
//------------------------------------------------------------------------------
void E_Random::Initialize()
{
    std::srand((unsigned)std::time(0));

    m_Initialized = true;
}
//------------------------------------------------------------------------------
unsigned E_Random::GetNumber(unsigned range)
{
    if (!m_Initialized)
        Initialize();

    return (std::rand() % range);
}
//------------------------------------------------------------------------------		
