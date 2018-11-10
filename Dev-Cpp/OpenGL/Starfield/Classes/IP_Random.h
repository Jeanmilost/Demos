/*****************************************************************************
 * ==> IP_Random ------------------------------------------------------------*
 * ***************************************************************************
 * Description : Random numbers generator                                    *
 * Developper  : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef IP_RandomH
#define IP_RandomH
#include <cstdlib>
#include <ctime>
#include <iostream>

class IP_Random
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
};
#endif
