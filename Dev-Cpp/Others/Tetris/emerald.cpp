/***********************************************************
 * Cette classe représente une pièce Emeraude dans le jeu. *
 ***********************************************************/

#include "emerald.h"

// Constructeur
Emerald::Emerald()
{
    p_Top        = 0;
    p_Left       = 0;
    p_Bottom     = 3;
    p_Right      = 2;

    p_NameLenght = 7;

    p_Name       = new char[p_NameLenght];
    p_Name       = "Emerald";

    p_NbCells    = 6;

    p_Color      = LIGHTGREEN;

    p_Cells      = new Cell[p_NbCells]();
    p_Tests      = new Cell[p_NbCells]();

    int Character[] = { 1 , 0 ,
                        1 , 1 ,
                        0 , 1 };

    for ( int i = 0; i < p_NbCells; i++ )
    {
        p_Cells[i].SetBusy( Character[i] );
        p_Cells[i].SetLock( false );
        p_Cells[i].SetColor( p_Color );
    }
}

// Destructeur
Emerald::~Emerald()
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
