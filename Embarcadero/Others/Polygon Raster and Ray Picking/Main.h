/****************************************************************************
 * ==> Main ----------------------------------------------------------------*
 ****************************************************************************
 * Description : This module contains the demo main form                    *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#ifndef MainH
#define MainH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.AppEvnts.hpp>

// std
#include <memory>

// classes
#include "Geometry.h"
#include "Polygon.h"

/**
* Demo main form
*@author Jean-Milost Reymond
*/
class TMainForm : public TForm
{
    __published:
        TApplicationEvents *aeEvents;

        void __fastcall aeEventsIdle(TObject* pSender, bool& done);

    public:
        /**
        * Constructor
        *@param pOwner - form owner
        */
        __fastcall TMainForm(TComponent* pOwner);

    private:
        std::auto_ptr<TBitmap> m_pBitmap;

        /**
        * Checks if a collision happended with the ray coming from the current mouse position
        *@param polygon - polygon to check
        *@return true if mouse ray collides the polygon, otherwise false
        */
        bool RayCollidePolygon(const Polygon3DF& polygon);
};
extern PACKAGE TMainForm* MainForm;
#endif
