#pragma hdrstop
#include "WVCLPlasma.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall WVCLPlasma::WVCLPlasma(int width, int height, ITfOnCalculatePlasmaElement fHandler) :
    WPlasma(512, fHandler)
{
    const unsigned offsetCount = 8;

    m_Offsets.reserve(offsetCount);

    // iterate through offsets and initialize them
    for (unsigned i = 0; i < offsetCount; ++i)
        m_Offsets.push_back(0);

    // initialize backbuffer
    m_pBackBuffer.reset(new TBitmap());
    m_pBackBuffer->PixelFormat   = pf32bit;
    m_pBackBuffer->IgnorePalette = true;
    m_pBackBuffer->Width         = width;
    m_pBackBuffer->Height        = height;
}
//---------------------------------------------------------------------------
WVCLPlasma::~WVCLPlasma()
{}
//---------------------------------------------------------------------------
void __fastcall WVCLPlasma::Recalculate(int width, int height)
{
    // update backbuffer
    m_pBackBuffer->Width  = width;
    m_pBackBuffer->Height = height;
}
//---------------------------------------------------------------------------
void __fastcall WVCLPlasma::Draw(TCanvas* pCanvas, const IVelocityMatrix& velocityMatrix)
{
    // incorrect velocity matrix?
    if (velocityMatrix.size() != 4)
        throw "Velocity matrix should contain 4 elements";

    // prepare vertical offsets
    m_Offsets[0] = m_Offsets[4];
    m_Offsets[1] = m_Offsets[5];

    // get back buffer width and height
    const int width  = m_pBackBuffer->Width;
    const int height = m_pBackBuffer->Height;

    // get element count
    const unsigned elementCount = m_Table.size();

    // iterate through back buffer lines
    for (int y = 0; y < height; ++y)
    {
        // get line
        TRGBQuad* pLine  = reinterpret_cast<TRGBQuad*>(m_pBackBuffer->ScanLine[y]);

        // prepare horizontal offsets
        m_Offsets[2] = m_Offsets[6];
        m_Offsets[3] = m_Offsets[7];

        // iterate through line pixels
        for (int x = 0; x < width; ++x)
        {
            // calculate pixel RGBA values
            pLine[x].rgbRed      = m_Table[m_Offsets[0]] + m_Table[m_Offsets[1]] + m_Table[m_Offsets[2]];
            pLine[x].rgbGreen    = m_Table[m_Offsets[1]] + m_Table[m_Offsets[2]] + m_Table[m_Offsets[3]];
            pLine[x].rgbBlue     = m_Table[m_Offsets[2]] + m_Table[m_Offsets[3]] + m_Table[m_Offsets[0]];
            pLine[x].rgbReserved = m_Table[m_Offsets[3]] + m_Table[m_Offsets[0]] + m_Table[m_Offsets[1]];

            // calculate next horizontal offsets
            m_Offsets[2] = CalculateOffset(m_Offsets[2], elementCount, 1);
            m_Offsets[3] = CalculateOffset(m_Offsets[3], elementCount, 2);
        }

        // calculate next vertical offsets
        m_Offsets[0] = CalculateOffset(m_Offsets[0], elementCount, 2);
        m_Offsets[1] = CalculateOffset(m_Offsets[1], elementCount, 1);
    }

    // calculate next plasma start offsets (this control the plasma speed too)
    m_Offsets[4] = CalculateOffset(m_Offsets[4], elementCount, velocityMatrix[0]);
    m_Offsets[5] = CalculateOffset(m_Offsets[5], elementCount, velocityMatrix[1]);
    m_Offsets[6] = CalculateOffset(m_Offsets[6], elementCount, velocityMatrix[2]);
    m_Offsets[7] = CalculateOffset(m_Offsets[7], elementCount, velocityMatrix[3]);

    // draw plasma to canvas
    pCanvas->Draw (0, 0, m_pBackBuffer.get());
}
//---------------------------------------------------------------------------

