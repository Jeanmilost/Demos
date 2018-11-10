/*****************************************************************************
 * ==> E_Random -------------------------------------------------------------*
 * ***************************************************************************
 * Description : To generate random numbers                                  *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include <cstdlib>
#include <ctime>
#include <iostream>

class E_Random
{
    public:
        /**
        * Initialize random number generator
        */
        static void Initialize();

        /**
        * Get randomized number
        *@param range - range
        *@returns randomized number
        */
        static unsigned GetNumber(unsigned range = RAND_MAX);

    private:
        static bool m_Initialized;
};
