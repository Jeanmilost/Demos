/*****************************************************************************
 * ==> Main -----------------------------------------------------------------*
 *****************************************************************************
 * Description : Application main entry point                                *
 * Developer   : Jean-Milost Reymond                                         *
 *****************************************************************************/

#pragma hdrstop
#pragma argsused

// std
#include <stdio.h>
#include <memory>

// interface
#include "QR_GameEngine.h"
#include "QR_GameInterface.h"

//------------------------------------------------------------------------------
// Application main entry point
//------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    // create game engine and interface
    std::auto_ptr<QR_GameInterface> pInterface(new QR_GameInterface());
    std::auto_ptr<QR_GameEngine>    pEngine(new QR_GameEngine());

    // failed?
    if (!pEngine.get() || !pInterface.get())
    {
        std::wprintf(L"Game initialisation failed !\n");
        std::system("PAUSE");
        return EXIT_SUCCESS;
    }

    // create output console
    pInterface->InitConsole();

    // loop until quit command is sent
    while (!pInterface->DoQuit())
    {
        // is game over?
        if (pEngine->IsGameOver())
        {
            // show game over, wait until user restarts or quits, then reset game
            pInterface->DrawGameOver(pEngine.get());
            pEngine->Reset();
        }
        else
        {
            // play and draw game
            pEngine->Play();
            pInterface->DrawGame(pEngine.get());
        }

        // is key pressed?
        if (kbhit())
            // handle pressed key
            pInterface->CheckKeyPressed(pEngine.get());
    }

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
