/******************************************************************************
 * Cette classe est le moteur du jeu. C'est ici que les diff�rentes phases du *
 * jeu seront appel�es et g�r�es.                                             *
 ******************************************************************************/

#include "engine.h"

// Constructeur
Engine::Engine()
{
    p_Playfield   = new Playfield();

    p_NbPieces    = NB_PIECES;
    p_PositionX   = 0;
    p_PositionY   = 0;

    p_CurPiece    = NULL;
    p_NextPiece   = NULL;

    p_StartTime   = clock();
    p_EndTime     = clock();

    p_GameOver    = false;
    p_BypassClock = false;

    p_Tick        = 999;
    p_Level       = 1;

    srand( 0 );
}

// Destructeur
Engine::~Engine()
{
    if ( p_Playfield != NULL )
    {
        delete p_Playfield;
        p_Playfield = NULL;
    }

    if ( p_NextPiece != NULL )
    {
         delete p_NextPiece;
         p_NextPiece = NULL;
    }

    if ( p_CurPiece != NULL )
    {
         delete p_CurPiece;
         p_CurPiece = NULL;
    }
}

// Fonction principale du jeu, on effectue une action � chaque lecture.
void Engine::Play()
{
    // Si le jeu est fini, il n'est pas utile de lire cette fonction.
    if ( p_GameOver == true )
    {
        return;
    }

    // Sauvegarde l'ancienne hauteur de la pi�ce, on en aura peut-�tre besoin
    // plus tard.
    int OldY = p_PositionY;

    // Puis, on nettoie le terrain de jeu pour permettre un nouvel affichage.
    p_Playfield->Clean();

    // On contr�le qu'une pi�ce existe dans la m�moire.
    if ( p_CurPiece == NULL )
    {
        // Si aucune pi�ce n'existe, on en tire une au sort...
        SetNextPiece();

        // ...puis, on la place � sa position de d�part dans le jeu.
        p_PositionX = ( p_Playfield->GetLenght() / 2 ) -
                          ( p_CurPiece->GetLenght() / 2 );
        p_PositionY = 0;
    }
    else
    {
        // Si une pi�ce existe, on commence par demander au syst�me le temps
        // qui s'est �coul� depuis la derni�re lecture, en millisecondes.
        p_EndTime = clock();

        // On teste s'il est temps de descendre la pi�ce d'une case, et si le
        // joueur n'a pas demand� � acc�l�rer la vitesse du jeu.
        if ( p_EndTime - p_StartTime > p_Tick || p_BypassClock == true )
        {
            // Ici, on r�initialise la r�f�rence du temps �coul�, ainsi que la
            // valeur du drapeau qui permet d'ignorer le test temporel.
            p_StartTime   = clock();
            p_BypassClock = false;

            // On d�finit la vitesse du jeu, en fonction du score du joueur, et
            // � condition que le compteur de secondes p_Tick soit plus grand
            // que 0.
            if ( p_Tick > 0 )
            {
                // Obtient le score actuel.
                int Score = p_Playfield->GetScore();

                if ( Score > 100 )
                {
                    p_Level = 2;
                    p_Tick  = 800;
                }

                if ( Score > 200 )
                {
                    p_Level = 3;
                    p_Tick  = 700;
                }

                if ( Score > 300 )
                {
                    p_Level = 4;
                    p_Tick  = 600;
                }

                if ( Score > 400 )
                {
                    p_Level = 5;
                    p_Tick  = 500;
                }

                if ( Score > 500 )
                {
                    p_Level = 6;
                    p_Tick  = 400;
                }

                if ( Score > 600 )
                {
                    p_Level = 7;
                    p_Tick  = 300;
                }

                if ( Score > 700 )
                {
                    p_Level = 8;
                    p_Tick  = 200;
                }

                if ( Score > 800 )
                {
                    p_Level = 9;
                    p_Tick  = 100;
                }

                if ( Score > 900 )
                {
                    p_Level = 10;
                    p_Tick  = 0;
                }
            }

            // On augmente la position de hauteur de la pi�ce de 1.
            p_PositionY++;

            // Avant de permettre � la pi�ce de descendre, on teste si tous les
            // blocs de la prochaine position sont libres dans le terrain de
            // jeu.
            if ( TestNextPosition() == false )
            {
                // Si ce n'est pas le cas, on teste s'il est possible d'afficher
                // une nouvelle pi�ce. Si l'ancienne position de la pi�ce
                // actuelle vaut 0, alors celle-ci n'a m�me pas pu descendre une
                // seule fois, dans ce cas, le jeu est fini.
                if ( OldY == 0 )
                {
                    p_GameOver = true;
                }

                // Sinon, on replace la pi�ce � sa position pr�c�dente, et on
                // demande le verrouillage des blocs qui la composent. Ainsi,
                // ces blocs resteront pr�sent dans le terrain de jeu jusqu'�
                // ce que la ligne sur laquelle ils se trouvent soit effac�e.
                p_CurPiece->SetPiece( p_PositionX, OldY, true, p_Playfield );

                // Et, pour finir, on efface la pi�ce courante de la m�moire,
                // afin d'obtenir une nouvelle pi�ce.
                if ( p_CurPiece != NULL )
                {
                    delete p_CurPiece;
                    p_CurPiece = NULL;
                }
            }
        }
    }

    // A chaque lecture de la fonction, on teste les lignes du terrain de jeu,
    // pour s'assurer que les lignes pleines soient effac�es.
    p_Playfield->TestLines();

    // Et, enfin, on place la pi�ce actuelle � sa nouvelle position dans le
    // terrain de jeu.
    if ( p_CurPiece != NULL )
    {
        // Notez que les blocs NE SONT PAS verrouill�s (param�tre false), ainsi
        // ils seront effac�s lorsque le terrain de jeu sera nettoy�. (Fonction
        // Clean, voir l'appel plus haut, au d�but de la fonction.)
        p_CurPiece->SetPiece( p_PositionX, p_PositionY, false, p_Playfield );
    }
}

