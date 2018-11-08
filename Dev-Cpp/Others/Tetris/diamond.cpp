/**********************************************************
 * Cette classe représente une pièce Diamant dans le jeu. *
 **********************************************************/

#include "diamond.h"

// Constructeur
Diamond::Diamond()
{
    p_Top        = 0;
    p_Left       = 0;
    p_Bottom     = 1;
    p_Right      = 4;

    p_NameLenght = 7;

    p_Name       = new char[p_NameLenght];
    p_Name       = "Diamond";

    p_NbCells    = 4;

    p_Cells      = new Cell[p_NbCells]();
    p_Tests      = new Cell[p_NbCells]();

    p_Color      = GREY;

    int Character[] = { 1 , 1 , 1 , 1 };

    for ( int i = 0; i < p_NbCells; i++ )
    {
        p_Cells[i].SetBusy( Character[i] );
        p_Cells[i].SetLock( false );
        p_Cells[i].SetColor( p_Color );
    }
}

// Destructeur
Diamond::~Diamond()
{
    if ( p_Cells != NULL )
    {
        delete[] p_Cells;
        p_Cells = NULL;
    }

    if ( p_Tests != NULL )
    {
        delete[] p_Tests;
        p_Tests = NULL;
    }

    if ( p_Name != NULL )
    {
        delete[] p_Name;
        p_Name = NULL;
    }
}
