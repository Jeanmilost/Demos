/******************************************************************************
 * ==> MD2Reader -------------------------------------------------------------*
 ******************************************************************************
 * Description : Application main entry point                                 *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/


#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

#include <tchar.h>

//------------------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//------------------------------------------------------------------------------

