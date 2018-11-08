/***************************************************************************
 * Cette classe g�re l'interface du jeu, c'est-�-dire la fa�on dont le jeu *
 * sera affich� � l'�cran, les interactions avec le joueur, etc...         *
 ***************************************************************************/

#include "gameinterface.h"

#include <cstdio>

// FullChar contient le code ASCII pour l'affichage d'un bloc plein.
const char FullChar = 0xDB;

// Constructeur
GameInterface::GameInterface()
{
    p_Win  = GetStdHandle( STD_OUTPUT_HANDLE );
    p_Quit = false;
}

// Destructeur
GameInterface::~GameInterface()
{
}

// Initialise la console pour l'affichage.
void GameInterface::Init()
{
    SetConsoleTitle( "Tetris" );

    CONSOLE_CURSOR_INFO InfoCur;
    InfoCur.dwSize   = 1;
    InfoCur.bVisible = FALSE;

    SetConsoleCursorInfo   ( p_Win, &InfoCur );
    SetCursorColor         ( LIGHTWHITE );
    SetConsoleTextAttribute( p_Win, GAME_LIGHTWHITE );
}

// Cette fonction permet l'affichage d'un message � un emplacement x/y.
void GameInterface::DisplayMessage( LPSTR theMessage,
                                    int   x,
                                    int   y,
                                    bool  DoResetCursor )
{
    // Positionne le curseur � l'emplacement d�sir�.
    p_PosCur.X = (SHORT)x;
    p_PosCur.Y = (SHORT)y;
    SetConsoleCursorPosition( p_Win, p_PosCur );

    // Affiche le message.
    printf( theMessage );

    // Si le drapeau DoResetCursor est pos� (c'est-�-dire �gal � true), on
    // positionne le curseur en haut � gauche de l'�cran, pour ne pas perturber
    // l'affichage normal du jeu.
    if ( DoResetCursor == true )
    {
        p_PosCur.X = (SHORT)0;
        p_PosCur.Y = (SHORT)0;
        SetConsoleCursorPosition( p_Win, p_PosCur );
    }
}