// Cette fonction r�initialise les param�tres du jeu � leurs valeurs d'origine.
void Engine::Reset()
{
    p_NbPieces    = NB_PIECES;
    p_PositionX   = 0;
    p_PositionY   = 0;

    p_StartTime   = clock();
    p_EndTime     = clock();

    p_GameOver    = false;
    p_BypassClock = false;

    p_Tick        = 999;
    p_Level       = 1;

    srand( 0 );

    if ( p_NextPiece != NULL )
    {
         delete p_NextPiece;
         p_NextPiece = NULL;
    }

    if ( p_CurPiece != NULL )
    {
         delete p_CurPiece;
         p_CurPiece = NULL;
    }

    p_Playfield->Reset();
}

// Permet d'obtenir le pointeur vers la classe du terrain de jeu.
Playfield* Engine::GetPlayfield()
{
    return p_Playfield;
}

// Permet d'obtenir la valeur du niveau.
int Engine::GetLevel()
{
    return p_Level;
}

// Cette fonction teste la validit� de la prochaine position d'une pi�ce.
bool Engine::TestNextPosition()
{
    // S'il n'y a pas de pi�ce en m�moire, la prochaine position ne peut pas
    // �tre d�termin�e, dans ce cas, le teste �choue.
    if ( p_CurPiece == NULL )
    {
        return false;
    }

    // Sinon, on fait appel � la fonction de test disponible dans la classe de
    // la pi�ce concern�e.
    return p_CurPiece->TestNextPosition( p_PositionX,
                                         p_PositionY,
                                         p_Playfield );
}

// Cette fonction d�termine la prochaine pi�ce � envoyer dans le terrain de jeu.
void Engine::SetNextPiece()
{
    // L'ordinateur doit conna�tre � l'avance la prochaine pi�ce � envoyer apr�s
    // la pi�ce courante, ceci afin de pouvoir l'afficher sur le bord du terrain
    // de jeu. Ici, on teste si cette pi�ce existe dans la m�moire.
    if ( p_NextPiece == NULL )
    {
        // Si elle n'existe pas, on demande � l'ordinateur d'en tirer une au
        // sort, et de nous envoyer son pointeur, que l'on copie dans la
        // variable p_NextPiece.
        p_NextPiece = GetNewPiece();
    }

    // On teste ici si la pi�ce courante est encore pr�sente dans la m�moire.
    if ( p_CurPiece != NULL )
    {
        // Si c'est le cas, on l'efface avant de continuer.
        delete p_CurPiece;
        p_CurPiece = NULL;
    }

    // Puisque l'on est s�r maintenant que la pi�ce courante n'est plus en
    // m�moire, on attribue le pointeur de la pi�ce suivante, affich�e au bord
    // du terrain de jeu, � celui de la pi�ce courante, puis on tire au sort
    // une nouvelle pi�ce suivante.
    p_CurPiece  = p_NextPiece;
    p_NextPiece = GetNewPiece();
}

