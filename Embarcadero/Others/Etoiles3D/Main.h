#ifndef MainH
#define MainH

// vcl
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

// classes
#include "Starfield.h"

#define NbStars 512

/**
* Demo main form
*@author Jean-Milost Reymond
*/
class TMainForm : public TForm
{
    __published:
        TTimer *tmDisplay;

        void __fastcall tmDisplayTimer(TObject* pSender);

    public:
        __fastcall TMainForm(TComponent* pOwner);

    private:
        Starfield m_StarField[NbStars]; // starfield
};
extern PACKAGE TMainForm *MainForm;
#endif
