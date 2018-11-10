/*****************************************************************************
 * ==> QR_Random ------------------------------------------------------------*
 *****************************************************************************
 * Description : Class to generate random numbers                            *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef QR_RandomH
#define QR_RandomH
#include <cstdlib>
#include <ctime>
#include <iostream>

/**
* Class to generate random numbers
*@author Jean-Milost Reymond
*/
class QR_Random
{
    public:
        /**
        * Initializes random number generator
        */
        static void Initialize();

        /**
        * Gets randomized number
        *@param range - range
        *@return randomized number
        */
        static unsigned GetNumber(unsigned range = RAND_MAX);

    private:
        static bool m_Initialized;
};

#endif // QR_RandomH
