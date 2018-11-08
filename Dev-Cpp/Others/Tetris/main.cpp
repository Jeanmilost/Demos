/***********************************************
 * Point d'entrée et boucle principale du jeu. *
 ***********************************************/

#include "engine.h"
#include "gameinterface.h"

#include <cstdio>

// Boucle principale du jeu.
int main( int argc, char *argv[] )
{
    // On crée le moteur du jeu, ainsi que l'interface.
    GameInterface* theInterface = new GameInterface();
    Engine*        theEngine    = new Engine();

    // Si la création du moteur ou de l'interface échoue, on ne peut
    // pas continuer.
    if ( theEngine == NULL || theInterface == NULL )
    {
        printf( "Game initialisation failed !\n" );
        system( "PAUSE" );
        return EXIT_SUCCESS;
    }

    // Initialise l'interface.
    theInterface->Init();

    // Tant que l'interface ne spécifie pas que le jeu doit être quitté,
    // on continue.
    while ( theInterface->DoQuit() == false )
    {
        // On teste si le moteur indique que le jeu est fini.
        if ( theEngine->IsGameOver() == true )
        {
            // Si oui, on demande à l'interface d'afficher ce qu'il faut, puis
            // on réinitialise le moteur pour une nouvelle partie.
            theInterface->DisplayGameOver( theEngine );
            theEngine->Reset();
        }
        else
        {
            // Sinon, on affiche le jeu.
            theEngine->Play();
            theInterface->DisplayGame( theEngine );
        }

        // On teste si une touche a été pressée.
        if ( kbhit() )
        {
            // Dans ce cas, on répond à la demande du joueur.
            theInterface->OnKeyPressed( theEngine );
        }
    }

    // On nettoie la mémoire à la fin du jeu.
    delete theInterface;
    theInterface = NULL;

    delete theEngine;
    theEngine = NULL;

    // Puis, on rend la main au système.
    return EXIT_SUCCESS;
}
