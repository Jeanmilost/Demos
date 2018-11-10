#ifndef MainH
#define MainH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

// std
#include <string>
#include <memory>

class TMainForm : public TForm
{
    __published:
    TTimer *tiRefresh;
    void __fastcall tiRefreshTimer(TObject* pSender);

    public:
        __fastcall TMainForm(TComponent* pOwner);

    private:
        std::auto_ptr<TBitmap> m_pBackBuffer;
        std::wstring           m_Text;
        int                    m_Position;
        float                  m_StartAngle;
};
extern PACKAGE TMainForm* MainForm;
#endif