// Cette fonction permet l'affichage du jeu � l'�cran.
void GameInterface::DisplayGame( Engine* theEngine )
{
    SetConsoleTextAttribute( p_Win, GAME_LIGHTWHITE );

    // Affiche le terrain de jeu.
    for ( int y = 0; y < theEngine->GetPlayfield()->GetHeight(); y++ )
    {
        // Affichage du bord gauche, avant la lecture de la ligne.
        printf( "X" );

        // Affichage de la ligne.
        for ( int x = 0; x < theEngine->GetPlayfield()->GetLenght(); x++ )
        {
            Cell aCell;

            // On lit ici le bloc inscrit dans la m�moire.
            if ( theEngine->GetPlayfield()->GetCell( x, y, aCell ) == true )
            {
                // On teste si le bloc est plein.
                if ( aCell.IsBusy() == true )
                {
                    // S'il est plein, on affiche un caract�re plein, de la
                    // couleur inscrite dans le bloc.
                    SetCursorColor( aCell.GetColor() );

                    cout << FullChar;
                }
                else
                {
                    // S'il est vide, on ins�re un espace.
                    printf( " " );
                }

                // Apr�s l'affichage de la ligne, on remet la console � sa
                // couleur de d�part.
                SetCursorColor( LIGHTWHITE );
            }
        }

        // Puis, on affiche le bord droite.
        printf( "X\n" );
    }

    // A la fin de l'affichage du terrain de jeu, on affiche le bord bas.
    printf ( "XXXXXXXXXXXXXXXXX" );

    // On obtient le pointeur de la prochaine pi�ce qui sera envoy�e
    // dans le jeu.
    Piece* NextPiece = theEngine->GetNextPiece();

    // On teste tout d'abord que cette pi�ce existe.
    if ( NextPiece != NULL )
    {
        // On efface ici la m�moire locale pour l'affichage de la pi�ce.
        for ( int y = 0; y < 3; y++ )
        {
            for ( int x = 0; x < 5; x++ )
            {
                p_NextPieceCells[y*5+x].SetBusy( false );
                p_NextPieceCells[y*5+x].SetLock( false );
            }
        }

        // Puis, on copie les donn�es d'affichage localement, depuis la pi�ce.
        for ( int y = 0; y < NextPiece->GetHeight(); y++ )
        {
            for ( int x = 0; x < NextPiece->GetLenght(); x++ )
            {
                Cell aCell;

                aCell.SetBusy( false );
                aCell.SetLock( false );

                // On obtient les donn�es depuis la pi�ce.
                NextPiece->GetCell( x, y, aCell );

                // Si la pi�ce contient un caract�re plein, on le copie dans
                // la m�moire d'affichage.
                if ( aCell.IsBusy() == true )
                {
                    p_NextPieceCells[y*5+x].SetBusy( true );
                    p_NextPieceCells[y*5+x].SetColor( NextPiece->GetColor() );
                }
            }
        }

        // Affiche le message "Prochaine pi�ce".
        DisplayMessage( "Next piece",
                        theEngine->GetPlayfield()->GetLenght() + 7,
                        3,
                        false );

        // Affiche le nom de la prochaine pi�ce.
        DisplayMessage( NextPiece->GetName(),
                        theEngine->GetPlayfield()->GetLenght() + 7,
                        4 );

        // Affiche un blanc apr�s le nom, pour assurer l'effacement des noms les
        // plus longs.
        printf( "          " );

        // Affiche la pi�ce (en r�alit�, le contenu de la m�moire locale).
        for ( int y = 0; y < 3; y++ )
        {
            for ( int x = 0; x < 5; x++ )
            {
                // Positionne le curseur.
                p_PosCur.X =
                    (SHORT)theEngine->GetPlayfield()->GetLenght() + 7 + x;
                p_PosCur.Y = (SHORT)6 + y;
                SetConsoleCursorPosition( p_Win, p_PosCur );

                // C'est le m�me principe que pour le terrain de jeu : Si la
                // m�moire locale contient un bloc plein, on l'affiche selon
                // la couleur sp�cifi�e, sinon on affiche un espace vide.
                if ( p_NextPieceCells[y*5+x].IsBusy() == true )
                {
                    SetCursorColor( p_NextPieceCells[y*5+x].GetColor() );
                    cout << FullChar;
                }
                else
                {
                    printf( " " );
                }

                // On remet la console � sa couleur par d�faut.
                SetCursorColor( LIGHTWHITE );
            }
        }
    }

    // On affiche ici les points. On ne peut pas utiliser la fonction
    // DisplayMessage, car le param�tre %d de la cha�ne de caract�res permet
    // la conversion correcte pour l'affichage des nombres.
    p_PosCur.X = (SHORT)theEngine->GetPlayfield()->GetLenght() + 7;
    p_PosCur.Y = (SHORT)10;
    SetConsoleCursorPosition( p_Win, p_PosCur );

    printf( "Points : %d", theEngine->GetPlayfield()->GetScore() );

    // On affiche ici le niveau. Comme pour ci-dessus, on ne peut pas utiliser
    // la fonction DisplayMessage, car le param�tre %d de la cha�ne de
    // caract�res permet la conversion correcte pour l'affichage des nombres.
    p_PosCur.X = (SHORT)theEngine->GetPlayfield()->GetLenght() + 7;
    p_PosCur.Y = (SHORT)11;
    SetConsoleCursorPosition( p_Win, p_PosCur );

    printf( "Level  : %d", theEngine->GetLevel() );

    // Et pour finir, on replace le curseur en haut � gauche de l'�cran, pour
    // ne pas perturber le prochain affichage.
    p_PosCur.X = (SHORT)0;
    p_PosCur.Y = (SHORT)0;
    SetConsoleCursorPosition( p_Win, p_PosCur );
}

// Cette fonction g�re les t�ches � effectuer lorsque le jeu est termin�.
void GameInterface::DisplayGameOver( Engine* theEngine )
{
    DisplayMessage( "GAME OVER",
                    theEngine->GetPlayfield()->GetLenght() + 7,
                    13 );

    DisplayMessage( "Press Enter to restart",
                    theEngine->GetPlayfield()->GetLenght() + 7,
                    14 );

    int c;

    // On attend l'appui de la touche Enter. Le code 13 correspond � la touche
    // dans le buffer clavier.
    while ( c != 13 )
    {
        c = getch();

        // Toutefois, si la touche Escape est press�e, on doit quitter le jeu.
        if ( c == VK_ESCAPE )
        {
            p_Quit = true;
            return;
        }
    }

    // Repositionne le curseur en haut � gauche de l'�cran.
    p_PosCur.X = (SHORT)0;
    p_PosCur.Y = (SHORT)0;
    SetConsoleCursorPosition( p_Win, p_PosCur );

    // Puis efface la surface d'affichage en entier.
    for ( int y = 0; y < theEngine->GetPlayfield()->GetHeight(); y++ )
    {
       for ( int x = 0; x < theEngine->GetPlayfield()->GetLenght() + 30; x++ )
       {
           printf( " " );
       }

       printf( "\n" );
    }

    // Et pour finir, repositionne le curseur en haut � gauche de l'�cran pour
    // ne pas perturber le prochain affichage.
    p_PosCur.X = (SHORT)0;
    p_PosCur.Y = (SHORT)0;
    SetConsoleCursorPosition( p_Win, p_PosCur );
}