// Tire au sort une pi�ce, et renvoie son pointeur.
Piece* Engine::GetNewPiece()
{
    // On utilise une fonction de hasard, que l'on limite � une plage situ�e
    // entre 0 et le nombre de pi�ces possibles dans de jeu.
    switch ( rand() % p_NbPieces )
    {
        // Si le nombre 0 sort, on construit un rubis.
        case 0:
        {
            return new Rubis();
            break;
        }

        // Si le nombre 1 sort, on construit une �meraude.
        case 1:
        {
            return new Emerald();
            break;
        }

        // Si le nombre 2 sort, on construit un diamant.
        case 2:
        {
            return new Diamond();
            break;
        }

        // Si le nombre 3 sort, on construit un saphyr.
        case 3:
        {
            return new Saphyr();
            break;
        }

        // Si le nombre 4 sort, on construit une agathe.
        case 4:
        {
            return new Agatha();
            break;
        }

        // Si le nombre 5 sort, on construit une citrine.
        case 5:
        {
            return new Citrin();
            break;
        }

        // Si le nombre 6 sort, on construit une aquamarine.
        case 6:
        {
            return new Aquamarine();
            break;
        }

        // Par d�faut, on ne construit rien, il s'agit d'une erreur.
        default:
        {
            return NULL;
            break;
        }
    }
}

// Permet de tourner la pi�ce d'1/4 de tour vers la droite.
void Engine::RotateRight()
{
    // D'abord, on teste que la pi�ce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On essaie ici de tourner la pi�ce d'1/4 de tour vers la droite.
        p_CurPiece->RotateRight();

        // Puis, on teste la pi�ce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste �choue, on nettoie le terrain de jeu, puis on remet
            // la pi�ce dans sa position d'origine.
            p_Playfield->Clean();
            p_CurPiece->RotateLeft();
        }
    }
}

// Permet de tourner la pi�ce d'1/4 de tour vers la gauche.
void Engine::RotateLeft()
{
    // D'abord, on teste que la pi�ce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On essaie ici de tourner la pi�ce d'1/4 de tour vers la gauche.
        p_CurPiece->RotateLeft();

        // Puis, on teste la pi�ce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste �choue, on nettoie le terrain de jeu, puis on remet
            // la pi�ce dans sa position d'origine.
            p_Playfield->Clean();
            p_CurPiece->RotateRight();
        }
    }
}

// Permet de faire avancer la pi�ce d'une case vers la droite.
void Engine::MoveRight()
{
    // D'abord, on teste que la pi�ce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On augmente la position de la pi�ce d'une case vers la droite.
        p_PositionX++;

        // Puis, on teste la pi�ce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste �choue, on remet la pi�ce � sa position d'origine.
            p_PositionX--;
        }
    }
}

// Permet de faire avancer la pi�ce d'une case vers la gauche.
void Engine::MoveLeft()
{
    // D'abord, on teste que la pi�ce existe bel et bien.
    if ( p_CurPiece != NULL )
    {
        // On augmente la position de la pi�ce d'une case vers la gauche.
        p_PositionX--;

        // Puis, on teste la pi�ce dans sa nouvelle position.
        if ( p_CurPiece->TestNextPosition
            ( p_PositionX, p_PositionY, p_Playfield ) == false )
        {
            // Si le teste �choue, on remet la pi�ce � sa position d'origine.
            p_PositionX++;
        }
    }
}

// Permet de court-circuiter le test temporel de la fonction Play.
void Engine::BypassClock()
{
    p_BypassClock = true;
}

// Retourne l'�tat du drapeau de fin du jeu.
bool Engine::IsGameOver()
{
    return p_GameOver;
}

// Retourne le pointeur de la prochaine pi�ce qui va appara�tre dans le jeu.
Piece* Engine::GetNextPiece()
{
    return p_NextPiece;
}
