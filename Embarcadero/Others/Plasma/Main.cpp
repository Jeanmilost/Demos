#include <vcl.h>
#pragma hdrstop
#include "Main.h"

// vcl
#include <algorithm>

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TMainForm* MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner) : TForm(pOwner)
{
    // set velocity matrix (should always contain 4 elements)
    m_VelocityMatrix.push_back(5);
    m_VelocityMatrix.push_back(-5);
    m_VelocityMatrix.push_back(2);
    m_VelocityMatrix.push_back(-3);

    // create plasma generator
    m_pPlasma.reset(new WVCLPlasma(ClientWidth, ClientHeight, OnCalculatePlasmaElement));
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{
    // recalculate plasma size
    m_pPlasma->Recalculate(ClientWidth, ClientHeight);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::tiRefreshTimer(TObject *Sender)
{
    // draw plasma on main form
    m_pPlasma->Draw(Canvas, m_VelocityMatrix);
}
//---------------------------------------------------------------------------
void TMainForm::OnCalculatePlasmaElement(unsigned index, unsigned size, int& element)
{
    // calculate plasma element
    element = 40.0f * (1.0f + std::cosf((float)index * 6.28f / size));
}
//---------------------------------------------------------------------------

