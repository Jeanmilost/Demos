#include <vcl.h>
#pragma hdrstop

#include "Main.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
TMainForm* MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner)
    : TForm(pOwner)
{
    // initialize starfield
    for (unsigned i = 0; i < NbStars; ++i)
    {
        Starfield::IVector3D position;
        position.m_X = random(this->ClientWidth)  > this->ClientWidth  / 2 ? random(this->ClientWidth)  : -random(this->ClientWidth);
        position.m_Y = random(this->ClientHeight) > this->ClientHeight / 2 ? random(this->ClientHeight) : -random(this->ClientHeight);
        position.m_Z = random(1600);
        m_StarField[i].SetPosition(position);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::tmDisplayTimer(TObject* pSender)
{
    try
    {
        // iterate through all stars
        for (unsigned i = 0; i < NbStars; ++i)
        {
            bool isOldVisible;
            bool isVisible;

            // get current screen resolution
            Starfield::IResolution resolution;
            resolution.m_X = this->ClientWidth;
            resolution.m_Y = this->ClientHeight;

            // get current 3D position
            Starfield::IVector3D curPosition;
            curPosition = m_StarField[i].GetPosition();

            // get old star position on screen
            Starfield::IVector2D oldPosition = m_StarField[i].GetPosition(resolution, 1600, isOldVisible);

            // check if z position is out of bounds
            if (curPosition.m_Z > 1.0f)
                // change star position
                curPosition.m_Z -= 10.0f;
            else
                // reinitialize star position
                curPosition.m_Z = 1600;

            // set new star position
            m_StarField[i].SetPosition(curPosition);

            // get new star position on screen
            Starfield::IVector2D position = m_StarField[i].GetPosition(resolution, 1600, isVisible);

            // check if old position is visible on screen
            if (isOldVisible)
            {
                // remove old star position
                Canvas->Pen->Color = this->Color;
                Canvas->Pen->Style = psSolid;
                Canvas->Rectangle(oldPosition.m_X, oldPosition.m_Y, oldPosition.m_X + 2, oldPosition.m_Y + 2);
            }

            // check if new position is visible on screen
            if (isVisible)
            {
                // draw star
                Canvas->Pen->Color = (TColor)Starfield::GetLuminance(curPosition.m_Z, 1600);
                Canvas->Pen->Style = psSolid;
                Canvas->Rectangle(position.m_X, position.m_Y, position.m_X + 2, position.m_Y + 2);
            }
        }
    }
    catch (...)
    {
        // the joke lasted enough
        tmDisplay->Enabled = false;
        this->Close();
    }
}
//---------------------------------------------------------------------------
