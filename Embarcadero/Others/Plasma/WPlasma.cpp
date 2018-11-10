#pragma hdrstop
#include "WPlasma.h"

// std
#include <cmath>

//---------------------------------------------------------------------------
WPlasma::WPlasma(unsigned size, ITfOnCalculatePlasmaElement fHandler) :
    m_fOnCalculatePlasmaElement(fHandler)
{
    Initialize(size);
}
//---------------------------------------------------------------------------
WPlasma::~WPlasma()
{}
//---------------------------------------------------------------------------
void WPlasma::Initialize(unsigned size)
{
    m_Table.resize(size);

    // get plasma element count
    const unsigned elementCount = m_Table.size();

    // iterate through plasma elements to initialize
    for (unsigned i = 0; i < elementCount; ++i)
    {
        // callback defined?
        if (m_fOnCalculatePlasmaElement)
        {
            // call custom plasma initialization function
            m_fOnCalculatePlasmaElement(i, size, m_Table[i]);
            continue;
        }

        // initialize element default value
        m_Table[i] = 30.0f * (1.0f + std::cosf(((float)i * 6.28f) / (float)elementCount));
    }
}
//---------------------------------------------------------------------------
void WPlasma::Clear()
{
    m_Table.clear();
}
//---------------------------------------------------------------------------
unsigned WPlasma::CalculateOffset(unsigned offset, unsigned max, int delta) const
{
    // nothing to do?
    if (!delta)
        return offset;

    // is delta negative?
    if (delta < 0)
    {
        // get delta absolute value
        unsigned absDelta = (unsigned)std::abs(delta);

        // is delta to subtract too high?
        if (offset < absDelta)
            // calculate and return resulting offset
            return ((max + offset) - absDelta);

        // calculate and return resulting offset
        return (offset - absDelta);
    }

    // calculate resulting offset
    unsigned result = (offset + delta);

    // is resulting offset out of bounds?
    if (result >= max)
        result -= max;

    return result;
}
//---------------------------------------------------------------------------

