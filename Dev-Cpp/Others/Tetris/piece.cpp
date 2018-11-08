/*****************************************************************************
 * Classe de base pour les pièces du jeu. Les routines communes à toutes les *
 * pièces, comme par exemple la rotation d'une pièce, sont écrites ici.      *
 *****************************************************************************/

#include "piece.h"

// Constructeur
Piece::Piece()
{
    p_Name       = NULL;
    p_Cells      = NULL;
    p_Tests      = NULL;
    p_Color      = WHITE;
    p_Right      = 0;
    p_Left       = 0;
    p_Top        = 0;
    p_Bottom     = 0;
    p_Position   = 0;
    p_NbCells    = 0;
    p_NameLenght = 0;
}

// Destructeur
Piece::~Piece()
{
}

// Retourne le nom de la pièce.
char* Piece::GetName()
{
    return p_Name;
}

// Retourne la longeur du nom de la pièce.
int Piece::GetNameLenght()
{
    return p_NameLenght;
}

// Retourne la couleur de la pièce.
Game_Color Piece::GetColor()
{
    return p_Color;
}

// Retourne la longeur de la pièce, en nombre de blocs.
int Piece::GetLenght()
{
    return p_Right - p_Left;
}

// Retourne la hauteur de la pièce, en nombre de blocs.
int Piece::GetHeight()
{
    return p_Bottom - p_Top;
}

// Cette fonction affiche la pièce dans le jeu, à l'emplacement désiré.
bool Piece::SetPiece( int        Left,
                      int        Top,
                      bool       SetLock,
                      Playfield* thePlayfield )
{
    // Si le terrain de jeu n'existe pas, on ne peut pas afficher la pièce.
    if ( thePlayfield == NULL )
    {
        return false;
    }

    // On choisit la routine d'affichage selon la position demandée.
    switch ( p_Position )
    {
        // Pièce normale.
        case 0:
        {
            // Affiche la pièce.
            for ( int y = p_Top; y < p_Bottom; y++ )
            {
                for ( int x = p_Left; x < p_Right; x++ )
                {
                    p_Cells[y*p_Right+x].SetLock( SetLock );

                    // On ne copie ici que les blocs pleins de la pièce.
                    if ( p_Cells[y*p_Right+x].IsBusy() == true )
                    {
                        thePlayfield->SetCell( Left + x,
                                               Top + y,
                                               p_Cells[y*p_Right+x] );
                    }
                }
            }

            break;
        }

        // Pièce tournée de 90 degrés.
        case 1:
        {
            // Affiche la pièce.
            for ( int y = p_Left; y < p_Right; y++ )
            {
                for ( int x = p_Bottom; x > p_Top; x-- )
                {
                    p_Cells[(x*p_Right-y)-1].SetLock( SetLock );

                    // On ne copie ici que les blocs pleins de la pièce.
                    if ( p_Cells[(x*p_Right-y)-1].IsBusy() == true )
                    {
                        thePlayfield->SetCell( ( Left + p_Bottom ) - x,
                                               ( Top + p_Right ) - y - 1,
                                               p_Cells[(x*p_Right-y)-1] );
                    }
                }
            }

            break;
        }

        // Pièce tournée de 180 degrés.
        case 2:
        {
            // Affiche la pièce.
            for ( int y = p_Bottom; y > p_Top; y-- )
            {
                for ( int x = p_Right; x > p_Left; x-- )
                {
                    p_Cells[(y-1)*p_Right+(x-1)].SetLock( SetLock );

                    // On ne copie ici que les blocs pleins de la pièce.
                    if ( p_Cells[(y-1)*p_Right+(x-1)].IsBusy() == true )
                    {
                        thePlayfield->SetCell( ( Left + p_Right ) - x,
                                               ( Top + p_Bottom ) - y,
                                               p_Cells[(y-1)*p_Right+(x-1)] );
                    }
                }
            }

            break;
        }

        // Pièce tournée de 270 degrés.
        case 3:
        {
            // Affiche la pièce.
            for ( int y = p_Right; y > p_Left; y-- )
            {
                for ( int x = p_Top; x < p_Bottom; x++ )
                {
                    p_Cells[(x*p_Right+y)-1].SetLock( SetLock );

                    // On ne copie ici que les blocs pleins de la pièce.
                    if ( p_Cells[(x*p_Right+y)-1].IsBusy() == true )
                    {
                        thePlayfield->SetCell( Left + x,
                                               ( Top + p_Right ) - y,
                                               p_Cells[(x*p_Right+y)-1] );
                    }
                }
            }

            break;
        }
    }

    return true;
}

