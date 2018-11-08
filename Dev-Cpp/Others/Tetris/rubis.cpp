/********************************************************
 * Cette classe représente une pièce Rubis dans le jeu. *
 ********************************************************/

#include "rubis.h"

// Constructeur
Rubis::Rubis()
{
    p_Top        = 0;
    p_Left       = 0;
    p_Bottom     = 2;
    p_Right      = 2;

    p_NameLenght = 5;

    p_Name       = new char[p_NameLenght];
    p_Name       = "Rubis";

    p_NbCells    = 4;

    p_Cells      = new Cell[p_NbCells]();
    p_Tests      = new Cell[p_NbCells]();

    p_Color      = LIGHTRED;

    for ( int i = 0; i < p_NbCells; i++ )
    {
        p_Cells[i].SetBusy( true );
        p_Cells[i].SetLock( false );
        p_Cells[i].SetColor( p_Color );
    }
}

// Destructeur
Rubis::~Rubis()
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