// Cette fonction param�tre le curseur d'affichage � la couleur d�sir�e.
void GameInterface::SetCursorColor( Game_Color Color )
{
    switch ( Color )
    {
        case RED:
        {
            SetConsoleTextAttribute( p_Win, GAME_RED );
            break;
        }

        case GREEN:
        {
            SetConsoleTextAttribute( p_Win, GAME_GREEN );
            break;
        }

        case BLUE:
        {
            SetConsoleTextAttribute( p_Win, GAME_BLUE );
            break;
        }

        case YELLOW:
        {
            SetConsoleTextAttribute( p_Win, GAME_YELLOW );
            break;
        }

        case VIOLET:
        {
            SetConsoleTextAttribute( p_Win, GAME_VIOLET );
            break;
        }

        case MARINE:
        {
            SetConsoleTextAttribute( p_Win, GAME_MARINE );
            break;
        }

        case WHITE:
        {
            SetConsoleTextAttribute( p_Win, GAME_WHITE );
            break;
        }

        case GREY:
        {
            SetConsoleTextAttribute( p_Win, GAME_GREY );
            break;
        }

        case LIGHTRED:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTRED );
            break;
        }

        case LIGHTGREEN:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTGREEN );
            break;
        }

        case LIGHTBLUE:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTBLUE );
            break;
        }

        case LIGHTYELLOW:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTYELLOW );
            break;
        }

        case LIGHTVIOLET:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTVIOLET );
            break;
        }

        case LIGHTMARINE:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTMARINE );
            break;
        }

        case LIGHTWHITE:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTWHITE );
            break;
        }

        default:
        {
            SetConsoleTextAttribute( p_Win, GAME_LIGHTVIOLET );
            break;
        }
    }
}

// Cette fonction effectue les t�ches n�cessaires lorsqu'une touche est press�e.
void GameInterface::OnKeyPressed( Engine* theEngine )
{
    // On obtient la touche depuis le buffer clavier.
    int c = getch();

    // Puis on teste si la touche est une touche �tendue.
    if ( c == 0 || c == 224 )
    {
        // Dans ce cas, il faut relire le code encore une fois.
        c = getch();

        switch( c )
        {
            // Code de la touche fl�ch�e gauche.
            case 75:
            {
                theEngine->MoveLeft();
                break;
            }

            // Code de la touche fl�ch�e droite.
            case 77:
            {
                theEngine->MoveRight();
                break;
            }

            // Code de la touche fl�ch�e bas.
            case 80:
            {
                theEngine->BypassClock();
                break;
            }
        }
    }
    else
    {
        // Escape, on doit quitter le jeu.
        if ( c == VK_ESCAPE )
        {
            p_Quit = true;
            return;
        }

        if ( c == 'y' )
        {
             theEngine->RotateLeft();
        }

        if ( c == 'x' )
        {
             theEngine->RotateRight();
        }

        // Pause.
        if ( c == 'p' )
        {
            // Affiche le message pour indiquer � l'utilisateur que le jeu
            // est en pause.
            DisplayMessage( "PAUSE",
                            theEngine->GetPlayfield()->GetLenght() / 2 - 1,
                            theEngine->GetPlayfield()->
                                GetHeight() / 2 - 1,
                            true );

            // Tant que la touche "p" n'est pas press�e � nouveau, on attend.
            do
            {
                c = getch();

                // Toutefois, si la touche Escape est press�e, il faut quitter
                // le jeu.
                if ( c == VK_ESCAPE )
                {
                    p_Quit = true;
                    return;
                }
            }
            while ( c != 'p');
        }
    }
}

// Cette fonction permet de savoir si le jeu doit �tre quitt�.
bool GameInterface::DoQuit()
{
    return p_Quit;
}
