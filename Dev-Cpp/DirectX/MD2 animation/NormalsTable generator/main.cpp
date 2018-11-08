/*****************************************************************************
 * ==> Fonction WinMain -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette fonction est le point d'entr�e de l'application.      *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include <windows.h>

#include "e_md2generatetable.h"

int WINAPI WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpszArgument,
                     int       nFunsterStil )
{
    bool IsCreated = false;

    // On cr�e l'objet de g�n�ration du fichier.
    E_MD2GenerateTable* p_Generator = new E_MD2GenerateTable();

    // Puis, on teste si cet objet a bien �t� cr�e en m�moire.
    if ( p_Generator != NULL )
    {
        // Si c'est le cas, on tente de g�n�rer le fichier.
        IsCreated = p_Generator->CreateFile();

        // Puis, on nettoie les ressources utilis�es par le g�n�rateur.
        delete p_Generator;
        p_Generator = NULL;
    }

    // On teste la r�ponse du g�n�rateur de fichiers.
    if ( IsCreated == true )
    {
        // Si la r�ponse est positive, on affiche un message de r�ussite.
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
