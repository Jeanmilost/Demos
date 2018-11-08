/******************************************************************************
 * Cette classe repr�sente un bloc. C'est le mat�riel de base pour les pi�ces *
 * et le terrain de jeu.                                                      *
 ******************************************************************************/

#include "cell.h"

// Constructeur
Cell::Cell()
{
    p_Color    = WHITE;
    p_IsBusy   = false;
    p_IsLocked = false;
}

// Destructeur
Cell::~Cell()
{
}

// Retourne la couleur du bloc.
Game_Color Cell::GetColor()
{
    return p_Color;
}

// D�finit la couleur du bloc.
void Cell::SetColor( Game_Color Color )
{
    p_Color = Color;
}

// Permet de savoir si le bloc est occup� (plein) ou libre (vide).
bool Cell::IsBusy()
{
    return p_IsBusy;
}

// D�finit l'�tat du bloc.
void Cell::SetBusy( bool Value )
{
    p_IsBusy = Value;
}

// Permet de savoir si le bloc est verrouill�.
bool Cell::IsLocked()
{
    return p_IsLocked;
}

// D�finit le verrouillage du bloc.
void Cell::SetLock( bool Value )
{
    p_IsLocked = Value;
}
