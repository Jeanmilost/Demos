/*********************************************
 * Cette classe représente le terrain de jeu *
 *********************************************/

#include "playfield.h"

// Constructeur
Playfield::Playfield()
{
    p_X     = PLAYFIELD_X;
    p_Y     = PLAYFIELD_Y;

    p_Score = 0;
}

// Destructeur
Playfield::~Playfield()
{
}

// Retourne la longueur du terrain de jeu, en nombre de blocs.
int Playfield::GetLenght()
{
    return p_X;
}

// Retourne la hauteur du terrain de jeu, en nombre de blocs.
int Playfield::GetHeight()
{
    return p_Y;
}

// Retourne le score.
int Playfield::GetScore()
{
    return p_Score;
}

// Rafraîchit le terrain de jeu avant un nouvel affichage.
void Playfield::Clean()
{
    for ( int y = 0; y < p_Y; y++ )
    {
        for ( int x = 0; x < p_X; x++ )
        {
            // On efface que les blocs qui ne sont pas verrouillés. Les autres
            // appartiennent aux pièces déjà en place.
            if ( p_Playfield[x][y].IsLocked() == false )
            {
                Cell EmptyCell;

                EmptyCell.SetBusy( false );
                EmptyCell.SetLock( false );

                p_Playfield[x][y] = EmptyCell;
            }
        }
    }
}

// Restaure les paramètres d'origine du terrain de jeu.
void Playfield::Reset()
{
    for ( int y = 0; y < p_Y; y++ )
    {
        for ( int x = 0; x < p_X; x++ )
        {
            Cell EmptyCell;

            EmptyCell.SetBusy( false );
            EmptyCell.SetLock( false );

            p_Playfield[x][y] = EmptyCell;
        }
    }

    p_Score = 0;
}

// Teste les lignes du terrain de jeu, et détermine si une ligne est complète.
void Playfield::TestLines()
{
    int Count = 0;

    for ( int y = 0; y < p_Y; y++ )
    {
        for ( int x = 0; x < p_X; x++ )
        {
            // Compte le nombre de blocs verrouillés sur la ligne courante.
            if ( p_Playfield[x][y].IsLocked() == true )
            {
                Count++;
            }
        }

        // Si le nombre de blocs est égal à la largeur du terrain, la ligne
        // est complète. Dans ce cas, on l'élimine, et on augmente le score.
        if ( Count == p_X )
        {
            p_Score += Count;
            DeleteLine( y );
        }

        Count = 0;
    }
}

// Efface une ligne dans le terrain de jeu.
void Playfield::DeleteLine( int Line )
{
    // On commence par copier la ligne supérieure sur la ligne courante, ainsi
    // la ligne située à la hauteur définie par le paramètre "Line" est effacée.
    for ( int y = Line - 1; y >= 0; y-- )
    {
        for ( int x = 0; x < p_X; x++ )
        {
            p_Playfield[x][y+1] = p_Playfield[x][y];
        }
    }

    // Ensuite, on efface la première ligne du terrain de jeu.
    for ( int x = 0; x < p_X; x++ )
    {
        Cell EmptyCell;

        EmptyCell.SetBusy( false );
        EmptyCell.SetLock( false );

        p_Playfield[x][0] = EmptyCell;
    }
}

// Obtient le bloc situé à la position x/y dans le terrain de jeu.
bool Playfield::GetCell( int x, int y, Cell& theCell )
{
    if ( x < 0 || y < 0 || x > p_X - 1 || y > p_Y - 1 )
    {
        return false;
    }

    theCell = p_Playfield[x][y];

    return true;
}

// Copie le bloc "theCell" à la position x/y dans le terrain de jeu.
bool Playfield::SetCell( int x, int y, Cell theCell )
{
    if ( x < 0 || y < 0 || x > p_X - 1 || y > p_Y - 1 )
    {
        return false;
    }

    p_Playfield[x][y] = theCell;

    return true;
}
