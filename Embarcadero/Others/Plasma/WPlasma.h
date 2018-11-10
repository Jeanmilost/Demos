#ifndef WPlasmaH
#define WPlasmaH

// std
#include <vector>

/**
* Plasma effect
*@author Jean-Milost Reymond
*/
class WPlasma
{
    public:
        /**
        * Called when plasma element should be calculated
        *@param index - element index
        *@param size - plasma table size
        *@param[out] element - calculated element
        */
        typedef void (__closure *ITfOnCalculatePlasmaElement)(unsigned index, unsigned size,
                int& element);

        /**
        * Constructor
        *@param size - plasma size
        *@param fHandler - OnCalculatePlasmaElement callback handler, can be NULL
        */
        WPlasma(unsigned size, ITfOnCalculatePlasmaElement fHandler);

        virtual ~WPlasma();

    protected:
        std::vector<int>            m_Table;
        ITfOnCalculatePlasmaElement m_fOnCalculatePlasmaElement;

        /**
        * Initialize plasma class
        *@param size - plasma size
        */
        void Initialize(unsigned size);

        /**
        * Clear plasma
        */
        void Clear();

        /**
        * Calculate offset inside the max limit
        *@param offset - current offset to calculate from
        *@param max - max limit
        *@param delta - delta from current offset, can be negative
        *@returns next offset
        */
        unsigned CalculateOffset(unsigned offset, unsigned max, int delta) const;
};
#endif
