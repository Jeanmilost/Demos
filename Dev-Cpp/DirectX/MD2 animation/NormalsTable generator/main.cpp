/*****************************************************************************
 * ==> Fonction WinMain -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette fonction est le point d'entrée de l'application.      *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include <windows.h>

#include "e_md2generatetable.h"

int WINAPI WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpszArgument,
                     int       nFunsterStil )
{
    bool IsCreated = false;

    // On crée l'objet de génération du fichier.
    E_MD2GenerateTable* p_Generator = new E_MD2GenerateTable();

    // Puis, on teste si cet objet a bien été crée en mémoire.
    if ( p_Generator != NULL )
    {
        // Si c'est le cas, on tente de générer le fichier.
        IsCreated = p_Generator->CreateFile();

        // Puis, on nettoie les ressources utilisées par le générateur.
        delete p_Generator;
        p_Generator = NULL;
    }

    // On teste la réponse du générateur de fichiers.
    if ( IsCreated == true )
    {
        // Si la réponse est positive, on affiche un message de réussite.
        MessageBox( NULL,
                    "File created successfully.",
                    "MD2 Normal Table Generator",
                    MB_OK );
    }
    else
    {
        // Sinon, on affiche une erreur.
        MessageBox( NULL,
                    "File creation failed.",
                    "MD2 Normal Table Generator",
                    MB_OK );
    }

    return 0;
}
