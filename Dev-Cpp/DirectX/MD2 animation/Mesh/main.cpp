/*****************************************************************************
 * ==> Fonction WinMain -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette fonction est le point d'entr�e de l'application.      *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "application.h"

int WINAPI WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpszArgument,
                     int       nFunsterStil )
{
    if ( Application::GetInstance()->Initialize( hInstance,
                                                 nFunsterStil ) == false )
    {
        Application::GetInstance()->ReleaseInstance();
        return 0;
    }

    int p_Result = Application::GetInstance()->Run();

    Application::GetInstance()->ReleaseInstance();

    return p_Result;
}
