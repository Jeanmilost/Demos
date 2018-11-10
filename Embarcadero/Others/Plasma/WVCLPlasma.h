#ifndef WVCLPlasmaH
#define WVCLPlasmaH

// vcl
#include <Vcl.Graphics.hpp>

// interface
#include "WPlasma.h"

class WVCLPlasma : public WPlasma
{
    public:
        typedef std::vector<int> IVelocityMatrix;

        /**
        * Constructor
        *@param width - plasma width
        *@param height - plasma height
        *@param fHandler - OnCalculatePlasmaElement callback handler, can be NULL
        */
        __fastcall WVCLPlasma(int width, int height, ITfOnCalculatePlasmaElement fHandler = NULL);

        virtual ~WVCLPlasma();

        /**
        * Recalculate plasma size
        *@param width - plasma width
        *@param height - plasma height
        */
        virtual void __fastcall Recalculate(int width, int height);

        /**
        * Draw plasma to canvas
        *@param pCanvas - canvas to draw on
        */
        virtual void __fastcall Draw(TCanvas* pCanvas, const IVelocityMatrix& velocityMatrix);

    private:
        typedef std::vector<unsigned> IOffsets;

        std::auto_ptr<TBitmap> m_pBackBuffer;
        IOffsets               m_Offsets;
};
#endif
