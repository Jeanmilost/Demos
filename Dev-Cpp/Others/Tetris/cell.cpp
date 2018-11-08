/******************************************************************************
 * Cette classe représente un bloc. C'est le matériel de base pour les pièces *
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

// Définit la couleur du bloc.
void Cell::SetColor( Game_Color Color )
{
    p_Color = Color;
}

// Permet de savoir si le bloc est occupé (plein) ou libre (vide).
bool Cell::IsBusy()
{
    return p_IsBusy;
}

// Définit l'état du bloc.
void Cell::SetBusy( bool Value )
{
    p_IsBusy = Value;
}

// Permet de savoir si le bloc est verrouillé.
bool Cell::IsLocked()
{
    return p_IsLocked;
}

// Définit le verrouillage du bloc.
void Cell::SetLock( bool Value )
{
    p_IsLocked = Value;
}
