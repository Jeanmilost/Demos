#ifndef MainH
#define MainH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// wclasses
#include "WVCLPlasma.h"

/**
* Plasma demo
*@author JMR
*/
class TMainForm : public TForm
{
    __published:
        TTimer *tiRefresh;

        void __fastcall FormResize(TObject* pSender);
        void __fastcall tiRefreshTimer(TObject* pSender);

    public:
        /**
        * Constructor
        *@param pOwner - form owner
        */
        __fastcall TMainForm(TComponent* pOwner);

    private:
        std::auto_ptr<WVCLPlasma>   m_pPlasma;
        WVCLPlasma::IVelocityMatrix m_VelocityMatrix;

        /**
        * Called when plasma element should be calculated
        *@param index - element index
        *@param size - plasma table size
        *@param[out] element - calculated element
        */
        void OnCalculatePlasmaElement(unsigned index, unsigned size, int& element);
};
extern PACKAGE TMainForm* MainForm;
#endif