// Cette fonction permet de tester la validité de la prochaine position.
bool Piece::TestNextPosition( int Left, int Top, Playfield* thePlayfield )
{
    // Si le terrain de jeu n'existe pas, on ne peut pas tester la pièce.
    if ( thePlayfield == NULL )
    {
        return false;
    }

    // On choisit l'algorythme pour la récupération des blocs dans le terrain
    // de jeu selon la position courante de la pièce.
    switch ( p_Position )
    {
        // Pièce normale.
        case 0:
        {
            // Teste si la pièce ne sort pas du terrain de jeu.
            if ( Left < 0 || Left + p_Right > thePlayfield->GetLenght() ||
                 Top < 0 || Top + p_Bottom > thePlayfield->GetHeight() )
            {
                return false;
            }

            // Obtient les données de l'emplacement à tester.
            for ( int y = p_Top; y < p_Bottom; y++ )
            {
                for ( int x = p_Left; x < p_Right; x++ )
                {
                    thePlayfield->GetCell( Left + x,
                                           Top + y,
                                           p_Tests[y*p_Right+x] );
                }
            }

            break;
        }

        // Pièce tournée de 90 degrés.
        case 1:
        {
            // Teste si la pièce ne sort pas du terrain de jeu.
            if ( Left < 0 || Left + p_Bottom > thePlayfield->GetLenght() ||
                 Top < 0 || Top + p_Right > thePlayfield->GetHeight() )
            {
                return false;
            }

            // Obtient les données de l'emplacement à tester.
            for ( int y = p_Left; y < p_Right; y++ )
            {
                for ( int x = p_Bottom; x > p_Top; x-- )
                {
                    thePlayfield->GetCell( ( Left + p_Bottom ) - x,
                                           ( Top + p_Right ) - y - 1,
                                           p_Tests[(x*p_Right-y)-1] );
                }
            }

            break;
        }

        // Pièce tournée de 180 degrés.
        case 2:
        {
            // Teste si la pièce ne sort pas du terrain de jeu.
            if ( Left < 0 || Left + p_Right > thePlayfield->GetLenght() ||
                 Top < 0 || Top + p_Bottom > thePlayfield->GetHeight() )
            {
                return false;
            }

            // Obtient les données de l'emplacement à tester.
            for ( int y = p_Bottom; y > p_Top; y-- )
            {
                for ( int x = p_Right; x > p_Left; x-- )
                {
                    thePlayfield->GetCell( ( Left + p_Right ) - x,
                                           ( Top + p_Bottom ) - y,
                                           p_Tests[(y-1)*p_Right+(x-1)] );
                }
            }

            break;
        }

        // Pièce tournée de 270 degrés.
        case 3:
        {
            // Teste si la pièce ne sort pas du terrain de jeu.
            if ( Left < 0 || Left + p_Bottom > thePlayfield->GetLenght() ||
                 Top < 0 || Top + p_Right > thePlayfield->GetHeight() )
            {
                return false;
            }

            // Obtient les données de l'emplacement à tester.
            for ( int y = p_Right; y > p_Left; y-- )
            {
                for ( int x = p_Top; x < p_Bottom; x++ )
                {
                    thePlayfield->GetCell( Left + x,
                                           ( Top + p_Right ) - y,
                                           p_Tests[(x*p_Right+y)-1] );
                }
            }

            break;
        }
    }

    // Teste les blocs récupérés.
    for ( int i = 0; i < p_NbCells; i++ )
    {
        // Si l'un des blocs du terrain de jeu est occupé et verrouillé, et que
        // le bloc de la pièce que l'on veut copier au même emplacement est
        // plein, alors la pièce ne peut pas occuper cet espace. Dans ce cas, le
        // test échoue.
        if ( p_Tests[i].IsBusy()   == true &&
             p_Tests[i].IsLocked() == true &&
             p_Cells[i].IsBusy()   == true )
        {
             return false;
        }
    }

    return true;
}

