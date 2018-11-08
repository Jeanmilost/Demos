/*****************************************************************************
 * ==> Classe E_MD2GenerateTable --------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet la g�n�ration d'un fichier binaire      *
 *               contenant les donn�es des normales pr�-calcul�es.           *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_md2generatetable.h"

// Constructeur de la classe E_MD2GenerateTable.
E_MD2GenerateTable::E_MD2GenerateTable()
{
    p_Version     = 1.0f;
    p_DatasLength = NUMVERTEXNORMALS;
}

// Destructeur de la classe E_MD2GenerateTable.
E_MD2GenerateTable::~E_MD2GenerateTable()
{
}

// Cette fonction cr�e un fichier binaire contenant les normales pr�-calcul�es.
bool E_MD2GenerateTable::CreateFile()
{
    // Pointeur vers le fichier.
    FILE* File = NULL;

    // On tente d'ouvrir le fichier en �criture.
    File = fopen( "MD2NormalsTable.bin", "wb" );

    // On teste ensuite si le fichier a bien �t� ouvert.
    if( File == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Puis, on �crit les donn�es d'en-t�te dans le fichier.
    fwrite( &p_Version,     sizeof(float), 1, File );
    fwrite( &p_DatasLength, sizeof(int),   1, File );

    // On �crit ensuite la table des normales dans le fichier.
    for ( int i = 0; i < p_DatasLength; i++ )
    {
        fwrite( &MD2PreCalculedNormalsTable[i], sizeof(D3DXVECTOR3), 1, File );
    }

    // On clos le fichier, ...
    fclose( File );

    // ... puis on retourne le message de r�ussite.
    return true;
}
