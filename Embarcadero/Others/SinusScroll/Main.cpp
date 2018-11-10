#include <vcl.h>
#pragma hdrstop
#include "Main.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TMainForm* MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner) : TForm(pOwner)
{
    m_Text = L"This is a sinus scrolling text, as existed 20 years ago on Amiga computers... See and enjoy !!! ";

    m_pBackBuffer.reset(new TBitmap());
    m_pBackBuffer->PixelFormat = pf32bit;
    m_pBackBuffer->AlphaFormat = afDefined;

    m_Position   = 0;
    m_StartAngle = 0.0f;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::tiRefreshTimer(TObject* pSender)
{
    const unsigned extraHeight = 10;

    TRect bufferRect;
    ::DrawTextW(Canvas->Handle, m_Text.c_str(), m_Text.length(), &bufferRect, DT_LEFT | DT_TOP
            | DT_SINGLELINE | DT_CALCRECT);

    m_pBackBuffer->Width  = bufferRect.Width();//REM  + ClientWidth;
    m_pBackBuffer->Height = bufferRect.Height() + extraHeight;

    m_pBackBuffer->Canvas->Brush->Style = bsSolid;
    m_pBackBuffer->Canvas->Brush->Color = Color;
    m_pBackBuffer->Canvas->FillRect(TRect(0, 0, m_pBackBuffer->Width, m_pBackBuffer->Height));

    m_pBackBuffer->Canvas->Font->Assign(Canvas->Font);

    ::DrawTextW(m_pBackBuffer->Canvas->Handle, m_Text.c_str(), m_Text.length(), &bufferRect, DT_LEFT
            | DT_TOP | DT_SINGLELINE);

    /*REM
    TRect remainingRect = bufferRect;
    remainingRect.Left  = bufferRect.Right;
    remainingRect.Right = remainingRect.Left + ClientWidth;

    ::DrawTextW(m_pBackBuffer->Canvas->Handle, m_Text.c_str(), m_Text.length(), &remainingRect,
            DT_LEFT | DT_TOP | DT_SINGLELINE);
    */

    std::auto_ptr<TBitmap> pOverlay(new TBitmap());
    pOverlay->PixelFormat = pf32bit;
    pOverlay->AlphaFormat = afDefined;
    pOverlay->Width       = ClientWidth;
    pOverlay->Height      = ClientHeight;

    pOverlay->Canvas->Brush->Style = bsSolid;
    pOverlay->Canvas->Brush->Color = Color;
    pOverlay->Canvas->FillRect(TRect(0, 0, pOverlay->Width, pOverlay->Height));

    const unsigned yPos = (ClientHeight >> 1) - (bufferRect.Height() >> 1);

    /*REM
    ::BitBlt(Canvas->Handle, 0, yPos, ClientWidth, bufferRect.Height(),
            m_pBackBuffer->Canvas->Handle, m_Position, 0, SRCCOPY);
    */
    float angle = m_StartAngle;

    const unsigned offset    = 10;
    const unsigned scrollMax = bufferRect.Width();

    for (unsigned i = 0; i < ClientWidth; i += offset)
    {
        const unsigned srcStartPosX = (m_Position + i) % scrollMax;
        const unsigned srcEndPosX   = srcStartPosX + offset;
        const unsigned dstStartPosX = i;
              unsigned dstEndPosX   = i + offset;
        const unsigned scrollY      = yPos + (20.0f * std::sinf(angle));
        const unsigned scrollHeight = m_pBackBuffer->Height;

        if (dstEndPosX > ClientWidth)
            dstEndPosX = ClientWidth;

        if (srcEndPosX > scrollMax)
        {
            const unsigned intermediateWidth = scrollMax - srcStartPosX;

            ::BitBlt(pOverlay->Canvas->Handle, dstStartPosX, scrollY, intermediateWidth,
                    scrollHeight, m_pBackBuffer->Canvas->Handle, srcStartPosX, 0, SRCCOPY);

            ::BitBlt(pOverlay->Canvas->Handle, dstStartPosX + intermediateWidth, scrollY,
                    srcEndPosX - scrollMax, scrollHeight,m_pBackBuffer->Canvas->Handle, 0, 0,
                    SRCCOPY);
        }
        else
            ::BitBlt(pOverlay->Canvas->Handle, dstStartPosX, scrollY, dstEndPosX, scrollHeight,
                    m_pBackBuffer->Canvas->Handle, srcStartPosX, 0, SRCCOPY);

        angle += 0.1f;

        if (angle >= 6.28f)
            angle -= 6.28f;
    }

    m_StartAngle += 0.1f;

    if (m_StartAngle >= 6.28f)
        m_StartAngle -= 6.28f;

    ++m_Position;

    if (m_Position >= m_pBackBuffer->Width)
        m_Position -= m_pBackBuffer->Width;

//    if (m_Position >= (m_pBackBuffer->Width - ClientWidth))
//    {
//        const unsigned remainingSize = (bufferRect.Width() - m_Position);
//        //REM const unsigned xPos          = remainingSize;
//
//        /*REM
//        ::BitBlt(pOverlay->Canvas->Handle, remainingSize, yPos, ClientWidth - remainingSize,
//                bufferRect.Height(), m_pBackBuffer->Canvas->Handle, 0, 0, SRCCOPY);
//        */
//        for (unsigned i = 0; i < ClientWidth; i += 10)
//        {
//            ::BitBlt(pOverlay->Canvas->Handle, remainingSize + i, yPos + (10.0f * std::sinf(angle)),
//                    remainingSize + i + 10, bufferRect.Height(), m_pBackBuffer->Canvas->Handle, i, 0, SRCCOPY);
//
//            angle += 0.1f;
//
//            if (angle >= 6.28f)
//                angle -= 6.28f;
//        }
//    }

    ::BitBlt(Canvas->Handle, 0, 0, ClientWidth, ClientHeight, pOverlay->Canvas->Handle, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------