// Cette fonction permet de faire pivoter la pièce d'1/4 de tour vers la droite.
void Piece::RotateRight()
{
    // L'opérande logique AND 3 (& 3) en fin d'équation est ce que l'on appelle
    // un masque binaire. Ceci permet de limiter la plage des valeurs de 0 à 3.
    // Si vous connaissez l'algèbre binaire, vous saurez assez vite pourquoi.
    // Je vous rappelle la règle suivante : 0 AND 0 = 0
    //                                      0 AND 1 = 0
    //                                      1 AND 0 = 0
    //                                      1 AND 1 = 1
    // Et voici les valeurs binaires des chiffres suivants :
    //                                      0 = 00
    //                                      1 = 01
    //                                      2 = 10
    //                                      3 = 11
    // Aussi, faites les calculs:
    //     00 (0)      01 (1)      10 (2)      11 (3)      100 (4)       101 (5)
    // AND 11 (3)  AND 11 (3)  AND 11 (3)  AND 11 (3)  AND 011 (3)   AND 011 (3)
    //   = 00 (0)    = 01 (1)    = 10 (2)    = 11 (3)    = 000 (0)     = 001 (1)
    // Et ainsi de suite... L'avantage d'un masque binaire, quand on peut en
    // utiliser un, est qu'il est beaucoup plus rapide qu'un test if, et
    // nettement moins coûteux en place dans le code.
    p_Position = ( p_Position + 1 ) & 3;
}

// Cette fonction permet de faire pivoter la pièce d'1/4 de tour vers la gauche.
void Piece::RotateLeft()
{
    // L'opérande logique AND 3 (& 3) en fin d'équation est ce que l'on appelle
    // un masque binaire. Ceci permet de limiter la plage des valeurs de 0 à 3.
    // Si vous connaissez l'algèbre binaire, vous saurez assez vite pourquoi.
    // Je vous rappelle la règle suivante : 0 AND 0 = 0
    //                                      0 AND 1 = 0
    //                                      1 AND 0 = 0
    //                                      1 AND 1 = 1
    // Et voici les valeurs binaires des chiffres suivants :
    //                                      0 = 00
    //                                      1 = 01
    //                                      2 = 10
    //                                      3 = 11
    // Aussi, faites les calculs:
    //     00 (0)      01 (1)      10 (2)      11 (3)      100 (4)       101 (5)
    // AND 11 (3)  AND 11 (3)  AND 11 (3)  AND 11 (3)  AND 011 (3)   AND 011 (3)
    //   = 00 (0)    = 01 (1)    = 10 (2)    = 11 (3)    = 000 (0)     = 001 (1)
    // Et ainsi de suite... L'avantage d'un masque binaire, quand on peut en
    // utiliser un, est qu'il est beaucoup plus rapide qu'un test if, et
    // nettement moins coûteux en place dans le code.
    p_Position = ( p_Position - 1 ) & 3;
}

// Permet d'obtenir l'un des blocs composant la pièce, à l'emplacement x/y.
bool Piece::GetCell( int x, int y, Cell& theCell )
{
    if ( x < 0 || y < 0 || x > p_Right - 1 || y > p_Bottom - 1 )
    {
        return false;
    }

    theCell = p_Cells[y*(p_Right-p_Left)+x];

    return true;
}
